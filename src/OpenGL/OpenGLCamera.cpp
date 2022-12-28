/**
 * Class name:    OpenGLCamera

 * Description:	Manages the view by dealing with model view projection matrix (MVP)   

 * Author:        Frank Dockhorn
 */

#include "stdafx.h"
#include "OpenGLCamera.h"

OpenGLCamera::OpenGLCamera() 
:	m_cameraPos	(0.0, 0.0, 1.0),
	m_nearPlane	(0.1),
	m_farPlane	(10)
{
	memset(m_viewport, 0, 4*sizeof(GLint));
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

const mat4& OpenGLCamera::updateMVPMatrix()
{	
	m_modelView	= m_matrices[VIEW] * m_matrices[MODEL];
	m_MVP			= m_matrices[PROJECTION] * m_modelView;	

	return m_MVP;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

const mat4& OpenGLCamera::updateCamera(const vec3& cameraPos)
{
	if (cameraPos.z < m_nearPlane || cameraPos.z > m_farPlane)
	{
		// ignore positions outside the viewing area
		return m_MVP;
	}
	m_cameraPos			=	cameraPos;
	m_matrices[VIEW]	=	glm::lookAt(cameraPos,
								vec3(cameraPos[0], cameraPos[1], cameraPos[2] - 1),
								vec3(0.0, 1.0, 0.0));

	return updateMVPMatrix();
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

bool OpenGLCamera::validateCameraPosition(vec3& cameraPos)
{
	// ensure that the mesh is within near and far clipping plane
	REAL z				= cameraPos.z;
	REAL eps				= 1e-3;
	REAL offsetNear	= m_nearPlane * eps;
	REAL offsetFar		= (m_farPlane - m_nearPlane) * eps;
	REAL limitNear		= m_nearPlane + offsetNear;
	REAL limitFar		= m_farPlane - offsetFar;
	
	if (cameraPos.z < limitNear)
	{
		cameraPos.z = limitNear;
	}
	else if (cameraPos.z > limitFar)
	{
		cameraPos.z = limitFar;
	}
	return z != cameraPos.z;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

const mat4& OpenGLCamera::setPerspective(LONG width, LONG height)
{
	REAL ratio		= (REAL)width / height;
	m_viewport[0]	= 0;
	m_viewport[1]	= 0;
	m_viewport[2]	= width;
	m_viewport[3]	= height;	

	m_matrices[PROJECTION]	= glm::perspective(glm::radians((REAL)60.0), ratio, 
															m_nearPlane, m_farPlane);
	glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
	return updateMVPMatrix();
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void OpenGLCamera::updateClippingPlanes(REAL distance)
{
	REAL distNear	= distance * 1e-3; 
	m_nearPlane		= distNear < 0.1 ? distNear : 0.1;
	m_farPlane		= (distance * 6.0);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void OpenGLCamera::computePointOnPlanes(int x, int y, 
														vec3& nearPlane, 
														vec3& farPlane)														
{
	vec4 viewPortVec(m_viewport[0], m_viewport[1],
		m_viewport[2], m_viewport[3]);

	y = m_viewport[1] + m_viewport[3] - y;

	nearPlane	= glm::unProject(vec3(x, y, 0), 
											m_modelView, 
											m_matrices[PROJECTION], 
											viewPortVec);
	farPlane		= glm::unProject(vec3(x, y, 1), 
											m_modelView, 
											m_matrices[PROJECTION], 
											viewPortVec);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

bool OpenGLCamera::computeCursorPos(int x, int y, vec2& cursorPos)
{
	// This function computes object coordinates from (x,y) screen coordinates
	// and saves them in the outgoing parameter cursorPos.
	bool	isInsideFrustum = true;   	
	vec3	nearPlane;
	vec3	farPlane;

	computePointOnPlanes(x, y, nearPlane, farPlane);
	// We compute parameter t based on mesh and clipping plane distances
	// along the z-axis. Parameter t is to be used for linear interpolation
	// later on.
	GLdouble t = (0 - nearPlane[2]) / (farPlane[2] - nearPlane[2]);

	if (t < 0.0 || t > 1.0)
	{
		// Point is outside viewing frustum.
		isInsideFrustum = false;
	}
	// The (x,y) object coordinates are linearly interpolated 
	// between near and far clipping planes based on the previously 
	// computed parameter t.
	cursorPos.x = nearPlane[0] + (farPlane[0] - nearPlane[0]) * t;
	cursorPos.y = nearPlane[1] + (farPlane[1] - nearPlane[1]) * t;   
	return isInsideFrustum;
}

