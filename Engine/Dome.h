#pragma once

#include "Vec2.h"
#include "Colors.h"
#include "Atmosphere.h"

struct Dome
{
	Vec2 loc{ 0.0f, 0.0f };
	float radius = 1000.0f;
	Atmosphere atmosphere;

	Color GetAtmosphereCombinedColor()
	{
		float newR = (((float)atmosphere.carbonDioxideColor.GetR() * atmosphere.carbonDioxideLevel + (float)atmosphere.oxygenColor.GetR() * atmosphere.oxygenLevel + (float)atmosphere.nitrogenColor.GetR() * atmosphere.nitrogenLevel) / 3.0f) / 100.0f;
		float newG = (((float)atmosphere.carbonDioxideColor.GetG() * atmosphere.carbonDioxideLevel + (float)atmosphere.oxygenColor.GetG() * atmosphere.oxygenLevel + (float)atmosphere.nitrogenColor.GetG() * atmosphere.nitrogenLevel) / 3.0f) / 100.0f;
		float newB = (((float)atmosphere.carbonDioxideColor.GetB() * atmosphere.carbonDioxideLevel + (float)atmosphere.oxygenColor.GetB() * atmosphere.oxygenLevel + (float)atmosphere.nitrogenColor.GetB() * atmosphere.nitrogenLevel) / 3.0f) / 100.0f;
		
		newR = newR > 255.0f ? 255.0f : newR;
		newG = newG > 255.0f ? 255.0f : newG;
		newB = newB > 255.0f ? 255.0f : newB;

		return { (unsigned char)newR, (unsigned char)newG, (unsigned char)newB };
	}
};