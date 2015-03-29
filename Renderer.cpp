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
		//std::cout << "Clicked: " << e->x() << ", " << e->y() << "\n";
		// Find the closest 4 grid locations and test tiles in area
		int xPos = int(e->x()*gridX/(pattern.getW()+pattern.getX()/pattern.getReadX()*(gridX-1))*zoom*zoom);
		int yPos = int(e->y()*gridY/(pattern.getH()+pattern.getY()/pattern.getReadY()*(gridY-1))*zoom*zoom);

		if (xPos<gridX && yPos<gridY)
		{
			image.setPixel(xPos,yPos,brush.rgb());
			update();
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


