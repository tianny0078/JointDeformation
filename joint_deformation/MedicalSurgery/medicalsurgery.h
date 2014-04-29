#ifndef MEDICALSURGERY_H
#define MEDICALSURGERY_H

#include <QtGui/QMainWindow>
#include "ui_medicalsurgery.h"

class MedicalSurgery : public QMainWindow
{
	Q_OBJECT

public:
	MedicalSurgery(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MedicalSurgery();

private:
	Ui::MedicalSurgeryClass ui;
};

#endif // MEDICALSURGERY_H
