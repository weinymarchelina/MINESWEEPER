#include "main.h"

class Start
{
private:
	string inputFileName;
	string outputFileName;
	ifstream inputFile;
	ofstream outputFile;

public:
	Start(string inputFileName, string outputFileName)
	{
		this->inputFileName = inputFileName;
		this->outputFileName = outputFileName;
	}

	~Start()
	{
		inputFile.close();
		outputFile.close();
	}

	void readInputFile()
	{
		inputFile.open(this->inputFileName, ios::in);
		outputFile.open(this->outputFileName, ios::out);

		if (!inputFile.is_open())
		{
			cout << formatString("Input Error") << inputFileName << " is not found" << endl;
			return;
		}
		else
		{
			cout << "File is found" << endl;
		}
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

	void printGameState(State& gameState)
	{
		string line = formatStateString("Print GameState", gameState);
		cout << line << endl;
		outputFile << line << '\n';
	}

	void printOutput(string line)
	{
		cout << line << endl;
		outputFile << line << '\n';
	}

	void printBoard(vector<vector<char>>& board)
	{
		for (auto& row : board)
		{
			for (auto& col : row)
			{
				cout << col << " ";
				outputFile << col << " ";
			}

			cout << endl;
			outputFile << "\n";
		}
	}

	void printInstructionError()
	{
		cout << formatString("Input Error") << "Instruction Invalid" << endl;
	}

	void printInvalidCommand(string command, istringstream& remaining)
	{
		if (command == "Load" || command == "LeftClick" || command == "RightClick")
		{
			string fullLine = command;
			string dummy = "";

			/*
			for (int i = 0; i < 2; i++)
			{
				remaining >> dummy;
				fullLine += " " + dummy;
			}
			*/

			while (remaining >> dummy)
			{
				fullLine += " " + dummy;
			}
			 
			cout << formatStatusString(fullLine, false) << endl;
			outputFile << formatStatusString(fullLine, false) << '\n';
		}
		else if (command == "Replay" || command == "Quit")
		{
			cout << formatStatusString(command, false) << endl;
			outputFile << formatStatusString(command, false) << '\n';
		}
		else
		{
			// printInstructionError();
			string fullLine = command;
			string dummy = "";

			while (remaining >> dummy)
			{
				fullLine += " " + dummy;
			}

			cout << formatStatusString(fullLine, false) << endl;
			outputFile << formatStatusString(fullLine, false) << '\n';
		}

		/*
		else if (command == "Print")
		{
			string fullLine = command;
			string dummy = "";
			remaining >> dummy;
			fullLine += " " + dummy;
			
			cout << formatStatusString(fullLine, false) << endl;
			outputFile << formatStatusString(fullLine, false) << '\n';
		}
		*/
	}

	void printWinLose(bool isWin)
	{
		if (isWin)
		{
			cout << "You win the game" << endl;
			outputFile << "You win the game" << "\n";
		}
		else
		{
			cout << "You lose the game" << endl;
			outputFile << "You lose the game" << "\n";
		}
	}
};