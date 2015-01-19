#include <Qt/qapplication.h>
#include "MeWidg.h"

//Setting up macro
template<class T> T* showIt()
{
	T* ret = new T();
	ret->initialize(); 
	ret->showMaximized();
	return ret;
}

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	
	MeWidg widg;
	widg.show();

	return app.exec();
}