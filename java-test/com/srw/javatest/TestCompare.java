package com.srw.javatest;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import com.example.lib.java.FormatLog;

public class TestCompare extends BaseTest {

    public static class FramerateRange {
        public int min;
        public int max;

        public FramerateRange(int min, int max) {
            this.min = min;
            this.max = max;
        }

        @Override
        public String toString() {
            return "[" + (min / 1000.0f) + ":" + (max / 1000.0f) + "]";
        }

        @Override
        public boolean equals(Object other) {
            if (!(other instanceof FramerateRange)) {
                return false;
            }
            final FramerateRange otherFramerate = (FramerateRange) other;
            return min == otherFramerate.min && max == otherFramerate.max;
        }

        @Override
        public int hashCode() {
            // Use prime close to 2^16 to avoid collisions for normal values less than 2^16.
            return 1 + 65537 * min + max;
        }
    }

    private static abstract class ClosestComparator<T> implements Comparator<T> {
        // Difference between supported and requested parameter.
        abstract int diff(T supportedParameter);

        @Override
        public int compare(T t1, T t2) {
            return diff(t1) - diff(t2);
        }
    }

    public static FramerateRange getClosestSupportedFramerateRange(
            List<FramerateRange> supportedFramerates, final int requestedFps) {
        return Collections.min(
                supportedFramerates, new ClosestComparator<FramerateRange>() {
                    // Progressive penalty if the upper bound is further away than |MAX_FPS_DIFF_THRESHOLD|
                    // from requested.
                    private static final int MAX_FPS_DIFF_THRESHOLD = 5000;
                    private static final int MAX_FPS_LOW_DIFF_WEIGHT = 1;
                    private static final int MAX_FPS_HIGH_DIFF_WEIGHT = 3;

                    // Progressive penalty if the lower bound is bigger than |MIN_FPS_THRESHOLD|.
                    private static final int MIN_FPS_THRESHOLD = 8000;
                    private static final int MIN_FPS_LOW_VALUE_WEIGHT = 1;
                    private static final int MIN_FPS_HIGH_VALUE_WEIGHT = 4;

                    // Use one weight for small |value| less than |threshold|, and another weight above.
                    private int progressivePenalty(int value, int threshold, int lowWeight, int highWeight) {
                        return (value < threshold) ? value * lowWeight
                                : threshold * lowWeight + (value - threshold) * highWeight;
                    }

                    @Override
                    int diff(FramerateRange range) {
                        final int minFpsError = progressivePenalty(
                                range.min, MIN_FPS_THRESHOLD, MIN_FPS_LOW_VALUE_WEIGHT, MIN_FPS_HIGH_VALUE_WEIGHT);
                        final int maxFpsError = progressivePenalty(Math.abs(requestedFps * 1000 - range.max),
                                MAX_FPS_DIFF_THRESHOLD, MAX_FPS_LOW_DIFF_WEIGHT, MAX_FPS_HIGH_DIFF_WEIGHT);
                        FormatLog.LogI("minxError: " + minFpsError
                                        + ", min: " + range.min);
                        FormatLog.LogI("maxxError: " + maxFpsError 
                                        + ", max: " + range.max);
                        return minFpsError + maxFpsError;
                    }
                });
    }

    @Override
    void test() {
        List<FramerateRange> testData = new ArrayList() {{
            add(new FramerateRange(12 * 1000, 12 * 1000));
            add(new FramerateRange(14 * 1000, 14 * 1000));
            add(new FramerateRange(12 * 1000, 15 * 1000));
            add(new FramerateRange(15 * 1000, 15 * 1000));
            add(new FramerateRange(14 * 1000, 20 * 1000));
            add(new FramerateRange(20 * 1000, 20 * 1000));
            add(new FramerateRange(14 * 1000, 25 * 1000));
            add(new FramerateRange(25 * 1000, 25 * 1000));
            add(new FramerateRange(14 * 1000, 30 * 1000));
            add(new FramerateRange(30 * 1000, 30 * 1000));
        }};

        FramerateRange close = getClosestSupportedFramerateRange(testData, 30);
        FormatLog.LogI("close " + close);
    }
}
