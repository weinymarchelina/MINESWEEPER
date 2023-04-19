#ifndef GAMEOVER_H
#define GAMEOVER_H

#include "main.h"

class GameOver
{
public:
	// Intent: Replay the game or quit and exit the program
	// Pre: gameState must be valid and isExit must be initialized to false
	// Pos: Changes the gameState to standByState
	void replay(STATE& gameState)
	{
		gameState = STANDBY;
	}

	// Intent: Quit the game and exit the program
	// Pre: gameState must be valid and isExit must be initialized to false
	// Pos: Changes the gameState to startState and sets isExit to true
	void quit(STATE& gameState, bool& isExit)
	{
		gameState = START;
		isExit = true;
	}

};

#endif // GAMEOVER_H
