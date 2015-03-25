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
}

Renderer::~Renderer()
{
	if (hasImage)
	{
		hasImage = false;
	}
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

void Renderer::rotateImage()
{
	if (hasImage)
	{
		QTransform matrix(0,-1,-1,0,1,1);
		QImage newImage = image.transformed(matrix);
		image.swap(newImage); 
		//std::cout << "Image rotated to size: " << gridX << "x" << gridY << "\n";

		gridX = image.width();
		gridY = image.height();
		
		updatePatternSize();
	}
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
	if (hasPattern && hasImage)
	{
		QPainter painter(this);
		int x=0;
		for (int i=0; i<=gridX/pattern.getReadX()+1; i++)
		{
			int y=0;
			for (int j=0; j<=gridY/pattern.getReadY()+1; j++)
			{
				painter.save();
				painter.scale(zoom,zoom);
				painter.translate(x,y);
				painter.scale(zoom,zoom);

				// Background.
				painter.setPen(QColor(0,0,0));		
				painter.drawPixmap(0,0,QBitmap(pattern.getBackground()));

				// Painted Tiles
				if (i<gridX/pattern.getReadX() && j<gridY/pattern.getReadY())
				{
					for (int tileNum=0; tileNum < pattern.getSubTileNum(); tileNum++)
					{
						painter.setPen(image.pixel(i*(pattern.getReadX())+pattern.getTileReadX(tileNum),j*(pattern.getReadY())+pattern.getTileReadY(tileNum)));
						painter.drawPixmap(pattern.getTileX(tileNum),pattern.getTileY(tileNum), QBitmap(pattern.getTile(tileNum)));
					}
				}
				painter.restore();
				y += int(pattern.getY()*zoom);
			}
			x += int(pattern.getX()*zoom);
		}

		//std::cout << "Painted scene\n";
		painter.end();
	}
}

void Renderer::updatePatternSize()
{
	if (hasImage && hasPattern)
	{
		sizeX = int((pattern.getW()+pattern.getX()/pattern.getReadX()*(gridX-1))*zoom*zoom);
		sizeY = int((pattern.getH()+pattern.getY()/pattern.getReadY()*(gridY-1))*zoom*zoom);
		//std::cout << "Min field size: " << sizeX << "x" << sizeY << "\n";
		update();
	}
}


