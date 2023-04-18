#include "main.h"

class StandBy
{
private:
	bool isBoardLoaded;
	ifstream inputFile;
	int row;
	int col;
    bool isError;
    string errorMsg;
	vector<vector<char>> board;
    vector<vector<char>> answerBoard;

public:
	StandBy()
	{
        this->errorMsg = "";
        this->isError = false;
        this->isBoardLoaded = false;
        this->row = 0;
        this->col = 0;
	}

	~StandBy()
	{
		inputFile.close();
	}

    void printBoardError()
    {
        cout << "<Board Error> : " << errorMsg << endl;
    }

    bool checkError()
    {
        if (this->isError)
        {
            printBoardError();
            return true;
        }

        return false;
    }

    void inputBoard(string inputFileName)
    {
        inputFile.open(inputFileName, ios::in);

        this->isBoardLoaded = false;

        if (!inputFile.is_open())
        {
            // cout << "<Board Error> : " << inputFileName << " is not found" << endl;
            errorMsg = inputFileName + " is not found";
            isError = true;
            return;
        }
        else
        {
            cout << "File is found: " << inputFileName << endl;
        }

        int row, col;

        string line;
        getline(inputFile, line);
        istringstream iss(line);

        iss >> row >> col;

        this->row = row;
        this->col = col;

        board.resize(row, vector<char>(col));

        int lineNum = 0;
        bool hasO = false;
        bool hasX = false;

        while (getline(inputFile, line))
        {
            // Check if there are too many rows in the input file
            if (lineNum >= row)
            {
                errorMsg = "Input file contains more rows than specified by row and col";
                isError = true;
                return;
            }

            // Check if the length of the line is equal to the number of columns
            if (line.length() != col)
            {
                errorMsg = "Row " + to_string(lineNum + 1) + " in the input file has an invalid number of columns";
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
                    errorMsg = "Board contains invalid character";
                    isError = true;
                    return;
                }

                board[lineNum][j] = c;
            }

            lineNum++;
        }

        if (!hasO || !hasX)
        {
            errorMsg = "Board does not contain both 'O' and 'X' characters";
            isError = true;
            return;
        }
            
        // Check if there are too few rows in the input file
        if (lineNum < row)
        {
            errorMsg = "Input file contains fewer rows than specified by row and col";
            isError = true;
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

        answerBoard.resize(row, vector<char>(col, '0'));

        for (int i = 0; i < row; i++) 
        {
            for (int j = 0; j < col; j++) 
            {
                if (board[i][j] == 'X') 
                {
                    // if current cell is a bomb, mark it as 'X'
                    answerBoard[i][j] = 'X'; 
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
                            if (board[r][c] == 'X') { 
                                count++;
                            }
                        }
                    }

                    // convert count to a character and store in modified board
                    answerBoard[i][j] = '0' + count; 
                }
            }
        }

        // cout << "Board is filled with numbers!" << endl;
	}

	void loadBoard(string inputFileName)
	{
		inputBoard(inputFileName);
		calculateRadius();
        inputFile.close();
	}

    void setPlayingBoard(vector<vector<char>>& board)
    {
        board.resize(row, vector<char>(col, '0'));

        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                board[i][j] = '#';
            }
        }

    }

    void setGameAnswer(vector<vector<char>>& board)
    {
        board.resize(row, vector<char>(col, '0'));

        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                board[i][j] = answerBoard[i][j];
            }
        }
    }

    bool checkBoardLoaded()
    {
        if (this->isBoardLoaded)
        {
            return true;
        }

        // cout << "You need to load board first!" << endl;
        return false;
    }

    //

    void startGame(State& gameState)
    {
        gameState = playingState;
    }

    int getRow ()
    {
        return this->row;
    }

    int getColumn()
    {
        return this->col;
    }

};