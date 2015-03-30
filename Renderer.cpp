#include <QtGui>
#include "Renderer.h"

const int IdRole = Qt::UserRole;

Renderer::Renderer(QWidget *parent)
    : QWidget(parent)
{
	// Initialize a couple variables
	hasPattern = false;
	hasImage = false;
    antialiased = true;
	gridX = 1;
	gridY = 1;
	outline = Qt::black;
	brush = Qt::white;

	this->setPalette(QPalette(this->backgroundRole(), Qt::white));
}

Renderer::~Renderer()
{
	if (hasImage)
	{
		hasImage = false;
	}
}

void Renderer::newImage(int x, int y)
{
	image = QImage(x,y,QImage::Format_RGB32);
	image.fill(brush.rgb());

	gridX = x;
	gridY = y;
	hasImage = true;
	
	updatePatternSize();
}

void Renderer::loadImage(QString imagePath)
{
	image.load(imagePath);
	if (!image.isNull())
	{
		gridX = image.width();
		gridY = image.height();
		//std::cout << "Image loaded: " << imagePath.toStdString() << " size: " << gridX << "x" << gridY << "\n";

		hasImage = true;
	}
	updatePatternSize();
}

void Renderer::changePalette(QString colorFile)
{
	if (!image.isNull())
	{
		QVector<QRgb> newColorTable;
		QImage paletteImage;
		paletteImage.load(colorFile);
		if (!paletteImage.isNull())
		{
			for (int i=0; i<paletteImage.width(); i++)
			{
				newColorTable.push_back(paletteImage.pixel(i,0));
			}
			image = image.convertToFormat(QImage::Format_Indexed8, newColorTable, Qt::ColorOnly);
			image = image.convertToFormat(QImage::Format_RGB32, newColorTable, Qt::ColorOnly);
			update();
		}
	}
}

void Renderer::setBackgroundColor(QColor color)
{
	this->setPalette(QPalette(this->backgroundRole(), color));
}

void Renderer::setOutlineColor(QColor color)
{
	outline = color;
}

void Renderer::setBrushColor(QColor color)
{
	brush = color;
}

void Renderer::rotateTranspose()
{
	rotate(QTransform(0,-1,-1,0,1,1));
}

void Renderer::rotateClockwise()
{
	rotate(QTransform(0,1,-1,0,1,1));
}

void Renderer::rotateCounterClockwise()
{
	rotate(QTransform(0,-1,1,0,1,1));
}

void Renderer::rotate180()
{
	rotate(QTransform(-1,0,0,-1,1,1));
}

void Renderer::rotateFlipX()
{
	rotate(QTransform(-1,0,0,1,1,1));
}

void Renderer::rotateFlipY()
{
	rotate(QTransform(1,0,0,-1,1,1));
}

void Renderer::zoomIn()
{
	zoom*=1.1;
	updatePatternSize();
}

void Renderer::zoomOut()
{
	zoom*=0.9;
	updatePatternSize();
}

void Renderer::zoomNormal()
{
	zoom=1.0;
	updatePatternSize();
}

void Renderer::setPattern(QDir dir)
{
	//std::cout << "Loaded Pattern: " << dir.dirName().toStdString() << "\n";
	pattern.loadPattern(dir);
	zoom = 1;
	hasPattern = true;
	updatePatternSize();
}

QSize Renderer::getMinSize()
{
	return QSize(sizeX,sizeY);
	
}

bool Renderer::saveImage(QString saveFileName)
{
	if (hasImage)
	{
		return image.save(saveFileName);
	}
	return false;
}

void Renderer::paintEvent(QPaintEvent *)
{
	QPainter painter(this);

	if (hasPattern && hasImage)
	{
		// Paint the outline background
		int x=0;
		int y=0;
		int gridCountX = int(gridX/pattern.getReadX());
		int gridCountY = int(gridY/pattern.getReadY());
		for (int i=0; i<=gridCountX+1; i++)
		{
			y=0;
			for (int j=0; j<=gridCountY+1; j++)
			{
				painter.save();
				painter.scale(zoom,zoom);
				painter.translate(x,y);
				painter.scale(zoom,zoom);

				painter.setPen(outline);
				painter.drawPixmap(0,0,QBitmap(pattern.getBackground()));

				painter.restore();
				y+=int(pattern.getY()*zoom);
			}
			x+=int(pattern.getX()*zoom);
		}

		// Paint the imageGrid
		x=0;
		y=0;
		for (int i=0; i<=gridCountX; i++)
		{
			y=0;
			for (int j=0; j<=gridCountY; j++)
			{
				painter.save();
				painter.scale(zoom,zoom);
				painter.translate(x,y);
				painter.scale(zoom,zoom);

				// Paint tiles
				for (int tileX=0; tileX < pattern.getReadX(); tileX++)
				{
					for (int tileY=0; tileY < pattern.getReadY(); tileY++)
					{
						int pixelX = i*(pattern.getReadX())+pattern.getTileReadX(tileX, tileY);
						int pixelY = j*(pattern.getReadY())+pattern.getTileReadY(tileX, tileY);
						if (pixelX < gridX && pixelY < gridY)
						{
							painter.setPen(image.pixel(pixelX, pixelY));
							painter.drawPixmap(pattern.getTileX(tileX, tileY),pattern.getTileY(tileX, tileY), QBitmap(pattern.getTile(tileX, tileY)));
						}
					}
				}
				painter.restore();
				y += int(pattern.getY()*zoom);
			}
			x += int(pattern.getX()*zoom);
		}
	}
	painter.end();
}

void Renderer::mousePressEvent(QMouseEvent *e)
{
	if (hasImage && hasPattern)
	{
		// Starting positions to test
		int eX = int(e->x()/zoom/zoom);
		int eY = int(e->y()/zoom/zoom);
		int startX = int(e->x()/zoom/zoom/pattern.getX());
		int startY = int(e->y()/zoom/zoom/pattern.getY());

		// Search 3x3 patterned area
		for (int i=-1; i<2; i++)
		{
			for (int j=-1; j<2; j++)
			{
				int curX = i+startX;
				int curY = j+startY;

				if (curX >= 0 && curX < gridX && curY >= 0 && curY < gridY)
				{
					// Search all subtiles
					for (int tileX=0; tileX<pattern.getReadX(); tileX++)
					{
						for (int tileY=0; tileY<pattern.getReadY(); tileY++)
						{
							QBitmap tile = pattern.getTile(tileX,tileY);
							int xTest = eX-(curX*pattern.getX()+pattern.getTileX(tileX,tileY));
							int yTest = eY-(curY*pattern.getY()+pattern.getTileY(tileX,tileY));
							if (tile.rect().contains(xTest,yTest))
							{
								// Check if clicked pixel is within tile mask
								if (QColor(tile.toImage().pixel(xTest,yTest)).black()>0)
								{
									int pixelX = curX*pattern.getReadX()+pattern.getTileReadX(tileX,tileY);
									int pixelY = curY*pattern.getReadY()+pattern.getTileReadY(tileX,tileY);
									// Make sure clicked image pixel is inside the image
									if (pixelX < gridX && pixelY < gridY)
									{
										image.setPixel(pixelX,pixelY,brush.rgb());
										update();
									}
								}
							}
						}
					}
				}
			}	
		}
	}
}

void Renderer::updatePatternSize()
{
	if (hasImage && hasPattern)
	{
		int gridXCount = int(gridX/pattern.getReadX());
		int gridYCount = int(gridY/pattern.getReadY());

		sizeX = int((pattern.getX()*gridXCount+pattern.getLargestTileOffsetX())*zoom*zoom);
		sizeY = int((pattern.getY()*gridYCount+pattern.getLargestTileOffsetY())*zoom*zoom);

		update();
	}
}

void Renderer::rotate(QTransform matrix)
{
	if (hasImage)
	{
		QImage newImage = image.transformed(matrix);
		image.swap(newImage); 

		gridX = image.width();
		gridY = image.height();
		
		updatePatternSize();
	}
}


