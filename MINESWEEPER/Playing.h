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
    bool isEnd;
    bool clickFailed;
    char previousChar;

public:
    Playing(int row, int col, vector<vector<char>>& board)
    {
        bombCount = 0;
        flagCount = 0;
        openBlankCount = 0;
        remainBlankCount = 0;
        this->row = row;
        this->col = col;
        isEnd = false;
        clickFailed = false;
        previousChar = ' ';

        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if (board[i][j] == 'X') {
                    bombCount++;
                }
            }
        }
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

        this->remainBlankCount -= bombCount;
    }

    int getBombCount()
    {
        // cout << bombCount << endl;
        return bombCount;
    }

    int getFlagCount()
    {
        // cout << flagCount << endl;
        return flagCount;
    }

    int getOpenBlankCount()
    {
        // cout << openBlankCount << endl;
        return openBlankCount;
    }

    int getRemainBlankCount()
    {
        // cout << remainBlankCount << endl;
        return remainBlankCount;
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

            remainBlankCount--;

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
            remainBlankCount--;
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

    void markFlag(vector<vector<char>>& board, int x, int y)
    {
        board[x][y] = 'f';
        flagCount++;
    }

    void markQuestion(vector<vector<char>>& board, int x, int y)
    {
        board[x][y] = '?';
        flagCount--;
    }

    void unmark(vector<vector<char>>& board, int x, int y)
    {
        board[x][y] = '#';
    }

    void rightClick(vector<vector<char>>& board, int x, int y)
    {
        if (isdigit(board[x][y]) || x < 0 || x >= board.size() || y < 0 || y >= board[0].size())
        {
            clickFailed = true;
            return;
        }

        if (board[x][y] == '#')
        {
            markFlag(board, x, y);
        }
        else if (board[x][y] == 'f')
        {
            markQuestion(board, x, y);
        }
        else if (board[x][y] == '?')
        {
            unmark(board, x, y);
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
        cout << "Remain: " << this->remainBlankCount << endl;

        if (this->remainBlankCount == 0)
        {
            gameState = gameOverState;
            this->isEnd = true;
            isEnd = true;
            isWin = true;
        }
    }

    bool checkClickFailed()
    {
        if (clickFailed)
        {
            clickFailed = false;
            return true;
        }

        return false;
    }

};


