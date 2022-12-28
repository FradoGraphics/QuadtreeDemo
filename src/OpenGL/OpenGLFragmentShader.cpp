/**
 * Class name:    OpenGLFragmentShader

 * Description:   class that implements the fragment shader interface 

 * Author:        Frank Dockhorn
 */


#include "stdafx.h"
#include "OpenGLShader.h"
#include "OpenGLFragmentShader.h"


const char* OpenGLFragmentShader::s_fragmentShaderSource =
			"flat in  	vec4 pass_Color;\n\
			out 			vec4 out_Color;\n\
			\n\
			void main(void)\n\
			{\n\
				out_Color = pass_Color;\n\
			}\n";



/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

OpenGLFragmentShader::OpenGLFragmentShader(GLint shadertype)
:	OpenGLShader(shadertype)
{
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


void	OpenGLFragmentShader::loadSource()
{			
	m_source = s_fragmentShaderSource;
	OpenGLShader::loadSource();
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

const	GLchar* OpenGLFragmentShader::printShaderInfoLog()
{
	m_finalInfoLog = "Fragment Shader Compiler Log:\n";
	return OpenGLShader::printShaderInfoLog();
}