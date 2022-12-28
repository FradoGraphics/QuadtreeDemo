/**
 * Class name:    OpenGLCamera

 * Description:	Manages the view by dealing with model view projection matrix (MVP)   

 * Author:        Frank Dockhorn
 */

#ifndef _OPENGL_CAMERA
#define _OPENGL_CAMERA

class OpenGLCamera
{
public:

	enum MATRIX_TYPE
	{
		MODEL,
		VIEW,
		PROJECTION,		
		NUM_MATRICES
	};
	OpenGLCamera	();
	~OpenGLCamera	() {};

	vec3					getCameraPos				() {return m_cameraPos;}
	const mat4&			updateMVPMatrix			();
	const mat4&			updateCamera				(const vec3& cameraPos);
	const mat4&			setPerspective				(LONG width, LONG height);
	bool					computeCursorPos			(int x, int y, vec2& cursorPos);
	bool					validateCameraPosition	(vec3& cameraPos);
	void					computePointOnPlanes		(int x, int y, vec3& nearPlane, 
																	vec3& farPlane);	
	void					updateClippingPlanes		(REAL distance);

private:
	GLint	m_viewport[4];
	REAL	m_nearPlane;
	REAL	m_farPlane;
	mat4	m_MVP;
	mat4	m_modelView;
	mat4	m_matrices[NUM_MATRICES]; 
	vec3	m_cameraPos;

};

#endif