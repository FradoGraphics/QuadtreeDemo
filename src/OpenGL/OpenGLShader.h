/**
 * Class name:    OpenGLShader

 * Description:   class that implements the shader interface 

 * Author:        Frank Dockhorn
 */


#ifndef OPENGL_SHADER
#define OPENGL_SHADER

class OpenGLShader
{
public:
	OpenGLShader				(GLint shadertype, const char*);
	OpenGLShader				(GLint shadertype);
	~OpenGLShader				();

	bool							load						();	
	bool							compile					();
	virtual	void				loadSource				();
	virtual	const	GLchar*	printShaderInfoLog	();

	GLuint getid()
	{
		return m_id;
	}

protected:
	astring					m_finalInfoLog;
	const char*				m_source;
	char*						loadFile(int& size);

private:
	GLuint					m_id;
	GLint						m_type;
	GLchar*					m_internalLog;
	const char*				m_filename;	
	bool						m_loadError;	

	static const char*	s_floatingPointPrecision;
	static const char*	s_doublePrecision;
};

#endif