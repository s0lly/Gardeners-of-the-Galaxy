#pragma once

#include "Vec2.h"

struct Player
{
	Vec2 centerBotLoc{ 0.0f, 801.0f };
	float angle = 0.0f;

	Vec2 velocity{ 0.0f, 0.0f };

	float width = 20.0f;
	float height = 50.0f;
	Vec2 transformShift{ -width / 2.0f,height };
};