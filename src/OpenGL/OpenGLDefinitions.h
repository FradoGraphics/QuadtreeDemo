/**
 * Header name:   OpenGLDefinitions.h

 * Description:   Keeps constants needed for various global opengl aspects.

 * Author:        Frank Dockhorn
 */



#ifndef	OPENGL_DEFINITIONS
#define	OPENGL_DEFINITIONS


//#define DOUBLE_PRECISION
#if defined DOUBLE_PRECISION
	typedef glm::dvec2	vec2;
	typedef glm::dvec3	vec3;
	typedef glm::dvec4	vec4;
	typedef glm::dmat4	mat4;
	typedef double			REAL;
	typedef PFNGLUNIFORMMATRIX4DVPROC	PFNGLUNIFORMMATRIX4VPROC;
	typedef PFNGLVERTEXATTRIB4DVPROC		PFNGLVERTEXATTRIB4VPROC;
	#define POLYGONMODE	GL_FRONT_AND_BACK
	#define GL_REAL		GL_DOUBLE
	#define PREC_SCAN		"%lf %lf"
	#define PREC_SCAN_2	"%lf"
	#define floatingPointPrecision	   s_doublePrecision
	#define glVertexAttrib4v				glVertexAttrib4dv
	#define glVertexAttrib4vStr			"glVertexAttrib4dv"
	#define glUniformMatrix4v				glUniformMatrix4dv
	#define glUniformMatrix4vStr			"glUniformMatrix4dv"
	#define VERTEXSHADER_NAME				"Shader_d.vert"  
	#define FRAGMENTSHADER_NAME			"Shader_d.frag"
	#define MAJOR_VERSION_GL 4
	#define MINOR_VERSION_GL 3
#else
	typedef glm::vec2		vec2;
	typedef glm::vec3		vec3;
	typedef glm::vec4		vec4;
	typedef glm::mat4		mat4;
	typedef float			REAL;	
	typedef PFNGLUNIFORMMATRIX4FVPROC	PFNGLUNIFORMMATRIX4VPROC;
	typedef PFNGLVERTEXATTRIB4FVPROC		PFNGLVERTEXATTRIB4VPROC;
	#define POLYGONMODE	GL_FRONT
	#define GL_REAL		GL_FLOAT
	#define PREC_SCAN		"%f %f"
	#define PREC_SCAN_2	"%f"
	#define floatingPointPrecision	   s_floatingPointPrecision
	#define glVertexAttrib4v				glVertexAttrib4fv
	#define glVertexAttrib4vStr			"glVertexAttrib4fv"
	#define glUniformMatrix4v				glUniformMatrix4fv
	#define glUniformMatrix4vStr			"glUniformMatrix4fv"
	#define VERTEXSHADER_NAME				"Shader.vert"  
	#define FRAGMENTSHADER_NAME			"Shader.frag"
	#define MAJOR_VERSION_GL 3
	#define MINOR_VERSION_GL 1
#endif

enum vaoAttributIndices
{
	VERTEX_POS_ATTRIB,
	VERTEX_COLOR_ATTRIB
};

#endif