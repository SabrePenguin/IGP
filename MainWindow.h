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
	void rotateImage();
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
	QMenu *colorMenu;
	QMenu *patternMenu;
	QMenu *aboutMenu;

	// Actions
	QAction *openAct;
	QAction *saveAct;
	QAction *rotateImageAct;
	QAction *zoomInAct;
	QAction *zoomOutAct;
	QAction *zoomNormalAct;
	QAction *aboutAct;
	QSignalMapper *signalMapper;
};

#endif
