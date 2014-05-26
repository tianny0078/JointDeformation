#include "medicalsurgery.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	glutInit(&argc, argv);
	MedicalSurgery w;
	w.show();
	return a.exec();
}
