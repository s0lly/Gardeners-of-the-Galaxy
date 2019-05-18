#pragma once

#include "Vec2.h"
#include "Colors.h"

struct Star
{
	Vec2 loc{ 0.0f, 0.0f };
	float radius = 800.0f;
	Color color;
};