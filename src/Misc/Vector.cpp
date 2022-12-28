/**
 * Class name:    Vec2D

 * Description:   Defines class members and vector operations.

 * Author:			Frank Dockhorn
 */


#include "stdafx.h"
#include "Vector.h"

/*
	Generally we are seeking the barycentric coordinates
	s, v, and 1-s-v based on the following equation:
	(1-s-v) * P0 + s * P1 + v * P2 = P.	
	Where P0 to P2 are the triangle vertices.
	Rearranging the above equation gives:
	P0 + s * (P1-P0) + v * (P2-P0) = P. 
*/

bool Vec2D::isPointInTriangle(const vec2& v1, 
										const vec2& v2, 
										const vec2& v3) const
{
	vec2 diff1 = v2 - v1;
	vec2 diff2 = v3 - v1;	
	vec2 diff3 = *this - v1;

	vec2 cross1, cross2;
	// cross1 = crossproduct of (0, 0, 1) and diff1
	cross1.x = -diff1.y;
	cross1.y = diff1.x;
	// cross2 = crossproduct of (0, 0, 1) and diff2
	cross2.x = -diff2.y;
	cross2.y = diff2.x;

	// linear combination
	// diff3 = s * diff1 + v * diff2
	// multiply both sides of the equation with cross1 and cross2
	// and solve for s and v respectively
	// s = diff3 * cross2 / (diff1 * cross2)
	// v = diff3 * cross1 / (diff2 * cross1)	

	REAL dot1 = glm::dot(diff1, cross2);
	REAL dot2 = glm::dot(diff2, cross1);

	if (dot1 != 0 && dot2 != 0)
	{	
		REAL s = glm::dot(diff3, cross2) / dot1;
		REAL v = glm::dot(diff3, cross1) / dot2;

		if (s >= 0 && v >= 0 && ((s + v) <= 1))
		{
			// point is inside or on the edge of this triangle
			return true;
		}
	}
	return false;
}


