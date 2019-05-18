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
		starmap.push_back(Star{ Vec2((float)(rand() % 4001) - 2000, (float)(rand() % 4001) - 2000), (float)(rand() % 3 + 1), Color{ (unsigned char)(rand() % 256), (unsigned char)(rand() % 256), (unsigned char)(rand() % 256) } });
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


	if (wnd.kbd.KeyIsPressed(VK_SPACE))
	{
		if (player.centerBotLoc.GetMagnitude() - world.radius < 10.0f)
		{
			plants.push_back(Plant(player.centerBotLoc.GetNormalized() * world.radius, PLANT_TYPE_CARBONEATER, plants.size()));
		}
		
	}

}

void Game::UpdateModel()
{
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
		Vec2 normal = player.centerBotLoc / world.radius;
		player.velocity = player.velocity - normal * 2.0f * (player.velocity.Dot(normal));
	}

	if (!((newPosition- dome.loc).GetMagnitudeSqrd() > (dome.radius * dome.radius))) // + Vec2{ 0.0f, player.height } 
	{
		player.centerBotLoc = newPosition;
	}
	else
	{
		Vec2 normal = player.centerBotLoc / dome.radius;
		player.velocity = player.velocity - normal * 2.0f * (player.velocity.Dot(normal));
	}

	Vec2 newCameraLoc = player.centerBotLoc + Vec2(0.0f, 0.0f); //(float)player.height / 2.0f
	float cameraSmoothing = 1.0f;
	camera.loc = camera.loc * (1.0f - cameraSmoothing) + newCameraLoc * cameraSmoothing;

	player.angle = 0.0f;

	Vec2 yUpNormalized{ 0.0f, 1.0f };
	Vec2 playerLocNormalized = player.centerBotLoc.GetNormalized();

	player.angle = acosf(yUpNormalized.Dot(playerLocNormalized)) * (player.centerBotLoc.x >= 0.0f ? 1.0f : -1.0f);

	camera.angle = acosf(yUpNormalized.Dot(camera.loc.GetNormalized())) * (camera.loc.x >= 0.0f ? 1.0f : -1.0f);

	//camera.angle = player.angle;



	player.Breathe(&dome.atmosphere);

	for (int i = 0; i < plants.size(); i++)
	{
		if (plants[i].CanBreathe(&dome.atmosphere))
		{
			plants[i].Breathe(&dome.atmosphere);
		}
		else
		{
			std::swap(plants.begin() + i, plants.end() - 1);
			plants.pop_back();
			i--;
		}
	}

	std::sort(plants.begin(), plants.end());
	for (int i = 0; i < plants.size(); i++)
	{
		plants[i].ID = i;
	}

}

void Game::ComposeFrame()
{
	moonAngleToPlanet += 0.001f;

	moonLoc.x = -200.0f + cos(moonAngleToPlanet) * 400.0f;
	moonZ = 1.0f + (sin(moonAngleToPlanet) + 1.0f);

	Vec2 screenTransformFlip{ 1.0f,-1.0f };
	Vec2 screenTransformShift{ (float)(gfx.ScreenWidth / 2),(float)(gfx.ScreenHeight / 2) };

	Mat2 cameraRotation = Mat2::RotationMatrix(camera.angle);

	for (int i = 0; i < starmap.size(); i++)
	{
		gfx.DrawCircle(((starmap[i].loc) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, starmap[i].radius, starmap[i].color);
	}

	if (moonZ >= planetZ)
	{
		gfx.DrawCircle(((moonLoc - camera.loc / (1000.0f * moonZ)) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, moonRadius / moonZ, Colors::Gray);
		gfx.DrawCircle(((planetLoc - camera.loc / (1000.0f * planetZ)) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, planetRadius, Colors::Green);
	}
	else
	{
		gfx.DrawCircle(((planetLoc - camera.loc / (1000.0f * moonZ)) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, planetRadius, Colors::Green);
		gfx.DrawCircle(((moonLoc - camera.loc / (1000.0f * planetZ)) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, moonRadius / moonZ, Colors::Gray);
	}

	gfx.DrawCircleWithIncreasingAlphaToEdge(((dome.loc - camera.loc) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, dome.radius, dome.GetAtmosphereCombinedColor(), world.radius, 0.1f, 0.5f);

	//gfx.DrawRect((player.centerBotLoc + player.transformShift - camera.loc) * screenTransformFlip + screenTransformShift, (int)player.width, (int)player.height, Colors::Blue);

	for (int i = 0; i < plants.size(); i++)
	{
		gfx.DrawCircle(((plants[i].centerBotLoc - camera.loc) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, plants[i].currentSize, Colors::Green);
	}

	gfx.DrawCircle((player.centerBotLoc + player.transformShiftCircle - camera.loc) * screenTransformFlip + screenTransformShift, 2.0f, Colors::Red);


	gfx.DrawCircleWithIncreasingAlphaToEdge(((dome.loc - camera.loc) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, dome.radius, dome.GetAtmosphereCombinedColor(), world.radius, 0.1f, 0.5f);

	gfx.DrawCircle(((world.loc - camera.loc) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, world.radius, Colors::Gray);

}
