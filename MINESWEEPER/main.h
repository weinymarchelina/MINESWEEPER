#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cctype>
#include <time.h>
#include <utility>
#include <iomanip>
using namespace std;

// Enum representing the different states of the game
enum STATE {
    START = 0,   // The starting state of the game
    STANDBY,     // The state when the game is waiting for user input
    PLAYING,     // The state when the game is in progress
    GAMEOVER     // The state when the game is over
};


#endif // MAIN_H
