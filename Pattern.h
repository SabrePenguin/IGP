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

	QPixmap getTile(int index);
	QPixmap getBackground();
	char *getBackgroundName();

	int getTileW(int index);
	int getTileH(int index);
	int getTileX(int index);
	int getTileY(int index);
	int getTileReadX(int index);
	int getTileReadY(int index);

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

	int *tileOffsetX;
	int *tileOffsetY;
	int *tileReadX;
	int *tileReadY;
	int *tileWidth;
	int *tileHeight;

	QPixmap background;
	QPixmap *tiles;
};

#endif
