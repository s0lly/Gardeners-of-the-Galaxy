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

bool Game::Go()
{
	gfx.BeginFrame();	
	ProcessInput();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();

	return gameOver;
}

void Game::ProcessInput()
{

	Mat2 playerRotation = Mat2::RotationMatrix(player.angle);

	if (!player.isSleeping)
	{
		if (wnd.kbd.KeyIsPressed('W'))
		{
			player.velocity = player.velocity + Vec2{ 0.0f, 0.02f } *playerRotation * (player.energy / player.maxEnergy);
			player.ExpendEnergy(0.01f);
		}
		if (wnd.kbd.KeyIsPressed('S'))
		{
			player.velocity = player.velocity + Vec2{ 0.0f, -0.02f } *playerRotation * (player.energy / player.maxEnergy);
			player.ExpendEnergy(0.01f);
		}
		if (wnd.kbd.KeyIsPressed('A'))
		{
			player.velocity = player.velocity + Vec2{ -0.02f, 0.0f } *playerRotation * (player.energy / player.maxEnergy);
			player.ExpendEnergy(0.01f);
			player.isFacingRight = false;
		}
		if (wnd.kbd.KeyIsPressed('D'))
		{
			player.velocity = player.velocity + Vec2{ 0.02f, 0.0f } *playerRotation * (player.energy / player.maxEnergy);
			player.ExpendEnergy(0.01f);
			player.isFacingRight = true;
		}


		if (wnd.kbd.KeyIsPressed(VK_SPACE))
		{
			if (player.centerBotLoc.GetMagnitude() - world.radius < 10.0f)
			{
				plants.push_back(Plant(player.centerBotLoc.GetNormalized() * world.radius, PLANT_TYPE_CARBONEATER, plants.size()));
			}
		}


		if (wnd.kbd.KeyIsPressed(VK_RETURN))
		{
			if ((player.centerBotLoc - habitat.doorLoc).GetMagnitude() < 20.0f)
			{
				player.isSleeping = true;
			}
		}

		if (wnd.kbd.KeyIsPressed(VK_CONTROL))
		{
			player.isCutting = true;
		}
		else
		{
			player.isCutting = false;
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

	if (!((newPosition + Vec2{ player.height * sin(player.angle), player.height * cos(player.angle) } - dome.loc).GetMagnitudeSqrd() > (dome.radius * dome.radius)))
	{
		player.centerBotLoc = newPosition;
	}
	else
	{
		Vec2 normal = player.centerBotLoc / (dome.radius - player.height);
		player.velocity = player.velocity - normal * 2.0f * (player.velocity.Dot(normal));
	}


	// player cutting applied to plants

	if (player.isCutting)
	{
		Vec2 cuttingLocation = player.centerBotLoc + (player.isFacingRight ? Vec2(40.0f, 10.0f) : Vec2(-40.0f, 10.0f)) * Mat2::RotationMatrix(player.angle);
		for (int i = 0; i < plants.size(); i++)
		{
			if ((cuttingLocation - (plants[i].centerBotLoc)).GetMagnitude() < (plants[i].currentSize + 5.0f))
			{
				if (!plants[i].isBeingCut)
				{
					plants[i].isBeingCut = true;
				}

				plants[i].currentCutAmount += 1.0f;
				
			}
			else
			{
				plants[i].currentCutAmount = 0.0f;
				plants[i].isBeingCut = false;
			}
		}
	}


	for (int i = 0; i < plants.size(); i++)
	{
		if (plants[i].currentCutAmount >= plants[i].maxCutAmount)
		{
			plants[i].isDead = true;
			player.foodStored += plants[i].maxFoodValue * (plants[i].currentSize / plants[i].maxSize);
		}
	}



	// plants attack each other

	for (int i = 0; i < plants.size(); i++)
	{
		if (!plants[i].isDead)
		{
			for (int j = 0; j < plants.size(); j++)
			{
				if ((plants[i].currentSize > plants[j].currentSize)
					&& ((plants[i].centerBotLoc - plants[j].centerBotLoc).GetMagnitude() < (plants[i].currentSize + plants[j].currentSize)))
				{
					plants[j].currentSize -= plants[j].growthSpeed * 4.0f;
				}
			}
		}
		
	}

	for (int i = 0; i < plants.size(); i++)
	{
		if (plants[i].currentSize <= 0.0f)
		{
			plants[i].isDead = true;
		}
	}
	


	// breathing

	player.Breathe(&dome.atmosphere);

	for (int i = 0; i < plants.size(); i++)
	{
		if (plants[i].CanBreathe(&dome.atmosphere) && !plants[i].isDead)
		{
			plants[i].Breathe(&dome.atmosphere);
		}
		else
		{
			plants[i].isDead = true;
		}
	}


	// deleting dead plants

	for (int i = 0; i < plants.size(); i++)
	{
		if (plants[i].isDead)
		{
			std::iter_swap(plants.begin() + i, plants.end() - 1);
			plants.pop_back();
			i--;
		}
	}

	std::sort(plants.begin(), plants.end());
	for (int i = 0; i < plants.size(); i++)
	{
		plants[i].ID = i;
	}


	// player sleeping mechanics

	if (!player.isSleeping)
	{
		player.ExpendEnergy(0.01f);
	}
	else
	{
		player.Sleep();
		if (player.isSleeping)
		{
			player.centerBotLoc = Vec2(-830.0f, -60.0f);
			player.velocity = Vec2(0.0f, 0.0f);
		}
		else
		{
			player.centerBotLoc = habitat.doorLoc;
		}
		
	}
	

	// check if game over

	if (!player.isAlive)
	{
		gameOver = true;
	}



	// update camera

	Vec2 newCameraLoc = player.centerBotLoc + Vec2(0.0f, 0.0f); //(float)player.height / 2.0f
	float cameraSmoothing = 1.0f;
	camera.loc = camera.loc * (1.0f - cameraSmoothing) + newCameraLoc * cameraSmoothing;

	player.angle = 0.0f;

	Vec2 yUpNormalized{ 0.0f, 1.0f };
	Vec2 playerLocNormalized = player.centerBotLoc.GetNormalized();

	player.angle = acosf(yUpNormalized.Dot(playerLocNormalized)) * (player.centerBotLoc.x >= 0.0f ? 1.0f : -1.0f);

	camera.angle = acosf(yUpNormalized.Dot(camera.loc.GetNormalized())) * (camera.loc.x >= 0.0f ? 1.0f : -1.0f);

	if (spaceship.zLoc > 19.0f)
	{
		spaceship.zLoc -= spaceship.zChange / 5.0f;
	}
	else if (spaceship.zLoc > 10.0f)
	{
		spaceship.zChange *= 0.97914836f;
		spaceship.zLoc -= spaceship.zChange / 5.0f;
	}
	else
	{
		spaceship.hasArrived = true;
	}
	

}

void Game::ComposeFrame()
{
	moonAngleToPlanet += 0.001f;

	moonLoc.x = -500.0f + cos(moonAngleToPlanet) * 400.0f;
	moonZ = 1.0f + (sin(moonAngleToPlanet) + 1.0f);

	Vec2 screenTransformFlip{ 1.0f,-1.0f };
	Vec2 screenTransformShift{ (float)(gfx.ScreenWidth / 2),(float)(gfx.ScreenHeight / 2) };

	Mat2 cameraRotation = Mat2::RotationMatrix(camera.angle);

	gfx.DrawCircleWithIncreasingAlphaToEdge(((planet1Loc)* cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, planet1Radius, Colors::Yellow, 0.0f, 0.2f, 0.0f);

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
		gfx.DrawCircle(((planetLoc - camera.loc / (1000.0f * planetZ)) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, planetRadius, Colors::Green);
		gfx.DrawCircle(((moonLoc - camera.loc / (1000.0f * moonZ)) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, moonRadius / moonZ, Colors::Gray);
	}

	gfx.DrawCircle(((spaceship.loc - camera.loc / (spaceship.zLoc)) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, spaceship.radius / spaceship.zLoc, spaceship.color);

	gfx.DrawCircleWithIncreasingAlphaToEdge(((dome.loc - camera.loc) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, dome.radius, dome.GetAtmosphereCombinedColor(), world.radius, 0.1f, 0.5f);



	
	for (int i = 0; i < plants.size(); i++)
	{
		gfx.DrawCircle(((plants[i].centerBotLoc - camera.loc) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, plants[i].currentSize, Colors::Green);
	}


	

	gfx.DrawCircle(((habitat.hutLoc - camera.loc) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, habitat.hutHeightRadius, habitat.hutColor);
	gfx.DrawCircle(((habitat.doorLoc - camera.loc) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, habitat.doorHeightRadius, habitat.doorColor);
	if (player.isSleeping)
	{
		gfx.DrawRectWithinCircle((player.centerBotLoc + player.transformShift - camera.loc) * screenTransformFlip + screenTransformShift, (int)player.width, (int)player.height, Colors::Blue, ((habitat.windowLoc - camera.loc) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, habitat.windowRadius);
	}
	gfx.DrawCircle(((habitat.windowLoc - camera.loc) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, habitat.windowRadius, habitat.windowColor, 0.75f);


	if (!player.isSleeping)
	{
		gfx.DrawRect((player.centerBotLoc + player.transformShift - camera.loc) * screenTransformFlip + screenTransformShift, (int)player.width, (int)player.height, Colors::Blue);
	}
	//gfx.DrawCircle((player.centerBotLoc + player.transformShiftCircle - camera.loc) * screenTransformFlip + screenTransformShift, 2.0f, Colors::Red);

	if (player.isCutting)
	{
		Vec2 cuttingLocation = player.centerBotLoc + (player.isFacingRight ? Vec2(40.0f, 10.0f) : Vec2(-40.0f, 10.0f)) * Mat2::RotationMatrix(player.angle);

		Vec2 axeHitLoc = player.isFacingRight ? Vec2(40.0f, 10.0f) : Vec2(-40.0f, 10.0f);
		gfx.DrawCircle((cuttingLocation - camera.loc) * cameraRotation.GetTranspose() * screenTransformFlip + screenTransformShift, 5.0f, Colors::Red);
	}



	gfx.DrawCircleWithIncreasingAlphaToEdge(((dome.loc - camera.loc) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, dome.radius, dome.GetAtmosphereCombinedColor(), world.radius, 0.1f, 0.5f);

	gfx.DrawCircle(((world.loc - camera.loc) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, world.radius, Colors::Gray);


	// GUI

	if (spaceship.arrivalTime > 0)
	{
		spaceship.arrivalTime -= ((spaceship.zChange / 5.0f) / 60.0f);
	}
	else
	{
		spaceship.arrivalTime = 0;
	}
	

	int arrivalTimeMin = (int)spaceship.arrivalTime;
	int arrivalTimeSec = (int)((spaceship.arrivalTime - (float)arrivalTimeMin) * 60.0f);

	RetroContent::DrawString(gfx, std::string("STARSHIP ARRIVAL"), { 1175.0f, 50.0f }, 2, Colors::White);
	RetroContent::DrawString(gfx, std::to_string(arrivalTimeMin) + " MIN  " + std::to_string(arrivalTimeSec) + " SEC", { 1425.0f, 50.0f }, 2, Colors::White);

	RetroContent::DrawString(gfx, std::string("OXYGEN LEVEL"), { 1375.0f, 100.0f }, 2, dome.atmosphere.oxygenColor);
	RetroContent::DrawString(gfx, std::to_string((int)(dome.atmosphere.oxygenLevel + 0.5f)) + "%", { 1525.0f, 100.0f }, 2, dome.atmosphere.oxygenColor);

	RetroContent::DrawString(gfx, std::string("CO2 LEVEL"), { 1375.0f, 150.0f }, 2, dome.atmosphere.carbonDioxideColor);
	RetroContent::DrawString(gfx, std::to_string((int)(dome.atmosphere.carbonDioxideLevel + 0.5f)) + "%", { 1525.0f, 150.0f }, 2, dome.atmosphere.carbonDioxideColor);
	
	RetroContent::DrawString(gfx, std::string("ENERGY LEVEL"), { 1375.0f, 200.0f }, 2, Colors::Cyan);
	RetroContent::DrawString(gfx, std::to_string((int)(player.energy + 0.5f)) + "%", { 1525.0f, 200.0f }, 2, Colors::Cyan);

	RetroContent::DrawString(gfx, std::string("FOOD PRODUCED"), { 1375.0f, 250.0f }, 2, Colors::Red);
	RetroContent::DrawString(gfx, std::to_string((int)(player.foodStored)) + "%", { 1525.0f, 250.0f }, 2, Colors::Red);
}
