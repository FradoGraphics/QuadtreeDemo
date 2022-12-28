/**
 * Class name:		OpenGLExetnsions

 * Description:   Loads needed OpenGL extension functions 

 * Author:        Frank Dockhorn
 */


#ifndef OPENGL_EXTENSIONS
#define OPENGL_EXTENSIONS


class OpenGLExtensions
{
public:
	static	bool				initExtensions			();
	static	bool				isExtensionSupported	(char* szTargetExtension);
	static	bool				initCoreFunctions		();
	static	PROC WINAPI		wglGetProcAddress		(LPCSTR lpszProc);
	static	HGLRC WINAPI	wglGetCurrentContext	();
};

#endif
