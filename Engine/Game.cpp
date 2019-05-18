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
	for (int i = 0; i < 10000; i++)
	{
		starmap.push_back(Star{ Vec2((float)(rand() % 4001) - 2000, (float)(rand() % 4001) - 2000), (float)(rand() % 5), Color{ (unsigned char)(rand() % 256), (unsigned char)(rand() % 256), (unsigned char)(rand() % 256) } });
	}
	
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
	Mat2 playerRotation = Mat2::RotationMatrix(player.angle);

	if (wnd.kbd.KeyIsPressed('W'))
	{
		player.velocity = player.velocity + Vec2{0.0f, 0.02f } * playerRotation;
	}
	if (wnd.kbd.KeyIsPressed('S'))
	{
		player.velocity = player.velocity + Vec2{ 0.0f, -0.02f } * playerRotation;
	}
	if (wnd.kbd.KeyIsPressed('A'))
	{
		player.velocity = player.velocity + Vec2{ -0.02f, 0.0f } * playerRotation;
	}
	if (wnd.kbd.KeyIsPressed('D'))
	{
		player.velocity = player.velocity + Vec2{ 0.02f, 0.0f } * playerRotation;
	}

}

void Game::UpdateModel()
{
	Vec2 domeLoc{ 0.0f, 0.0f };
	float domeRadius = 1000.0f;

	float gravityAccelerationMagnitude = 0.01f;

	Vec2 vectorFromPlayerToWorld = (world.loc - player.centerBotLoc);

	Vec2 gravityAcceleration = vectorFromPlayerToWorld.GetNormalized() * gravityAccelerationMagnitude;


	player.velocity = player.velocity + gravityAcceleration;

	Vec2 newPosition = player.centerBotLoc + player.velocity;

	if (!((newPosition - world.loc).GetMagnitudeSqrd() < (world.radius * world.radius)))
	{
		player.centerBotLoc = newPosition;
	}
	else
	{
		player.velocity = { 0.0f, 0.0f };
	}

	if (!((newPosition + Vec2{ 0.0f, player.height } - domeLoc).GetMagnitudeSqrd() > (domeRadius * domeRadius)))
	{
		player.centerBotLoc = newPosition;
	}
	else
	{
		player.velocity = { 0.0f, -player.velocity.y };
	}

	camera.loc = player.centerBotLoc + Vec2(0.0f, (float)player.height / 2.0f);

	player.angle = 0.0f;

	Vec2 yUpNormalized{ 0.0f, 1.0f };
	Vec2 playerLocNormalized = player.centerBotLoc.GetNormalized();

	player.angle = acosf(yUpNormalized.Dot(playerLocNormalized)) * (player.centerBotLoc.x >= 0.0f ? 1.0f : -1.0f);

	camera.angle = player.angle;

}

void Game::ComposeFrame()
{

	Vec2 domeLoc{ 0.0f, 0.0f };
	float domeRadius = 1000.0f;

	Vec2 planetLoc{ -800.0f, 800.0f };
	float planetRadius = 240.0f;


	Vec2 screenTransformFlip{ 1.0f,-1.0f };
	Vec2 screenTransformShift{ (float)(gfx.ScreenWidth / 2),(float)(gfx.ScreenHeight / 2) };

	Mat2 cameraRotation = Mat2::RotationMatrix(camera.angle);

	for (int i = 0; i < starmap.size(); i++)
	{
		gfx.DrawCircle(((starmap[i].loc - camera.loc) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, starmap[i].radius, starmap[i].color);
	}

	gfx.DrawCircle(((planetLoc - camera.loc) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, planetRadius, Colors::Green);
	gfx.DrawCircle(((domeLoc - camera.loc) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, domeRadius, Colors::LightGray, 0.2f);
	gfx.DrawCircle(((world.loc - camera.loc) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, world.radius, Colors::Gray);

	gfx.DrawRect((player.centerBotLoc + player.transformShift - camera.loc) * screenTransformFlip + screenTransformShift, (int)player.width, (int)player.height, Colors::Blue);

}
