/**
 * Class name:    OpenGLProgram

 * Description:   Shader program that keeps vertex and fragment shaders 

 * Author:        Frank Dockhorn
 */



#include "stdafx.h"
#include "OpenGLProgram.h"
#include "glext.h"
#include "OpenGLInterface.h"
#include "OpenGLShader.h"
#include "OpenGLVertexShader.h"
#include "OpenGLFragmentShader.h"


OpenGLProgram::OpenGLProgram()
:	m_id					(0), 
	m_mvpMatrixIndex	(0),
	m_infoLog			(""),
	m_internalLog		(NULL),
	m_vertexShader		(NULL),
	m_fragmentShader	(NULL)
{
	m_id = glCreateProgram();
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

OpenGLProgram::~OpenGLProgram()
{
	delete m_vertexShader;
	delete m_fragmentShader;
	delete[] m_internalLog;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void OpenGLProgram::cleanup()
{
	detachShader(m_vertexShader->getid());
	detachShader(m_fragmentShader->getid());
	glDeleteShader(m_vertexShader->getid());
	glDeleteShader(m_fragmentShader->getid());
	unbind();
	glDeleteProgram(m_id);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

bool OpenGLProgram::buildShaderProgram()
{
	m_vertexShader		= new OpenGLVertexShader(GL_VERTEX_SHADER);
	m_fragmentShader	= new OpenGLFragmentShader(GL_FRAGMENT_SHADER);

	m_vertexShader->loadSource();
	m_fragmentShader->loadSource();

	if (!m_vertexShader->compile())
	{
		m_infoLog = m_vertexShader->printShaderInfoLog();
		return false;
	}

	if (!m_fragmentShader->compile())
	{
		m_infoLog = m_fragmentShader->printShaderInfoLog();
		return false;
	}

	attachShader(m_vertexShader->getid());
	attachShader(m_fragmentShader->getid());

	// bind vertex attributes and fragment color output variable
	glBindAttribLocation(m_id, VERTEX_POS_ATTRIB, "in_Position");
	glBindAttribLocation(m_id, VERTEX_COLOR_ATTRIB, "in_Color");
	glBindFragDataLocation(m_id, 0, "out_Color");

	if (!link())
	{
		printProgramInfoLog();
		return false;
	}

	bind();
	setMVPMatrixIndex();
	return true;
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void OpenGLProgram::attachShader(GLuint shader_id)
{
	glAttachShader(m_id, shader_id);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void OpenGLProgram::detachShader(GLuint shader_id)
{
	glDetachShader(m_id, shader_id);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void OpenGLProgram::setMVPMatrixIndex()
{
	m_mvpMatrixIndex = glGetUniformLocation(m_id, "mvpMatrix");
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void OpenGLProgram::updateMVPMatrix(const mat4& mvp)
{
	glUniformMatrix4v(m_mvpMatrixIndex, 1, FALSE, &(mvp[0].x));
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void OpenGLProgram::bind()
{ 
    glUseProgram(m_id);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void OpenGLProgram::unbind() 
{
    glUseProgram(0);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

bool OpenGLProgram::link()
{
	GLint isLinked = 0;
	glLinkProgram(m_id);
	glGetProgramiv(m_id, GL_LINK_STATUS, &isLinked);
	if(isLinked == FALSE)
	{
		return false;
	}
	return true;	
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

const GLchar* OpenGLProgram::printProgramInfoLog()
{
	m_infoLog				= "Program Linker Log:\n";
	int	infoLogLen		= 0;
	int	charsWritten	= 0;	
	
	glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &infoLogLen);

	if (infoLogLen > 0)
	{
		delete[] m_internalLog;
		m_internalLog = new GLchar[infoLogLen];
		memset(m_internalLog, 0, infoLogLen);
		glGetProgramInfoLog(m_id, infoLogLen, &charsWritten, m_internalLog);
		if (charsWritten > 0)
		{
			TRACE_A("Program InfoLog : %s\n", m_internalLog);
		}		
	}	
	m_infoLog += m_internalLog;
	return m_infoLog.c_str();
}

