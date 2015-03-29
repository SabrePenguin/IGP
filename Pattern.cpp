#include "Pattern.h"

Pattern::Pattern()
{
	loadedTiles = false;
}

Pattern::~Pattern()
{
	for (int i=0; i<xReadWrite; i++)
	{
		delete[] tileOffsetX[i];
		delete[] tileOffsetY[i];
		delete[] tileReadX[i];
		delete[] tileReadY[i];
		delete[] tileWidth[i];
		delete[] tileHeight[i];
		delete[] tiles[i];
	}
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
		for (int i=0; i<xReadWrite; i++)
		{
			delete[] tileOffsetX[i];
			delete[] tileOffsetY[i];
			delete[] tileReadX[i];
			delete[] tileReadY[i];
			delete[] tileWidth[i];
			delete[] tileHeight[i];
			delete[] tiles[i];
		}
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
	tileOffsetX = new int*[xReadWrite];
	tileOffsetY = new int*[xReadWrite];
	tileWidth = new int*[xReadWrite];
	tileHeight = new int*[xReadWrite];
	tileReadX = new int*[xReadWrite];
	tileReadY = new int*[xReadWrite];
	tiles = new QPixmap*[xReadWrite];

	// Loop for the x subtiles
	bool allFound = false;
	for (int i=0; i<xReadWrite; i++)
	{
		tileOffsetX[i] = new int[yReadWrite];
		tileOffsetY[i] = new int[yReadWrite];
		tileWidth[i] = new int[yReadWrite];
		tileHeight[i] = new int[yReadWrite];
		tileReadX[i] = new int[yReadWrite];
		tileReadY[i] = new int[yReadWrite];
		tiles[i] = new QPixmap[yReadWrite];
		for (int j=0; j<yReadWrite; j++)
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
					tiles[i][j].load(std::string(tileLoadName).substr(0,strlen(tileLoadName)-1).c_str());
					//std::cout << "Loaded tile: " << std::string(tileLoadName).substr(0,strlen(tileLoadName)-1).c_str() << "\n";
				}
				else if (std::string(pch)=="Width")
				{
					pch = strtok(NULL,"=");
					tileWidth[i][j] = atoi(pch);
				}
				else if (std::string(pch)=="Height")
				{
					pch = strtok(NULL,"=");
					tileHeight[i][j] = atoi(pch);
				}
				else if (std::string(pch)=="PatternXOffset")
				{
					pch = strtok(NULL,"=");
					tileOffsetX[i][j] = atoi(pch);
				}
				else if (std::string(pch)=="PatternYOffset")
				{
					pch = strtok(NULL,"=");
					tileOffsetY[i][j] = atoi(pch);
				}
				else if (std::string(pch)=="ReadWriteXOffset")
				{
					pch = strtok(NULL,"=");
					tileReadX[i][j] = atoi(pch);
				}
				else if (std::string(pch)=="ReadWriteYOffset")
				{
					pch = strtok(NULL,"=");
					tileReadY[i][j] = atoi(pch);
					allFound = true;
				}

				inifile.getline(value, 256);
			}
		}
	}

	// Fill in largest x and y values
	largestX = 0;
	largestY = 0;
	for (int i=0; i<xReadWrite; i++)
	{
		for (int j=0; j<yReadWrite; j++)
		{
			if (largestX < tileWidth[i][j] + tileOffsetX[i][j])
				largestX = tileWidth[i][j] + tileOffsetX[i][j];
			if (largestY < tileHeight[i][j] + tileOffsetY[i][j])
				largestY = tileHeight[i][j] + tileOffsetY[i][j];
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

int Pattern::getLargestTileOffsetX()
{
	return largestX;
}

int Pattern::getLargestTileOffsetY()
{
	return largestY;
}

QPixmap Pattern::getTile(int x, int y)
{
	return tiles[x][y];
}

QPixmap Pattern::getBackground()
{
	return background;
}

int Pattern::getTileW(int x, int y)
{
	return tileWidth[x][y];
}

int Pattern::getTileH(int x, int y)
{
	return tileHeight[x][y];
}

int Pattern::getTileX(int x, int y)
{
	return tileOffsetX[x][y];
}

int Pattern::getTileY(int x, int y)
{
	return tileOffsetY[x][y];
}

int Pattern::getTileReadX(int x, int y)
{
	return tileReadX[x][y];
}

int Pattern::getTileReadY(int x, int y)
{
	return tileReadY[x][y];
}

