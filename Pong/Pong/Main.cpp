//
//  Main.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"

int main(int argc, char** argv)
{
	Game game;

	if (game.Intialize())
	{
		game.RunLoop();
	}

	game.Shutdown();

	return 0;
}
