#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QSignalMapper>
#include <QScrollArea>
#include <QIcon>
#include "Renderer.h"
#include "ResizeDialog.h"
#include "ColorCountDialog.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();

public slots:
	void newFile();
	void openFile();
	void openFileImport();
	void openFileSmart();
	void resizeImage();
	void smartResizeImage();
	void saveFile();
	void getColorCount();
	void setBrushColor();
	void setBackgroundColor();
	void setOutlineColor();
	void setColorPalette();
	void openPattern(QString dir);
	void rotateTranspose();
	void rotateClockwise();
	void rotateCounterClockwise();
	void rotate180();
	void rotateFlipX();
	void rotateFlipY();
	void zoomIn();
	void zoomOut();
	void zoomNormal();
	void about();

	void closeEvent(QCloseEvent *);

private:
	Renderer *renderWidget;

	// Initalizer Functions
	void createActions();
    void createMenus();
	void findPatterns(QMenu *menu, QDir dir = QDir("./Patterns/."));
	QSize startupSize();

	// Menus
	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *transformSubMenu;
	QMenu *zoomSubMenu;
	QMenu *colorMenu;
	QMenu *patternMenu;
	QMenu *aboutMenu;

	// Actions
	QAction *newAct;
	QAction *openAct;
	QAction *openImportAct;
	QAction *openSmartImportAct;
	QAction *resizeImageAct;
	QAction *saveAct;
	QAction *rotateTransposeAct;
	QAction *rotateClockwiseAct;
	QAction *rotateCounterClockwiseAct;
	QAction *rotate180Act;
	QAction *rotateFlipXAct;
	QAction *rotateFlipYAct;
	QAction *zoomInAct;
	QAction *zoomOutAct;
	QAction *zoomNormalAct;
	QAction *getColorCountAct;
	QAction *setBrushColorAct;
	QAction *setBackgroundColorAct;
	QAction *setOutlineColorAct;
	QAction *setColorPaletteAct;
	QAction *aboutAct;
	QSignalMapper *signalMapper;
};

#endif
