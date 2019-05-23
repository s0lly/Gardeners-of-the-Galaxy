#pragma once

#include "Vec2.h"
#include "Atmosphere.h"
#include "Colors.h"


enum PLANT_TYPE
{
	PLANT_TYPE_CARBONEATER,
	PLANT_TYPE_BIGCARBONEATER,
	PLANT_TYPE_FOODFOROXY,
	PLANT_TYPE_CARBONGUZZLER,
	PLANT_TYPE_SUPERHARD,
	MAX_PLANT_NUM
};

struct Plant
{
	Plant(Vec2 in_loc, PLANT_TYPE in_type, int in_ID)
		:
		centerBotLoc(in_loc),
		type(in_type),
		ID(in_ID)
	{
		switch (type)
		{
		case PLANT_TYPE_CARBONEATER:
		{
			carbonDioxideBreathe = 0.0001f;
			oxygenBreathe = -0.0001f;

			currentSize = 1.0f;
			maxSize = 10.0f;
			growthSpeed = 0.01f;

			widthToHeight = 0.3f;

			maxFoodValue = 1.0f;

			maxCutAmount = 60.0f;

			color = Color(50, 205, 50);

		}break;
		case PLANT_TYPE_BIGCARBONEATER:
		{
			carbonDioxideBreathe = 0.000001f;
			oxygenBreathe = -0.000001f;

			currentSize = 1.0f;
			maxSize = 300.0f;
			growthSpeed = 0.2f;

			widthToHeight = 0.1f;

			maxFoodValue = 180.0f;

			maxCutAmount = 1200.0f;

			color = Color(50, 205, 255);

		}break;
		case PLANT_TYPE_FOODFOROXY:
		{
			carbonDioxideBreathe = -0.001f;
			oxygenBreathe = +0.001f;

			currentSize = 1.0f;
			maxSize = 100.0f;
			growthSpeed = 0.2f;

			widthToHeight = 0.1f;

			maxFoodValue = 70.0f;

			maxCutAmount = 1000.0f;

			color = Color(150, 0, 255);

		}break;
		case PLANT_TYPE_CARBONGUZZLER:
		{
			carbonDioxideBreathe = +0.01f;
			oxygenBreathe = -0.01f;

			currentSize = 1.0f;
			maxSize = 300.0f;
			growthSpeed = 0.1f;

			widthToHeight = 0.1f;

			maxFoodValue = 5.0f;

			maxCutAmount = 400.0f;

			color = Color(255, 127, 0);

		}break;
		case PLANT_TYPE_SUPERHARD:
		{
			carbonDioxideBreathe = +0.00000001f;
			oxygenBreathe = -0.00000001f;

			currentSize = 1.0f;
			maxSize = 50.0f;
			growthSpeed = 0.05f;

			widthToHeight = 0.1f;

			maxFoodValue = 250.0f;

			maxCutAmount = 2500.0f;

			color = Color(255, 192, 203);
		}break;
		}
	}


	Vec2 centerBotLoc;
	PLANT_TYPE type;

	float currentSize;
	float maxSize;
	float growthSpeed;

	float widthToHeight;

	int ID;

	float maxFoodValue;
	bool isBeingCut = false;
	float currentCutAmount = 0.0f;
	float maxCutAmount;

	Color color;

	float carbonDioxideBreathe;
	float oxygenBreathe;

	bool isDead = false;

	bool CanBreathe(Atmosphere *atmosphere)
	{
		if (carbonDioxideBreathe > oxygenBreathe)
		{
			return (atmosphere->carbonDioxideLevel >= carbonDioxideBreathe);
		}
		else
		{
			return (atmosphere->oxygenLevel >= oxygenBreathe);
		}
	}

	void Breathe(Atmosphere *atmosphere)
	{
		atmosphere->carbonDioxideLevel -= carbonDioxideBreathe * currentSize;
		atmosphere->oxygenLevel -= oxygenBreathe * currentSize;

		if (currentSize < maxSize)
		{
			currentSize += growthSpeed;
		}
	}

	bool operator<(Plant &rhs)
	{
		return (ID < rhs.ID);
	}
	
};