#include <QApplication>

#include "Renderer.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;

 #if defined(Q_OS_SYMBIAN)
	window.showMaximized();
 #else
	window.show();
	window.resize(600,400);
 #endif
    return app.exec();
}
