/**
 * Class name:    OpenGLVertexShader

 * Description:   class that implements the vertex shader interface 

 * Author:        Frank Dockhorn
 */


#ifndef VERTEX_SHADER
#define VERTEX_SHADER

class OpenGLVertexShader : public OpenGLShader
{
public:
	OpenGLVertexShader				(GLint shadertype);
	virtual ~OpenGLVertexShader	() {}

	virtual	void				loadSource				();
	virtual	const	GLchar*	printShaderInfoLog	();
protected:
	static const char* s_vertexShaderSource;
};

#endif