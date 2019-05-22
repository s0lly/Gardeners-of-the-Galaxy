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
	if (gamestate == GAMESTATE_STARTSCREEN)
	{
		if (wnd.kbd.KeyIsPressed(VK_ESCAPE))
		{
			if (!isPausedPressed)
			{
				gamestate = GAMESTATE_STARTMENU;
				isPausedPressed = true;
				selectedOption = 0;
			}
		}
		else
		{
			isPausedPressed = false;
		}
	}
	else if (gamestate == GAMESTATE_RUNNING)
	{
		Mat2 playerRotation = Mat2::RotationMatrix(player.angle);

		if (!player.isSleeping)
		{
			player.isMoving = false;

			if (wnd.kbd.KeyIsPressed('W'))
			{
				player.velocity = player.velocity + Vec2{ 0.0f, 0.02f } *playerRotation * (player.energy / player.maxEnergy);
				player.ExpendEnergy(0.01f);
				player.isMoving = true;
			}
			if (wnd.kbd.KeyIsPressed('S'))
			{
				player.velocity = player.velocity + Vec2{ 0.0f, -0.02f } *playerRotation * (player.energy / player.maxEnergy);
				player.ExpendEnergy(0.01f);
				player.isMoving = true;
			}
			if (wnd.kbd.KeyIsPressed('A'))
			{
				player.velocity = player.velocity + Vec2{ -0.02f, 0.0f } *playerRotation * (player.energy / player.maxEnergy);
				player.ExpendEnergy(0.01f);
				player.isFacingRight = false;
				player.isMoving = true;
			}
			if (wnd.kbd.KeyIsPressed('D'))
			{
				player.velocity = player.velocity + Vec2{ 0.02f, 0.0f } *playerRotation * (player.energy / player.maxEnergy);
				player.ExpendEnergy(0.01f);
				player.isFacingRight = true;
				player.isMoving = true;
			}

			if (wnd.kbd.KeyIsPressed('Q'))
			{
				if (!isPrevSeenPressed)
				{
					player.selectedSeed = (PLANT_TYPE)(player.selectedSeed - 1);
					isPrevSeenPressed = true;
				}
			}
			else
			{
				isPrevSeenPressed = false;
			}
			if (wnd.kbd.KeyIsPressed('E'))
			{
				if (!isNextSeedPressed)
				{
					player.selectedSeed = (PLANT_TYPE)(player.selectedSeed + 1);
					isNextSeedPressed = true;
				}
			}
			else
			{
				isNextSeedPressed = false;
			}

			player.selectedSeed = (PLANT_TYPE)(player.selectedSeed < 0 ? 0 : player.selectedSeed);
			player.selectedSeed = (PLANT_TYPE)(player.selectedSeed >= MAX_PLANT_NUM ? MAX_PLANT_NUM - 1 : player.selectedSeed);


			if (wnd.kbd.KeyIsPressed(VK_SPACE))
			{
				if (player.centerBotLoc.GetMagnitude() - world.radius < 10.0f)
				{
					plants.push_back(Plant(player.centerBotLoc.GetNormalized() * world.radius, player.selectedSeed, plants.size()));
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

			if (wnd.kbd.KeyIsPressed(VK_ESCAPE))
			{
				if (!isPausedPressed)
				{
					gamestate = GAMESTATE_INGAMEMENU;
					isPausedPressed = true;
					selectedOption = 0;
				}
			}
			else
			{
				isPausedPressed = false;
			}

		}
	}
	else if (gamestate == GAMESTATE_RESTARTMENU)
	{
		if (wnd.kbd.KeyIsPressed('W'))
		{
			selectedOption = 0;
		}
		if (wnd.kbd.KeyIsPressed('S'))
		{
			selectedOption = 1;
		}

		if (wnd.kbd.KeyIsPressed(VK_RETURN) || wnd.kbd.KeyIsPressed(VK_SPACE))
		{
			if (!isActionedPressed)
			{
				if (selectedOption == 0)
				{
					gamestate = GAMESTATE_RESTARTING;
				}
				else
				{
					gamestate = GAMESTATE_STARTMENU;
				}
				isActionedPressed = true;
				selectedOption = 0;
			}
		}
		else
		{
			isActionedPressed = false;
		}
	}
	else if (gamestate == GAMESTATE_INGAMEMENU)
	{
		if (wnd.kbd.KeyIsPressed('W'))
		{
			selectedOption = 0;
		}
		if (wnd.kbd.KeyIsPressed('S'))
		{
			selectedOption = 1;
		}

		if (wnd.kbd.KeyIsPressed(VK_ESCAPE))
		{
			if (!isPausedPressed)
			{
				gamestate = GAMESTATE_RUNNING;
				isPausedPressed = true;
				selectedOption = 0;
			}
		}
		else
		{
			isPausedPressed = false;
		}

		if (wnd.kbd.KeyIsPressed(VK_RETURN) || wnd.kbd.KeyIsPressed(VK_SPACE))
		{
			if (!isActionedPressed)
			{
				if (selectedOption == 0)
				{
					gamestate = GAMESTATE_RESTARTING;
				}
				else
				{
					gamestate = GAMESTATE_STARTMENU;
				}
				isActionedPressed = true;
				selectedOption = 0;
			}
		}
		else
		{
			isActionedPressed = false;
		}
	}
	else if (gamestate == GAMESTATE_STARTMENU)
	{
		if (wnd.kbd.KeyIsPressed('W'))
		{
			selectedOption = 0;
		}
		if (wnd.kbd.KeyIsPressed('S'))
		{
			selectedOption = 1;
		}

		if (wnd.kbd.KeyIsPressed(VK_RETURN) || wnd.kbd.KeyIsPressed(VK_SPACE))
		{
			if (!isActionedPressed)
			{
				if (selectedOption == 0)
				{
					gamestate = GAMESTATE_RESTARTING;
				}
				else
				{
					gamestate = GAMESTATE_GAMEOVER;
				}
				isActionedPressed = true;
				selectedOption = 0;
			}
		}
		else
		{
			isActionedPressed = false;
		}
	}

}

void Game::UpdateModel()
{
	if (gamestate == GAMESTATE::GAMESTATE_STARTSCREEN)
	{
		if (fadingIn)
		{
			currentFadeInEffect += 3.0f;
			if (currentFadeInEffect >= maxFadeInEffect)
			{
				fadingIn = false;
			}
		}
		else
		{
			currentFadeInEffect -= 3.0f;
		}
		
		
		if (currentFadeInEffect < 0.0f)
		{
			currentFadeInEffect = 0.0f;
			gamestate = GAMESTATE::GAMESTATE_STARTMENU;
		}
	}
	else if (gamestate == GAMESTATE_RESTARTING)
	{
		camera = Camera();
		player = Player();
		world = World();
		dome = Dome();
		habitat = Habitat();
		spaceship = Spaceship();
		plants = std::vector<Plant>();

		currentFadeInEffect = 0.0f;
		selectedOption = 0;

		gamestate = GAMESTATE_RUNNING;
	}
	else if (gamestate == GAMESTATE_RUNNING)
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

		if (!((newPosition + Vec2{ player.height * sin(player.angle), player.height * cos(player.angle) } -dome.loc).GetMagnitudeSqrd() > (dome.radius * dome.radius)))
		{
			player.centerBotLoc = newPosition;
		}
		else
		{
			Vec2 normal = player.centerBotLoc / (dome.radius - player.height);
			player.velocity = player.velocity - normal * 2.0f * (player.velocity.Dot(normal));
		}

		if (vectorFromPlayerToWorld.GetMagnitude() < 800.0f)
		{
			player.velocity = player.velocity - vectorFromPlayerToWorld.GetNormalized() * (800.0f - vectorFromPlayerToWorld.GetMagnitude()) * 0.5f;
		}

		float sinAngle = sin(player.animationAngle);

		player.headLoc = newPosition + player.headLocOffset;
		player.bodyLoc = newPosition + player.bodyLocOffset;
		player.leftLegLoc = newPosition + player.leftLegLocOffset + player.legAnimationOffset * (-sinAngle);
		player.rightLegLoc = newPosition + player.rightLegLocOffset + player.legAnimationOffset * (sinAngle);
		player.leftArmLoc = newPosition + player.leftArmLocOffset + player.armAnimationOffset * (sinAngle);
		player.rightArmLoc = newPosition + player.rightArmLocOffset + player.armAnimationOffset * (-sinAngle);

		if (player.isMoving)
		{
			player.animationAngle += 0.05f;
		}
		else
		{
			player.animationAngle -= 0.01f;
		}

		// player cutting applied to plants

		if (player.isCutting)
		{
			Vec2 cuttingLocation = player.centerBotLoc + (player.isFacingRight ? Vec2(40.0f, 10.0f) : Vec2(-40.0f, 10.0f)) * Mat2::RotationMatrix(player.angle);
			for (int i = 0; i < plants.size(); i++)
			{
				if ((cuttingLocation - (plants[i].centerBotLoc)).GetMagnitude() < (plants[i].currentSize + 15.0f))
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
			if (plants[i].currentCutAmount >= plants[i].maxCutAmount * (plants[i].currentSize / plants[i].maxSize))
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
			if (plants[i].CanBreathe(&dome.atmosphere) && !plants[i].isDead && !plants[i].isBeingCut)
			{
				plants[i].Breathe(&dome.atmosphere);
			}
			else
			{
				//plants[i].isDead = true;
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
				player.centerBotLoc = Vec2(-830.0f, -70.0f);
				player.velocity = Vec2(0.0f, 0.0f);
			}
			else
			{
				player.centerBotLoc = habitat.doorLoc;
			}

		}


		for (int i = 0; i < plants.size(); i++)
		{
			if (plants[i].currentSize > 200.0f)
			{
				player.isAlive = false;
				player.result = WINLOSE::LOSEBYBREAK;
			}
		}


		// check if game over

		if (!player.isAlive)
		{
			//gameOver = true;
			gamestate = GAMESTATE_RESTARTMENU;
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

		if (spaceship.arrivalTime > 0)
		{
			spaceship.arrivalTime -= ((spaceship.zChange / 5.0f) / 60.0f);
		}
		else
		{
			spaceship.arrivalTime = 0;
		}

	}
}

void Game::ComposeFrame()
{
	float fadeInAlpha = currentFadeInEffect / maxFadeInEffect;


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
		gfx.DrawCircle(((planetLoc - camera.loc / (1000.0f * planetZ)) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, planetRadius, Color(0, 255, 0));
	}
	else
	{
		gfx.DrawCircle(((planetLoc - camera.loc / (1000.0f * planetZ)) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, planetRadius, Color(0, 255, 0));
		gfx.DrawCircle(((moonLoc - camera.loc / (1000.0f * moonZ)) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, moonRadius / moonZ, Colors::Gray);
	}

	gfx.DrawCircle(((spaceship.loc - camera.loc / (spaceship.zLoc)) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, spaceship.radius / spaceship.zLoc, spaceship.color);

	gfx.DrawCircleWithIncreasingAlphaToEdge(((dome.loc - camera.loc) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, dome.radius, dome.GetAtmosphereCombinedColor(), world.radius, 0.1f, 0.5f);




	for (int i = 0; i < plants.size(); i++)
	{
		gfx.DrawCircle(((plants[i].centerBotLoc - camera.loc) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, plants[i].currentSize, plants[i].color);
	}




	gfx.DrawCircle(((habitat.hutLoc - camera.loc) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, habitat.hutHeightRadius, habitat.hutColor);
	gfx.DrawCircle(((habitat.doorLoc - camera.loc) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, habitat.doorHeightRadius, habitat.doorColor);
	if (player.isSleeping)
	{
		//gfx.DrawRectWithinCircle((player.centerBotLoc + player.transformShift - camera.loc) * screenTransformFlip + screenTransformShift, (int)player.width, (int)player.height, Colors::Blue, ((habitat.windowLoc - camera.loc) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, habitat.windowRadius, (player.isAlive ? 1.0f : powf(1.0f - fadeInAlpha, 2.0f)));
		gfx.DrawCircleWithinCircle((player.headLoc + Vec2{0.0f, 0.0f} - camera.loc) * screenTransformFlip + screenTransformShift, player.headRadius, Color(100, 50, 255), ((habitat.windowLoc - camera.loc) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, habitat.windowRadius, (player.isAlive ? 1.0f : powf(1.0f - fadeInAlpha, 2.0f)));
		gfx.DrawCircleWithinCircle((player.bodyLoc + Vec2{ 0.0f, 0.0f } - camera.loc) * screenTransformFlip + screenTransformShift, player.bodyRadius, Colors::Blue, ((habitat.windowLoc - camera.loc) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, habitat.windowRadius, (player.isAlive ? 1.0f : powf(1.0f - fadeInAlpha, 2.0f)));
		

	}
	gfx.DrawCircle(((habitat.windowLoc - camera.loc) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, habitat.windowRadius, habitat.windowColor, 0.75f);

	if (player.isSleeping)
	{
		RetroContent::DrawString(gfx, std::string("Z"), { 818.0f, 390.0f }, 2, Colors::Yellow, ((cosf((float)player.sleepAnimation / 10.0f) + 1) / 2.0f) * (player.isAlive ? 1.0f : powf(1.0f - fadeInAlpha, 2.0f)));
		((player.sleepAnimation / 10.0f) > (PI / 4.0f)) ? RetroContent::DrawString(gfx, std::string("Z"), { 832.0f, 375.0f }, 2, Colors::Yellow, ((sinf((float)player.sleepAnimation / 10.0f + PI / 4.0f) + 1) / 2.0f) * (player.isAlive ? 1.0f : powf(1.0f - fadeInAlpha, 2.0f))) : true;
		((player.sleepAnimation / 10.0f) > (PI / 2.0f)) ? RetroContent::DrawString(gfx, std::string("Z"), { 844.0f, 360.0f }, 2, Colors::Yellow, ((sinf((float)player.sleepAnimation / 10.0f) + 1) / 2.0f) * (player.isAlive ? 1.0f : powf(1.0f - fadeInAlpha, 2.0f))) : true;
		player.sleepAnimation++;
	}
	else
	{
		player.sleepAnimation = 0;
	}

	if (!player.isSleeping)
	{
		gfx.DrawCircle((player.headLoc - camera.loc) * screenTransformFlip + screenTransformShift, player.headRadius, Color(100, 50, 255), (player.isAlive ? 1.0f : powf(1.0f - fadeInAlpha, 2.0f)));
		if (player.isFacingRight)
		{
			gfx.DrawCircle((player.leftArmLoc - camera.loc) * screenTransformFlip + screenTransformShift, player.leftArmRadius, Color(100, 0, 205), (player.isAlive ? 1.0f : powf(1.0f - fadeInAlpha, 2.0f)));
			gfx.DrawCircle((player.leftLegLoc - camera.loc) * screenTransformFlip + screenTransformShift, player.leftLegRadius, Color(0, 100, 150), (player.isAlive ? 1.0f : powf(1.0f - fadeInAlpha, 2.0f)));
			gfx.DrawCircle((player.bodyLoc - camera.loc) * screenTransformFlip + screenTransformShift, player.bodyRadius, Colors::Blue, (player.isAlive ? 1.0f : powf(1.0f - fadeInAlpha, 2.0f)));
			gfx.DrawCircle((player.rightLegLoc - camera.loc) * screenTransformFlip + screenTransformShift, player.rightLegRadius, Color(50, 150, 200), (player.isAlive ? 1.0f : powf(1.0f - fadeInAlpha, 2.0f)));
			gfx.DrawCircle((player.rightArmLoc - camera.loc) * screenTransformFlip + screenTransformShift, player.rightArmRadius, Color(150, 50, 255), (player.isAlive ? 1.0f : powf(1.0f - fadeInAlpha, 2.0f)));

		}
		else
		{
			gfx.DrawCircle((player.rightArmLoc - camera.loc) * screenTransformFlip + screenTransformShift, player.rightArmRadius, Color(150, 50, 255), (player.isAlive ? 1.0f : powf(1.0f - fadeInAlpha, 2.0f)));
			gfx.DrawCircle((player.rightLegLoc - camera.loc) * screenTransformFlip + screenTransformShift, player.rightLegRadius, Color(50, 150, 200), (player.isAlive ? 1.0f : powf(1.0f - fadeInAlpha, 2.0f)));
			gfx.DrawCircle((player.bodyLoc - camera.loc) * screenTransformFlip + screenTransformShift, player.bodyRadius, Colors::Blue, (player.isAlive ? 1.0f : powf(1.0f - fadeInAlpha, 2.0f)));
			gfx.DrawCircle((player.leftLegLoc - camera.loc) * screenTransformFlip + screenTransformShift, player.leftLegRadius, Color(0, 100, 150), (player.isAlive ? 1.0f : powf(1.0f - fadeInAlpha, 2.0f)));
			gfx.DrawCircle((player.leftArmLoc - camera.loc) * screenTransformFlip + screenTransformShift, player.leftArmRadius, Color(100, 0, 205), (player.isAlive ? 1.0f : powf(1.0f - fadeInAlpha, 2.0f)));
		}
	}
	//gfx.DrawCircle((player.centerBotLoc + player.transformShiftCircle - camera.loc) * screenTransformFlip + screenTransformShift, 2.0f, Colors::Red);

	if (player.isCutting)
	{
		Vec2 cuttingLocation = player.centerBotLoc + (player.isFacingRight ? Vec2(40.0f, 10.0f) : Vec2(-40.0f, 10.0f)) * Mat2::RotationMatrix(player.angle);

		Vec2 axeHitLoc = player.isFacingRight ? Vec2(40.0f, 10.0f) : Vec2(-40.0f, 10.0f);
		gfx.DrawCircle((cuttingLocation - camera.loc) * cameraRotation.GetTranspose() * screenTransformFlip + screenTransformShift, 15.0f, Colors::Red);
	}



	gfx.DrawCircleWithIncreasingAlphaToEdge(((dome.loc - camera.loc) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, dome.radius, dome.GetAtmosphereCombinedColor(), world.radius, 0.1f, 0.5f);

	gfx.DrawCircle(((world.loc - camera.loc) * cameraRotation.GetTranspose()) * screenTransformFlip + screenTransformShift, world.radius, Colors::Gray);


	

	if (gamestate == GAMESTATE_RUNNING)
	{
		// GUI

		


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


		RetroContent::DrawString(gfx, "SEED TYPE", { 800.0f, 680.0f }, 3, Colors::Red);
		gfx.DrawRect(Vec2(760.0f, 720.0f), 80.0f, 80.0f, Colors::Black);
		gfx.DrawRect(Vec2(765.0f, 725.0f), 70.0f, 70.0f, player.GetSelectedSeedColor());



		if (player.isCutting)
		{
			for (int i = 0; i < plants.size(); i++)
			{
				if (plants[i].isBeingCut)
				{
					gfx.DrawRect((player.centerBotLoc + Vec2(-60.0f, -60.0f) - camera.loc) * screenTransformFlip + screenTransformShift, 120.0f, 15.0f, Colors::Black);
					gfx.DrawRect((player.centerBotLoc + Vec2(-57.5f, -62.5f) - camera.loc) * screenTransformFlip + screenTransformShift, 115.0f * (plants[i].currentCutAmount / (plants[i].maxCutAmount* (plants[i].currentSize / plants[i].maxSize))), 10.0f, Colors::Blue);
					break;
				}
			}
		}
	}

	if (gamestate == GAMESTATE::GAMESTATE_STARTSCREEN)
	{
		RetroContent::DrawString(gfx, std::string("BY"), { 800.0f, 100.0f }, 6, Color(128, 0, 128), sqrt(fadeInAlpha));
		RetroContent::DrawString(gfx, std::string("SOLLY"), { 800.0f, 280.0f }, 12, Color(128, 0, 128), sqrt(fadeInAlpha));
	}
	else if (gamestate == GAMESTATE::GAMESTATE_RESTARTMENU)
	{
		std::string loseCondition;

		if (player.result == WINLOSE::LOSEBYAIR)
		{
			loseCondition = "TOO MUCH OF THE OLD CARBON";
		}
		if (player.result == WINLOSE::LOSEBYBREAK)
		{
			loseCondition = "DOME BREACH";
		}
		if (player.result == WINLOSE::LOSEBYFOOD)
		{
			loseCondition = "DEATH BY FOOD POLICE";
		}
		if (player.result == WINLOSE::LOSEBYENERGY)
		{
			loseCondition = "TOO TIRED TO GO ON";
		}

		RetroContent::DrawString(gfx, std::string("ALAS"), { 800.0f, 100.0f }, 12, Color(128, 0, 128), sqrt(fadeInAlpha));
		RetroContent::DrawString(gfx, std::string("YOU ARE NO LONGER"), { 800.0f, 280.0f }, 12, Color(128, 0, 128), sqrt(fadeInAlpha));

		RetroContent::DrawString(gfx, loseCondition, { 800.0f, 460.0f }, 12, Colors::Red, sqrt(fadeInAlpha));

		RetroContent::DrawString(gfx, std::string("RESTART"), { 800.0f, 650.0f }, 6, selectedOption == 0 ? Colors::Red : Colors::Yellow, fadeInAlpha);
		RetroContent::DrawString(gfx, std::string("MAIN MENU"), { 800.0f, 750.0f }, 6, selectedOption == 1 ? Colors::Red : Colors::Yellow, fadeInAlpha);

		if (selectedOption == 0)
		{
			gfx.DrawCircle({ 500.0f, 655.0f }, 20.0f, Colors::Red, fadeInAlpha);
			gfx.DrawCircle({ 1100.0f, 655.0f }, 20.0f, Colors::Red, fadeInAlpha);
		}
		else
		{
			gfx.DrawCircle({ 500.0f, 775.0f }, 20.0f, Colors::Red, fadeInAlpha);
			gfx.DrawCircle({ 1100.0f, 775.0f }, 20.0f, Colors::Red, fadeInAlpha);
		}




		if (currentFadeInEffect < maxFadeInEffect)
		{
			currentFadeInEffect += 1.0f;
		}
		else
		{
			currentFadeInEffect = maxFadeInEffect;
		}
	}
	else if (gamestate == GAMESTATE::GAMESTATE_INGAMEMENU)
	{
		RetroContent::DrawString(gfx, std::string("GAME PAUSED"), { 800.0f, 190.0f }, 12, Color(128, 0, 128), sqrt(fadeInAlpha));

		RetroContent::DrawString(gfx, std::string("RESTART"), { 800.0f, 650.0f }, 6, selectedOption == 0 ? Colors::Red : Colors::Yellow, fadeInAlpha);
		RetroContent::DrawString(gfx, std::string("MAIN MENU"), { 800.0f, 750.0f }, 6, selectedOption == 1 ? Colors::Red : Colors::Yellow, fadeInAlpha);

		if (selectedOption == 0)
		{
			gfx.DrawCircle({ 500.0f, 655.0f }, 20.0f, Colors::Red, fadeInAlpha);
			gfx.DrawCircle({ 1100.0f, 655.0f }, 20.0f, Colors::Red, fadeInAlpha);
		}
		else
		{
			gfx.DrawCircle({ 500.0f, 775.0f }, 20.0f, Colors::Red, fadeInAlpha);
			gfx.DrawCircle({ 1100.0f, 775.0f }, 20.0f, Colors::Red, fadeInAlpha);
		}

		if (currentFadeInEffect < maxFadeInEffect)
		{
			currentFadeInEffect += 4.0f;
		}
		else
		{
			currentFadeInEffect = maxFadeInEffect;
		}
	}
	else if (gamestate == GAMESTATE::GAMESTATE_STARTMENU)
	{
		currentFadeInEffect = maxFadeInEffect;

		RetroContent::DrawString(gfx, std::string("GARDENERS"), { 800.0f, 100.0f }, 10, Color(250, 210, 90), sqrt(fadeInAlpha));
		RetroContent::DrawString(gfx, std::string("OF THE"), { 800.0f, 210.0f }, 4, Color(250, 210, 90), sqrt(fadeInAlpha));
		RetroContent::DrawString(gfx, std::string("GALAXY"), { 800.0f, 280.0f }, 10, Color(250, 210, 90), sqrt(fadeInAlpha));


		RetroContent::DrawString(gfx, std::string("START GAME"), { 800.0f, 650.0f }, 6, selectedOption == 0 ? Colors::Red : Colors::Yellow, fadeInAlpha);
		RetroContent::DrawString(gfx, std::string("EXIT"), { 800.0f, 750.0f }, 6, selectedOption == 1 ? Colors::Red : Colors::Yellow, fadeInAlpha);

		if (selectedOption == 0)
		{
			gfx.DrawCircle({ 500.0f, 655.0f }, 20.0f, Colors::Red, fadeInAlpha);
			gfx.DrawCircle({ 1100.0f, 655.0f }, 20.0f, Colors::Red, fadeInAlpha);
		}
		else
		{
			gfx.DrawCircle({ 500.0f, 775.0f }, 20.0f, Colors::Red, fadeInAlpha);
			gfx.DrawCircle({ 1100.0f, 775.0f }, 20.0f, Colors::Red, fadeInAlpha);
		}

		if (currentFadeInEffect < maxFadeInEffect)
		{
			currentFadeInEffect += 4.0f;
		}
		else
		{
			currentFadeInEffect = maxFadeInEffect;
		}
	}
	else
	{
		currentFadeInEffect = 0.0f;
	}
}
