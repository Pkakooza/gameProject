#pragma once
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define WALL_SIZE 15
#define PADDLE_WIDTH 15
#define PADDLE_HEIGHT 100
#define PADDLE1_X_OFFSET 15
#define PADDLE2_X_OFFSET (WINDOW_WIDTH - 15)
#define BALL_RADIUS 15
#define BALL_START_X_VELOCITY -200
#define BALL_START_Y_VELOCITY 235
#define BALLS_NUM 1

#include "SDL2/SDL.h"
#include <vector>

class Paddle
{
	friend class Game;
private:
	SDL_Point pos;
	int h = PADDLE_HEIGHT;
	int w = PADDLE_WIDTH;
	int dir = 0;
public:
	Paddle()
	{}
	Paddle(bool player1)
	{
		if (player1)
		{
			pos.x = PADDLE1_X_OFFSET;
		}
		else
		{
			pos.x = PADDLE2_X_OFFSET;
		}
		pos.y = WINDOW_HEIGHT / 2;
	}
};

class Ball
{
	friend class Game;
private:
	SDL_Point pos{ WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 };
	SDL_Point vel{ -200, 345 };
	int r = BALL_RADIUS;
public:
	Ball()
	{}
};

class Game
{
private:
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Paddle mPaddle1 = Paddle(true);
	Paddle mPaddle2 = Paddle(false);

	Uint32 mTicksCount = SDL_GetTicks();
	float mDeltaTime;

	bool mIsRunning = true;

	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	void UpdateDeltaTime();
	void UpdatePaddle(Paddle& mPaddle);
	void UpdateBall(Ball& mBall);
	void UpdateBalls();
	std::vector<Ball*> mBalls;

public:
	Game();
	bool Intialize();
	void Shutdown();
	void RunLoop();
};