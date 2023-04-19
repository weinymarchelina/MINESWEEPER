#ifndef STANDBY_H
#define STANDBY_H

#include "main.h"

class StandBy
{
private:
	int row;
	int col;
	bool isBoardLoaded;
	bool isError;
	ifstream inputFile;
	vector<vector<char>> board;
	vector<vector<char>> answerBoard;

public:
	// Intent: To initialize the StandBy object
	// Pre:  None
	// Post:  A StandBy object is created with isError, isBoardLoaded, row, and col values set to false, false, 0, and 0 respectively
	StandBy()
	{
		this->isError = false;
		this->isBoardLoaded = false;
		this->row = 0;
		this->col = 0;
	}

	// Intent: To destroy the StandBy object
	// Pre:  A StandBy object exists
	// Post:  The StandBy object is destroyed and the inputFile is closed
	~StandBy()
	{
		this->inputFile.close();
	}

	// Intent: To get the row value of StandBy object
	// Pre: A StandBy object exists
	// Post:  The row value of the StandBy object is returned
	int getRow()
	{
		return this->row;
	}

	// Intent: To get the column value of StandBy object
	// Pre: A StandBy object exists
	// Post:  The column value of the StandBy object is returned
	int getColumn()
	{
		return this->col;
	}

	// Intent: To check if the StandBy object has an error
	// Pre: A StandBy object exists
	// Post:  Returns true if the StandBy object has an error, otherwise returns false
	bool checkError()
	{
		if (this->isError)
		{
			return true;
		}

		return false;
	}

	// Intent: To check if the StandBy object's board is loaded
	// Pre: A StandBy object exists
	// Post: Returns true if the StandBy object's board is loaded, otherwise returns false
	bool checkBoardLoaded()
	{
		if (this->isBoardLoaded)
		{
			return true;
		}

		return false;
	}

	// Intent: To calculate the radius of each cell on the game board based on the location of bombs in neighboring cells
	// Pre: The game board must be loaded and valid
	// Post:  The answer board will be populated with the radius of each cell on the game board
	void calculateRadius()
	{
		// If the game board is not loaded, return
		if (!this->isBoardLoaded)
		{
			return;
		}

		int row = this->row;
		int col = this->col;

		// Initialize the answer board with zeros
		this->answerBoard.resize(row, vector<char>(col, '0'));

		// Loop through each cell on the board
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
			{
				if (this->board[i][j] == 'X')
				{
					// If current cell is a bomb, mark it as 'X' in the answer board
					this->answerBoard[i][j] = 'X';
				}
				else
				{
					int count = 0;

					// Check all neighboring cells for bombs
					for (int r = max(i - 1, 0); r <= min(i + 1, row - 1); r++)
					{
						for (int c = max(j - 1, 0); c <= min(j + 1, col - 1); c++)
						{

							// If the neighboring cell contains a bomb, increment the count variable
							if (this->board[r][c] == 'X') {
								count++;
							}
						}
					}

					// Convert count to a character and store in the answer board
					this->answerBoard[i][j] = '0' + count;
				}
			}
		}
	}

	// Intent: This function is intended to input a game board from a file and validate it, then calls calculateRadius function
	// Pre: The function expects a string representing the input file name. The file should contain the board as a rectangular grid of 'X' and 'O' characters. The first line of the file should contain two integers representing the number of rows and columns in the board respectively.
	// Post:  The function loads the board from the input file and sets the row, col, and board class members. It returns without setting the isBoardLoaded flag if the board is invalid or if there was an error opening the file
	void inputFileBoard(string inputFileName)
	{
		// Open the input file in read mode
		this->inputFile.open(inputFileName, ios::in);

		// Set isBoardLoaded flag to false
		this->isBoardLoaded = false;

		// Check if the file could not be opened
		if (!this->inputFile.is_open())
		{
			// Set isError flag to true and return
			this->isError = true;
			return;
		}

		// Declare row and col variables
		int row, col;

		// Read the first line of the input file and store it in the string variable line
		string line;
		getline(this->inputFile, line);

		// Parse the first line to get the values of row and col
		istringstream iss(line);
		iss >> row >> col;

		// Set the member variables row and col to the parsed values
		this->row = row;
		this->col = col;

		// Check if row and col are valid
		if (row < 0 || col < 0)
		{
			// Set isError flag to true and return
			isError = true;
			return;
		}

		// Resize the board vector to the specified row and col
		this->board.resize(row, vector<char>(col));

		// Declare lineNum variable
		int lineNum = 0;

		// Read the rest of the lines from the input file
		while (getline(this->inputFile, line))
		{
			// Check if there are too many rows in the input file
			if (lineNum >= row)
			{
				// Set isError flag to true and return
				isError = true;
				return;
			}

			// Check if the length of the line is equal to the number of columns
			if (line.length() != col)
			{
				// Set isError flag to true and return
				isError = true;
				return;
			}

			// Copy the characters from the line into the board vector
			for (int j = 0; j < col; j++)
			{
				char c = line[j];

				// Check if the character is valid
				if (c != 'O' && c != 'X')
				{
					// Set isError flag to true and return
					this->isError = true;
					return;
				}

				// Set the character at the specified position in the board vector
				this->board[lineNum][j] = c;
			}

			// Increment lineNum
			lineNum++;
		}

		// Check if there are too few rows in the input file
		if (lineNum < row)
		{
			// Set isError flag to true and return
			this->isError = true;
			return;
		}

		// Set isBoardLoaded flag to true
		this->isBoardLoaded = true;

		// Calculate the radius for each cell in the board
		calculateRadius();
	}

	// Intent: Make answer board if the user gives fixed row, col, and amount of bombs
	// Pre: row, col, fixedBomb have a valid value
	// Post: Set the new answer board
	void loadFixedBoardCount(int row, int col, int fixedBomb)
	{
		// Clear the current board and answer board
		board.clear();
		answerBoard.clear();

		// Initialize vector to keep the coordinates of the bombs
		vector<pair<int, int>> memo;

		// Set the amount of rows and columns
		this->row = row;
		this->col = col;

		// Resize the board according to the given row and col, and set it to 'O'
		this->board.resize(row, vector<char>(col, 'O'));

		// Seed the random number generator
		srand(time(NULL));

		// Generate random coordinates for the bombs and ensure there are no duplicates
		for (int i = 0; i < fixedBomb;)
		{
			// Randomly select a row and column
			int randRow = rand() % row;
			int randCol = rand() % col;

			// Flag variable
			bool found = false;

			// Check if the coordinate has already been selected as a bomb
			for (int j = 0; j < memo.size(); j++)
			{
				if (randRow == memo[j].first && randCol == memo[j].second)
				{
					found = true;
					break;
				}
			}

			// If the coordinate has not been selected as a bomb, add it to the list
			if (!found)
			{
				memo.push_back(make_pair(randRow, randCol));
				i++;
			}
		}

		// Place the bombs on the board
		for (int i = 0; i < memo.size(); i++)
		{
			int x, y;
			x = memo[i].first;
			y = memo[i].second;
			this->board[x][y] = 'X';
		}

		// Set the flag to indicate that the board has been loaded
		this->isBoardLoaded = true;

		// Calculate the radius of each cell on the board
		calculateRadius();
	}

	// Intent: Initialize a new game board with fixed row, col, and bomb rate specified by the user
	// Pre: row, col, and rate have valid values
	// Post: The answer board is set up with bombs and safe spots based on the specified rate
	void loadFixedBoardRate(int row, int col, float rate)
	{
		// Clear the existing board and answer board
		board.clear();
		answerBoard.clear();

		// Multiply the rate by 10 and cast it to an integer
		rate *= 10;
		rate = (int)rate;

		// Set the board dimensions to the provided row and col values
		this->row = row;
		this->col = col;

		// Seed the random number generator with the current time
		srand(time(NULL));

		// Resize the board to the specified dimensions and initialize all positions to safe
		this->board.resize(row, vector<char>(col, 'O'));

		// For each position in the board, randomly decide whether to place a bomb based on the specified rate
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
			{
				// Generate a random number from 1 to 10
				int random = rand() % 10 + 1;

				// If the random number is less than or equal to the specified rate, place a bomb at the current position
				if (random <= rate)
				{
					board[i][j] = 'X';
				}
			}
		}

		// Calculate the radius for each bomb position on the board
		calculateRadius();
	}

	// Intent: Load and set up the board
	// Pre:  A valid input file name for the board is provided and the board has not already been loaded
	// Post:  The board is loaded and the input file is closed
	void loadFileBoard(string inputFileName)
	{
		// Reset the board and the answer board
		board.clear();
		answerBoard.clear();

		// Load the board from input file
		inputFileBoard(inputFileName);

		// Close input file
		this->inputFile.close();
	}

	// Intent: Set up the playing board with hidden cells
	// Pre:  The game board dimensions have already been determined
	// Post:  A 2D vector representing the game board is created, with each cell represented as a hidden character '#'
	void setPlayingBoard(vector<vector<char>>& board)
	{
		// Resize the board with the given row and col, and set all cells to '0'
		board.resize(this->row, vector<char>(this->col, '0'));

		// Iterate over all cells and set them to '#'
		for (int i = 0; i < this->row; i++)
		{
			for (int j = 0; j < this->col; j++)
			{
				board[i][j] = '#';
			}
		}
	}

	// Intent: Initialize the game answer board with bombs and numbers
	// Pre: The board dimensions and the answer board have been calculated
	// Post:  A 2D vector representing the answer board is created, with each cell representing either a bomb or the number of neighboring bombs
	void setGameAnswer(vector<vector<char>>& board)
	{
		// Resize the board with the given row and col, and set all cells to '0'
		board.resize(this->row, vector<char>(this->col, '0'));

		// Iterate over all cells and set them to the corresponding value in the answer board
		for (int i = 0; i < this->row; i++)
		{
			for (int j = 0; j < this->col; j++)
			{
				board[i][j] = this->answerBoard[i][j];
			}
		}
	}

	// Intent: Start the Minesweeper game
	// Pre: The game board and answer board have already been set up
	// Post:  The game state is set to 'PLAYING'
	void startGame(STATE& gameState)
	{
		gameState = PLAYING;
	}
};

#endif // STANDBY_H
