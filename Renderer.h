#ifndef RENDERER_H
#define RENDERER_H

#include <QPixmap>
#include <QWidget>
#include <QScrollArea>
#include <QLabel>
#include <QTransform>
#include <QDir>
#include "Pattern.h"

class Renderer : public QWidget
{
	Q_OBJECT

public:
	Renderer(QWidget *parent = 0);
	~Renderer();

	void newImage(int x, int y);
	bool loadImage(QString imagePath);
	bool smartResize(int x, int y);
	void resizeImage(int x, int y);
	void changePalette(QString colorFile);
	void setBackgroundColor(QColor color);
	void setOutlineColor(QColor color);
	void setBrushColor(QColor color);
	void rotateTranspose();
	void rotateClockwise();
	void rotateCounterClockwise();
	void rotate180();
	void rotateFlipX();
	void rotateFlipY();
	void zoomIn();
	void zoomOut();
	void zoomNormal();
	bool setPattern(QDir dir);
	QSize getMinSize();
	QSize getImageSize();
	bool saveImage(QString saveFileName);
	bool imageChanged();
	void countColors(QVector<QRgb> *colors, QVector<int> *counts);

protected:
    void paintEvent(QPaintEvent *e);
	void mousePressEvent(QMouseEvent *e);

private:
	// Helper Functions
	void updatePatternSize();
	void rotate(QTransform matrix);

	// Painting Variables
	bool antialiased;
	bool hasPattern;
	bool hasImage;
	bool paintedOutline;
	bool hasImageChanged;
	Pattern pattern;
	double zoom;
	int gridX;
	int gridY;
	int sizeX;
	int sizeY;
	QImage image;
	QColor outline;
	QColor background;
	QColor brush;
};

#endif
