#include "Vec2.h"
#include <math.h>

Vec2::Vec2()
{
}

// assigns member variables
Vec2::Vec2(float xin, float yin) : x(xin), y(yin)
{
}

// vector equality
bool Vec2::operator==(const Vec2& rhs) const
{
	return (x == rhs.x && y == rhs.y);
}

// vector inequality
bool Vec2::operator!=(const Vec2& rhs) const
{
	return (x != rhs.x && y != rhs.y);
}

// vector addition returning new vector
Vec2 Vec2::operator+(const Vec2& rhs) const
{
	return Vec2(x + rhs.x, y + rhs.y);
}

// vector substraction returning new vector
Vec2 Vec2::operator-(const Vec2& rhs) const
{
	return Vec2(x - rhs.x, y - rhs.y);
}

// vector division by val returning new vector
Vec2 Vec2::operator/(const float val) const
{
	return Vec2(x / val, y / val);
}

// vector multiplication by val returning new vector
Vec2 Vec2::operator*(const float val) const
{
	return Vec2(x * val, y * val);
}

// adds rhs to this vector
void Vec2::operator+=(const Vec2& rhs)
{
	x += rhs.x;
	y += rhs.y;
}

// substracts rhs from this vector
void Vec2::operator-=(const Vec2& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
}

// multiplies this vector by val
void Vec2::operator*=(const float val)
{
	x *= val;
	y *= val;
}

// divides this vector by val
void Vec2::operator/=(const float val)
{
	x /= val;
	y /= val;
}


// returns distance between rhs and this vector
float Vec2::dist(const Vec2& rhs) const
{
	float sqrd = (rhs.x - x) * (rhs.x - x) + (rhs.y - y) * (rhs.y - y);
	return sqrtf(sqrd);
}

// returns length of this vector
float Vec2::length() const
{
	return sqrtf(x * x + y * y);
}

// normalizes this vector and returns unit vector
Vec2 Vec2::normalize() const
{
	return Vec2(x / length(), y / length());
}

// returns the direction vector between rhs and this vector
Vec2 Vec2::direction(const Vec2& rhs) const
{
	return Vec2(rhs.x - x, rhs.y - y);
}



