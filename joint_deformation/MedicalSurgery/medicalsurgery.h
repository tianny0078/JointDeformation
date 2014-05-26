#ifndef MEDICALSURGERY_H
#define MEDICALSURGERY_H

#include <QtGui/QMainWindow>
#include <QTimer>
#include "ui_medicalsurgery.h"

class MedicalSurgery : public QMainWindow
{
	Q_OBJECT

public:
	MedicalSurgery(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MedicalSurgery();

	void setGridDensity();
public slots:
	void loadMesh();
	void scaleMesh();
	void showMesh(bool);
	void showVox(bool);
	void showGoalPosition(bool);
	void setAnchor(bool);
	void setConstraint(bool);

	void chooseSimulator(int t);

private:
	Ui::MedicalSurgeryClass ui;

	Kernel* p_kernel;
	QTimer simulation_timer;	
	//if caputre screen
	bool flag_captureScreen;
	bool flag_captureSubScreen;
	//if export obj per frame
	bool flag_exportObj;
};

#endif // MEDICALSURGERY_H
