#include "menu.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QSplashScreen>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QApplication::addLibraryPath("./plugins");

	QPixmap splashImg("Resources/splash/splash.png");
	QSplashScreen splash(splashImg);
	splash.show();
	a.processEvents();

	Menu menu(348, 446);
	splash.finish(&menu);
	menu.show();

	return a.exec();
}
