#include<WinSock2.h>//dont move this for windows.h mess with winsock
#include<Windows.h>
#ifdef _WIN32
	#include "network/HNWindowsObjectFactory.h"
	#include "network/HNWindowsNetworkObjectFactory.h"
#endif
#include "joint_deformation.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{

#ifdef _WIN32
	HNWindowsObjectFactory::setAsDefaultFactory();
	HNWindowsNetworkObjectFactory::setAsDefaultFactory();
#endif

	QApplication a(argc, argv);
	glutInit(&argc, argv);
	joint_deformation w;
	w.show();
	return a.exec();
}
