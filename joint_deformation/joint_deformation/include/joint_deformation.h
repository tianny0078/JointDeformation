#ifndef JOINT_DEFORMATION_H
#define JOINT_DEFORMATION_H

#include <QtGui/QMainWindow>
#include <QTimer>
#include "ui_joint_deformation.h"

class joint_deformation : public QMainWindow
{
	Q_OBJECT

public:
	joint_deformation(QWidget *parent = 0, Qt::WFlags flags = 0);
	~joint_deformation();

	void setGridDensity(int d);
	// slot function
public slots:
	void loadMesh();
	void scaleMesh();
	void showGrid();
	void showMesh(bool);
	void showVox(bool);
	void showSelection(bool);
	void showGoalPosition(bool);
	void setGridDensity();
	void setRenderMode(bool);
	

	void updateTreeDepth();
	void nextSameLevel();
	void lastSameLevel();
	void updateCurrentParentCluster(int);

	void startSimulation();
	void pauseSimulation();
	
	void simulate();
	void initializeSimulator();

	void chooseSimulator(int);

	void addLevel();
	void clearLevel();

	void chooseLevelDisplay(int);

	void setShowNum();
	
	void testNeighbor();
	void testParentNeighbor();
	void testVox4Vertex();
	void testVox4Idx();
	void testFunction();
	void testVoxConnection();

	void showTest(bool);
	void showConstraint(bool);
	void setPositionConstraint(bool);
	void setOrientationConstraint(bool);
	void setCubeOperation(bool);
	void setCubeStaticConstraint(bool);
	void setCubeActiveConstraint(bool);
	void setDampParameter();
	void setMultigrid(bool);
	void setTimesShapeMatching();
	void setForce();
	void enableSetForce(bool);
	void enableSetWindForce(bool);
	void setWindMagnitude();
	void setForceScalar();
	void setDynamics(bool);
	void setGravity(bool);
	void setGravityMagnitude();
	void setExportObj(bool);
	void setForceConstraint(bool);
	void setNetwork(bool);
	void setServer(bool);
	void setClient(bool);
	void setNumOfClients();
	void configNetwork();
	void setCaptureScreen(bool);
	void setCaptureSubScreen(bool);
	void loadCamConfig();
	void saveCamConfig();
	void sendInitData();
	void setExportTxt(bool);
	void setCompressedData(bool);
	void testSleepTime();

	void setMass();
	void resetMass();
	void setEnableMass(bool);
	void setEnergyThreshold();
private:
	Ui::joint_deformationClass ui;
	Kernel* p_kernel;
	QTimer simulation_timer;	
	//if caputre screen
	bool flag_captureScreen;
	bool flag_captureSubScreen;
	//if export obj per frame
	bool flag_exportObj;
};

#endif // JOINT_DEFORMATION_H
