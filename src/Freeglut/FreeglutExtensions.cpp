/**
 * Class name:		FreeglutExtensions

 * Description:   Loads needed Freeglut extension functions 

 * Author:        Frank Dockhorn
 */

#include "stdafx.h"
#include "freeglut_std.h"
#include "freeglut_ext.h"
#include "FreeglutExtensions.h"

HMODULE FreeglutExtensions::s_hFreeglutDll = NULL;

// Extension Function Pointers
#if defined FREEGLUT_RUNTIME_LINKING
pFuncGlutInit						glutInit						= NULL;			
pFuncGlutSetCursor				glutSetCursor				= NULL;
pFuncGlutDestroyWindow			glutDestroyWindow			= NULL;
pFuncGlutGetModifiers			glutGetModifiers			= NULL;
pFuncGlutMainLoop					glutMainLoop				= NULL;
pFuncGlutInitWindowPosition	glutInitWindowPosition	= NULL;
pFuncGlutInitWindowSize			glutInitWindowSize		= NULL;
pFuncGlutInitDisplayMode		glutInitDisplayMode		= NULL;
pFuncGlutSwapBuffers				glutSwapBuffers			= NULL;
pFuncGlutCreateWindow			glutCreateWindow			= NULL;
pFuncGlutInitContextVersion	glutInitContextVersion	= NULL;
pFuncGlutInitContextProfile	glutInitContextProfile	= NULL;
pFuncGlutSetOption				glutSetOption				= NULL;

pFuncGlutKeyboardFunc			glutKeyboardFunc			= NULL;
pFuncGlutSpecialFunc				glutSpecialFunc			= NULL;
pFuncGlutReshapeFunc				glutReshapeFunc			= NULL;
pFuncGlutMouseFunc				glutMouseFunc				= NULL;
pFuncGlutMotionFunc				glutMotionFunc				= NULL;
pFuncGlutDisplayFunc				glutDisplayFunc			= NULL;
pFuncGlutInitErrorFunc			glutInitErrorFunc			= NULL;
pFuncGlutPositionFunc			glutPositionFunc			= NULL;
pFuncGlutCloseFunc				glutCloseFunc				= NULL;



/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void FreeglutExtensions::freeLibrary()
{
	if (s_hFreeglutDll)
	{
		::FreeLibrary(s_hFreeglutDll);
	}	
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

bool FreeglutExtensions::loadLibrary()
{
	// load library

	s_hFreeglutDll = ::LoadLibrary(L"freeglut.dll");
	if (!s_hFreeglutDll)
	{
		return false;
	}
	// Get pointers To freeglut functions
	glutInit						= (pFuncGlutInit)
										GetProcAddress(s_hFreeglutDll, "glutInit");
	if (!glutInit)
		return false;

	glutSetCursor				= (pFuncGlutSetCursor)
										GetProcAddress(s_hFreeglutDll, "glutSetCursor");
	if (!glutSetCursor)
		return false;

	glutDestroyWindow			= (pFuncGlutDestroyWindow)
										GetProcAddress(s_hFreeglutDll, "glutDestroyWindow");
	if (!glutDestroyWindow)
		return false;

	glutGetModifiers			= (pFuncGlutGetModifiers)
										GetProcAddress(s_hFreeglutDll, "glutGetModifiers");
	if (!glutGetModifiers)
		return false;

	glutMainLoop				= (pFuncGlutMainLoop)
										GetProcAddress(s_hFreeglutDll, "glutMainLoop");
	if (!glutMainLoop)
		return false;

	glutInitWindowPosition	= (pFuncGlutInitWindowPosition)
										GetProcAddress(s_hFreeglutDll, "glutInitWindowPosition");
	if (!glutInitWindowPosition)
		return false;

	glutInitWindowSize		= (pFuncGlutInitWindowSize)
										GetProcAddress(s_hFreeglutDll, "glutInitWindowSize");
	if (!glutInitWindowSize)
		return false;

	glutInitDisplayMode		= (pFuncGlutInitDisplayMode)
										GetProcAddress(s_hFreeglutDll, "glutInitDisplayMode");
	if (!glutInitDisplayMode)
		return false;

	glutSwapBuffers			= (pFuncGlutSwapBuffers)
										GetProcAddress(s_hFreeglutDll, "glutSwapBuffers");
	if (!glutSwapBuffers)
		return false;

	glutCreateWindow			= (pFuncGlutCreateWindow)
										GetProcAddress(s_hFreeglutDll, "glutCreateWindow");
	if (!glutCreateWindow)
		return false;

	glutKeyboardFunc			= (pFuncGlutKeyboardFunc)
										GetProcAddress(s_hFreeglutDll, "glutKeyboardFunc");
	if (!glutKeyboardFunc)
		return false;

	glutSpecialFunc			= (pFuncGlutSpecialFunc)
										GetProcAddress(s_hFreeglutDll, "glutSpecialFunc");
	if (!glutSpecialFunc)
		return false;

	glutReshapeFunc			= (pFuncGlutReshapeFunc)
										GetProcAddress(s_hFreeglutDll, "glutReshapeFunc");
	if (!glutReshapeFunc)
		return false;

	glutMouseFunc				= (pFuncGlutMouseFunc)
										GetProcAddress(s_hFreeglutDll, "glutMouseFunc");
	if (!glutMouseFunc)
		return false;

	glutMotionFunc				= (pFuncGlutMotionFunc)
										GetProcAddress(s_hFreeglutDll, "glutMotionFunc");
	if (!glutMotionFunc)
		return false;

	glutDisplayFunc			= (pFuncGlutDisplayFunc)
										GetProcAddress(s_hFreeglutDll, "glutDisplayFunc");
	if (!glutDisplayFunc)
		return false;

	glutInitErrorFunc			= (pFuncGlutInitErrorFunc)
										GetProcAddress(s_hFreeglutDll, "glutInitErrorFunc");
	if (!glutInitErrorFunc)
		return false;

	glutPositionFunc			= (pFuncGlutPositionFunc)
										GetProcAddress(s_hFreeglutDll, "glutPositionFunc");
	if (!glutPositionFunc)
		return false;

	glutCloseFunc				= (pFuncGlutCloseFunc)
										GetProcAddress(s_hFreeglutDll, "glutCloseFunc");
	if (!glutCloseFunc)
		return false;

	glutInitContextVersion	= (pFuncGlutInitContextVersion)
										GetProcAddress(s_hFreeglutDll, "glutInitContextVersion");
	if (!glutInitContextVersion)
		return false;

	glutInitContextProfile	= (pFuncGlutInitContextProfile)
										GetProcAddress(s_hFreeglutDll, "glutInitContextProfile");
	if (!glutInitContextProfile)
		return false;

	glutSetOption				= (pFuncGlutSetOption)
										GetProcAddress(s_hFreeglutDll, "glutSetOption");
	if (!glutSetOption)
		return false;

	return true;
}
#endif

