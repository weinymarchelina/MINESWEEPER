#include "main.h"

class Playing
{
private:
    int row;
    int col;
    int bombCount;
    int flagCount;
    int openBlankCount;
    int remainBlankCount;
    char previousChar;
    bool isEnd;
    bool clickFailed;

public:
    Playing(int row, int col, vector<vector<char>>& board)
    {
        this->bombCount = 0;
        this->flagCount = 0;
        this->openBlankCount = 0;
        this->remainBlankCount = 0;
        this->row = row;
        this->col = col;
        this->isEnd = false;
        this->clickFailed = false;
        this->previousChar = ' ';

        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if (board[i][j] == 'X') {
                    this->bombCount++;
                }
            }
        }
    }

    int getBombCount()
    {
        return this->bombCount;
    }

    int getFlagCount()
    {
        return this->flagCount;
    }

    int getOpenBlankCount()
    {
        return this->openBlankCount;
    }

    int getRemainBlankCount()
    {
        return this->remainBlankCount;
    }

    bool checkClickFailed()
    {
        if (this->clickFailed)
        {
            this->clickFailed = false;
            return true;
        }

        return false;
    }

    void calculateBlankCount(vector<vector<char>>& board)
    {
        this->remainBlankCount = 0;
        this->openBlankCount = 0;

        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if (isdigit(board[i][j]))
                {
                    this->openBlankCount++;
                }
                else
                {
                    this->remainBlankCount++;
                }
            }
        }

        this->remainBlankCount -= this->bombCount;
    }

    void setPreviousChar(vector<vector<char>>& board, int x, int y)
    {
        this->previousChar = board[x][y];
    }

    void leftClick(vector<vector<char>>& board, vector<vector<char>>& ans, int x, int y)
    {
        if (x < 0 || x >= board.size() || y < 0 || y >= board[0].size())
        {
            this->clickFailed = true;
            return;
        }

        if (board[x][y] == 'f' || isdigit(board[x][y]))
        {
            this->clickFailed = true;
            return;
        }

        if ((board[x][y] == '#' || board[x][y] == '?') && ans[x][y] == '0') 
        {
            board[x][y] = '0';

            this->remainBlankCount--;

            for (int i = -1; i <= 1; i++) 
            {
                for (int j = -1; j <= 1; j++) 
                {
                    if (i == 0 && j == 0) 
                    {
                        continue;
                    }

                    int nx = x + i;
                    int ny = y + j;

                    leftClick(board, ans, nx, ny);
                }
            }
        }
        else if ((board[x][y] == '#' || board[x][y] == '?') && isdigit(ans[x][y]) && ans[x][y] != '0')
        {
            board[x][y] = ans[x][y];
            this->remainBlankCount--;
        }
        else if (ans[x][y] == 'X')
        {
            this->isEnd = true;
            return;
        }
    }

    void recheckLeftClick(vector<vector<char>>& board, int x, int y)
    {
        if (isdigit(board[x][y]) && (this->previousChar == '#' || this->previousChar == '?'))
        {
            this->clickFailed = false;
        }
    }

    void rightClick(vector<vector<char>>& board, int x, int y)
    {
        if (isdigit(board[x][y]) || x < 0 || x >= board.size() || y < 0 || y >= board[0].size())
        {
            this->clickFailed = true;
            return;
        }

        if (board[x][y] == '#')
        {
            board[x][y] = 'f';
            this->flagCount++;
        }
        else if (board[x][y] == 'f')
        {
            board[x][y] = '?';
            this->flagCount--;
        }
        else if (board[x][y] == '?')
        {
            board[x][y] = '#';
        }
    }

    void judgeWin(bool& isWin, bool& isEnd, State& gameState, vector<vector<char>>& board)
    {
        if (this->isEnd)
        {
            gameState = gameOverState;
            isEnd = true;
            isWin = false;
            return;
        }
        
        calculateBlankCount(board);

        if (this->remainBlankCount == 0)
        {
            gameState = gameOverState;
            this->isEnd = true;
            isEnd = true;
            isWin = true;
        }
    }

};


