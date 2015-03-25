#include <QtGui>

#include "MainWindow.h"

MainWindow::MainWindow()
{
	QScrollArea *scrollArea = new QScrollArea;
	renderWidget = new Renderer;
	renderWidget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

	scrollArea->setAlignment(Qt::AlignCenter);

	QWidget *widget = new QWidget;
	setCentralWidget(widget);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->setMargin(5);
	scrollArea->setWidget(renderWidget);
	layout->addWidget(scrollArea);
	widget->setLayout(layout);

	signalMapper = new QSignalMapper(this);
	createActions();
	createMenus();

	this->setWindowTitle("IGP 3.0");
	this->setWindowIcon(QIcon("./Resources/IGP.png"));
}

void MainWindow::openFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "./Img/", tr("Image Files (*.png *.jpg *.bmp)"));
	if (!fileName.isEmpty())
		renderWidget->loadImage(fileName);

	renderWidget->resize(renderWidget->getMinSize());
}

void MainWindow::saveFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Save Image"), "./Img/", tr("Image Files (*.png *.jpg *.bmp)"));
	if (!fileName.isEmpty())
		renderWidget->saveImage(fileName);
}

void MainWindow::openPattern(QString dir)
{
	renderWidget->setPattern(QDir(dir));
	renderWidget->resize(renderWidget->getMinSize());
}

void MainWindow::rotateImage()
{
	renderWidget->rotateImage();
	renderWidget->resize(renderWidget->getMinSize());
}

void MainWindow::zoomIn()
{
	renderWidget->zoomIn();
	renderWidget->resize(renderWidget->getMinSize());
}

void MainWindow::zoomOut()
{
	renderWidget->zoomOut();
	renderWidget->resize(renderWidget->getMinSize());
}

void MainWindow::zoomNormal()
{
	renderWidget->zoomNormal();
	renderWidget->resize(renderWidget->getMinSize());
}

void MainWindow::about()
{
	QMessageBox::about(this, tr("About IGP 3.0"), tr("The <b>Irregular Grid Painter</b> [IGP] allows the user to paint patterned tiles. This program's first version originated within the chainmaille community at M.A.I.L. by user Zlosk. The second version, written by user aganim, ended up being a HTML5 port. This final version is written by user TrenchCoatGuy [Brian Geuther] using the QT framework."));
}

void MainWindow::createActions()
{
	// Open Action
	openAct = new QAction(tr("&Open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(openFile()));

	// Save Action
	saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save an opened file"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(saveFile()));

	// Rotate Image Action
	rotateImageAct = new QAction(tr("&Rotate Image"), this);
    rotateImageAct->setStatusTip(tr("Rotates the source image file"));
    connect(rotateImageAct, SIGNAL(triggered()), this, SLOT(rotateImage()));

	// Zoom In Action
	zoomInAct = new QAction(tr("Zoom In"), this);
	zoomInAct->setShortcuts(QKeySequence::ZoomIn);
    zoomInAct->setStatusTip(tr("Increases size of image by 10%"));
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

	// Zoom Out Action
	zoomOutAct = new QAction(tr("Zoom Out"), this);
	zoomOutAct->setShortcuts(QKeySequence::ZoomOut);
    zoomOutAct->setStatusTip(tr("Decreases size of image by 10%"));
    connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

	// Zoom Normal Action
	zoomNormalAct = new QAction(tr("Zoom Reset"), this);
    zoomNormalAct->setStatusTip(tr("Resets the zoom to 100%"));
    connect(zoomNormalAct, SIGNAL(triggered()), this, SLOT(zoomNormal()));

	// About Action
	aboutAct = new QAction(tr("&About"), this);
    aboutAct->setShortcuts(QKeySequence::HelpContents);
    aboutAct->setStatusTip(tr("Description of this program"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(openAct);
	fileMenu->addAction(saveAct);

	editMenu = menuBar()->addMenu(tr("&Edit"));
	editMenu->addAction(rotateImageAct);
	editMenu->addAction(zoomInAct);
	editMenu->addAction(zoomOutAct);
	editMenu->addAction(zoomNormalAct);

	colorMenu = menuBar()->addMenu(tr("&Color"));

	patternMenu = menuBar()->addMenu(tr("&Patterns"));
	connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(openPattern(QString)));
	findPatterns(patternMenu);
	
	menuBar()->addSeparator();

    aboutMenu = menuBar()->addMenu(tr("About"));
    aboutMenu->addAction(aboutAct);
}

void MainWindow::findPatterns(QMenu *menu, QDir dir)
{
	// Filter for subfolders
	QStringList filters;
	filters << "*";
	dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
	QStringList subDirs = dir.entryList(filters);

	while (subDirs.length())
	{
		QString folder = subDirs.takeFirst();		
		QDir newDir(dir.path());
		newDir.cd(folder);

		// Check subfolder for ini file
		if (newDir.exists(folder+QString(".ini")))
		{
			QAction *newAct = new QAction(folder, this);
			signalMapper->setMapping(newAct, newDir.path());
			connect(newAct, SIGNAL(triggered()), signalMapper, SLOT(map()));
			menu->addAction(newAct);
			//std::cout << "Pattern Found: " << folder.toStdString() << "\n";
		}
		// Recursively repeat
		else
		{
			QMenu *subMenu;
			subMenu = menu->addMenu(folder);
			findPatterns(subMenu, newDir);
		}
	}
}

