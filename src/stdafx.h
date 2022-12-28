// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifdef USE_TARGETVERSION
// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER USE_TARGETVERSION		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT USE_TARGETVERSION	// Change this to the appropriate value to target other versions of Windows.
#endif						
#else
#include <sdkddkver.h>
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define FREEGLUT_LIB_PRAGMAS 1	// Use freeglut API as shared library (DLL)

#define _CRTDBG_MAP_ALLOC
#define _CRT_RAND_S

//#define FREEGLUT_RUNTIME_LINKING

#include <stdlib.h>
#include <crtdbg.h>

// Windows Header Files:
#include <windows.h>
#include <atlbase.h>
#include <atlwin.h>
#include <atlstr.h>

#include <stdio.h>
#include <tchar.h>
#include <vector>
#include <map>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <string>
#include <commctrl.h>
#include <Commdlg.h>
#include <ShellAPI.h>

#define GLM_FORCE_RADIANS

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "OpenGLInterface.h"
#include "Vector.h"
#include "trace.h"


#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __LOC__ __FILE__ "("__STR1__(__LINE__)") : Warning:"

#define	MAX_BUFSIZE		10
#define	MAX_BUFSIZE_2	20

#if defined _DEBUG_RELEASE
#pragma message (__LOC__" _DEBUG_RELEASE macro is still enabled.")
#endif

using namespace std;
typedef vector<size_t> IntArray;
typedef vector<vec2> PointArray;
typedef vector<vec2*> PointPtrArray;
typedef std::basic_string<TCHAR> tstring;
typedef std::basic_string<CHAR> astring;
typedef std::basic_ostringstream<TCHAR , std::char_traits<TCHAR> > tstringstream;
typedef std::basic_ostringstream<CHAR , std::char_traits<CHAR> > astringstream;


// conversion from 'double' to 'float', possible loss of data
#pragma warning( disable : 4244 )
//'initializing' : truncation from 'double' to 'const REAL'
#pragma warning( disable : 4305 )
// forcing value to bool 'true' or 'false' (performance warning)
#pragma warning( disable : 4800 )

// TODO: reference additional headers your program requires here

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

