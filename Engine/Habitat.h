/*******************************************************************************************
*	Code and amendments by s0lly														   *
*	https://www.youtube.com/c/s0lly							                               *
*	https://s0lly.itch.io/																   *
*	https://www.instagram.com/s0lly.gaming/												   *
********************************************************************************************/

#pragma once

#include "Vec2.h"
#include "Colors.h"

struct Habitat
{
	Vec2 hutLoc{ -800.0f, 0.0f };
	float hutWidthRadius = 90.0f;
	float hutHeightRadius = 160.0f;
	Color hutColor = Colors::Cyan;

	Vec2 doorLoc{ -800.0f, 40.0f };
	float doorWidthRadius = 60.0f;
	float doorHeightRadius = 60.0f;
	Color doorColor = Colors::Red;

	Vec2 windowLoc{ -880.0f, -60.0f };
	float windowRadius = 20.0f;
	Color windowColor = Colors::LightGray;
};