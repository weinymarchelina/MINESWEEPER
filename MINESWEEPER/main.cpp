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
    // cin >> inputMethod >> inputMethod >> inputFile >> outputFile;
    // Start gameStart("inputFile", "outputFile");
    Start gameStart("command1.txt", "output1.txt");
    // Start gameStart("command2.txt", "output2.txt");
    // Start gameStart("command3.txt", "output3.txt");
    gameStart.readInputFile();
    gameState = standByState;
    
    while (!isExit)
    {
        vector<vector<char>> answerBoard;
        vector<vector<char>> playingBoard;
        int row = 0, col = 0;
        bool isWin = false, isEnd = false;

        StandBy gameStandBy;

        while (gameState == standByState)
        {
            // cout << endl << "STANDBY STATE COMMAND" << endl;
            istringstream inputLine = gameStart.getCommand();
            string command;
            string fullInputLine = "";

            inputLine >> command;
            fullInputLine = command;

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
                        row = gameStandBy.getRow();
                        col = gameStandBy.getColumn();
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
                    gameStart.printInvalidCommand(fullInputLine, inputLine);
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
                    gameStart.printInvalidCommand(fullInputLine, inputLine);
                    break;
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
                gameStart.printInvalidCommand(command, inputLine);
            }
        }

        Playing gamePlaying(row, col, answerBoard);

        while (gameState == playingState)
        {
            cout << endl << "PLAYING COMMAND" << endl;
            istringstream inputLine = gameStart.getCommand();
            string command;
            string fullInputLine = "";

            inputLine >> command;
            fullInputLine = command;

            if (command == "LeftClick" || command == "RightClick")
            {
                int x = 0, y = 0;
                inputLine >> x >> y;
                fullInputLine += " " + to_string(x) + " " + to_string(y);

                if (x < 0 || y < 0 || x >= row || y >= col)
                {
                    gameStart.printOutput(gameStart.formatStatusString(fullInputLine, false));
                    continue;
                }

                if (command == "LeftClick")
                {
                    gamePlaying.setPreviousChar(playingBoard, x, y);
                    gamePlaying.leftClick(playingBoard, answerBoard, x, y);
                    gamePlaying.recheckLeftClick(playingBoard, x, y);
                }
                else
                {
                    gamePlaying.rightClick(playingBoard, x, y);
                }


                if (gamePlaying.checkClickFailed())
                {
                    gameStart.printOutput(gameStart.formatStatusString(fullInputLine, false));
                    // gameStart.printBoard(playingBoard);
                }
                else
                {
                    gameStart.printOutput(gameStart.formatStatusString(fullInputLine, true));
                    gamePlaying.judgeWin(isWin, isEnd, gameState, playingBoard);

                    if (isEnd)
                    {
                        gameStart.printWinLose(isWin);
                    }
                    // gameStart.printBoard(playingBoard);
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
                }
                else if (printType == "GameBoard")
                {
                    gameStart.printOutput(gameStart.formatString(fullInputLine));
                    gameStart.printBoard(playingBoard);
                }
                else if (printType == "GameAnswer")
                {
                    gameStart.printOutput(gameStart.formatString(fullInputLine));
                    gameStart.printBoard(answerBoard);
                }
                else if (printType == "BombCount")
                {
                    fullInputLine = gameStart.formatString(fullInputLine) + (to_string(gamePlaying.getBombCount()));
                    gameStart.printOutput(fullInputLine);
                }
                else if (printType == "FlagCount")
                {
                    fullInputLine = gameStart.formatString(fullInputLine) + (to_string(gamePlaying.getFlagCount()));
                    gameStart.printOutput(fullInputLine);
                }
                else if (printType == "OpenBlankCount")
                {
                    fullInputLine = gameStart.formatString(fullInputLine) + (to_string(gamePlaying.getOpenBlankCount()));
                    gameStart.printOutput(fullInputLine);
                }
                else if (printType == "RemainBlankCount")
                {
                    fullInputLine = gameStart.formatString(fullInputLine) + (to_string(gamePlaying.getRemainBlankCount()));
                    gameStart.printOutput(fullInputLine);
                }
                else
                {
                    gameStart.printInvalidCommand(fullInputLine, inputLine);
                    
                }
            }
            else
            {
                gameStart.printInvalidCommand(fullInputLine, inputLine);
              
            }
        }

        GameOver gameGameOver;

        while (gameState == gameOverState)
        {
            // cout << endl << "GAME OVER COMMAND" << endl;
            istringstream inputLine = gameStart.getCommand();
            string command;
            string fullInputLine = "";

            inputLine >> command;
            fullInputLine = command;

            if (command == "Replay")
            {
                gameGameOver.replay(gameState);
                gameStart.printOutput(gameStart.formatStatusString(command, true));
            }
            else if (command == "Quit")
            {
                gameGameOver.quit(gameState, isExit);
                gameStart.printOutput(gameStart.formatStatusString(command, true));
            }
            else if (command == "Print")
            {
                string printType = "";
                inputLine >> printType;
                fullInputLine += " " + printType;

                if (printType == "GameState")
                {
                    gameStart.printGameState(gameState);
                }
                else if (printType == "BombCount")
                {
                    fullInputLine = gameStart.formatString(fullInputLine) + (to_string(gamePlaying.getBombCount()));
                    gameStart.printOutput(fullInputLine);
                }
                else if (printType == "FlagCount")
                {
                    fullInputLine = gameStart.formatString(fullInputLine) + (to_string(gamePlaying.getFlagCount()));
                    gameStart.printOutput(fullInputLine);
                }
                else if (printType == "OpenBlankCount")
                {
                    fullInputLine = gameStart.formatString(fullInputLine) + (to_string(gamePlaying.getOpenBlankCount()));
                    gameStart.printOutput(fullInputLine);
                }
                else if (printType == "RemainBlankCount")
                {
                    fullInputLine = gameStart.formatString(fullInputLine) + (to_string(gamePlaying.getRemainBlankCount()));
                    gameStart.printOutput(fullInputLine);
                }
                else
                {
                    gameStart.printInvalidCommand(fullInputLine, inputLine);
                    
                }
            }
            else
            {
                gameStart.printInvalidCommand(fullInputLine, inputLine);
                
            }
        }
    }


    return 0;
}

