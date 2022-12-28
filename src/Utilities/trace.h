#include <windows.h>

#if defined _DEBUG || defined _DEBUG_RELEASE
void _trace(const TCHAR *format, ...);
void _traceA(const char *format,...);
#define TRACE		_trace
#define TRACE_A	_traceA
#else
#define TRACE		__noop
#define TRACE_A	__noop
#endif