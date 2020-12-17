#include <algorithm>
#include <iostream>
#include "TestTimestampExtrapolator.h"

/* test data
start 390012248
update 390012839:1
extrapolate 1=>390012839
update 390012884:3781
update 390012897:7471
extrapolate 3781=>390012874
extrapolate 7471=>390012905
update 390012977:12061
extrapolate 12061=>390012971
update 390013048:19081
extrapolate 19081=>390013049
update 390013079:22501
extrapolate 22501=>390013083
update 390013110:26191
extrapolate 26191=>390013118
update 390013164:30151
extrapolate 30151=>390013162
update 390013199:33841
extrapolate 33841=>390013201
update 390013292:41311
extrapolate 41311=>390013286
update 390013322:45181
extrapolate 45181=>390013326
update 390013360:48781
extrapolate 48781=>390013364
update 390013396:52561
extrapolate 52561=>390013402
update 390013446:56251
extrapolate 56251=>390013443
update 390013544:63901
extrapolate 63901=>390013530
update 390013568:67501
extrapolate 67501=>390013569
update 390013609:71551
extrapolate 71551=>390013612
update 390013655:75061
extrapolate 75061=>390013651
update 390013710:78931
extrapolate 78931=>390013697
update 390013809:86671
extrapolate 86671=>390013787
update 390013816:90091
extrapolate 90091=>390013823
update 390013867:93961
extrapolate 93961=>390013866
update 390013914:98101
extrapolate 98101=>390013912
update 390013946:101341
extrapolate 101341=>390013948
update 390014021:108811
extrapolate 108811=>390014028
update 390014068:112681
extrapolate 112681=>390014071
update 390014116:116461
extrapolate 116461=>390014113
update 390014153:120331
extrapolate 120331=>390014155
update 390014194:124021
extrapolate 124021=>390014195
update 390014275:131581
extrapolate 131581=>390014278
update 390014324:135181
extrapolate 135181=>390014318
update 390014360:138961
extrapolate 138961=>390014360
update 390014402:142651
extrapolate 142651=>390014401
update 390014449:146431
extrapolate 146431=>390014443
update 390014527:153811
extrapolate 153811=>390014525
update 390014597:157681
extrapolate 157681=>390014571
update 390014612:161461
extrapolate 161461=>390014612
update 390014651:165241
extrapolate 165241=>390014654
update 390014734:168931
extrapolate 168931=>390014698
update 390014799:176491
extrapolate 176491=>390014784
update 390014819:180181
extrapolate 180181=>390014824
update 390014862:183961
extrapolate 183961=>390014866
update 390014918:187651
extrapolate 187651=>390014908
update 390014940:191431
extrapolate 191431=>390014949
update 390015042:198901
extrapolate 198901=>390015032
update 390015079:202861
extrapolate 202861=>390015077
update 390015120:206551
extrapolate 206551=>390015118
*/

TimestampExtrapolator::TimestampExtrapolator(int64_t start_ms)
    : _startMs(0),
      _firstTimestamp(0),
      _wrapArounds(0),
      _prevUnwrappedTimestamp(-1),
      _prevWrapTimestamp(-1),
      _lambda(1),
      _firstAfterReset(true),
      _packetCount(0),
      _startUpFilterDelayInPackets(2),
      _detectorAccumulatorPos(0),
      _detectorAccumulatorNeg(0),
      _alarmThreshold(60e3),
      _accDrift(6600),  // in timestamp ticks, i.e. 15 ms
      _accMaxError(7000),
      _pP11(1e10) {
  Reset(start_ms);
}

TimestampExtrapolator::~TimestampExtrapolator() {
}

void TimestampExtrapolator::setUp() {
  printf("TimestampExtrapolator::setUp \n");
}

void TimestampExtrapolator::run() {
  Update(390012839, 1);
  ExtrapolateLocalTime(1);
  Update(390012884, 3781);
  Update(390012897, 7471);
  ExtrapolateLocalTime(3781);
  ExtrapolateLocalTime(7471);
}

void TimestampExtrapolator::tearDown() {
  printf("TimestampExtrapolator::tearDown\n");
}

void TimestampExtrapolator::Reset(int64_t start_ms) {
  _startMs = start_ms;
  _prevMs = _startMs;
  _firstTimestamp = 0;
  _w[0] = 90.0;
  _w[1] = 0;
  _pP[0][0] = 1;
  _pP[1][1] = _pP11;
  _pP[0][1] = _pP[1][0] = 0;
  _firstAfterReset = true;
  _prevUnwrappedTimestamp = -1;
  _prevWrapTimestamp = -1;
  _wrapArounds = 0;
  _packetCount = 0;
  _detectorAccumulatorPos = 0;
  _detectorAccumulatorNeg = 0;
  std::cout << "TimestampExtrapolator Reset with startMs " << start_ms << " W0 " << _w[0] << std::endl;
}

void TimestampExtrapolator::Update(int64_t tMs, uint32_t ts90khz) {
  std::cout << "==> update tMs " << tMs << " ts90khz " << ts90khz << std::endl;
  if (tMs - _prevMs > 10e3) {
    // Ten seconds without a complete frame.
    // Reset the extrapolator
    Reset(tMs);
  } else {
    _prevMs = tMs;
  }

  // Remove offset to prevent badly scaled matrices
  tMs -= _startMs;

  CheckForWrapArounds(ts90khz);

  int64_t unwrapped_ts90khz =
      static_cast<int64_t>(ts90khz) +
      _wrapArounds * ((static_cast<int64_t>(1) << 32) - 1);
  std::cout << "unwrapped_ts90khz " << unwrapped_ts90khz << std::endl;

  if (_firstAfterReset) {
    // Make an initial guess of the offset,
    // should be almost correct since tMs - _startMs
    // should about zero at this time.
    _w[1] = -_w[0] * tMs;
    _firstTimestamp = unwrapped_ts90khz;
    std::cout << "first after reset,  w0 "
            << _w[0] << " w1 "
            << _w[1] << " _firstTimestamp "
            << _firstTimestamp << std::endl;
    _firstAfterReset = false;
  }

  double residual = (static_cast<double>(unwrapped_ts90khz) - _firstTimestamp) -
                    static_cast<double>(tMs) * _w[0] - _w[1];
  std::cout << "w0 " << _w[0]
            << " w1 " << _w[1]
            << " residual " << residual << std::endl;
  if (DelayChangeDetection(residual) &&
      _packetCount >= _startUpFilterDelayInPackets) {
    // A sudden change of average network delay has been detected.
    // Force the filter to adjust its offset parameter by changing
    // the offset uncertainty. Don't do this during startup.
    _pP[1][1] = _pP11;
  }

  if (_prevUnwrappedTimestamp >= 0 &&
      unwrapped_ts90khz < _prevUnwrappedTimestamp) {
    // Drop reordered frames.
    return;
  }

  // T = [t(k) 1]';
  // that = T'*w;
  // K = P*T/(lambda + T'*P*T);
  double K[2];
  K[0] = _pP[0][0] * tMs + _pP[0][1];
  K[1] = _pP[1][0] * tMs + _pP[1][1];
  double TPT = _lambda + tMs * K[0] + K[1];
  std::cout << "p00 " << _pP[0][0]
          << " p01 " << _pP[0][1]
          << " p10 " << _pP[1][0]
          << " p11 " << _pP[1][1]
          << std::endl;
  K[0] /= TPT;
  K[1] /= TPT;
  // w = w + K*(ts(k) - that);
  _w[0] = _w[0] + K[0] * residual;
  _w[1] = _w[1] + K[1] * residual;
  // P = 1/lambda*(P - K*T'*P);
  double p00 =
      1 / _lambda * (_pP[0][0] - (K[0] * tMs * _pP[0][0] + K[0] * _pP[1][0]));
  double p01 =
      1 / _lambda * (_pP[0][1] - (K[0] * tMs * _pP[0][1] + K[0] * _pP[1][1]));
  _pP[1][0] =
      1 / _lambda * (_pP[1][0] - (K[1] * tMs * _pP[0][0] + K[1] * _pP[1][0]));
  _pP[1][1] =
      1 / _lambda * (_pP[1][1] - (K[1] * tMs * _pP[0][1] + K[1] * _pP[1][1]));
  _pP[0][0] = p00;
  _pP[0][1] = p01;
  _prevUnwrappedTimestamp = unwrapped_ts90khz;
  if (_packetCount < _startUpFilterDelayInPackets) {
    _packetCount++;
  }
}

int64_t TimestampExtrapolator::ExtrapolateLocalTime(uint32_t timestamp90khz) {
  int64_t localTimeMs = 0;
  CheckForWrapArounds(timestamp90khz);
  double unwrapped_ts90khz =
      static_cast<double>(timestamp90khz) +
      _wrapArounds * ((static_cast<int64_t>(1) << 32) - 1);
  if (_packetCount == 0) {
    localTimeMs = -1;
  } else if (_packetCount < _startUpFilterDelayInPackets) {
    localTimeMs =
        _prevMs +
        static_cast<int64_t>(
            static_cast<double>(unwrapped_ts90khz - _prevUnwrappedTimestamp) /
                90.0 +
            0.5);
  } else {
    if (_w[0] < 1e-3) {
      localTimeMs = _startMs;
    } else {
      double timestampDiff =
          unwrapped_ts90khz - static_cast<double>(_firstTimestamp);
      localTimeMs = static_cast<int64_t>(static_cast<double>(_startMs) +
                                         (timestampDiff - _w[1]) / _w[0] + 0.5);
    }
  }
  std::cout << "Extrapolate timestamp90khz " << timestamp90khz << " return local " << localTimeMs << std::endl;
  return localTimeMs;
}

// Investigates if the timestamp clock has overflowed since the last timestamp
// and keeps track of the number of wrap arounds since reset.
void TimestampExtrapolator::CheckForWrapArounds(uint32_t ts90khz) {
  if (_prevWrapTimestamp == -1) {
    _prevWrapTimestamp = ts90khz;
    return;
  }
  if (ts90khz < _prevWrapTimestamp) {
    // This difference will probably be less than -2^31 if we have had a wrap
    // around (e.g. timestamp = 1, _previousTimestamp = 2^32 - 1). Since it is
    // casted to a Word32, it should be positive.
    if (static_cast<int32_t>(ts90khz - _prevWrapTimestamp) > 0) {
      // Forward wrap around
      _wrapArounds++;
    }
  } else {
    // This difference will probably be less than -2^31 if we have had a
    // backward wrap around. Since it is casted to a Word32, it should be
    // positive.
    if (static_cast<int32_t>(_prevWrapTimestamp - ts90khz) > 0) {
      // Backward wrap around
      _wrapArounds--;
    }
  }
  _prevWrapTimestamp = ts90khz;
  std::cout << "CheckForWrapArounds ts90khz " << ts90khz << " _prevWrapTimestamp " << _prevWrapTimestamp << std::endl;
}

bool TimestampExtrapolator::DelayChangeDetection(double error) {
  // CUSUM detection of sudden delay changes
  error = (error > 0) ? std::min(error, _accMaxError)
                      : std::max(error, -_accMaxError);
  _detectorAccumulatorPos =
      std::max(_detectorAccumulatorPos + error - _accDrift, double{0});
  _detectorAccumulatorNeg =
      std::min(_detectorAccumulatorNeg + error + _accDrift, double{0});
  if (_detectorAccumulatorPos > _alarmThreshold ||
      _detectorAccumulatorNeg < -_alarmThreshold) {
    // Alarm
    _detectorAccumulatorPos = _detectorAccumulatorNeg = 0;
    return true;
  }
  return false;
}
