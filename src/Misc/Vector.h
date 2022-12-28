/**
 * Class name:    Vec2D

 * Description:   Defines class members and vector operations.

 * Author:			Frank Dockhorn
 */


#ifndef _VECTOR2D
#define _VECTOR2D


enum DecimalPlaces
{
	ZERO,
	ONE,
	TWO,
	THREE,
	FOUR
};	

void roundValue(REAL& val, DecimalPlaces decPlace);
REAL cross		(const vec2& v1, const vec2& v2);

class Vec2D : public vec2
{
public:

	Vec2D()
	{
		this->x = 0.0;
		this->y = 0.0;
	}
	bool isPointInTriangle(const vec2& v1, 
								  const vec2& v2, 
								  const vec2& v3) const;

	void roundValues(DecimalPlaces decPlace)
	{
		roundValue(this->x, decPlace);
		roundValue(this->y, decPlace);	
	}
	REAL crossProduct(Vec2D& vec2)
	{
		return cross(*this, vec2); 
	}
};


inline REAL cross(const vec2& v1, const vec2& v2)
{
	return v1.x * v2.y - v1.y * v2.x; 
}

inline void roundValue(REAL& val, DecimalPlaces decPlace)
{
	int base[] = {1, 10, 100, 1000, 10000};
	val *= base[decPlace];
	val += 0.5;
	val  = ::floor(val);
	val /= base[decPlace];
}


#endif