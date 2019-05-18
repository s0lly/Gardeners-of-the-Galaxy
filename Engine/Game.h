/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.h																				  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Camera.h"
#include "Player.h"
#include "World.h"
#include "Star.h"
#include "Dome.h"
#include "Plant.h"
#include <vector>

class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();
private:
	void ComposeFrame();
	void UpdateModel();
	void ProcessInput();
	/********************************/
	/*  User Functions              */
	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  User Variables              */
	/********************************/

	Camera camera;
	Player player;
	World world;

	std::vector<Star> starmap;

	Dome dome;

	std::vector<Plant> plants;




	Vec2 planetLoc{ -400.0f, 200.0f };
	float planetRadius = 240.0f;
	float planetZ = 2.0f;

	Vec2 moonLoc{ -400.0f, 200.0f };
	float moonRadius = 48.0f;
	float moonZ = 1.0f; // -> 1.4f
	float moonAngleToPlanet = 0.0f;

	Vec2 planet1Loc{ -300.0f, -400.0f };
	float planet1Radius = 100.0f;
	float planet1Z = 0.1f;

};