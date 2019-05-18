/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
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
#include "MainWindow.h"
#include "Game.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
}

void Game::ComposeFrame()
{
	Vec2 camLoc = { 0.0f, 800.0f };
	float camAngle = 0.0f;

	Vec2 worldLoc{ 0.0f, 0.0f };
	float worldRadius = 800.0f;

	Vec2 domeLoc{ 0.0f, 0.0f };
	float domeRadius = 1000.0f;

	Vec2 playerCenterLoc{ 0.0f, worldRadius };
	float playerWidth = 20.0f;
	float playerHeight = 50.0f;

	Vec2 playerTransformShift{ playerWidth / 2.0f,playerHeight };

	Vec2 screenTransformFlip{ 1.0f,-1.0f };
	Vec2 screenTransformShift{ (float)(gfx.ScreenWidth / 2),(float)(gfx.ScreenHeight / 2) };


	gfx.DrawCircle((worldLoc - camLoc) * screenTransformFlip + screenTransformShift, worldRadius, Colors::LightGray, 0.5f);
	gfx.DrawCircle((domeLoc - camLoc) * screenTransformFlip + screenTransformShift, domeRadius, Colors::Gray);

	gfx.DrawRect((playerCenterLoc + playerTransformShift - camLoc) * screenTransformFlip + screenTransformShift, (int)playerWidth, (int)playerHeight, Colors::Blue);

}
