#include "main.h"

State gameState = startState;

bool isExit = false;

#include "Start.h"
#include "StandBy.h"
#include "Playing.h"
#include "GameOver.h"

int main()
{
    string inputMethod, inputFile, outputFile;
    cin >> inputMethod >> inputMethod >> inputFile >> outputFile;
    // Start gameStart("inputFile", "outputFile");
    // Start gameStart("input.txt", "output.txt");
    Start gameStart("standByInput.txt", "output.txt");
    gameStart.readInputFile();
    gameState = standByState;
    
    while (!isExit)
    {
        vector<vector<char>> answerBoard;
        vector<vector<char>> playingBoard;
        int row = 0, col = 0;

        StandBy gameStandBy;

        while (gameState == standByState)
        {
            cout << "NEW COMMAND!" << endl << endl;;
            istringstream inputLine = gameStart.getCommand();
            string command;
            string fullInputLine = "";

            inputLine >> command;
            fullInputLine = command;

            // cout << "You inserted: " << command << endl;
            // cout << "Status now, board loaded? " << gameStandBy.checkBoardLoaded() << endl;

            if (command == "Load")
            {
                string boardInputType = "";
                inputLine >> boardInputType;
                fullInputLine += " " + boardInputType;

                if (boardInputType == "BoardFile")
                {
                    string boardFileName = "";
                    inputLine >> boardFileName;

                    gameStandBy.loadBoard(boardFileName);
                    fullInputLine += " " + boardFileName;

                    if (!gameStandBy.checkError() || gameStandBy.checkBoardLoaded())
                    {
                        gameStandBy.setGameAnswer(answerBoard);
                        gameStandBy.setPlayingBoard(playingBoard);
                        gameStart.printOutput(gameStart.formatStatusString(fullInputLine, true));
                    }
                    else
                    {
                        gameStart.printOutput(gameStart.formatStatusString(fullInputLine, false));
                        break;
                    }
                }
                else
                {
                    gameStart.printInstructionError();
                    break;
                }
            }
            else if (command == "Print")
            {
                string printType = "";
                inputLine >> printType;
                fullInputLine += " " + printType;

                if (printType == "GameState")
                {
                    gameStart.printGameState(gameState);
                    continue;
                }

                if (gameStandBy.checkError() || !gameStandBy.checkBoardLoaded())
                {
                    gameStart.printOutput(gameStart.formatStatusString(fullInputLine, false));
                    break;
                }

                if (printType == "GameBoard")
                {
                    gameStart.printOutput(gameStart.formatString(fullInputLine));
                    gameStart.printBoard(playingBoard);
                }
                else if (printType == "GameAnswer")
                {
                    gameStart.printOutput(gameStart.formatString(fullInputLine));
                    gameStart.printBoard(answerBoard);
                }
                else
                {
                    gameStart.printInstructionError();
                }
            }
            else if (command == "StartGame")
            {
                if (gameStandBy.checkError() || !gameStandBy.checkBoardLoaded())
                {
                    gameStart.printOutput(gameStart.formatStatusString("StartGame", false));
                    break;
                }

                gameStandBy.startGame(gameState);
                string fullInputLine = "";
                gameStart.printOutput(gameStart.formatStatusString("StartGame", true));
            }
            else
            {
                gameStart.printInstructionError();
            }
        }

        Playing gamePlaying(row, col, answerBoard);

        while (gameState == playingState)
        {
            isExit = true;
        }

        GameOver gameGameOver;

        while (gameState == gameOverState)
        {
            
        }
    }


    return 0;
}

