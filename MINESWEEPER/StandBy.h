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

        return false;
    }

    void inputBoard(string inputFileName)
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
        bool hasO = false;
        bool hasX = false;

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

                if (c == 'O')
                {
                    hasO = true;
                }
                else if (c == 'X')
                {
                    hasX = true;
                }
                else
                {
                    // Board contains invalid character
                    this->isError = true;
                    return;
                }

                this->board[lineNum][j] = c;
            }

            lineNum++;
        }

        if (!hasO || !hasX)
        {
            // Board does not contain both 'O' and 'X' characters
            this->isError = true;
            return;
        }
            
        // Check if there are too few rows in the input file
        if (lineNum < row)
        {
            // Input file contains fewer rows than specified by row and col
            this->isError = true;
            return;
        }

        this->isBoardLoaded = true;
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

	void loadBoard(string inputFileName)
	{
		inputBoard(inputFileName);
		calculateRadius();
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