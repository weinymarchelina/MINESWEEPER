/***********************************************************************
 * File: Main.cpp
 * Author:  B11102124 林家陞
 *          B11115010 鍾詩靈
 *          B11115011 陳仕興
 *          B11115014 魏美芳 
 * Create Date: 2023/04/10
 * Editor:  B11102124 林家陞
 *          B11115010 鍾詩靈
 *          B11115011 陳仕興
 *          B11115014 魏美芳 
 * Update Date: 2023/04/19
 * Description: This C++ program implements several classes to simulate MineSweeper Game by accepting commands from a input file then print the game result on output file.
***********************************************************************/

#include "main.h"

// Define the initial game state as "START"
STATE gameState = START;

// Define a boolean flag to indicate whether the user wants to exit the game
bool isExit = false;

// Include the header files for the different game states (to use the classes)
#include "Start.h"
#include "StandBy.h"
#include "Playing.h"
#include "GameOver.h"

int main()
{
    // Declare variables to store the input method, input file name, and output file name
    string inputMethod, inputFile, outputFile;

    // Create a Start object to read the input file and set the input/output file names
    Start gameStart;

    // Accepting user input of input file and output file name, then keep reading the input file until it is found and successfully read
    do
    {
        cin >> inputMethod >> inputMethod >> inputFile >> outputFile;

        // Set the input/output file names
        gameStart.setFiles(inputFile, outputFile);
    } 
    while (!gameStart.readInputFile());

    // Set the game state to "STANDBY" once the input file has been read successfully
    gameState = STANDBY;

    // Loop until the user exits the game
    while (!isExit)
    {
         // Declare variables to store the game answer board, playing board, and the dimensions of the board
        vector<vector<char>> answerBoard;
        vector<vector<char>> playingBoard;
        int row = 0, col = 0;

        // Declare variables to indicate whether the game is won or ended
        bool isWin = false, isEnd = false;

        // Create a StandBy object to load the board and set up the game
        StandBy gameStandBy;

        // Loop until the game state changes from "STANDBY"
        while (gameState == STANDBY)
        {
            // Get the next command from the input file
            istringstream inputLine = gameStart.getCommand();
            string command;
            string fullInputLine = "";

            inputLine >> command;
            fullInputLine = command;

            // Check the command and execute the corresponding action
            if (command == "Load")
            {
                // Reset the board, row and col each time the user will load a new board
                answerBoard.clear();
                playingBoard.clear();
                row = 0;
                col = 0;

                // Reading Board Input Type
                string boardInputType = "";
                inputLine >> boardInputType;
                fullInputLine += " " + boardInputType;

                // Check the Board Input Type and execute the corresponding action; if the command is invalid, then print failed command
                if (boardInputType == "BoardFile")
                {
                    // Reading board's file name
                    string boardFileName = "";
                    inputLine >> boardFileName;

                    // Load the board from the file
                    gameStandBy.loadFileBoard(boardFileName);

                    // Appending file name to the full input command line
                    fullInputLine += " " + boardFileName;
                }
                else if (boardInputType == "RandomCount")
                {
                    int fixedBomb = 0;

                    // Reading the random count custom board's properties
                    inputLine >> row >> col >> fixedBomb;

                    // Appending the properties to the full input command line
                    fullInputLine += (" " + to_string(row) + " " + to_string(col) + " " + to_string(fixedBomb));

                    // Check whether the inputed properties are invalid
                    if (row <= 0 || col <= 0 || fixedBomb < 0 || fixedBomb > (row*col))
                    {
                        gameStart.printOutput(gameStart.formatStatusString(fullInputLine, false));
                        continue;
                    }

                    // Load the custom board based on the inputted properties
                    gameStandBy.loadFixedBoardCount(row, col, fixedBomb);
                }
                else if (boardInputType == "RandomRate")
                {
                    float rate = 0.0;

                    // Reading the random rate custom board's properties
                    inputLine >> row >> col >> rate;

                    // Converting float to string number with decimal places to 2
                    ostringstream streamObj;
                    streamObj << fixed << setprecision(2) << rate;
                    string stringFloat = streamObj.str();

                    // Appending the properties to the full input command line
                    fullInputLine += (" " + to_string(row) + " " + to_string(col) + " " + stringFloat);

                    // Check whether the inputed properties are invalid
                    if (row <= 0 || col <= 0 || rate < 0 || rate > 1)
                    {
                        // Print failed command and read the next command
                        gameStart.printOutput(gameStart.formatStatusString(fullInputLine, false));
                        continue;
                    }

                    // Load the custom board based on the inputted properties
                    gameStandBy.loadFixedBoardRate(row, col, rate);
                }
                else
                {
                    // Print failed command execution and read the next command
                    gameStart.printInvalidCommand(fullInputLine, inputLine);
                    continue;
                }

                // Check whether there are errors and board is loaded, then set up the game properties; if the command is invalid, then print failed command
                if (!gameStandBy.checkError() || gameStandBy.checkBoardLoaded())
                {
                    // Set up the row
                    row = gameStandBy.getRow();

                    // Set up the column
                    col = gameStandBy.getColumn();

                    // Set up the game board
                    gameStandBy.setGameAnswer(answerBoard);

                    // Set up the answer board
                    gameStandBy.setPlayingBoard(playingBoard);

                    // Print success command execution
                    gameStart.printOutput(gameStart.formatStatusString(fullInputLine, true));
                }
                else
                {
                    // Print failed command and read the next command
                    gameStart.printOutput(gameStart.formatStatusString(fullInputLine, false));
                    break;
                }
            }
            else if (command == "Print")
            {
                // Reading print type
                string printType = "";
                inputLine >> printType;

                // Append the print type to the full input line for formatting purposes
                fullInputLine += " " + printType;

                // If the print type is for the game state, print the game state and read the next command
                if (printType == "GameState")
                {
                    gameStart.printGameState(gameState);
                    continue;
                }

                // If board is not loaded yet but the user try to print the boards, print an error message and read the next command
                if (gameStandBy.checkError() || !gameStandBy.checkBoardLoaded())
                {
                    gameStart.printOutput(gameStart.formatStatusString(fullInputLine, false));
                    break;
                }

                // Check the print type and execute the corresponding action; if the command is invalid, then print failed command
                if (printType == "GameBoard")
                {
                    // Print command execution
                    gameStart.printOutput(gameStart.formatString(fullInputLine));
                    
                    // Print playing board
                    gameStart.printBoard(playingBoard);
                }
                else if (printType == "GameAnswer") 
                {
                    // Print command execution
                    gameStart.printOutput(gameStart.formatString(fullInputLine));
                 
                    // Print answer board
                    gameStart.printBoard(answerBoard);
                }
                else
                {
                    // Print failed command and read the next command
                    gameStart.printInvalidCommand(fullInputLine, inputLine);
                    break;
                }
            }
            else if (command == "StartGame")
            {
                // If board is not loaded yet but the user try to start the game, print an error message and read the next command
                if (gameStandBy.checkError() || !gameStandBy.checkBoardLoaded())
                {
                    gameStart.printOutput(gameStart.formatStatusString("StartGame", false));
                    break;
                }

                string fullInputLine = "";

                // Start the game
                gameStandBy.startGame(gameState);

                // Print success command execution
                gameStart.printOutput(gameStart.formatStatusString("StartGame", true));
            }
            else
            {
                // Print failed command and read the next command
                gameStart.printInvalidCommand(command, inputLine);
            }
        }

        // Creating Playing object and play the game with the given row, column, and answer board
        Playing gamePlaying(row, col, answerBoard);

        // Continue playing while the game state is still "playing"
        while (gameState == PLAYING)
        {
            // Get the command from the user
            istringstream inputLine = gameStart.getCommand();
            string command;
            string fullInputLine = "";

            // Read the first word of the input as the command
            inputLine >> command;
            fullInputLine = command;

            // Check the command and execute the corresponding action
            if (command == "LeftClick" || command == "RightClick")
            {
                // Read in the x and y coordinates of the click
                int x = 0, y = 0;
                inputLine >> x >> y;

                // Appending x and y coordinate to the full input command line
                fullInputLine += " " + to_string(x) + " " + to_string(y);

                // If the coordinates are out of range, print failed command and read the next command
                if (x < 0 || y < 0 || x >= row || y >= col)
                {
                    // Print failed command and read the next command
                    gameStart.printOutput(gameStart.formatStatusString(fullInputLine, false));
                    continue;
                }

                // Check the click type and execute the corresponding action
                if (command == "LeftClick")
                {
                    // Store the current character of the coordinate for checking purposes
                    gamePlaying.setPreviousChar(playingBoard, x, y);

                    // Open the cell of the coordinate and change the value based on the answer board
                    gamePlaying.leftClick(playingBoard, answerBoard, x, y);

                    // Recheck the clicking status of left click
                    gamePlaying.recheckLeftClick(playingBoard, x, y);
                }
                else
                {
                    // Right click the cell to mark flag, question, or unmark
                    gamePlaying.rightClick(playingBoard, x, y);
                }

                // Check whethe rthe click is successfull
                if (gamePlaying.checkClickFailed())
                {
                    // Print failed command and read the next command
                    gameStart.printOutput(gameStart.formatStatusString(fullInputLine, false));
                }
                else
                {
                    // Print success command execution
                    gameStart.printOutput(gameStart.formatStatusString(fullInputLine, true));
                    
                    // Judge if the game has been won
                    gamePlaying.judgeWin(isWin, isEnd, gameState, playingBoard);

                    // If the game is over, print the win/lose message
                    if (isEnd)
                    {
                        gameStart.printWinLose(isWin);
                    }
                }
            }
            else if (command == "Print")
            {
                // Reading print type
                string printType = "";
                inputLine >> printType;

                // Append the print type to the full input line for formatting purposes
                fullInputLine += " " + printType;

                // Check the print type and execute the corresponding action
                if (printType == "GameState")
                {
                    // Print the current game state
                    gameStart.printGameState(gameState);
                    continue;
                }
                else if (printType == "GameBoard")
                {
                    // Print command execution
                    gameStart.printOutput(gameStart.formatString(fullInputLine));
                    
                    // Print playing board
                    gameStart.printBoard(playingBoard);
                }
                else if (printType == "GameAnswer") 
                {
                    // Print command execution
                    gameStart.printOutput(gameStart.formatString(fullInputLine));
                 
                    // Print answer board
                    gameStart.printBoard(answerBoard);
                }
                else if (printType == "BombCount")
                {
                    // Appending bomb count to the full input command line
                    fullInputLine = gameStart.formatString(fullInputLine) + (to_string(gamePlaying.getBombCount()));

                    // Print command execution
                    gameStart.printOutput(fullInputLine);
                }
                else if (printType == "FlagCount")
                {
                    // Appending flag count to the full input command line
                    fullInputLine = gameStart.formatString(fullInputLine) + (to_string(gamePlaying.getFlagCount()));

                    // Print command execution
                    gameStart.printOutput(fullInputLine);
                }
                else if (printType == "OpenBlankCount")
                {
                    // Appending opened blank cells count to the full input command line
                    fullInputLine = gameStart.formatString(fullInputLine) + (to_string(gamePlaying.getOpenBlankCount()));

                    // Print command execution
                    gameStart.printOutput(fullInputLine);
                }
                else if (printType == "RemainBlankCount")
                {
                    // Appending remaing blank cells count to the full input command line
                    fullInputLine = gameStart.formatString(fullInputLine) + (to_string(gamePlaying.getRemainBlankCount()));

                    // Print command execution
                    gameStart.printOutput(fullInputLine);
                }
                else
                {
                    // Print failed command and read the next command if the command is unrecognized
                    gameStart.printInvalidCommand(fullInputLine, inputLine);
                }
            }
            else
            {
                // Print failed command and read the next command if the command is unrecognized
                gameStart.printInvalidCommand(fullInputLine, inputLine);
            }
        }

        // Create a GameOver object
        GameOver gameGameOver;

        // Loop until the game state changes from "GAMEOVER"
        while (gameState == GAMEOVER)
        {
            // Get the command from the user
            istringstream inputLine = gameStart.getCommand();
            string command;
            string fullInputLine = "";

            // Read the first word of the input as the command
            inputLine >> command;
            fullInputLine = command;

            // Check the command and execute the corresponding action
            if (command == "Replay")
            {
                // Replay the game and print a success message
                gameGameOver.replay(gameState);
                gameStart.printOutput(gameStart.formatStatusString(command, true));
            }
            else if (command == "Quit")
            {
                // Quit the game and set the isExit flag to true, then print a success message
                gameGameOver.quit(gameState, isExit);
                gameStart.printOutput(gameStart.formatStatusString(command, true));
            }
            // Check if the command is "Print"
            else if (command == "Print")
            {
                // Reading print type
                string printType = "";
                inputLine >> printType;

                // Append the print type to the full input line for formatting purposes
                fullInputLine += " " + printType;

                // Check the print type and execute the corresponding action
                if (printType == "GameState")
                {
                    // Print the current game state
                    gameStart.printGameState(gameState);
                }
                if (printType == "BombCount")
                {
                    // Appending bomb count to the full input command line
                    fullInputLine = gameStart.formatString(fullInputLine) + (to_string(gamePlaying.getBombCount()));

                    // Print command execution
                    gameStart.printOutput(fullInputLine);
                }
                else if (printType == "FlagCount")
                {
                    // Appending flag count to the full input command line
                    fullInputLine = gameStart.formatString(fullInputLine) + (to_string(gamePlaying.getFlagCount()));

                    // Print command execution
                    gameStart.printOutput(fullInputLine);
                }
                else if (printType == "OpenBlankCount")
                {
                    // Appending opened blank cells count to the full input command line
                    fullInputLine = gameStart.formatString(fullInputLine) + (to_string(gamePlaying.getOpenBlankCount()));

                    // Print command execution
                    gameStart.printOutput(fullInputLine);
                }
                else if (printType == "RemainBlankCount")
                {
                    // Appending remaing blank cells count to the full input command line
                    fullInputLine = gameStart.formatString(fullInputLine) + (to_string(gamePlaying.getRemainBlankCount()));

                    // Print command execution
                    gameStart.printOutput(fullInputLine);
                }
                else
                {
                    // Print failed command and read the next command if the command is unrecognized
                    gameStart.printInvalidCommand(fullInputLine, inputLine);
                }
            }
            else
            {
                // Print failed command and read the next command if the command is unrecognized
                gameStart.printInvalidCommand(fullInputLine, inputLine);
            }
        }
    }

    // Print a message indicating that the input and output files have been checked and output is successfully printed
    gameStart.printCheckFiles(inputFile, outputFile, true);

    return 0;
}
