#include "QtWidgets"
#include "Pattern.h"

Pattern::Pattern()
{
	loadedTiles = false;
	supportTransparency = false;
}

Pattern::~Pattern()
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
			delete[] clickMasks[i];
		}
		delete[] tileOffsetX;
		delete[] tileOffsetY;
		delete[] tileReadX;
		delete[] tileReadY;
		delete[] tileWidth;
		delete[] tileHeight;
		delete[] tiles;
		delete[] clickMasks;
	}
}

// Imports all the pattern data to memory
bool Pattern::loadPattern(QDir dir)
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
			delete[] clickMasks[i];
		}
		delete[] tileOffsetX;
		delete[] tileOffsetY;
		delete[] tileReadX;
		delete[] tileReadY;
		delete[] tileWidth;
		delete[] tileHeight;
		delete[] tiles;
		delete[] clickMasks;
		loadedTiles = false;
	}

	QString fileName(dir.path()+"/"+dir.dirName()+".ini");
	QFile file(fileName);
	file.open(QIODevice::ReadOnly|QIODevice::Text);
	QTextStream iniFile(&file);
	QString line;
	bool allFound = false;
	supportTransparency = false;
	if (file.exists())
	{
		line = iniFile.readLine(100); // Ignore [General] tag
		line = iniFile.readLine(100);
		while (iniFile.status()==QTextStream::Ok && !line.contains("["))
		{
			if (line.isNull())
				return false;
			if (line.contains("="))
			{
				QStringList list = line.split("=");
				QString id, value;
				id = QString(list.at(0).toLocal8Bit().constData());
				value = QString(list.at(1).toLocal8Bit().constData());
				if (id=="Description")
				{
					description = value;
				}
				else if (id=="OutlineFilename")
				{
					background.load(QString(dir.path()+"/"+value));
				}
				else if (id=="Transparency")
				{
					supportTransparency = value.toInt();
				}
				else if (id=="Layers")
				{
					layerCount = value.toInt();
				}
				else if (id=="Width")
				{
					width = value.toInt();
				}
				else if (id=="Height")
				{
					height = value.toInt();
				}
				else if (id=="XRepeat")
				{
					xRepeat = value.toInt();
				}
				else if (id=="YRepeat")
				{
					yRepeat = value.toInt();
				}
				else if (id=="NumSubTiles")
				{
					subTiles = value.toInt();
				}
				else if (id=="xReadWrite")
				{
					xReadWrite = value.toInt();
				}
				else if (id=="yReadWrite")
				{
					yReadWrite = value.toInt();
				}
			}
			line = iniFile.readLine(100);
		}
		// Expect the number of subtiles
		loadedTiles = true;
		tileOffsetX = new int*[xReadWrite];
		tileOffsetY = new int*[xReadWrite];
		tileWidth = new int*[xReadWrite];
		tileHeight = new int*[xReadWrite];
		tileReadX = new int*[xReadWrite];
		tileReadY = new int*[xReadWrite];
		tiles = new QPixmap*[xReadWrite];
		clickMasks = new QPixmap*[xReadWrite];

		// Loop for the x subtiles
		for (int i=0; i<xReadWrite; i++)
		{
			tileOffsetX[i] = new int[yReadWrite];
			tileOffsetY[i] = new int[yReadWrite];
			tileWidth[i] = new int[yReadWrite];
			tileHeight[i] = new int[yReadWrite];
			tileReadX[i] = new int[yReadWrite];
			tileReadY[i] = new int[yReadWrite];
			tiles[i] = new QPixmap[yReadWrite];
			clickMasks[i] = new QPixmap[yReadWrite];
		}
		allFound = true;
		for (int i=0; i<xReadWrite; i++)
		{
			for (int j=0; j<yReadWrite; j++)
			{ 
				// Navigate to the next tag
				while (iniFile.status()==QTextStream::Ok && !line.contains("["))
					line = iniFile.readLine(100);
				line = iniFile.readLine(100);
				if (allFound)
					allFound = false;
				else
					return false;

				while(!allFound && iniFile.status()==QTextStream::Ok && !line.contains("["))
				{
					if (line.isNull())
						return false;
					if (line.contains("="))
					{
						QStringList list = line.split("=");
						QString id, value;
						id = QString(list.at(0).toLocal8Bit().constData());
						value = QString(list.at(1).toLocal8Bit().constData());

						if (id=="Filename")
						{
							tiles[i][j].load(QString(dir.path()+"/"+value));
							QImage tileImage;
							tileImage.load(QString(dir.path()+"/"+value));
							clickMasks[i][j]=QPixmap().fromImage(tileImage.createMaskFromColor(QColor(0,0,0,0).rgb(),Qt::MaskOutColor));
							// The following would work if QT supported alpha channels for QPixmaps
							//clickMasks[i][j]=tiles[i][j].createMaskFromColor(QColor(0,0,0,0),Qt::MaskOutColor);
						}
						else if (id=="Width")
						{
							tileWidth[i][j] = value.toInt();
						}
						else if (id=="Height")
						{
							tileHeight[i][j] = value.toInt();
						}
						else if (id=="PatternXOffset")
						{
							tileOffsetX[i][j] = value.toInt();
						}
						else if (id=="PatternYOffset")
						{
							tileOffsetY[i][j] = value.toInt();
						}
						else if (id=="ReadWriteXOffset")
						{
							tileReadX[i][j] = value.toInt();
						}
						else if (id=="ReadWriteYOffset")
						{
							tileReadY[i][j] = value.toInt();
							allFound = true;
						}
					}
					line = iniFile.readLine(100);
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

		// Fix some data to handle nicely
		if (background.size()!=QSize(xRepeat,yRepeat))
			background = background.copy(QRect(0,0,xRepeat,yRepeat));

		if (allFound)
			patternName = dir.dirName();
		return allFound;
	}
	return false;
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

bool Pattern::getTransparencySupport()
{
	return supportTransparency;
}

int Pattern::getLayerCount()
{
	return layerCount;
}

QPixmap Pattern::getTile(int x, int y)
{
	return tiles[x][y];
}

QPixmap Pattern::getClickMask(int x, int y)
{
	return clickMasks[x][y];
}

QPixmap Pattern::getBackground()
{
	return background;
}

QString Pattern::getPatternName()
{
	return patternName;
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

