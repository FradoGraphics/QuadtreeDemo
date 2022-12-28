/**
 * Class name:    OpenGLVertexShader

 * Description:   class that implements the vertex shader interface 

 * Author:        Frank Dockhorn
 */


#include "stdafx.h"
#include "OpenGLShader.h"
#include "OpenGLVertexShader.h"



const char* OpenGLVertexShader::s_vertexShaderSource =
			"uniform MAT4 mvpMatrix;\n\
			\n\
			in  			VEC3 in_Position;\n\
			in 			vec4 in_Color;\n\
			flat out 	vec4 pass_Color;\n\
			\n\
			void main(void)\n\
			{\n\
			gl_Position	= vec4(mvpMatrix * VEC4(in_Position, 1.0));\n\
			pass_Color 	= in_Color;\n\
			}";

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

OpenGLVertexShader::OpenGLVertexShader(GLint shadertype)
:	OpenGLShader(shadertype)
{
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void	OpenGLVertexShader::loadSource()
{			
	m_source = s_vertexShaderSource;
	OpenGLShader::loadSource();
}
/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

const	GLchar* OpenGLVertexShader::printShaderInfoLog()
{
	m_finalInfoLog = "Vertex Shader Compiler Log:\n";
	return OpenGLShader::printShaderInfoLog();
}