
/**
 * Class name:    OpenGLProgram

 * Description:   Shader program that keeps vertex and fragment shaders 

 * Author:        Frank Dockhorn
 */


#ifndef _OPENGL_PROGRAM
#define _OPENGL_PROGRAM

class OpenGLShader;

class OpenGLProgram
{
public:
	OpenGLProgram								();
	~OpenGLProgram								();
	void				updateMVPMatrix		(const mat4& mvp);
	void				setMVPMatrixIndex		();
	void				attachShader			(GLuint shader_id);
	void				detachShader			(GLuint shader_id);
	void				bind						();
	void				unbind					();
	void				cleanup					();
	const GLchar*	printProgramInfoLog	();
	bool				link						();	
	bool				buildShaderProgram	();

	const char* getInfoLog()
	{
		return m_infoLog.c_str();
	}

private:
	OpenGLShader*	m_vertexShader;
	OpenGLShader*	m_fragmentShader;
	GLchar*			m_internalLog;
	astring			m_infoLog;
	GLint				m_mvpMatrixIndex;
	GLint				m_id;
};

#endif