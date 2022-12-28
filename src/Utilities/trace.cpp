
#include "stdafx.h"
#include "trace.h"

#define TRACEMAXSTRING  1024
#if defined _UNICODE
	#define RPT _RPTW0
#else
	#define RPT _RPT0
#endif

void _trace(const TCHAR *format,...)
{
	TCHAR szBuffer[TRACEMAXSTRING];
	va_list args;
	va_start(args,format);
	int nBuf;
	nBuf = _vsntprintf_s(szBuffer, TRACEMAXSTRING, format, args);
	va_end(args);
	RPT(_CRT_WARN,szBuffer);
}


void _traceA(const char *format,...)
{
	char szBuffer[TRACEMAXSTRING];
	va_list args;
	va_start(args,format);
	int nBuf;
	nBuf = _vsnprintf_s(szBuffer, TRACEMAXSTRING, format, args);
	va_end(args);
	_RPT0(_CRT_WARN, szBuffer);
}

