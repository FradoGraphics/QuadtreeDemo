/**
 * Header name:   OpenGLInterface

 * Description:   Header keeps all external OpenGL extension references 

 * Author:        Frank Dockhorn
 */


#ifndef OPENGL_INTERFACE
#define OPENGL_INTERFACE

#include <GL/glu.h>	
#include <GL/gl.h>	

#include "glext.h"
#include "OpenGLDefinitions.h"

extern PFNGLCREATESHADERPROC					glCreateShader;
extern PFNGLCOMPILESHADERPROC					glCompileShader;
extern PFNGLSHADERSOURCEPROC					glShaderSource;
extern PFNGLGENBUFFERSPROC						glGenBuffers;			// VBO Name Generation Procedure
extern PFNGLBINDBUFFERPROC						glBindBuffer;			// VBO Bind Procedure
extern PFNGLBUFFERDATAPROC						glBufferData;			// VBO Data Loading Procedure
extern PFNGLDELETEBUFFERSPROC					glDeleteBuffers;		// VBO Deletion Procedure
extern PFNGLGETSTRINGIPROC						glGetStringi;
extern PFNGLGETSHADERIVPROC					glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC				glGetShaderInfoLog;
extern PFNGLDELETESHADERPROC					glDeleteShader;
extern PFNGLCREATEPROGRAMPROC					glCreateProgram;
extern PFNGLATTACHSHADERPROC					glAttachShader;
extern PFNGLGETUNIFORMLOCATIONPROC			glGetUniformLocation;
extern PFNGLUNIFORMMATRIX4VPROC				glUniformMatrix4v;
extern PFNGLUSEPROGRAMPROC						glUseProgram;
extern PFNGLBINDATTRIBLOCATIONPROC			glBindAttribLocation;
extern PFNGLBINDFRAGDATALOCATIONPROC		glBindFragDataLocation;

extern PFNGLLINKPROGRAMPROC					glLinkProgram;
extern PFNGLGETPROGRAMIVPROC					glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC			glGetProgramInfoLog;
extern PFNGLDETACHSHADERPROC					glDetachShader;
extern PFNGLGENVERTEXARRAYSPROC				glGenVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC				glBindVertexArray;
extern PFNGLVERTEXATTRIBPOINTERPROC			glVertexAttribPointer;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC	glEnableVertexAttribArray;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC	glDisableVertexAttribArray;
extern PFNGLDELETEVERTEXARRAYSPROC			glDeleteVertexArrays;
extern PFNGLVERTEXATTRIB4VPROC				glVertexAttrib4v;
extern PFNGLDELETEPROGRAMPROC					glDeleteProgram;
extern PFNGLVERTEXATTRIBLPOINTERPROC		glVertexAttribLPointer;


#endif