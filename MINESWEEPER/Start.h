#include "main.h"

class Start
{
private:
	string inputFileName;
	string outputFileName;
	ifstream inputFile;
	ofstream outputFile;

public:
	Start()
	{
		this->inputFileName = "";
		this->outputFileName = "";
	}

	~Start()
	{
		inputFile.close();
		outputFile.close();
	}

	istringstream getCommand()
	{
		string fullCommand;

		getline(inputFile, fullCommand);

		istringstream iss(fullCommand);

		return iss;
	}

	string formatString(string line, string additional = "")
	{
		string formattedString = line;
		
		formattedString.insert(0, "<");
		formattedString += "> : " + additional;

		return formattedString;
	}

	string formatStatusString(string line, bool status)
	{
		string formattedString = formatString(line);

		if (status) formattedString += "Success";
		else formattedString += "Failed";

		return formattedString;
	}

	string formatStateString(string line, State& gameState)
	{
		string formattedString = formatString(line);

		switch (gameState)
		{
		case standByState:
			formattedString += "Standby";
			break;

		case playingState:
			formattedString += "Playing";
			break;

		case gameOverState:
			formattedString += "GameOver";
			break;

		default:
			formattedString += "Start";
			break;
		}

		return formattedString;
	}

	bool readInputFile()
	{
		inputFile.open(this->inputFileName, ios::in);
		outputFile.open(this->outputFileName, ios::out);

		if (!inputFile.is_open())
		{
			printCheckFiles(this->inputFileName, this->outputFileName, false);
			return false;
		}

		return true;
	}

	void setFiles(string inputFileName, string outputFileName)
	{
		this->inputFileName = inputFileName;
		this->outputFileName = outputFileName;
	}

	void printCheckFiles(string inputFileName, string outputFileName, bool isSuccess)
	{
		string line = "MineSweeper.exe CommandFile " + inputFileName + " " + outputFileName;
		cout << formatStatusString(line, isSuccess) << endl;
	}

	void printGameState(State& gameState)
	{
		string line = formatStateString("Print GameState", gameState);
		outputFile << line << '\n';
	}

	void printOutput(string line)
	{
		outputFile << line << '\n';
	}

	void printBoard(vector<vector<char>>& board)
	{
		for (auto& row : board)
		{
			for (auto& col : row)
			{
				outputFile << col << " ";
			}

			outputFile << "\n";
		}
	}

	void printInvalidCommand(string command, istringstream& remaining)
	{
		if (command == "Load" || command == "LeftClick" || command == "RightClick")
		{
			string fullLine = command;
			string dummy = "";

			while (remaining >> dummy)
			{
				fullLine += " " + dummy;
			}
			 
			outputFile << formatStatusString(fullLine, false) << '\n';
		}
		else if (command == "Replay" || command == "Quit")
		{
			outputFile << formatStatusString(command, false) << '\n';
		}
		else
		{
			string fullLine = command;
			string dummy = "";

			while (remaining >> dummy)
			{
				fullLine += " " + dummy;
			}

			outputFile << formatStatusString(fullLine, false) << '\n';
		}
	}

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