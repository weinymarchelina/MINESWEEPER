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
			formattedString += "Game Over";
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

};