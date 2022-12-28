/**
 * Class name:		OpenGLExetnsions

 * Description:   Loads needed OpenGL extension functions 

 * Author:        Frank Dockhorn
 */

#include "stdafx.h"
#include "OpenGLExtensions.h"

typedef HGLRC (WINAPI *pFuncWglGetCurrentContext)	(VOID);
typedef PROC  (WINAPI *pFuncWglGetProcAddress)		(LPCSTR);


PROC WINAPI OpenGLExtensions::wglGetProcAddress(LPCSTR lpszProc)
{
	pFuncWglGetProcAddress pGetProcAddress;
	HMODULE hOpenGl = GetModuleHandle(L"opengl32.dll");
	if (hOpenGl)
	{
		pGetProcAddress = (pFuncWglGetProcAddress)::GetProcAddress(hOpenGl, "wglGetProcAddress");
		if (pGetProcAddress)
		{
			return pGetProcAddress(lpszProc);
		}
	}
	return NULL;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

HGLRC WINAPI OpenGLExtensions::wglGetCurrentContext()
{
	pFuncWglGetCurrentContext pGetContext;
	HMODULE hOpenGl = GetModuleHandle(L"opengl32.dll");
	if (hOpenGl)
	{
		pGetContext = (pFuncWglGetCurrentContext)::GetProcAddress(hOpenGl, "wglGetCurrentContext");
		if (pGetContext)
		{
			return pGetContext();
		}
	}
	return NULL;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

#if defined FREEGLUT_RUNTIME_LINKING

pFuncGlDrawArrays		glDrawArrays	= NULL;
pFuncGlLineWidth		glLineWidth		= NULL;
pFuncGlGetIntegerv	glGetIntegerv	= NULL;
pFuncGlDepthRange		glDepthRange	= NULL;
pFuncGlDepthFunc		glDepthFunc		= NULL;
pFuncGlEnable			glEnable			= NULL;
pFuncGlViewport		glViewport		= NULL;
pFuncGlPolygonMode	glPolygonMode	= NULL;
pFuncGlClearColor		glClearColor	= NULL;
pFuncGlClear			glClear			= NULL;
pFuncGlDrawElements	glDrawElements = NULL;
pFuncGlPointSize		glPointSize		= NULL;
pFuncGlDisable			glDisable		= NULL;	
pFuncGlBlendFunc		glBlendFunc		= NULL;
pFuncGlGetString		glGetString    = NULL;

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

bool OpenGLExtensions::initCoreFunctions()
{
	HMODULE hOpenGl = GetModuleHandle(L"opengl32.dll");
	if (!hOpenGl)
		return false;

	glDrawArrays				= (pFuncGlDrawArrays)GetProcAddress(hOpenGl, "glDrawArrays");
	if (!glDrawArrays)
		return false;

	glLineWidth					= (pFuncGlLineWidth)GetProcAddress(hOpenGl, "glLineWidth");
	if (!glLineWidth)
		return false;

	glGetIntegerv				= (pFuncGlGetIntegerv)GetProcAddress(hOpenGl, "glGetIntegerv");
	if (!glGetIntegerv)
		return false;   

	glDepthRange				= (pFuncGlDepthRange)GetProcAddress(hOpenGl, "glDepthRange");
	if (!glDepthRange)
		return false;

	glDepthFunc					= (pFuncGlDepthFunc)GetProcAddress(hOpenGl, "glDepthFunc");
	if (!glDepthFunc)
		return false;

	glEnable						= (pFuncGlEnable)GetProcAddress(hOpenGl, "glEnable");
	if (!glEnable)
		return false;

	glViewport					= (pFuncGlViewport)GetProcAddress(hOpenGl, "glViewport");
	if (!glViewport)
		return false;

	glPolygonMode				= (pFuncGlPolygonMode)GetProcAddress(hOpenGl, "glPolygonMode");
	if (!glPolygonMode)
		return false;

	glGetString					= (pFuncGlGetString)GetProcAddress(hOpenGl, "glGetString");
	if (!glGetString)
		return false;

	glClearColor				= (pFuncGlClearColor)GetProcAddress(hOpenGl, "glClearColor");
	if (!glClearColor)
		return false;

	glClear						= (pFuncGlClear)GetProcAddress(hOpenGl, "glClear");
	if (!glClear)
		return false;

	glDrawElements				= (pFuncGlDrawElements)GetProcAddress(hOpenGl, "glDrawElements");
	if (!glDrawElements)
		return false;

	glPointSize					= (pFuncGlPointSize)GetProcAddress(hOpenGl, "glPointSize");
	if (!glPointSize)
		return false;

	glDisable					= (pFuncGlDisable)GetProcAddress(hOpenGl, "glDisable");
	if (!glDisable)
		return false;

	glBlendFunc					= (pFuncGlBlendFunc)GetProcAddress(hOpenGl, "glBlendFunc");
	if (!glBlendFunc)
		return false;

	return true;
}
#endif

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

// Extension Function Pointers
PFNGLGENBUFFERSPROC						glGenBuffers					= NULL;			// VBO Name Generation Procedure
PFNGLBINDBUFFERPROC						glBindBuffer					= NULL;			// VBO Bind Procedure
PFNGLBUFFERDATAPROC						glBufferData					= NULL;			// VBO Data Loading Procedure
PFNGLDELETEBUFFERSPROC					glDeleteBuffers				= NULL;			// VBO Deletion Procedure
PFNGLGETSTRINGIPROC						glGetStringi					= NULL;			
PFNGLCREATESHADERPROC					glCreateShader					= NULL;
PFNGLSHADERSOURCEPROC					glShaderSource					= NULL;
PFNGLCOMPILESHADERPROC					glCompileShader				= NULL;	
PFNGLGETSHADERIVPROC						glGetShaderiv					= NULL;
PFNGLGETSHADERINFOLOGPROC				glGetShaderInfoLog			= NULL;
PFNGLDELETESHADERPROC					glDeleteShader					= NULL;
PFNGLCREATEPROGRAMPROC					glCreateProgram				= NULL;
PFNGLATTACHSHADERPROC					glAttachShader					= NULL;
PFNGLGETUNIFORMLOCATIONPROC			glGetUniformLocation			= NULL;
PFNGLUNIFORMMATRIX4VPROC				glUniformMatrix4v				= NULL;
PFNGLUSEPROGRAMPROC						glUseProgram					= NULL;
PFNGLBINDATTRIBLOCATIONPROC			glBindAttribLocation			= NULL;
PFNGLBINDFRAGDATALOCATIONPROC			glBindFragDataLocation		= NULL;
PFNGLLINKPROGRAMPROC						glLinkProgram					= NULL;
PFNGLGETPROGRAMIVPROC					glGetProgramiv					= NULL;
PFNGLGETPROGRAMINFOLOGPROC				glGetProgramInfoLog			= NULL;
PFNGLDETACHSHADERPROC					glDetachShader					= NULL;
PFNGLGENVERTEXARRAYSPROC				glGenVertexArrays				= NULL;
PFNGLBINDVERTEXARRAYPROC				glBindVertexArray				= NULL;
PFNGLVERTEXATTRIBPOINTERPROC			glVertexAttribPointer		= NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC		glEnableVertexAttribArray	= NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC	glDisableVertexAttribArray = NULL;
PFNGLDELETEVERTEXARRAYSPROC			glDeleteVertexArrays			= NULL;
PFNGLVERTEXATTRIB4VPROC					glVertexAttrib4v				= NULL;
PFNGLDELETEPROGRAMPROC					glDeleteProgram				= NULL;
PFNGLVERTEXATTRIBLPOINTERPROC			glVertexAttribLPointer		= NULL;


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


bool OpenGLExtensions::initExtensions()
{
	// Get Pointers To The GL Functions
	glGenBuffers				= (PFNGLGENBUFFERSPROC)				wglGetProcAddress("glGenBuffers");
	if (!glGenBuffers)
		return false;

	glBindBuffer				= (PFNGLBINDBUFFERPROC)				wglGetProcAddress("glBindBuffer");
	if (!glBindBuffer)
		return false;

	glBufferData				= (PFNGLBUFFERDATAPROC)				wglGetProcAddress("glBufferData");
	if (!glBufferData)
		return false;   

	glDeleteBuffers			= (PFNGLDELETEBUFFERSPROC)			wglGetProcAddress("glDeleteBuffers");
	if (!glDeleteBuffers)
		return false;

	glGetStringi				= (PFNGLGETSTRINGIPROC)				wglGetProcAddress("glGetStringi");
	if (!glGetStringi)
		return false;

	glCreateShader				= (PFNGLCREATESHADERPROC)			wglGetProcAddress("glCreateShader");
	if (!glCreateShader)
		return false;

	glShaderSource				= (PFNGLSHADERSOURCEPROC)			wglGetProcAddress("glShaderSource");
	if (!glShaderSource)
		return false;

	glCompileShader			= (PFNGLCOMPILESHADERPROC)			wglGetProcAddress("glCompileShader");
	if (!glCompileShader)
		return false;

	glGetShaderiv				= (PFNGLGETSHADERIVPROC)			wglGetProcAddress("glGetShaderiv");
	if (!glGetShaderiv)
		return false;

	glGetShaderInfoLog		= (PFNGLGETSHADERINFOLOGPROC)		wglGetProcAddress("glGetShaderInfoLog");
	if (!glGetShaderInfoLog)
		return false;

	glDeleteShader				= (PFNGLDELETESHADERPROC)			wglGetProcAddress("glDeleteShader");
	if (!glDeleteShader)
		return false;

	glCreateProgram			= (PFNGLCREATEPROGRAMPROC)			wglGetProcAddress("glCreateProgram");
	if (!glCreateProgram)
		return false;

	glAttachShader				= (PFNGLATTACHSHADERPROC)			wglGetProcAddress("glAttachShader");
	if (!glAttachShader)
		return false;

	glGetUniformLocation		= (PFNGLGETUNIFORMLOCATIONPROC)	wglGetProcAddress("glGetUniformLocation");
	if (!glGetUniformLocation)
		return false;

	glUniformMatrix4v			= (PFNGLUNIFORMMATRIX4VPROC)		wglGetProcAddress(glUniformMatrix4vStr);
	if (!glUniformMatrix4v)
		return false;

	glUseProgram				= (PFNGLUSEPROGRAMPROC)				wglGetProcAddress("glUseProgram");
	if (!glUseProgram)
		return false;

	glBindAttribLocation		= (PFNGLBINDATTRIBLOCATIONPROC)	wglGetProcAddress("glBindAttribLocation");
	if (!glBindAttribLocation)
		return false;

	glBindFragDataLocation	= (PFNGLBINDFRAGDATALOCATIONPROC)wglGetProcAddress("glBindFragDataLocation");
	if (!glBindFragDataLocation)
		return false;

	glLinkProgram				= (PFNGLLINKPROGRAMPROC)			wglGetProcAddress("glLinkProgram");
	if (!glLinkProgram)
		return false;

	glGetProgramiv				= (PFNGLGETPROGRAMIVPROC)			wglGetProcAddress("glGetProgramiv");
	if (!glGetProgramiv)
		return false;

	glGetProgramInfoLog		= (PFNGLGETPROGRAMINFOLOGPROC)	wglGetProcAddress("glGetProgramInfoLog");
	if (!glGetProgramInfoLog)
		return false;

	glDetachShader				= (PFNGLDETACHSHADERPROC)			wglGetProcAddress("glDetachShader");
	if (!glDetachShader)
		return false;

	glGenVertexArrays			= (PFNGLGENVERTEXARRAYSPROC)		wglGetProcAddress("glGenVertexArrays");
	if (!glGenVertexArrays)
		return false;

	glBindVertexArray			= (PFNGLBINDVERTEXARRAYPROC)		wglGetProcAddress("glBindVertexArray");
	if (!glBindVertexArray)
		return false;

	glVertexAttribPointer	= (PFNGLVERTEXATTRIBPOINTERPROC)	wglGetProcAddress("glVertexAttribPointer");
	if (!glVertexAttribPointer)
		return false;

	glEnableVertexAttribArray	= (PFNGLENABLEVERTEXATTRIBARRAYPROC) wglGetProcAddress("glEnableVertexAttribArray");
	if (!glEnableVertexAttribArray)
		return false;

	glDisableVertexAttribArray	= (PFNGLDISABLEVERTEXATTRIBARRAYPROC) wglGetProcAddress("glDisableVertexAttribArray");
	if (!glDisableVertexAttribArray)
		return false;

	glDeleteVertexArrays		= (PFNGLDELETEVERTEXARRAYSPROC) wglGetProcAddress("glDeleteVertexArrays");
	if (!glDeleteVertexArrays)
		return false;

	glVertexAttrib4v			= (PFNGLVERTEXATTRIB4VPROC) wglGetProcAddress(glVertexAttrib4vStr);
	if (!glVertexAttrib4v)
		return false;

	glDeleteProgram			= (PFNGLDELETEPROGRAMPROC) wglGetProcAddress("glDeleteProgram");
	if (!glDeleteProgram)
		return false;
#if defined DOUBLE_PRECISION
	glVertexAttribLPointer	= (PFNGLVERTEXATTRIBLPOINTERPROC) wglGetProcAddress("glVertexAttribLPointer");
	if (!glVertexAttribLPointer)
		return false;
#endif

	return true;
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

bool OpenGLExtensions::isExtensionSupported(char* szTargetExtension)
{
	GLint n = 0; 
	glGetIntegerv(GL_NUM_EXTENSIONS, &n); 
	for (GLint i = 0; i < n; i++) 
	{ 
		const char* extension = (const char*)glGetStringi(GL_EXTENSIONS, i);
		if (!strcmp(szTargetExtension, extension)) 
		{
			return true;
		}
	}
	return false;
}


