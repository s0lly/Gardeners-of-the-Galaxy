
/*******************************************************************************************
*	Code and amendments by s0lly														   *
*	https://www.youtube.com/c/s0lly							                               *
*	https://s0lly.itch.io/																   *
*	https://www.instagram.com/s0lly.gaming/												   *
********************************************************************************************/

#pragma once

#include <math.h>

#define PI 3.1415926f



struct Vec2
{
	Vec2()
		:
		x(0.0f),
		y(0.0f)
	{
	}

	Vec2(float a, float b)
		:
		x(a),
		y(b)
	{
	}

	Vec2 operator +(Vec2& rhs)
	{
		return Vec2(x + rhs.x, y + rhs.y);
	}

	Vec2 operator -(Vec2& rhs)
	{
		return Vec2(x - rhs.x, y - rhs.y);
	}

	Vec2 operator *(Vec2& rhs)
	{
		return Vec2(x * rhs.x, y * rhs.y);
	}

	Vec2 operator *(float rhs)
	{
		return Vec2(x * rhs, y * rhs);
	}

	Vec2 operator /(float rhs)
	{
		return Vec2(x / rhs, y / rhs);
	}

	float GetMagnitudeSqrd()
	{
		return x * x + y * y;
	}

	float GetVec2Magnitude()
	{
		return sqrt(GetMagnitudeSqrd());
	}

	Vec2 GetNormalized()
	{
		float magnitude = GetVec2Magnitude();
		if (magnitude > 0.0f)
		{
			return Vec2(x / magnitude, y / magnitude);
		}
		else
		{
			return Vec2(0.0f, 0.0f);
		}
	}

	float Dot(Vec2& rhs)
	{
		return x * rhs.x + y * rhs.y;
	}


	float x;
	float y;
};