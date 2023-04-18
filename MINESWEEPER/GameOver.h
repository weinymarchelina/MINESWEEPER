#include "main.h"

class GameOver
{
public:
	void replay(State& gameState)
	{
		gameState = standByState;
	}

	void quit(State& gameState, bool& isExit)
	{
		gameState = startState;
		isExit = true;
	}

};