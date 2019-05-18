#pragma once

#include "Colors.h"

struct Atmosphere
{
	float oxygenLevel = 95.0f;
	float carbonDioxideLevel = 5.0f;
	float nitrogenLevel = 0.0f; //78.0f

	Color oxygenColor = Colors::Blue;
	Color carbonDioxideColor = Color(255, 127, 0);
	Color nitrogenColor = Colors::White;

	float airPressure = 100.0f;
};