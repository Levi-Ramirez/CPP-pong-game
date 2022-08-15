#define isDown(x) input->buttons[x].down
#define pressed(x) (input->buttons[x].down && input->buttons[x].change)
#define release(x) (!input->buttons[x].down && input->buttons[x].change)

float player1PosY = 0;
float player1Velocity;
float player2PosY = 0;
float player2Velocity;

float arenaHalfSizeX = 85;
float arenaHalfSizeY = 45;
float playerHalfSizeX = 2.5;
float playerHalfSizeY = 12;

float ballPosX, ballPosY, ballVelX = 100, ballVelY, ballHalfSize = 1;

int player1Score, player2Score;

static void simulatPlayer(float *pos, float *vel, float acc, float time)
{
	acc -= *vel * 10;

	*pos = *pos + (*vel * time) + (acc * time * time * 0.5);
	*vel = *vel + acc * time;

	if (*pos + playerHalfSizeY > arenaHalfSizeY)
	{
		*pos = arenaHalfSizeY - playerHalfSizeY;
		*vel = 0;
	}
	else if (*pos - playerHalfSizeY < -arenaHalfSizeY)
	{
		*pos = -arenaHalfSizeY + playerHalfSizeY;
		*vel = 0;
	}
}

enum Gamemode {
	GM_MENU,
	GM_GAMEPLAY
};
Gamemode myGamemode;


static void simulateGame(Input* input, float dt)
{
	clearScreen(0x25c11f);

	drawRect(0, 0, arenaHalfSizeX, arenaHalfSizeY, 0x000000);





		float player1Acceleration = 0;
#if 1
		if (isDown(BUTTON_UP))
		{
			player1Acceleration += 2000;
		}
		if (isDown(BUTTON_DOWN))
		{
			player1Acceleration -= 2000;
		}
#else
		player1Acceleration = (ballPosY - player1PosY) * 100;
		if (player1Acceleration > 1400) player1Acceleration = 1400;
		if (player1Acceleration < -1400) player1Acceleration = -1400;
#endif
		float player2Acceleration = 0;
		if (isDown(BUTTON_W))
		{
			player2Acceleration += 2000;
		}
		if (isDown(BUTTON_S))
		{
			player2Acceleration -= 2000;
		}


		simulatPlayer(&player1PosY, &player1Velocity, player1Acceleration, dt);
		simulatPlayer(&player2PosY, &player2Velocity, player2Acceleration, dt);



		//simulate ball
		{
			ballPosX += ballVelX * dt;
			ballPosY += ballVelY * dt;

			drawRect(ballPosX, ballPosY, ballHalfSize, ballHalfSize, 0xffffff);

			if (ballPosX + ballHalfSize > 80 - playerHalfSizeX &&
				ballPosX - ballHalfSize < 80 + playerHalfSizeX &&
				ballPosY + ballHalfSize > player1PosY - playerHalfSizeY &&
				ballPosY + ballHalfSize < player1PosY + playerHalfSizeY)
			{
				ballPosX = 80 - playerHalfSizeX - ballHalfSize;
				ballVelX *= -1;
				ballVelY = (ballPosY - player1PosY) * 2 + player1Velocity * 0.75;
			}
			else if (ballPosX + ballHalfSize > -80 - playerHalfSizeX &&
				ballPosX - ballHalfSize < -80 + playerHalfSizeX &&
				ballPosY + ballHalfSize > player2PosY - playerHalfSizeY &&
				ballPosY + ballHalfSize < player2PosY + playerHalfSizeY)
			{
				ballPosX = -80 + playerHalfSizeX + ballHalfSize;
				ballVelX *= -1;
				ballVelY = (ballPosY - player2PosY) * 2 + player2Velocity * 0.75;
			}

			if (ballPosY + ballHalfSize > arenaHalfSizeY)
			{
				ballPosY = arenaHalfSizeY - ballHalfSize;
				ballVelY *= -1;
			}
			else if (ballPosY - ballHalfSize < -arenaHalfSizeY)
			{
				ballPosY = -arenaHalfSizeY + ballHalfSize;
				ballVelY *= -1;
			}

			if (ballPosX + ballHalfSize > arenaHalfSizeX)
			{
				ballVelX *= -1;
				ballVelY = 0;
				ballPosX = 0;
				ballPosY = 0;
				player1Score++;
			}
			else if (ballPosX - ballHalfSize < -arenaHalfSizeX)
			{
				ballVelX *= -1;
				ballVelY = 0;
				ballPosX = 0;
				ballPosY = 0;
				player2Score++;
			}
		}

		float scoreSpacing = -80;
		for (int i = 0; i < player1Score; i++)
		{
			drawRect(scoreSpacing, 47, 1, 1, 0xaaaaaa);
			scoreSpacing += 2.5;
		}
		scoreSpacing = 80;
		for (int i = 0; i < player2Score; i++)
		{
			drawRect(scoreSpacing, 47, 1, 1, 0xaaaaaa);
			scoreSpacing -= 2.5;
		}
		drawRect(80, player1PosY, playerHalfSizeX, playerHalfSizeY, 0x3fff00);
		drawRect(-80, player2PosY, playerHalfSizeX, playerHalfSizeY, 0x3fff00);
	/*
if (isDown(BUTTON_UP))
{
	playerPosY += speed * dt;
}
if (isDown(BUTTON_DOWN))
{
	playerPosY -= speed * dt;
}
if (isDown(BUTTON_RIGHT))
{
	playerPosX += speed * dt;
}
if (isDown(BUTTON_LEFT))
{
	playerPosX -= speed * dt;
}
	drawRect(playerPosX, playerPosY, 20, 8, 0xff0000);
*/ //old code

}