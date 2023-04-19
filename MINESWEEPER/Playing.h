#ifndef PLAYING_H
#define PLAYING_H

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
    // Intent: To initialize the Playing object with game parameters and count the number of bombs
    // Pre: row, col and board are valid and non-empty
    // Post: Playing object is initialized with member variables set to default values, and bomb count is calculated
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

        // Count the X in the board to indicate the bomb count
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

    // Intent: To return the number of bombs in the game board
    // Pre: None
    // Post: Returns the number of bombs in the game board
    int getBombCount()
    {
        return this->bombCount;
    }

    // Intent: To return the number of flagged tiles in the game board
    // Pre: None
    // Post: Returns the number of flagged tiles in the game board
    int getFlagCount()
    {
        return this->flagCount;
    }

    // Intent: To return the number of opened blank tiles in the game board
    // Pre: None
    // Post: Returns the number of opened blank tiles in the game board
    int getOpenBlankCount()
    {
        return this->openBlankCount;
    }

    // Intent: To return the number of remaining blank tiles in the game board
    // Pre: None
    // Post: Returns the number of remaining blank tiles in the game board
    int getRemainBlankCount()
    {
        return this->remainBlankCount;
    }

    // Intent: To check whether the choosen tile is valid or not
    // Pre: clickFailed has a value
    // Post: Return true if the chosen tile is valid, and false if it is not valid
    bool checkClickFailed()
    {
        if (this->clickFailed)
        {
            this->clickFailed = false;
            return true;
        }

        return false;
    }

    // Intent: Count all the blank tiles in the board
    // Pre: board is not NULL
    // Post: Update the remaining blank tiles to remainBlankCount
    void calculateBlankCount(vector<vector<char>>& board)
    {
        // Set remainBlankCount and openBlankCountto 0
        this->remainBlankCount = 0; 
        this->openBlankCount = 0;   

        // Iterates through the board
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                // If the current tiles is a number, increment the openBlankCount
                if (isdigit(board[i][j]))
                {
                    this->openBlankCount++;
                }

                // If the current tiles is not a number, increment the remainBlankCount
                else
                {
                    this->remainBlankCount++;
                }
            }
        }

        // Subtracts the remainBlankCount with bombCount, so that we can get the amount of the blank tiles 
        this->remainBlankCount -= this->bombCount;
    }

    // Intent: To store the current character in the current tile of the game board, later after changes it will become the previous character
    // Pre: The x and y coordinates must be within the range of the game board
    // Post: The previousChar variable is updated with the character in the current tile of the game board
    void setPreviousChar(vector<vector<char>>& board, int x, int y)
    {
        this->previousChar = board[x][y];
    }

    // Intent: Handling when the user want to open a tile
    // Pre: The answer board, game board, and positions are valid not NULL
    // Post: Update all the effected variables
    void leftClick(vector<vector<char>>& board, vector<vector<char>>& ans, int x, int y)
    {
        // If the current tile is out of range, return and set clickFailed to true
        if (x < 0 || x >= board.size() || y < 0 || y >= board[0].size())
        {
            // Set clickFailed to true
            this->clickFailed = true;
            return;
        }

        // If the current tile in the game board is a flag or a number, return and set clickFailed to true
        if (board[x][y] == 'f' || isdigit(board[x][y]))
        {
            // Set clickFailed to true
            this->clickFailed = true;
            return;
        }

        // Check whether the current tile in the game board is a closed tile or a question mark and the current tile in the answer board is 0
        if ((board[x][y] == '#' || board[x][y] == '?') && ans[x][y] == '0')
        {
            // Set the current tile in the game board as 0
            board[x][y] = '0';

            // Decrement the remaining blank tiles counter
            this->remainBlankCount--;

            // Iterates through the board
            for (int i = -1; i <= 1; i++)
            {
                for (int j = -1; j <= 1; j++)
                {
                    // If the current tile at the top left edge, skip
                    if (i == 0 && j == 0)
                    {
                        continue;
                    }

                    // Set new x to x + i
                    int nx = x + i;

                    // Set new y to y + i
                    int ny = y + j;

                    // Recall the function with new x and new y
                    leftClick(board, ans, nx, ny);
                }
            }
        }
        else if ((board[x][y] == '#' || board[x][y] == '?') && isdigit(ans[x][y]) && ans[x][y] != '0')
        {
            // Check whether the current tile in the game board is open or a question mark and the current tile in the answer board is a number and not 0
            // Change the current tile in the game board to the answer board current tile
            board[x][y] = ans[x][y];

            // Decrement the blank tile counter
            this->remainBlankCount--;
        }
        else if (ans[x][y] == 'X')
        {
            // If the current tile is a bomb, then set isEnd to true and return
            this->isEnd = true;
            return;
        }
    }

    // Intent: Recheck whetehr the right click is valid
    // Pre: The given board and coordinates are valid and not NULL
    // Post: Return whether the chosen tile is a vlid tile that can be opened
    void recheckLeftClick(vector<vector<char>>& board, int x, int y)
    {
        // If the current tile is a number and the previous char is open or a question mark, it can be opened and it is a valis click, so set clickFailed to false
        if (isdigit(board[x][y]) && (this->previousChar == '#' || this->previousChar == '?'))
        {
            this->clickFailed = false;
        }
    }

    // Intent: Handling the case when the user want to mark a tile as a flag or question mark
    // Pre: The given board and coordinates are valid and not NULL
    // Post: Update all effected variables
    void rightClick(vector<vector<char>>& board, int x, int y)
    {
        // If the current tile in the gameboard is a number or the coordinates still in the board, it can't be marked
        // So set click failed to true and return
        if (isdigit(board[x][y]) || x < 0 || x >= board.size() || y < 0 || y >= board[0].size())
        {
            this->clickFailed = true;
            return;
        }

        // Based on the character of the current tile, do corrresponding actions
        if (board[x][y] == '#')
        {
            // If the current tile in the gameboard is a closed tile, it can be marked as a flag
            // So set the current tile in the gameboard to 'f' to represent flag and increment the flag counter
            board[x][y] = 'f';
            this->flagCount++;
        }
        else if (board[x][y] == 'f')
        {
            // If the current tile in the gameboard is marked as a flag, it can be marked as a question mark
            // So set the current tile in the gameboard to '?' to represent the question mark and decrement the flag counter.
            board[x][y] = '?';
            this->flagCount--;
        }
        else if (board[x][y] == '?')
        {
            // If the current tile in the gameboard is marked as a question mark, it can be unmarked
            // So set the current tile in the gameboard to '#' to closed tile
            board[x][y] = '#';
        }
    }

    // Intent: To know whether the player is lost or not
    // Pre: The board is not NULL
    // Post: Update the game state
    void judgeWin(bool& isWin, bool& isEnd, STATE& gameState, vector<vector<char>>& board)
    {
        // If the player is lost, change the game state to game over and set isEnd to true and isWin to false, and return
        if (this->isEnd)
        {
            gameState = GAMEOVER;
            isEnd = true;
            isWin = false;
            return;
        }

        // Check all the blank tiles
        calculateBlankCount(board);

        // If there are no more remaining blank tiles, change the game state to game over and set isEnd and isWin to true.
        if (this->remainBlankCount == 0)
        {
            gameState = GAMEOVER;
            this->isEnd = true;
            isEnd = true;
            isWin = true;
        }
    }

};

#endif // PLAYING_H
