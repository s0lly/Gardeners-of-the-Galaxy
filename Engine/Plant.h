#pragma once

#include "Vec2.h"
#include "Atmosphere.h"


enum PLANT_TYPE
{
	PLANT_TYPE_CARBONEATER,
	PLANT_TYPE_BIGCARBONEATER
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

		}break;
		case PLANT_TYPE_BIGCARBONEATER:
		{
			carbonDioxideBreathe = 0.0005f;
			oxygenBreathe = -0.0005f;

			currentSize = 1.0f;
			maxSize = 300.0f;
			growthSpeed = 0.02f;

			widthToHeight = 0.1f;

			maxFoodValue = 100.0f;

			maxCutAmount = 500.0f;
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


	float carbonDioxideBreathe;
	float oxygenBreathe;

	bool isDead = false;

	bool CanBreathe(Atmosphere *atmosphere)
	{
		return (atmosphere->carbonDioxideLevel >= 0.01f);
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