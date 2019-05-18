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
	ProcessInput();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::ProcessInput()
{
	if (wnd.kbd.KeyIsPressed('W'))
	{
		player.velocity.y += 0.02f;
	}
	if (wnd.kbd.KeyIsPressed('S'))
	{
		player.velocity.y -= 0.02f;
	}
	if (wnd.kbd.KeyIsPressed('A'))
	{
		player.velocity.x -= 0.02f;
	}
	if (wnd.kbd.KeyIsPressed('D'))
	{
		player.velocity.x += 0.02f;
	}

}

void Game::UpdateModel()
{
	Vec2 domeLoc{ 0.0f, 0.0f };
	float domeRadius = 1000.0f;

	float gravityAcceleration = 0.01f;

	player.velocity = player.velocity + Vec2{ 0.0f, -gravityAcceleration };

	Vec2 newPosition = player.centerLoc + player.velocity;

	if (!((newPosition - world.loc).GetMagnitudeSqrd() < (world.radius * world.radius)))
	{
		player.centerLoc = newPosition;
	}
	else
	{
		player.velocity = { 0.0f, 0.0f };
	}

	if (!((newPosition + Vec2{ 0.0f, player.height } - domeLoc).GetMagnitudeSqrd() > (domeRadius * domeRadius)))
	{
		player.centerLoc = newPosition;
	}
	else
	{
		player.velocity = { 0.0f, -player.velocity.y };
	}

}

void Game::ComposeFrame()
{

	Vec2 domeLoc{ 0.0f, 0.0f };
	float domeRadius = 1000.0f;

	Vec2 planetLoc{ -800.0f, 800.0f };
	float planetRadius = 240.0f;


	Vec2 screenTransformFlip{ 1.0f,-1.0f };
	Vec2 screenTransformShift{ (float)(gfx.ScreenWidth / 2),(float)(gfx.ScreenHeight / 2) };


	gfx.DrawCircle((planetLoc - camera.loc) * screenTransformFlip + screenTransformShift, planetRadius, Colors::Blue);
	gfx.DrawCircle((domeLoc - camera.loc) * screenTransformFlip + screenTransformShift, domeRadius, Colors::LightGray, 0.5f);
	gfx.DrawCircle((world.loc - camera.loc) * screenTransformFlip + screenTransformShift, world.radius, Colors::Gray);

	gfx.DrawRect((player.centerLoc + player.transformShift - camera.loc) * screenTransformFlip + screenTransformShift, (int)player.width, (int)player.height, Colors::Blue);

}
