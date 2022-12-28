/**
 * Class name:    OpenGLFragmentShader

 * Description:   class that implements the fragment shader interface 

 * Author:        Frank Dockhorn
 */


#ifndef FRAGMENT_SHADER
#define FRAGMENT_SHADER

class OpenGLFragmentShader : public OpenGLShader
{
public:
	OpenGLFragmentShader				(GLint shadertype);
	virtual ~OpenGLFragmentShader	() {}

	virtual	void				loadSource				();
	virtual	const	GLchar*	printShaderInfoLog	();

protected:
	static const char* s_fragmentShaderSource;
};

#endif