#include <QApplication>
#include <QDesktopWidget>

#include "window.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QDesktopWidget dw;
	Window window;
	int x = dw.width() * 0.5;
	int y = dw.height() * 0.5;
	window.resize(QSize(x, y));
	window.show();
	return app.exec();
}
