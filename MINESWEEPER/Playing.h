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

        // find bombCount first so that no error when calculating remainBlankCount later
        // just need to count bombs once because the number of bombs will not be changed
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

    void printBombCount()
    {
        cout << bombCount << endl;
    }

    void printFlagCount(vector<vector<char>>& board)
    {
        cout << flagCount << endl;
    }

    void printOpenBlankCount(vector<vector<char>>& board)
    {
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if (isdigit(board[i][j]))
                {
                    openBlankCount++;
                }
            }
        }

        cout << openBlankCount << endl;
    }

    void printRemainBlankCount(vector<vector<char>>& board)
    {
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if (board[i][j] == '#')
                {
                    remainBlankCount++;
                }
            }
        }

        remainBlankCount -= bombCount;

        cout << remainBlankCount << endl;
    }

    void openCell(vector<vector<char>>& board, vector<vector<char>>& ans, int x, int y)
    {
        if (x < 0 || x >= board.size() || y < 0 || y >= board[0].size())
        {
            this->clickFailed = true;
            return;
        }

        if (board[x][y] == 'f' || board[x][y] == '?' || isdigit(board[x][y]))
        {
            this->clickFailed = true;
            return;
        }
        else if (board[x][y] == '#' && ans[x][y] == '0')
        {
            openCell(board, ans, x - 1, y - 1);
            openCell(board, ans, x - 1, y);
            openCell(board, ans, x - 1, col + 1);
            openCell(board, ans, x, y - 1);
            openCell(board, ans, x, y + 1);
            openCell(board, ans, x + 1, y - 1);
            openCell(board, ans, x + 1, y);
            openCell(board, ans, x + 1, y + 1);
            remainBlankCount--;
        }
        else if (board[x][y] == '#' && isdigit(ans[x][y]) && ans[x][y] != '0')
        {
            board[x][y] = ans[x][y];
            remainBlankCount--;
        }
        else if (ans[x][y] == 'X')
        {
            cout << "You lose the game" << endl;
            this->isEnd = true;
            return;
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

    void judgeWin()
    {
        cout << "remain: " << this->remainBlankCount << endl;

        if (this->remainBlankCount == 0)
        {
            cout << "You win the game" << endl;
            this->isEnd = true;
        }
    }

    void judgeEnd(State& gameState)
    {
        if (this->isEnd)
        {
            gameState = gameOverState;
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


    void handleEnd(State& gameState)
    {
        clickFailed = false;

        if (checkClickFailed())
        {
            // print error
            cout << "lmao error" << endl;
        }
        else
        {
            cout << "do u win?" << endl;
            judgeWin();
            judgeEnd(gameState);
        }
    }
};


