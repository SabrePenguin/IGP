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

void MainWindow::setBrushColor()
{
	QColor color = QColorDialog::getColor(Qt::white, this, tr("Select Brush Color"));
	if (color.isValid())
		renderWidget->setBrushColor(color);
}

void MainWindow::setBackgroundColor()
{
	QColor color = QColorDialog::getColor(Qt::white, this, tr("Select Background Color"));
	if (color.isValid())
		renderWidget->setBackgroundColor(color);
}

void MainWindow::setOutlineColor()
{
	QColor color = QColorDialog::getColor(Qt::black, this, tr("Select Background Color"));
	if (color.isValid())
		renderWidget->setOutlineColor(color);
}

void MainWindow::openPattern(QString dir)
{
	renderWidget->setPattern(QDir(dir));
	renderWidget->resize(renderWidget->getMinSize());
}

void MainWindow::rotateTranspose()
{
	renderWidget->rotateTranspose();
	renderWidget->resize(renderWidget->getMinSize());
}

void MainWindow::rotateClockwise()
{
	renderWidget->rotateClockwise();
	renderWidget->resize(renderWidget->getMinSize());
}

void MainWindow::rotateCounterClockwise()
{
	renderWidget->rotateCounterClockwise();
	renderWidget->resize(renderWidget->getMinSize());
}

void MainWindow::rotate180()
{
	renderWidget->rotate180();
	renderWidget->resize(renderWidget->getMinSize());
}

void MainWindow::rotateFlipX()
{
	renderWidget->rotateFlipX();
	renderWidget->resize(renderWidget->getMinSize());
}

void MainWindow::rotateFlipY()
{
	renderWidget->rotateFlipY();
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

	// Rotate Image Actions
	rotateTransposeAct = new QAction(tr("Transpose Image"), this);
    rotateTransposeAct->setStatusTip(tr("Transposes the source image file"));
    connect(rotateTransposeAct, SIGNAL(triggered()), this, SLOT(rotateTranspose()));

	rotateClockwiseAct = new QAction(tr("Rotate Clockwise"), this);
    rotateClockwiseAct->setStatusTip(tr("Rotates the source image file by 90 degrees clockwise"));
    connect(rotateClockwiseAct, SIGNAL(triggered()), this, SLOT(rotateClockwise()));

	rotateCounterClockwiseAct = new QAction(tr("Rotate Counter-Clockwise"), this);
    rotateCounterClockwiseAct->setStatusTip(tr("Rotates the source image file by 90 degrees counter-clockwise"));
    connect(rotateCounterClockwiseAct, SIGNAL(triggered()), this, SLOT(rotateCounterClockwise()));

	rotate180Act = new QAction(tr("Rotate 180"), this);
    rotate180Act->setStatusTip(tr("Rotates the source image file by 180 degrees"));
    connect(rotate180Act, SIGNAL(triggered()), this, SLOT(rotate180()));

	rotateFlipXAct = new QAction(tr("Reflect X axis"), this);
    rotateFlipXAct->setStatusTip(tr("Reflects the source image files X axis"));
    connect(rotateFlipXAct, SIGNAL(triggered()), this, SLOT(rotateFlipX()));

	rotateFlipYAct = new QAction(tr("Reflect Y axis"), this);
    rotateFlipYAct->setStatusTip(tr("Reflects the source image files Y axis"));
    connect(rotateFlipYAct, SIGNAL(triggered()), this, SLOT(rotateFlipY()));

	// Zoom Actions
	zoomInAct = new QAction(tr("Zoom In"), this);
	zoomInAct->setShortcuts(QKeySequence::ZoomIn);
    zoomInAct->setStatusTip(tr("Increases size of image by 10%"));
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

	zoomOutAct = new QAction(tr("Zoom Out"), this);
	zoomOutAct->setShortcuts(QKeySequence::ZoomOut);
    zoomOutAct->setStatusTip(tr("Decreases size of image by 10%"));
    connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

	zoomNormalAct = new QAction(tr("Zoom Reset"), this);
    zoomNormalAct->setStatusTip(tr("Resets the zoom to 100%"));
    connect(zoomNormalAct, SIGNAL(triggered()), this, SLOT(zoomNormal()));

	// Color Actions
	setBrushColorAct = new QAction(tr("&Brush"), this);
    setBrushColorAct->setStatusTip(tr("Select a new brush color"));
    connect(setBrushColorAct, SIGNAL(triggered()), this, SLOT(setBrushColor()));

	setBackgroundColorAct = new QAction(tr("Back&ground"), this);
    setBackgroundColorAct->setStatusTip(tr("Select a new background color"));
    connect(setBackgroundColorAct, SIGNAL(triggered()), this, SLOT(setBackgroundColor()));

	setOutlineColorAct = new QAction(tr("&Outline"), this);
    setOutlineColorAct->setStatusTip(tr("Select a new outline color"));
    connect(setOutlineColorAct, SIGNAL(triggered()), this, SLOT(setOutlineColor()));

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
	transformSubMenu = editMenu->addMenu(tr("&Transforms"));
	transformSubMenu->addAction(rotateTransposeAct);
	transformSubMenu->addAction(rotateClockwiseAct);
	transformSubMenu->addAction(rotateCounterClockwiseAct);
	transformSubMenu->addAction(rotate180Act);
	transformSubMenu->addAction(rotateFlipXAct);
	transformSubMenu->addAction(rotateFlipYAct);
	zoomSubMenu = editMenu->addMenu(tr("&Zoom"));
	zoomSubMenu->addAction(zoomInAct);
	zoomSubMenu->addAction(zoomOutAct);
	zoomSubMenu->addAction(zoomNormalAct);

	colorMenu = menuBar()->addMenu(tr("&Color"));
	colorMenu->addAction(setBrushColorAct);
	colorMenu->addAction(setBackgroundColorAct);
	colorMenu->addAction(setOutlineColorAct);

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

