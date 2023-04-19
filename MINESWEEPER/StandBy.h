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
	StandBy()
	{
		this->isError = false;
		this->isBoardLoaded = false;
		this->row = 0;
		this->col = 0;
	}

	~StandBy()
	{
		this->inputFile.close();
	}

	int getRow()
	{
		return this->row;
	}

	int getColumn()
	{
		return this->col;
	}

	bool checkError()
	{
		if (this->isError)
		{
			cout << "ERROR FOUND" << endl;
			return true;
		}

		return false;
	}

	bool checkBoardLoaded()
	{
		if (this->isBoardLoaded)
		{
			return true;
		}

		cout << "BOARD NOT LOADED" << endl;
		return false;
	}

	void calculateRadius()
	{
		if (!this->isBoardLoaded)
		{
			return;
		}

		int row = this->row;
		int col = this->col;

		this->answerBoard.resize(row, vector<char>(col, '0'));

		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
			{
				if (this->board[i][j] == 'X')
				{
					// if current cell is a bomb, mark it as 'X'
					this->answerBoard[i][j] = 'X';
				}
				else
				{
					int count = 0;

					// check all neighboring cells for bombs
					for (int r = max(i - 1, 0); r <= min(i + 1, row - 1); r++)
					{
						for (int c = max(j - 1, 0); c <= min(j + 1, col - 1); c++)
						{

							//if the neighboring cell contains a bomb, increment the count variable
							if (this->board[r][c] == 'X') {
								count++;
							}
						}
					}

					// convert count to a character and store in modified board
					this->answerBoard[i][j] = '0' + count;
				}
			}
		}
	}

	void inputFileBoard(string inputFileName)
	{
		this->inputFile.open(inputFileName, ios::in);
		this->isBoardLoaded = false;

		// File not found
		if (!this->inputFile.is_open())
		{
			this->isError = true;
			return;
		}

		int row, col;

		string line;
		getline(this->inputFile, line);
		istringstream iss(line);

		iss >> row >> col;

		this->row = row;
		this->col = col;

		// Invalid row and column
		if (row < 0 || col < 0)
		{
			isError = true;
			return;
		}

		this->board.resize(row, vector<char>(col));

		int lineNum = 0;

		while (getline(this->inputFile, line))
		{
			// Check if there are too many rows in the input file
			if (lineNum >= row)
			{
				isError = true;
				return;
			}

			// Check if the length of the line is equal to the number of columns
			if (line.length() != col)
			{
				isError = true;
				return;
			}

			// Copy the characters from the line into the board vector

			for (int j = 0; j < col; j++)
			{
				char c = line[j];

				if (c != 'O' && c != 'X')
				{
					// Board contains invalid character
					cout << "INVALID CHAR" << endl;
					this->isError = true;
					return;
				}

				this->board[lineNum][j] = c;
			}

			lineNum++;
		}

		// Check if there are too few rows in the input file
		if (lineNum < row)
		{
			// Input file contains fewer rows than specified by row and col
			this->isError = true;
			return;
		}

		this->isBoardLoaded = true;

		calculateRadius();
	}

	/*
	 / Intent: Make answer board if the user gives fixed row, col, and amount of bombs
	 / Pre: row, col, fixedBomb have a valid value
	 / Post: Set the new answer board
	*/
	void loadFixedBoardCount(int row, int col, int fixedBomb)
	{
		board.clear();
		answerBoard.clear();

		cout << "INPUT FIXED BOARD" << endl;
		vector<pair<int, int>> memo;	// To keep the coordinate of the bombs
		this->row = row;	// Set the amount of row
		this->col = col;	// Set the amount of col

		cout << "ROW: " << row << endl;
		cout << "COL: " << col << endl;

		this->board.resize(row, vector<char>(col, 'O'));	// Resize the board according to given row and col, and set it to 0

		srand(time(NULL));	// For random

		// Generate random coordinate and make sure there will be no same positions
		for (int i = 0; i < fixedBomb;)
		{
			int randRow = rand() % row;	// Random the row coordinate
			int randCol = rand() % col;	// Random the col coordinate

			bool found = false;	// Flag Variable

			for (int j = 0; j < memo.size(); j++)
			{
				if (randRow == memo[j].first && randCol == memo[j].second)
				{
					found = true;
					break;
				}
			}

			if (!found)
			{
				memo.push_back(make_pair(randRow, randCol));
				i++;
			}
		}

		for (int i = 0; i < memo.size(); i++)
		{
			int x, y;
			x = memo[i].first;
			y = memo[i].second;
			this->board[x][y] = 'X';
		}

		/*
		cout << "PRINTING BASIC ANSWER BOARD" << endl;
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
			{
				cout << board[i][j] << " ";
			}
			cout << endl;
		}
		*/

		this->isBoardLoaded = true;

		calculateRadius();
	}
	
	/*
	 / Intent: Make answer board if the user gives fixed row, col, and the rate of bombs
	 / Pre: row, col, fixedBomb hava a valid value
	 / Post: Set the new answer board
	*/
	void loadFixedBoardRate(int row, int col, float rate)
	{
		board.clear();
		answerBoard.clear();

		cout << "CALLING INPUT FIXED BOARD RATE" << endl;
		rate *= 10;	// Multiplies rate by 10
		rate = (int) rate;	// Cast it to integer

		this->row = row;	// Set row
		this->col = col;	// Set col
		//int count = 0;

		cout << "ROW: " << row << endl;
		cout << "COL: " << col << endl;


		srand(time(NULL));
		this->board.resize(row, vector<char>(col, 'O'));	// Resize the board according to given row and col, and set it to 0
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
			{
				// Random a number from 1 to 10
				int random = rand() % 10 + 1;

				// To make the rate of the bomb according to the user input,
				// We generate a random number from 1 to 10, if the number is
				// equal to the rate or smaller than the rate, set the current
				// position to X
				if (random <= rate)
				{
					board[i][j] = 'X';
					//count++;
				}
			}
		}

		/*
cout << "PRINTING BASIC ANSWER BOARD" << endl;
for (int i = 0; i < row; i++)
{
	for (int j = 0; j < col; j++)
	{
		cout << board[i][j] << " ";
	}
	cout << endl;
}
*/

		calculateRadius();
	}

	void loadFileBoard(string inputFileName)
	{
		board.clear();
		answerBoard.clear();
		inputFileBoard(inputFileName);
		this->inputFile.close();
	}

	void setPlayingBoard(vector<vector<char>>& board)
	{
		board.resize(this->row, vector<char>(this->col, '0'));

		for (int i = 0; i < this->row; i++)
		{
			for (int j = 0; j < this->col; j++)
			{
				board[i][j] = '#';
			}
		}

	}

	void setGameAnswer(vector<vector<char>>& board)
	{
		board.resize(this->row, vector<char>(this->col, '0'));

		for (int i = 0; i < this->row; i++)
		{
			for (int j = 0; j < this->col; j++)
			{
				board[i][j] = this->answerBoard[i][j];
			}
		}
	}

	void startGame(State& gameState)
	{
		gameState = playingState;
	}
};