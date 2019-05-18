
/*******************************************************************************************
*	Code and amendments by s0lly														   *
*	https://www.youtube.com/c/s0lly							                               *
*	https://s0lly.itch.io/																   *
*	https://www.instagram.com/s0lly.gaming/												   *
********************************************************************************************/

#pragma once

#include <math.h>

#define PI 3.1415926f

struct Mat2;

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

	float GetMagnitude()
	{
		return sqrt(GetMagnitudeSqrd());
	}

	Vec2 GetNormalized()
	{
		float magnitude = GetMagnitude();
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

	Vec2 operator *(Mat2& rhs);


	float x;
	float y;
};



struct Mat2
{
	Mat2 GetTranspose()
	{
		return Mat2{ a, c, b, d };
	}


	static Mat2 RotationMatrix (float angle)
	{
		return Mat2{ cosf(angle), sinf(-angle), sinf(angle), cosf(angle) };
	}


	float a;
	float b;
	float c;
	float d;
};