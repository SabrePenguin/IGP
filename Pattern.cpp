#include "Pattern.h"

Pattern::Pattern()
{
	loadedTiles = false;
}

Pattern::~Pattern()
{
	delete[] tileOffsetX;
	delete[] tileOffsetY;
	delete[] tileReadX;
	delete[] tileReadY;
	delete[] tileWidth;
	delete[] tileHeight;
	delete[] tiles;
}

// Imports all the pattern data to memory
// ToDo: Better handle file reading loop. Possible infinite loop if the last parameter is never found for any of the subtiles.
void Pattern::loadPattern(QDir dir)
{
	// Delete previously loaded tiles
	if (loadedTiles)
	{
		delete[] tileOffsetX;
		delete[] tileOffsetY;
		delete[] tileReadX;
		delete[] tileReadY;
		delete[] tileWidth;
		delete[] tileHeight;
		delete[] tiles;
	}

	// Set up background image
	char pathName[256] = "";
	strcat(pathName,dir.path().toStdString().c_str());
	strcat(pathName,"/");
	char iniFileName[256];
	strcpy(iniFileName, pathName);
	strcat(iniFileName,dir.dirName().toStdString().c_str());
	strcat(iniFileName,".ini");

	// Set up file reader values
	std::ifstream inifile;
	inifile.open(iniFileName);
	char value[256];

	// Read and import general information
	inifile.getline(value, 256); // ignore first line with [General] tag
	inifile.getline(value, 256);
	while(!inifile.eof() && std::string(value).find("["))
	{
		char * pch;
		pch = strtok (value,"=");

		if (std::string(pch)=="Description")
		{
			pch = strtok(NULL,"=");
			description = std::string(pch);
		}
		else if (std::string(pch)=="OutlineFilename")
		{
			pch = strtok(NULL,"=");
			char backgroundFileName[256];
			strcpy(backgroundFileName, pathName);
			strcat(backgroundFileName, pch);
			background.load(std::string(backgroundFileName).substr(0,strlen(backgroundFileName)-1).c_str());
			//std::cout << "Background loaded: " << std::string(backgroundFileName).substr(0,strlen(backgroundFileName)-1).c_str() << "\n";;
		}
		else if (std::string(pch)=="Width")
		{
			pch = strtok(NULL,"=");
			width = atoi(pch);
		}
		else if (std::string(pch)=="Height")
		{
			pch = strtok(NULL,"=");
			height = atoi(pch);
		}
		else if (std::string(pch)=="XRepeat")
		{
			pch = strtok(NULL,"=");
			xRepeat = atoi(pch);
		}
		else if (std::string(pch)=="YRepeat")
		{
			pch = strtok(NULL,"=");
			yRepeat = atoi(pch);
		}
		else if (std::string(pch)=="NumSubTiles")
		{
			pch = strtok(NULL,"=");
			subTiles = atoi(pch);
		}
		else if (std::string(pch)=="xReadWrite")
		{
			pch = strtok(NULL,"=");
			xReadWrite = atoi(pch);
		}
		else if (std::string(pch)=="yReadWrite")
		{
			pch = strtok(NULL,"=");
			yReadWrite = atoi(pch);
		}

		inifile.getline(value,256);
	}

	// Expect the number of subtiles
	tileOffsetX = new int[subTiles];
	tileOffsetY = new int[subTiles];
	tileWidth = new int[subTiles];
	tileHeight = new int[subTiles];
	tileReadX = new int[subTiles];
	tileReadY = new int[subTiles];
	tiles = new QPixmap[subTiles];

	// Loop for the subtiles
	bool allFound = false;
	for (int i=0; i<subTiles; i++)
	{
		// Ignore the subtile square bracket tags
		inifile.getline(value, 256);
		// Ignore return upon next iteration
		if (allFound)
			inifile.getline(value, 256);
		allFound = false;
		while(!allFound && std::string(value).find("["))
		{
			char * pch;
			pch = strtok (value,"=");

			if (std::string(pch)=="Filename")
			{
				pch = strtok(NULL,"=");
				char tileLoadName[256];
				strcpy(tileLoadName, pathName);
				strcat(tileLoadName, pch);
				tiles[i].load(std::string(tileLoadName).substr(0,strlen(tileLoadName)-1).c_str());
				//std::cout << "Loaded tile: " << std::string(tileLoadName).substr(0,strlen(tileLoadName)-1).c_str() << "\n";
			}
			else if (std::string(pch)=="Width")
			{
				pch = strtok(NULL,"=");
				tileWidth[i] = atoi(pch);
			}
			else if (std::string(pch)=="Height")
			{
				pch = strtok(NULL,"=");
				tileHeight[i] = atoi(pch);
			}
			else if (std::string(pch)=="PatternXOffset")
			{
				pch = strtok(NULL,"=");
				tileOffsetX[i] = atoi(pch);
			}
			else if (std::string(pch)=="PatternYOffset")
			{
				pch = strtok(NULL,"=");
				tileOffsetY[i] = atoi(pch);
			}
			else if (std::string(pch)=="ReadWriteXOffset")
			{
				pch = strtok(NULL,"=");
				tileReadX[i] = atoi(pch);
			}
			else if (std::string(pch)=="ReadWriteYOffset")
			{
				pch = strtok(NULL,"=");
				tileReadY[i] = atoi(pch);
				allFound = true;
			}

			inifile.getline(value, 256);
		}
	}
}

int Pattern::getW()
{
	return width;
}

int Pattern::getH()
{
	return height;
}

int Pattern::getX()
{
	return xRepeat;
}

int Pattern::getY()
{
	return yRepeat;
}

int Pattern::getReadX()
{
	return xReadWrite;
}

int Pattern::getReadY()
{
	return yReadWrite;
}

int Pattern::getSubTileNum()
{
	return subTiles;
}

QPixmap Pattern::getTile(int index)
{
	return tiles[index];
}

QPixmap Pattern::getBackground()
{
	return background;
}

int Pattern::getTileW(int index)
{
	return tileWidth[index];
}

int Pattern::getTileH(int index)
{
	return tileHeight[index];
}

int Pattern::getTileX(int index)
{
	return tileOffsetX[index];
}

int Pattern::getTileY(int index)
{
	return tileOffsetY[index];
}

int Pattern::getTileReadX(int index)
{
	return tileReadX[index];
}

int Pattern::getTileReadY(int index)
{
	return tileReadY[index];
}

