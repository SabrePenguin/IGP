#ifndef PATTERN_H
#define PATTERN_H

#include <fstream>
#include <iostream>
#include <string>
#include <QPixmap>
#include <QWidget>
#include <QDir>

class Pattern
{

public:
	Pattern();
	~Pattern();
	void loadPattern(QDir dir);
	
	int getW();
	int getH();
	int getX();
	int getY();
	int getReadX();
	int getReadY();
	int getSubTileNum();
	int getLargestTileOffsetX();
	int getLargestTileOffsetY();

	QPixmap getTile(int x, int y);
	QPixmap getBackground();
	char *getBackgroundName();

	int getTileW(int x, int y);
	int getTileH(int x, int y);
	int getTileX(int x, int y);
	int getTileY(int x, int y);
	int getTileReadX(int x, int y);
	int getTileReadY(int x, int y);

protected:
	std::string description;
	bool loadedTiles;

	int width;
	int height;
	int xRepeat;
	int yRepeat;
	int subTiles;
	int xReadWrite;
	int yReadWrite;
	int largestX;
	int largestY;

	int **tileOffsetX;
	int **tileOffsetY;
	int **tileReadX;
	int **tileReadY;
	int **tileWidth;
	int **tileHeight;

	QPixmap background;
	QPixmap **tiles;
};

#endif
