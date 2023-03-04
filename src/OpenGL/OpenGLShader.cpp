/**
 * Class name:    OpenGLShader

 * Description:   class that implements the shader interface 

 * Author:        Frank Dockhorn
 */



#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "OpenGLShader.h"
#include "OpenGLInterface.h"


const char* OpenGLShader::s_doublePrecision =
			"#version 430\n\
			#define VEC3 dvec3\n\
			#define VEC4 dvec4\n\
			#define MAT4 dmat4\n\
			";

const char* OpenGLShader::s_floatingPointPrecision =
			"#version 150\n\
			precision highp float;\n\
			#define VEC3 vec3\n\
			#define VEC4 vec4\n\
			#define MAT4 mat4\n\
			";


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


OpenGLShader::OpenGLShader(GLint shadertype, const char* filename)
:	m_loadError		(false),
	m_filename		(filename),
	m_internalLog	(NULL),
	m_id				(0),
	m_type			(0),
	m_source			(""),
	m_finalInfoLog	("")
{ 	
	m_id = glCreateShader(shadertype);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

OpenGLShader::OpenGLShader(GLint shadertype)
:	m_loadError		(false),
	m_filename		(NULL),
	m_internalLog	(NULL),
	m_id				(0),
	m_type			(0),
	m_source			(""),
	m_finalInfoLog	("")
{ 	
	m_id = glCreateShader(shadertype);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

OpenGLShader::~OpenGLShader()
{
	delete[] m_internalLog;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


bool OpenGLShader::load()
{
	int	size;	

	const char* source = loadFile(size);
	if (m_loadError || source == NULL)
	{
		return false;
	}
	glShaderSource(m_id, 1, &source, &size);

	delete[] source;
	return true;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


void OpenGLShader::loadSource()
{
	string dest		 = floatingPointPrecision;
	dest				+= m_source; 
	int	len		 = (int)dest.length();
	const char* s   = dest.c_str();
	glShaderSource(m_id, 1, &s, &len);		
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


char* OpenGLShader::loadFile(int& size)
{	
	string	text;
	char*		buffer = NULL;
	
	ifstream file (m_filename, ios::in|ios::binary|ios::ate);
	if (file.is_open())
	{		
		size		= file.tellg();
		buffer	= new char[size];
		file.seekg(0, ios::beg);
		file.read(buffer, size);
		file.close();
	}
	else
	{		
		TRACE(_T("Unable to open file %s\n"), m_filename);
		m_loadError = true;
	}	
	return buffer;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

bool OpenGLShader::compile()
{
	GLint isCompiled = 0;
	glCompileShader(m_id);
	glGetShaderiv(m_id, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == FALSE)
	{
		return false;
	}
	return true;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

const GLchar* OpenGLShader::printShaderInfoLog()
{
	int		infoLogLen		= 0;
	int		charsWritten	= 0;

	glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &infoLogLen);

	if (infoLogLen > 0)
	{
		delete[] m_internalLog;
		m_internalLog = new GLchar[infoLogLen];		
		memset(m_internalLog, 0, infoLogLen);
		glGetShaderInfoLog(m_id, infoLogLen, &charsWritten, m_internalLog);
		if (charsWritten > 0)
		{
			TRACE_A("Shader InfoLog : %s\n", m_internalLog);
		}				
	}
	m_finalInfoLog += m_internalLog;
	return m_finalInfoLog.c_str();
}
