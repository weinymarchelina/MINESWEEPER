#ifndef START_H
#define START_H

#include "main.h"

class Start
{
private:
	string inputFileName;
	string outputFileName;
	ifstream inputFile;
	ofstream outputFile;

public:
	// Intent: This code initializes the "Start" class with default values for its member variables.
	// Pre: None
	// Post: The "Start" class is initialized with default values for its member variables.
	Start()
	{
		this->inputFileName = "";
		this->outputFileName = "";
	}

	// Intent: To destroy the Start object
	// Pre: A Start object exists
	// Post: The Start object is destroyed and closes the input and output files
	~Start()
	{
		inputFile.close();
		outputFile.close();
	}

	// Intent: Get command from input file
	// Pre: The input file should be open and accessible
	// Post:  Returns an istringstream object containing the next line of input
	istringstream getCommand()
	{
		string fullCommand;

		// Read a line from the input file
		getline(this->inputFile, fullCommand);

		// Creates istringstream object containing the next line of input
		istringstream iss(fullCommand);

		return iss;
	}

	// Intent: Format a string with additional information
	// Pre: The line parameter must be a valid string
	// Post:  Returns the formatted string with additional information
	string formatString(string line, string additional = "")
	{
		string formattedString = line;

		formattedString.insert(0, "<");
		formattedString += "> : " + additional;

		return formattedString;
	}

	// Intent: Format a status string with "Success" or "Failed"
	// Pre: The line parameter must be a valid string
	// Post:  Returns the formatted status string with "Success" or "Failed"
	string formatStatusString(string line, bool status)
	{
		// Format the command string
		string formattedString = formatString(line);

		if (status)
		{
			formattedString += "Success";
		}
		else
		{
			formattedString += "Failed";
		}

		return formattedString;
	}

	// Intent: To format a string to include a STATE enum value
	// Pre: The line parameter must be a valid string and gameState must be a valid STATE enum value
	// Post:  A string containing the formatted line parameter and the corresponding STATE enum value is returned
	string formatStateString(string line, STATE& gameState)
	{
		// Format the command string
		string formattedString = formatString(line);

		// Switch statement to append the corresponding STATE enum value to the formatted string
		switch (gameState)
		{
		case STANDBY:
			formattedString += "Standby";
			break;

		case PLAYING:
			formattedString += "Playing";
			break;

		case GAMEOVER:
			formattedString += "GameOver";
			break;

		default:
			formattedString += "Start";
			break;
		}

		return formattedString;
	}

	// Intent: To read input and output files and verify if the input file is open
	// Pre:  The inputFileName and outputFileName parameters must be valid string values
	// Post:  The inputFile is opened in read mode, the outputFile is opened in write mode, and a boolean value is returned indicating if the input file was successfully opened or not
	bool readInputFile()
	{
		// Opening input and output files
		inputFile.open(this->inputFileName, ios::in);
		outputFile.open(this->outputFileName, ios::out);

		// Checking if the input file is open and returning a boolean value accordingly
		if (!inputFile.is_open())
		{
			printCheckFiles(this->inputFileName, this->outputFileName, false);
			return false;
		}

		return true;
	}

	// Intent: To set the input and output file names
	// Pre:  The inputFileName and outputFileName parameters must be valid string values
	// Post: The inputFileName and outputFileName member variables are set to the corresponding parameter values
	void setFiles(string inputFileName, string outputFileName)
	{
		this->inputFileName = inputFileName;
		this->outputFileName = outputFileName;
	}

	// Intent: To print a formatted string indicating if the command file was successfully opened or not
	// Pre:  The inputFileName and outputFileName parameters must be valid string values and isSuccess must be a boolean value
	// Post:  A formatted string indicating if the command file was successfully opened or not is printed to the console
	void printCheckFiles(string inputFileName, string outputFileName, bool isSuccess)
	{
		// Formatting the string to be printed to the console
		string line = "MineSweeper.exe CommandFile " + inputFileName + " " + outputFileName;

		// Print the line to the console
		cout << formatStatusString(line, isSuccess) << endl;
	}

	// Intent: To print the current state of the game
	// Pre: The gameState must be valid and initialized properly
	// Post:  The current state of the game is printed to the outputFile
	void printGameState(STATE& gameState)
	{
		// Formatting the string to be printed
		string line = formatStateString("Print GameState", gameState);

		// Print the string on the output file
		outputFile << line << '\n';
	}

	// Intent: To print the output message to the outputFile
	// Pre: The line parameter must be a valid string
	// Post:  The line is printed to the outputFile
	void printOutput(string line)
	{
		outputFile << line << '\n';
	}

	// Intent: To print the game board to the outputFile
	// Pre: The board parameter must be a valid vector of vectors of chars, representing the game board
	// Post:  The game board is printed to the outputFile
	void printBoard(vector<vector<char>>& board)
	{
		// Loop through board
		for (auto& row : board)
		{
			for (auto& col : row)
			{
				outputFile << col << " ";
			}

			// Print break line after each row
			outputFile << "\n";
		}
	}

	// Intent: To print that the command is failed to be executed message when an invalid command is received
	// Pre: The command parameter must be a valid string representing the invalid command, and remaining parameter must be a valid istringstream containing the remaining invalid command arguments
	// Post:  An message is printed to the outputFile stating that the command is invalid
	void printInvalidCommand(string command, istringstream& remaining)
	{
		// Check the command and execute the corresponding action
		if (command == "Load" || command == "LeftClick" || command == "RightClick")
		{
			// Create a string with the full line including the remaining arguments
			string fullLine = command;
			string dummy = "";

			// Concatenate remaining arguments to fullLine string
			while (remaining >> dummy)
			{
				fullLine += " " + dummy;
			}

			// Write the formatted string to the output file
			outputFile << formatStatusString(fullLine, false) << '\n';
		}
		else if (command == "Replay" || command == "Quit")
		{
			// Write the formatted string to the output file
			outputFile << formatStatusString(command, false) << '\n';
		}
		else
		{
			string fullLine = command;
			string dummy = "";

			// Concatenate remaining arguments to fullLine string
			while (remaining >> dummy)
			{
				fullLine += " " + dummy;
			}

			// Write the formatted string to the output file
			outputFile << formatStatusString(fullLine, false) << '\n';
		}
	}

	// Intent: To print a message indicating whether the game is won or lost
	// Pre: The isWin parameter must be a valid boolean
	// Post:  A message is printed to the outputFile indicating whether the game is won or lost
	void printWinLose(bool isWin)
	{
		if (isWin)
		{
			outputFile << "You win the game" << "\n";
		}
		else
		{
			outputFile << "You lose the game" << "\n";
		}
	}
};

#endif // START_H
