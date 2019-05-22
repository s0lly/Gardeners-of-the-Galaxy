#pragma once

#include "Vec2.h"
#include "Atmosphere.h"
#include "Plant.h"

enum WINLOSE
{
	LOSEBYBREAK,
	LOSEBYAIR,
	LOSEBYFOOD,
	LOSEBYENERGY,
	WINBYFOOD,
	STILLPLAYING
};


struct Player
{
	Vec2 centerBotLoc{ 0.0f, 801.0f };
	float angle = 0.0f;

	Vec2 headLoc{ 0.0f, 845.0f };
	Vec2 bodyLoc{ 0.0f, 826.0f };
	Vec2 leftLegLoc{ 0.0f, 806.0f };
	Vec2 rightLegLoc{ 0.0f, 806.0f };
	Vec2 leftArmLoc{ 0.0f, 827.5f };
	Vec2 rightArmLoc{ 0.0f, 827.5f };


	Vec2 headLocOffset{ 0.0f, 44.0f };
	Vec2 bodyLocOffset{ 0.0f, 25.0f };
	Vec2 leftLegLocOffset{ 0.0f, 6.0f };
	Vec2 rightLegLocOffset{ 0.0f, 6.0f };
	Vec2 leftArmLocOffset{ 0.0f, 27.5f };
	Vec2 rightArmLocOffset{ 0.0f, 27.5f };

	Vec2 legAnimationOffset{ 10.0f, 0.0f };
	Vec2 armAnimationOffset{ 10.0f, 0.0f };

	float animationAngle = 0.0f;


	float headRadius = 6.0f;
	float bodyRadius = 15.0f;
	float leftLegRadius = 6.0f;
	float rightLegRadius = 6.0f;
	float leftArmRadius = 6.0f;
	float rightArmRadius = 6.0f;

	bool isMoving = false;

	Vec2 velocity{ 0.0f, 0.0f };

	float energy = 100.0f;
	float maxEnergy = 100.0f;

	bool isAlive = true;
	bool isSleeping = false;

	float foodStored = 0.0f;
	bool isCutting = false;
	
	bool isFacingRight = true;

	PLANT_TYPE selectedSeed = PLANT_TYPE_CARBONEATER; //?? take plant instead?

	WINLOSE result = STILLPLAYING;

	float width = 20.0f;
	float height = 50.0f;
	Vec2 transformShift{ -width / 2.0f,height };

	Vec2 transformShiftCircle{ 0.0f,0.0f };

	int sleepAnimation = 0;
	int sleepAnimationFade = 0;

	void Breathe(Atmosphere *atmosphere)
	{
		float breathingRate = 0.01f * (isMoving ? 2.0f : 1.0f);
		if (atmosphere->oxygenLevel >= breathingRate)
		{
			atmosphere->oxygenLevel -= breathingRate;
			atmosphere->carbonDioxideLevel += breathingRate;
		}
		else
		{
			isAlive = false;
			result = WINLOSE::LOSEBYAIR;
		}
	}

	void ExpendEnergy(float energyUsed)
	{
		energy -= energyUsed;
		if (energy <= 0.0f)
		{
			isAlive = false;
			result = WINLOSE::LOSEBYENERGY;
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

	Color GetSelectedSeedColor()
	{
		return Plant(Vec2(), selectedSeed, -1).color;
	}
};