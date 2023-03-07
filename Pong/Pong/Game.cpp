//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//
#include "Game.h"
#include <stdio.h>

Game::Game()
	: mWindow(nullptr)
	, mRenderer(nullptr)
	, mIsRunning(true)
	, mTicksCount(0)
	, mDeltaTime(0.0f)
{}

bool Game::Intialize()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// create window
	mWindow = SDL_CreateWindow("Pong!", // Window Title 
		SDL_WINDOWPOS_CENTERED,	// Top left x-coordinate of window
		SDL_WINDOWPOS_CENTERED,	// Top left y-coordinate of window 
		WINDOW_WIDTH,           // Width of window 
		WINDOW_HEIGHT,          // Height of window 
		0                       // Flags (0 for no flags set)
	);

	// if window failed to be created
	if (mWindow == NULL)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// create renderer 
	mRenderer = SDL_CreateRenderer(
		mWindow, // Window to create renderer for 
		-1,      // Usually -1 
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	// if renderer failed to be created
	if (mRenderer == NULL)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	// intialize mBalls
	for (size_t i = 0; i < BALLS_NUM; ++i)
	{
		Ball* mBall = new Ball();
		mBall->pos.x = WINDOW_WIDTH / 2;
		mBall->pos.y = WINDOW_HEIGHT / 2;

		switch (i) {
		case 0:
			mBall->vel.x = -145;
			mBall->vel.y = 145;
			break;

		case 1:
			mBall->vel.x = -187;
			mBall->vel.y = -56;
			break;
		case 2:
			mBall->vel.x = 75;
			mBall->vel.y = 100;
			break;
		case 3:
			mBall->vel.x = 154;
			mBall->vel.y = -87;
			break;
		default:
			mBall->vel.x = 50;
			mBall->vel.y = 50;
			break;
		}
		mBalls.emplace_back(mBall);
	}
	return true;
}

void Game::ProcessInput()
{
	SDL_Event event;

	// while there are still events in the queue 

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			// Handle different event types here 

			case SDL_QUIT:
				mIsRunning = false;
				break;

			default:
				break;
		}


	}

	// Get state of keyboard 

	const Uint8* state = SDL_GetKeyboardState(NULL);

	// If escape is pressed, also end loop 
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}	

	// if up or w is pressed, move paddle1 up
	if (state[SDL_SCANCODE_W])
	{
		mPaddle1.dir -= 1;
	}

	// if down or s is pressed, move padddle1 down
	if (state[SDL_SCANCODE_S])
	{
		mPaddle1.dir += 1;
	}

	// if i is pressed, move paddle2 up
	if (state[SDL_SCANCODE_I])
	{
		mPaddle2.dir -= 1;
	}

	// if k is pressed, move paddle2 down
	if (state[SDL_SCANCODE_K])
	{
		mPaddle2.dir += 1;
	}
}

void Game::UpdateGame()
{
	// find/calucate deltaTime, specifics abstracted away in function getDeltaTime()
	UpdateDeltaTime();

	//update position of paddles if needed 
	UpdatePaddle(mPaddle1);
	UpdatePaddle(mPaddle2);

	//update position of balls if needed 
	UpdateBalls();
}

void  Game::UpdateDeltaTime()
{
	Uint32 currTick = SDL_GetTicks();

	// set frame limit by using while loop
	while (currTick - mTicksCount < 16)
	{
		currTick = SDL_GetTicks();
	}

	// calculate delta time 
	mDeltaTime = static_cast<float>(currTick - mTicksCount) / 1000.0f;

	// capping maximum delta time
	if (mDeltaTime > 0.033f)
	{
		mDeltaTime = 0.033f;
	}

	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks();
}

void Game::UpdatePaddle(Paddle& mPaddle)
{
	// if paddle needs to move up or down
	if (mPaddle.dir != 0)
	{
		// update paddle position with set speed
		mPaddle.pos.y += mPaddle.dir * static_cast<int>(300.0f * mDeltaTime);

		// check if paddle went past wall bounds 
		if (mPaddle.pos.y - mPaddle.h / 2 <= 0 + WALL_SIZE)
		{
			mPaddle.pos.y = mPaddle.h / 2 + WALL_SIZE;
		}
		else if (mPaddle.pos.y + mPaddle.h / 2 >= WINDOW_HEIGHT - WALL_SIZE)
		{
			mPaddle.pos.y = WINDOW_HEIGHT - mPaddle.h / 2 - WALL_SIZE;
		}
		mPaddle.dir = 0;
	}
}

void Game::UpdateBall(Ball& mBall)
{
	// update position of ball using velocities
	mBall.pos.x += static_cast<int>(static_cast<float>(mBall.vel.x) * mDeltaTime);
	mBall.pos.y += static_cast<int>(static_cast<float>(mBall.vel.y) * mDeltaTime);

	// handle wall collisions
	// hit top wall
	if (mBall.pos.y - mBall.r / 2 <= WALL_SIZE)
	{
		mBall.vel.y *= -1;
		mBall.pos.y = mBall.r + WALL_SIZE;
	}

	// hit bottom wall
	else if (mBall.pos.y + mBall.r / 2 >= WINDOW_HEIGHT - WALL_SIZE)
	{
		mBall.vel.y *= -1;
		mBall.pos.y = WINDOW_HEIGHT - mBall.r - WALL_SIZE;
	}

	// hit right wall
	//if (mBall.pos.x + mBall.r / 2 >= WINDOW_WIDTH - WALL_SIZE)
	//{
	//	mBall.vel.x *= -1;
	//	mBall.pos.x = WINDOW_WIDTH - mBall.r - WALL_SIZE;
	//}

	// hit paddle1 (kinda acts like left wall)  
	else if (mBall.pos.x - mBall.r / 2 <= mPaddle1.pos.x + mPaddle1.w / 2
		&& (mBall.pos.y - mBall.r / 2 <= mPaddle1.pos.y + mPaddle1.h / 2
			&& mBall.pos.y + mBall.r / 2 >= mPaddle1.pos.y - mPaddle1.h / 2))
	{
		mBall.vel.x *= -1;
		mBall.pos.x = mPaddle1.pos.x + mPaddle1.w / 2 + mBall.r / 2;
	}

	// hit paddle2 (kinda acts like right wall)
	else if (mBall.pos.x + mBall.r / 2 >= mPaddle2.pos.x - mPaddle1.w / 2
		&& (mBall.pos.y - mBall.r / 2 <= mPaddle2.pos.y + mPaddle2.h / 2
			&& mBall.pos.y + mBall.r / 2 >= mPaddle2.pos.y - mPaddle2.h / 2))
	{
		mBall.vel.x *= -1;
		mBall.pos.x = mPaddle2.pos.x - mPaddle2.w / 2 - mBall.r / 2;
	}

	// ball went off screen 
	if (mBall.pos.x < 0 || mBall.pos.x > WINDOW_WIDTH)
	{
		mIsRunning = false;
	}
}

void Game::UpdateBalls()
{
	for (size_t i = 0; i < mBalls.size(); ++i)
	{
		UpdateBall(*mBalls[i]);
	}
}

void Game::GenerateOutput()
{
	// set draw color 
	SDL_SetRenderDrawColor(
		mRenderer, 
		0,   // R
		0,   // G
		255, // B
		255  // A
	);

	// clear back buffer 
	SDL_RenderClear(mRenderer);

	// all draw objects must be created BELOW here
	
	// draw border walls
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	SDL_Rect topRect{ 0, 0, WINDOW_WIDTH, WALL_SIZE };
	SDL_Rect bottomRect{ 0, (WINDOW_HEIGHT - WALL_SIZE), WINDOW_WIDTH, WALL_SIZE };
	//SDL_Rect rightRect{ (WINDOW_WIDTH - WALL_SIZE), 0, WINDOW_HEIGHT, WALL_SIZE };

	SDL_RenderFillRect(mRenderer, &topRect);
	SDL_RenderFillRect(mRenderer, &bottomRect);
	// SDL_RenderFillRect(mRenderer, &rightRect);

	// draw paddle for player 1 
	SDL_Rect paddle1Rect{ mPaddle1.pos.x - mPaddle1.w / 2, mPaddle1.pos.y - mPaddle1.h / 2, mPaddle1.w, mPaddle1.h };
	SDL_RenderFillRect(mRenderer, &paddle1Rect);

	// draw paddle for player 2
	SDL_Rect paddle2Rect{ mPaddle2.pos.x - mPaddle2.w / 2, mPaddle2.pos.y - mPaddle2.h / 2, mPaddle2.w, mPaddle2.h };
	SDL_RenderFillRect(mRenderer, &paddle2Rect);

	// draw balls
	for (size_t i = 0; i < mBalls.size(); ++i)
	{
		SDL_Rect ballRect{ mBalls[i]->pos.x - mBalls[i]->r / 2, mBalls[i]->pos.y - mBalls[i]->r / 2, mBalls[i]->r,  mBalls[i]->r };
		SDL_RenderFillRect(mRenderer, &ballRect);
	}

	// all draw objects created ABOVE here

	SDL_RenderPresent(mRenderer);
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}



