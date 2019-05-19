#pragma once

#include "Vec2.h"
#include "Atmosphere.h"

struct Player
{
	Vec2 centerBotLoc{ 0.0f, 801.0f };
	float angle = 0.0f;

	Vec2 velocity{ 0.0f, 0.0f };

	float energy = 100.0f;
	float maxEnergy = 100.0f;

	bool isAlive = true;
	bool isSleeping = false;

	float width = 20.0f;
	float height = 50.0f;
	Vec2 transformShift{ -width / 2.0f,height };

	Vec2 transformShiftCircle{ 0.0f,0.0f };

	void Breathe(Atmosphere *atmosphere)
	{
		if (atmosphere->oxygenLevel >= 0.1f)
		{
			atmosphere->oxygenLevel -= 0.1f;
			atmosphere->carbonDioxideLevel += 0.1f;
		}
	}

	void ExpendEnergy(float energyUsed)
	{
		energy -= energyUsed;
		if (energy <= 0.0f)
		{
			isAlive = false;
		}
	}

	void Sleep()
	{
		ExpendEnergy(-0.05f);
		if (energy > maxEnergy)
		{
			energy = maxEnergy;
			isSleeping = false;
		}
	}
};