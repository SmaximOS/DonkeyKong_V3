#include "FileHandler.h"
#include <fstream>
bool FileHandler::starts_with(const std::string& str, const std::string& prefix)
{
	return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
}
bool FileHandler::ends_with(const std::string& str, const std::string& suffix)
{
	return str.size() >= suffix.size() &&
		str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}
int FileHandler::containValidNumber(const std::string& str)
{
	int mul = 1;
	int sum = 0;
	if (str[6] < '0' || str[6]>'9' || str[7] < '0' || str[7] > '9')
		return -1;

	sum += (str[6] - '0') + (str[7] - '0') * 10;
		return sum;
	
}
int FileHandler::checkFileNameValidity(const string& name)
{
	if (name.size() == FILELEN && starts_with(name, "dkong_") && ends_with(name, ".screen"))
	{
		return containValidNumber(name);
	}
	return 0;
}
bool FileHandler::loadAllFiles(map<int, Level>& alllevels)
{
	bool res=false;

	for (const auto& entry : fs::directory_iterator(fs::current_path()))
	{
		int curLevel;
		if ((curLevel = checkFileNameValidity(entry.path().filename().string())) != 0)
		{
			if (!readFileContent(entry.path().filename().string(), alllevels[curLevel]))
				alllevels.erase(curLevel);
			else
				res = true;
		}
	}

	return res;
}
bool FileHandler:: lookForLegend(const Point& startpos, const string& str, Point* legendPos) 
{ 
	size_t pos = str.find("Lives", startpos.getX());
	
	if (pos != string::npos) //Found
	{
		legendPos->setY(startpos.getY());
		legendPos->setX(pos);
		return true;
	}

	return false;
}

bool FileHandler::readFileContent(const string& fileName, Level& toBuild) {
    std::ifstream myFile(fileName);

    // Check if the file opened successfully
    if (!myFile.is_open()) {
        std::cerr << "Error: Unable to open file: " << fileName << std::endl;
        return false;
    }

    char c;
    int currentRow = 0, currentCol = 0;
    char(*board)[GameConfig::WIDTH - 2] = toBuild.getBoardPointer();
    if (!board) {
        std::cerr << "Error: Board pointer is null." << std::endl;
        myFile.close();  // Ensure we close the file before returning
        return false;
    }

    // Read the file character by character
    while (myFile.get(c) && currentRow < GameConfig::HEIGHT) {
        if (c == '\n') {
            // Fill the remaining columns with spaces if we haven't filled all columns
            while (currentCol < GameConfig::WIDTH - 2) {
                board[currentRow][currentCol++] = ' ';
            }
            board[currentRow][currentCol] = '\0';
            ++currentRow;
            currentCol = 0;
            continue;
        }

       
        if (currentCol < GameConfig::WIDTH - 2) {
            if (c == '@')
                toBuild.setstartPosMario({ currentCol, currentRow });
            else if (c == '$')
                toBuild.setstartPosPauline({ currentCol, currentRow });
            else if (c == '&')
                toBuild.setstartPosDonkeyKong({ currentCol, currentRow });
            else if (c == 'P')
                toBuild.setPosHammer({ currentCol, currentRow });
            else if (c == 'H')  
                toBuild.addLadder(Ladder(Point(currentCol, currentRow)));
            else if (c == '=')
                board[currentRow][currentCol] = '=';
            else if (c == '>')
                board[currentRow][currentCol] = '>';
            else if (c == '<')
                board[currentRow][currentCol] = '<';

            board[currentRow][currentCol++] = c;
        }
        else {
            std::cerr << "Warning: Exceeding column limit in file " << fileName << std::endl;
        }
    }

    if (myFile.is_open()) {
        myFile.close();
    }

    return true;
}




