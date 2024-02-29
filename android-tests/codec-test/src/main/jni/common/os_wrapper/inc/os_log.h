#ifndef _OS_LOG_H_
#define _OS_LOG_H_

#include <sstream>
#include <os_typedefs.h>

namespace os {

class FatalLog {
private:
  std::ostringstream _stream;
public:
  FatalLog(const char* file, int line);
  FatalLog(const char* file, int line, std::string * str);
  ~FatalLog();

  std::ostream& stream() { return _stream; }
};

} // namespace os

#ifdef __cplusplus
extern "C" {
#endif

enum LogLevel {
  eLogVerbose = 0x1,
  eLogInfo    = 0x2,
  eLogDebug   = 0x3,
  eLogWarn    = 0x4,
  eLogError   = 0x5,
};

extern int32_t log_setlevel(int32_t level);
extern int32_t log_getlevel();


extern void os_printf(int32_t level, const char *tag, const char *fmt, va_list args);

extern void log_verbose(const char *tag, const char *fmt, ...);
extern void log_info(const char *tag, const char *fmt, ...);
extern void log_debug(const char *tag, const char *fmt, ...);
extern void log_warn(const char *tag, const char *fmt, ...);
extern void log_error(const char *tag, const char *fmt, ...);
extern void log_trace(int32_t level, const char *tag, const char *fmt, ...);

typedef void (*pLogTraceFun)(int32_t level, const char *tag, const char *fmt, va_list args);
extern void log_set_log_redirect(pLogTraceFun redirect_trace);

#ifndef LOGTAG
#define LOGTAG "Tag"
#endif
#define logv(fmt, ...) log_verbose(LOGTAG, fmt, ##__VA_ARGS__)
#define logi(fmt, ...) log_info(LOGTAG, fmt, ##__VA_ARGS__)
#define logd(fmt, ...) log_debug(LOGTAG, fmt, ##__VA_ARGS__)
#define logw(fmt, ...) log_warn(LOGTAG, fmt, ##__VA_ARGS__)
#define loge(fmt, ...) log_error(LOGTAG, fmt, ##__VA_ARGS__)

#ifdef __cplusplus
} // extern "C"
#endif

#endif //_OS_LOG_H_
