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
    Start gameStart;

    do 
    {
        // cin >> inputMethod >> inputMethod >> inputFile >> outputFile;
        // inputFile = "command1.txt";
        // outputFile = "output1.txt";
        // inputFile = "command2.txt";
        // outputFile = "output2.txt";
        inputFile = "command3.txt";
        outputFile = "output3.txt";

        gameStart.setFiles(inputFile, outputFile);
    } while (!gameStart.readInputFile());

    /*
    MineSweeper.exe CommandFile (input.txt) (output.txt)
    MineSweeper.exe CommandFile LMAO.txt outputLmao.txt
    MineSweeper.exe CommandFile command1.txt output1.txt
    MineSweeper.exe CommandFile command2.txt output2.txt
    MineSweeper.exe CommandFile command3.txt output3.txt
    */
    
    gameState =
        standByState;
    
    while (!isExit)
    {
        vector<vector<char>> answerBoard;
        vector<vector<char>> playingBoard;
        int row = 0, col = 0;
        bool isWin = false, isEnd = false;

        StandBy gameStandBy;

        while (gameState == standByState)
        {
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

                    gameStandBy.loadFileBoard(boardFileName);
                    fullInputLine += " " + boardFileName;
                }
                else if (boardInputType == "RandomCount")
                {
                    int fixedBomb = 0;
                    inputLine >> row >> col >> fixedBomb;

                    fullInputLine += (" " + to_string(row) + " " + to_string(col) + " " + to_string(fixedBomb));
                    cout << "INPUTTING:  " << fullInputLine << endl;

                    if (row <= 0 || col <= 0 || fixedBomb < 0 || fixedBomb > (row*col))
                    {
                        cout << "INVALID RANDOM COUNT " << fullInputLine << endl;
                        gameStart.printOutput(gameStart.formatStatusString(fullInputLine, false));
                        continue;
                    }

                    cout << "VALID RANDOM COUNT" << endl;
                    gameStandBy.loadFixedBoardCount(row, col, fixedBomb);
                }
                else if (boardInputType == "RandomRate")
                {
                    float rate = 0.0;
                    inputLine >> row >> col >> rate;

                    ostringstream streamObj;
                    streamObj << fixed << setprecision(2) << rate;
                    string stringFloat = streamObj.str();

                    fullInputLine += (" " + to_string(row) + " " + to_string(col) + " " + stringFloat);
                    cout << "INPUTTING: " << fullInputLine << endl;

                    if (row <= 0 || col <= 0 || rate < 0 || rate > 1)
                    {
                        cout << "INVALID RANDOM RATE " << fullInputLine << endl;
                        gameStart.printOutput(gameStart.formatStatusString(fullInputLine, false));
                        continue;
                    }

                    cout << "VALID RANDOM RATE" << endl;
                    gameStandBy.loadFixedBoardRate(row, col, rate);
                }
                else
                {
                    cout << "COMMAND NOT FOUND" << endl;
                    gameStart.printInvalidCommand(fullInputLine, inputLine);
                    break;
                }

                cout << "CHECKING TIME" << endl;
                if (!gameStandBy.checkError() || gameStandBy.checkBoardLoaded())
                {
                    row = gameStandBy.getRow();
                    col = gameStandBy.getColumn();
                    gameStandBy.setGameAnswer(answerBoard);
                    gameStandBy.setPlayingBoard(playingBoard);
                    gameStart.printOutput(gameStart.formatStatusString(fullInputLine, true));
                    cout << "PASS" << endl;
                }
                else
                {
                    gameStart.printOutput(gameStart.formatStatusString(fullInputLine, false));
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
                }
                else
                {
                    gameStart.printOutput(gameStart.formatStatusString(fullInputLine, true));
                    gamePlaying.judgeWin(isWin, isEnd, gameState, playingBoard);

                    if (isEnd)
                    {
                        gameStart.printWinLose(isWin);
                    }
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

    gameStart.printCheckFiles(inputFile, outputFile, true);

    return 0;
}

