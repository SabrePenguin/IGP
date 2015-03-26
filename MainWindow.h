#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QSignalMapper>
#include <QIcon>
#include "Renderer.h"

class QAction;
class QMenu;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();

public slots:
	void openFile();
	void saveFile();
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

private:
	Renderer *renderWidget;

	// Initalizer Functions
	void createActions();
    void createMenus();
	void findPatterns(QMenu *menu, QDir dir = QDir("./Patterns/."));

	// Menus
	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *transformSubMenu;
	QMenu *zoomSubMenu;
	QMenu *colorMenu;
	QMenu *patternMenu;
	QMenu *aboutMenu;

	// Actions
	QAction *openAct;
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
	QAction *aboutAct;
	QSignalMapper *signalMapper;
};

#endif
