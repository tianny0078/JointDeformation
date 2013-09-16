/********************************************************************************
** Form generated from reading UI file 'joint_deformation.ui'
**
** Created: Sun Sep 15 17:44:27 2013
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_JOINT_DEFORMATION_H
#define UI_JOINT_DEFORMATION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>
#include "renderer.h"

QT_BEGIN_NAMESPACE

class Ui_joint_deformationClass
{
public:
    QAction *actionLoadMesh;
    QAction *actionScaleMesh;
    QAction *actionShowGrid;
    QAction *actionShowMesh;
    QAction *actionShowVox;
    QAction *actionShowSelection;
    QAction *actionSetClusterTree;
    QAction *actionShowGoalPosition;
    QAction *actionShowTest;
    QAction *actionSetConstraint;
    QAction *actionSetMass;
    QAction *actionCubeOperation;
    QAction *actionSetCubeStaticConstraint;
    QAction *actionSetCubeActiveConstraint;
    QWidget *centralWidget;
    Renderer *renderWidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpinBox *spinBox_gridDensity;
    QPushButton *pushButton_setGridDensity;
    QWidget *horizontalLayoutWidget_3;
    QHBoxLayout *horizontalLayout_3;
    QComboBox *comboBox_selectTreeDepth;
    QPushButton *pushButton_lastSameLevel;
    QPushButton *pushButton_nextSameLevel;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QWidget *horizontalLayoutWidget_4;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_2;
    QComboBox *comboBox_level;
    QPushButton *pushButton_addLevel;
    QPushButton *pushButton_deleteLevel;
    QWidget *horizontalLayoutWidget_5;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_3;
    QLineEdit *lineEdit_damping;
    QPushButton *pushButton_setDamping;
    QWidget *horizontalLayoutWidget_6;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_4;
    QSpinBox *spinBox_TimesShapeMatching;
    QPushButton *pushButton_setTimesSM;
    QWidget *horizontalLayoutWidget_7;
    QHBoxLayout *horizontalLayout_7;
    QCheckBox *checkBox_multigrid;
    QCheckBox *checkBox_dynamics;
    QWidget *horizontalLayoutWidget_10;
    QHBoxLayout *horizontalLayout_10;
    QCheckBox *checkBox_gravity;
    QLabel *label_18;
    QLineEdit *lineEdit_gravityMagnitude;
    QPushButton *pushButton_setGravityMagnitude;
    QWidget *horizontalLayoutWidget_11;
    QHBoxLayout *horizontalLayout_11;
    QCheckBox *checkBox_exportObj;
    QCheckBox *checkBox_captureScreen;
    QCheckBox *checkBox_captureSubScreen;
    QWidget *horizontalLayoutWidget_9;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_17;
    QLineEdit *lineEdit_forceScalar;
    QPushButton *pushButton_setForceScalar;
    QWidget *tab_2;
    QWidget *horizontalLayoutWidget_8;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_5;
    QLineEdit *lineEdit_forceX;
    QLabel *label_6;
    QLineEdit *lineEdit_forceY;
    QLabel *label_7;
    QLineEdit *lineEdit_forceZ;
    QPushButton *pushButton_setForce;
    QWidget *horizontalLayoutWidget_15;
    QHBoxLayout *horizontalLayout_16;
    QCheckBox *checkBox_setForce;
    QCheckBox *checkBox_exportTxt;
    QWidget *horizontalLayoutWidget_16;
    QHBoxLayout *horizontalLayout_15;
    QPushButton *pushButton_loadCamConfig;
    QPushButton *pushButton_saveCameraConfig;
    QWidget *horizontalLayoutWidget_17;
    QHBoxLayout *horizontalLayout_17;
    QPushButton *pushButton_loadConstraint;
    QPushButton *pushButton_saveConstraint;
    QWidget *horizontalLayoutWidget_19;
    QHBoxLayout *horizontalLayout_19;
    QCheckBox *checkBox_setWindForce;
    QLabel *label_19;
    QLineEdit *lineEdit_windMagnitude;
    QPushButton *pushButton_setWindMagnitude;
    QWidget *horizontalLayoutWidget_20;
    QHBoxLayout *horizontalLayout_20;
    QLabel *label_20;
    QLineEdit *lineEdit_mass;
    QPushButton *pushButton_setMass;
    QPushButton *pushButton_resetMass;
    QWidget *tab_3;
    QWidget *horizontalLayoutWidget_12;
    QHBoxLayout *horizontalLayout_12;
    QCheckBox *checkBox_Network;
    QRadioButton *radioButton_Server;
    QRadioButton *radioButton_Client;
    QWidget *horizontalLayoutWidget_13;
    QHBoxLayout *horizontalLayout_13;
    QSpinBox *spinBox_NumClients;
    QPushButton *pushButton_setNumOfClients;
    QPushButton *pushButton_configNetwork;
    QListWidget *listWidget_Status;
    QWidget *horizontalLayoutWidget_14;
    QHBoxLayout *horizontalLayout_14;
    QLabel *label_9;
    QLineEdit *lineEdit_IP;
    QLabel *label_10;
    QLineEdit *lineEdit_Port;
    QWidget *horizontalLayoutWidget_18;
    QHBoxLayout *horizontalLayout_18;
    QPushButton *pushButton_dataFirst;
    QWidget *horizontalLayoutWidget_21;
    QHBoxLayout *horizontalLayout_21;
    QCheckBox *checkBox_isCompressed;
    QWidget *test;
    QGroupBox *groupBox_2;
    QSpinBox *spinBox_gridDensity_2;
    QSpinBox *spinBox_gridDensity_3;
    QPushButton *pushButton_3;
    QPushButton *pushButton_neighbor;
    QPushButton *pushButton_parent_neighbor;
    QSpinBox *spinBox_gridDensity_vertex;
    QPushButton *pushButton_vox4vertex;
    QSpinBox *spinBox_voxX;
    QSpinBox *spinBox_voxY;
    QSpinBox *spinBox_voxZ;
    QPushButton *pushButton_vox4idx;
    QPushButton *pushButton_showLowerMesh;
    QWidget *horizontalLayoutWidget_22;
    QHBoxLayout *horizontalLayout_22;
    QPushButton *pushButton_testSleep;
    QLineEdit *lineEdit_sleep;
    QSpinBox *spinBox_vox_vertex;
    QPushButton *pushButton_testFunction;
    QPushButton *pushButton_testVoxConnection;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_initSimulator;
    QPushButton *pushButton_startSimulation;
    QPushButton *pushButton_nextStep;
    QPushButton *pushButton_pauseSimulation;
    QPushButton *pushButton_resetSimulation;
    QComboBox *comboBox_chooseSimulator;
    QGroupBox *groupBox_3;
    QRadioButton *radioButton_setForceConstraint;
    QRadioButton *radioButton_setPositionConstraint;
    QRadioButton *radioButton_setOrientationConstraint;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *joint_deformationClass)
    {
        if (joint_deformationClass->objectName().isEmpty())
            joint_deformationClass->setObjectName(QString::fromUtf8("joint_deformationClass"));
        joint_deformationClass->resize(1400, 850);
        joint_deformationClass->setMinimumSize(QSize(1400, 850));
        joint_deformationClass->setMaximumSize(QSize(1400, 850));
        actionLoadMesh = new QAction(joint_deformationClass);
        actionLoadMesh->setObjectName(QString::fromUtf8("actionLoadMesh"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/joint_deformation/Resources/fileopen.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLoadMesh->setIcon(icon);
        actionScaleMesh = new QAction(joint_deformationClass);
        actionScaleMesh->setObjectName(QString::fromUtf8("actionScaleMesh"));
        actionScaleMesh->setCheckable(true);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/joint_deformation/Resources/scale_ruler.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionScaleMesh->setIcon(icon1);
        actionShowGrid = new QAction(joint_deformationClass);
        actionShowGrid->setObjectName(QString::fromUtf8("actionShowGrid"));
        actionShowGrid->setCheckable(true);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/joint_deformation/Resources/Matrix.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionShowGrid->setIcon(icon2);
        actionShowMesh = new QAction(joint_deformationClass);
        actionShowMesh->setObjectName(QString::fromUtf8("actionShowMesh"));
        actionShowMesh->setCheckable(true);
        actionShowMesh->setChecked(false);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/joint_deformation/Resources/mesh.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionShowMesh->setIcon(icon3);
        actionShowVox = new QAction(joint_deformationClass);
        actionShowVox->setObjectName(QString::fromUtf8("actionShowVox"));
        actionShowVox->setCheckable(true);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/joint_deformation/Resources/CMS.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionShowVox->setIcon(icon4);
        actionShowSelection = new QAction(joint_deformationClass);
        actionShowSelection->setObjectName(QString::fromUtf8("actionShowSelection"));
        actionShowSelection->setCheckable(true);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/joint_deformation/Resources/Selection.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionShowSelection->setIcon(icon5);
        actionSetClusterTree = new QAction(joint_deformationClass);
        actionSetClusterTree->setObjectName(QString::fromUtf8("actionSetClusterTree"));
        actionSetClusterTree->setCheckable(true);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/joint_deformation/Resources/tree_diagramm_new.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSetClusterTree->setIcon(icon6);
        actionShowGoalPosition = new QAction(joint_deformationClass);
        actionShowGoalPosition->setObjectName(QString::fromUtf8("actionShowGoalPosition"));
        actionShowGoalPosition->setCheckable(true);
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/joint_deformation/Resources/target.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionShowGoalPosition->setIcon(icon7);
        actionShowTest = new QAction(joint_deformationClass);
        actionShowTest->setObjectName(QString::fromUtf8("actionShowTest"));
        actionShowTest->setCheckable(true);
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/joint_deformation/Resources/dialog_apply.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionShowTest->setIcon(icon8);
        actionSetConstraint = new QAction(joint_deformationClass);
        actionSetConstraint->setObjectName(QString::fromUtf8("actionSetConstraint"));
        actionSetConstraint->setCheckable(true);
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/joint_deformation/Resources/Selection4Constraint.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSetConstraint->setIcon(icon9);
        actionSetMass = new QAction(joint_deformationClass);
        actionSetMass->setObjectName(QString::fromUtf8("actionSetMass"));
        actionSetMass->setCheckable(true);
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/joint_deformation/Resources/TwoCubes.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSetMass->setIcon(icon10);
        actionCubeOperation = new QAction(joint_deformationClass);
        actionCubeOperation->setObjectName(QString::fromUtf8("actionCubeOperation"));
        actionCubeOperation->setCheckable(true);
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/joint_deformation/Resources/gnome_settings_theme.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCubeOperation->setIcon(icon11);
        actionSetCubeStaticConstraint = new QAction(joint_deformationClass);
        actionSetCubeStaticConstraint->setObjectName(QString::fromUtf8("actionSetCubeStaticConstraint"));
        actionSetCubeStaticConstraint->setCheckable(true);
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/joint_deformation/Resources/gtk_media_stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSetCubeStaticConstraint->setIcon(icon12);
        actionSetCubeActiveConstraint = new QAction(joint_deformationClass);
        actionSetCubeActiveConstraint->setObjectName(QString::fromUtf8("actionSetCubeActiveConstraint"));
        actionSetCubeActiveConstraint->setCheckable(true);
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/joint_deformation/Resources/gtk_missing_image.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSetCubeActiveConstraint->setIcon(icon13);
        centralWidget = new QWidget(joint_deformationClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        renderWidget = new Renderer(centralWidget);
        renderWidget->setObjectName(QString::fromUtf8("renderWidget"));
        renderWidget->setGeometry(QRect(10, 20, 1024, 768));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(1050, 50, 331, 371));
        tabWidget->setTabPosition(QTabWidget::East);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        horizontalLayoutWidget = new QWidget(tab);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 40, 291, 31));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(horizontalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(150, 25));
        label->setMaximumSize(QSize(150, 25));

        horizontalLayout->addWidget(label);

        spinBox_gridDensity = new QSpinBox(horizontalLayoutWidget);
        spinBox_gridDensity->setObjectName(QString::fromUtf8("spinBox_gridDensity"));
        spinBox_gridDensity->setMinimumSize(QSize(0, 25));
        spinBox_gridDensity->setMaximumSize(QSize(16777215, 25));
        spinBox_gridDensity->setMinimum(1);
        spinBox_gridDensity->setMaximum(256);
        spinBox_gridDensity->setValue(3);

        horizontalLayout->addWidget(spinBox_gridDensity);

        pushButton_setGridDensity = new QPushButton(horizontalLayoutWidget);
        pushButton_setGridDensity->setObjectName(QString::fromUtf8("pushButton_setGridDensity"));
        pushButton_setGridDensity->setMinimumSize(QSize(70, 25));
        pushButton_setGridDensity->setMaximumSize(QSize(70, 25));
        pushButton_setGridDensity->setIcon(icon8);

        horizontalLayout->addWidget(pushButton_setGridDensity);

        horizontalLayoutWidget_3 = new QWidget(tab);
        horizontalLayoutWidget_3->setObjectName(QString::fromUtf8("horizontalLayoutWidget_3"));
        horizontalLayoutWidget_3->setGeometry(QRect(10, 80, 291, 41));
        horizontalLayout_3 = new QHBoxLayout(horizontalLayoutWidget_3);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        comboBox_selectTreeDepth = new QComboBox(horizontalLayoutWidget_3);
        comboBox_selectTreeDepth->setObjectName(QString::fromUtf8("comboBox_selectTreeDepth"));
        comboBox_selectTreeDepth->setEnabled(false);
        comboBox_selectTreeDepth->setMinimumSize(QSize(70, 25));
        comboBox_selectTreeDepth->setMaximumSize(QSize(70, 25));

        horizontalLayout_3->addWidget(comboBox_selectTreeDepth);

        pushButton_lastSameLevel = new QPushButton(horizontalLayoutWidget_3);
        pushButton_lastSameLevel->setObjectName(QString::fromUtf8("pushButton_lastSameLevel"));
        pushButton_lastSameLevel->setMinimumSize(QSize(30, 25));
        pushButton_lastSameLevel->setMaximumSize(QSize(30, 25));

        horizontalLayout_3->addWidget(pushButton_lastSameLevel);

        pushButton_nextSameLevel = new QPushButton(horizontalLayoutWidget_3);
        pushButton_nextSameLevel->setObjectName(QString::fromUtf8("pushButton_nextSameLevel"));
        pushButton_nextSameLevel->setMinimumSize(QSize(30, 25));
        pushButton_nextSameLevel->setMaximumSize(QSize(25, 16777215));

        horizontalLayout_3->addWidget(pushButton_nextSameLevel);

        pushButton = new QPushButton(horizontalLayoutWidget_3);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMinimumSize(QSize(55, 25));
        pushButton->setMaximumSize(QSize(55, 25));

        horizontalLayout_3->addWidget(pushButton);

        pushButton_2 = new QPushButton(horizontalLayoutWidget_3);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setMinimumSize(QSize(55, 25));
        pushButton_2->setMaximumSize(QSize(55, 25));

        horizontalLayout_3->addWidget(pushButton_2);

        horizontalLayoutWidget_4 = new QWidget(tab);
        horizontalLayoutWidget_4->setObjectName(QString::fromUtf8("horizontalLayoutWidget_4"));
        horizontalLayoutWidget_4->setGeometry(QRect(10, 0, 291, 31));
        horizontalLayout_4 = new QHBoxLayout(horizontalLayoutWidget_4);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(horizontalLayoutWidget_4);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_4->addWidget(label_2);

        comboBox_level = new QComboBox(horizontalLayoutWidget_4);
        comboBox_level->setObjectName(QString::fromUtf8("comboBox_level"));

        horizontalLayout_4->addWidget(comboBox_level);

        pushButton_addLevel = new QPushButton(horizontalLayoutWidget_4);
        pushButton_addLevel->setObjectName(QString::fromUtf8("pushButton_addLevel"));

        horizontalLayout_4->addWidget(pushButton_addLevel);

        pushButton_deleteLevel = new QPushButton(horizontalLayoutWidget_4);
        pushButton_deleteLevel->setObjectName(QString::fromUtf8("pushButton_deleteLevel"));

        horizontalLayout_4->addWidget(pushButton_deleteLevel);

        horizontalLayoutWidget_5 = new QWidget(tab);
        horizontalLayoutWidget_5->setObjectName(QString::fromUtf8("horizontalLayoutWidget_5"));
        horizontalLayoutWidget_5->setGeometry(QRect(10, 130, 291, 31));
        horizontalLayout_5 = new QHBoxLayout(horizontalLayoutWidget_5);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(horizontalLayoutWidget_5);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(150, 25));
        label_3->setMaximumSize(QSize(150, 25));

        horizontalLayout_5->addWidget(label_3);

        lineEdit_damping = new QLineEdit(horizontalLayoutWidget_5);
        lineEdit_damping->setObjectName(QString::fromUtf8("lineEdit_damping"));

        horizontalLayout_5->addWidget(lineEdit_damping);

        pushButton_setDamping = new QPushButton(horizontalLayoutWidget_5);
        pushButton_setDamping->setObjectName(QString::fromUtf8("pushButton_setDamping"));
        pushButton_setDamping->setMinimumSize(QSize(70, 25));
        pushButton_setDamping->setMaximumSize(QSize(70, 25));
        pushButton_setDamping->setIcon(icon8);

        horizontalLayout_5->addWidget(pushButton_setDamping);

        horizontalLayoutWidget_6 = new QWidget(tab);
        horizontalLayoutWidget_6->setObjectName(QString::fromUtf8("horizontalLayoutWidget_6"));
        horizontalLayoutWidget_6->setGeometry(QRect(10, 170, 291, 31));
        horizontalLayout_6 = new QHBoxLayout(horizontalLayoutWidget_6);
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(horizontalLayoutWidget_6);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMinimumSize(QSize(150, 25));
        label_4->setMaximumSize(QSize(150, 25));

        horizontalLayout_6->addWidget(label_4);

        spinBox_TimesShapeMatching = new QSpinBox(horizontalLayoutWidget_6);
        spinBox_TimesShapeMatching->setObjectName(QString::fromUtf8("spinBox_TimesShapeMatching"));
        spinBox_TimesShapeMatching->setMinimumSize(QSize(0, 25));
        spinBox_TimesShapeMatching->setMaximumSize(QSize(16777215, 25));
        spinBox_TimesShapeMatching->setMinimum(1);
        spinBox_TimesShapeMatching->setMaximum(300);
        spinBox_TimesShapeMatching->setValue(1);

        horizontalLayout_6->addWidget(spinBox_TimesShapeMatching);

        pushButton_setTimesSM = new QPushButton(horizontalLayoutWidget_6);
        pushButton_setTimesSM->setObjectName(QString::fromUtf8("pushButton_setTimesSM"));
        pushButton_setTimesSM->setMinimumSize(QSize(70, 25));
        pushButton_setTimesSM->setMaximumSize(QSize(70, 25));
        pushButton_setTimesSM->setIcon(icon8);

        horizontalLayout_6->addWidget(pushButton_setTimesSM);

        horizontalLayoutWidget_7 = new QWidget(tab);
        horizontalLayoutWidget_7->setObjectName(QString::fromUtf8("horizontalLayoutWidget_7"));
        horizontalLayoutWidget_7->setGeometry(QRect(10, 210, 291, 31));
        horizontalLayout_7 = new QHBoxLayout(horizontalLayoutWidget_7);
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(0, 0, 0, 0);
        checkBox_multigrid = new QCheckBox(horizontalLayoutWidget_7);
        checkBox_multigrid->setObjectName(QString::fromUtf8("checkBox_multigrid"));
        checkBox_multigrid->setChecked(true);

        horizontalLayout_7->addWidget(checkBox_multigrid);

        checkBox_dynamics = new QCheckBox(horizontalLayoutWidget_7);
        checkBox_dynamics->setObjectName(QString::fromUtf8("checkBox_dynamics"));
        checkBox_dynamics->setChecked(true);

        horizontalLayout_7->addWidget(checkBox_dynamics);

        horizontalLayoutWidget_10 = new QWidget(tab);
        horizontalLayoutWidget_10->setObjectName(QString::fromUtf8("horizontalLayoutWidget_10"));
        horizontalLayoutWidget_10->setGeometry(QRect(10, 250, 291, 31));
        horizontalLayout_10 = new QHBoxLayout(horizontalLayoutWidget_10);
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        horizontalLayout_10->setContentsMargins(0, 0, 0, 0);
        checkBox_gravity = new QCheckBox(horizontalLayoutWidget_10);
        checkBox_gravity->setObjectName(QString::fromUtf8("checkBox_gravity"));
        checkBox_gravity->setChecked(false);

        horizontalLayout_10->addWidget(checkBox_gravity);

        label_18 = new QLabel(horizontalLayoutWidget_10);
        label_18->setObjectName(QString::fromUtf8("label_18"));

        horizontalLayout_10->addWidget(label_18);

        lineEdit_gravityMagnitude = new QLineEdit(horizontalLayoutWidget_10);
        lineEdit_gravityMagnitude->setObjectName(QString::fromUtf8("lineEdit_gravityMagnitude"));

        horizontalLayout_10->addWidget(lineEdit_gravityMagnitude);

        pushButton_setGravityMagnitude = new QPushButton(horizontalLayoutWidget_10);
        pushButton_setGravityMagnitude->setObjectName(QString::fromUtf8("pushButton_setGravityMagnitude"));

        horizontalLayout_10->addWidget(pushButton_setGravityMagnitude);

        horizontalLayoutWidget_11 = new QWidget(tab);
        horizontalLayoutWidget_11->setObjectName(QString::fromUtf8("horizontalLayoutWidget_11"));
        horizontalLayoutWidget_11->setGeometry(QRect(10, 330, 296, 31));
        horizontalLayout_11 = new QHBoxLayout(horizontalLayoutWidget_11);
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        horizontalLayout_11->setContentsMargins(0, 0, 0, 0);
        checkBox_exportObj = new QCheckBox(horizontalLayoutWidget_11);
        checkBox_exportObj->setObjectName(QString::fromUtf8("checkBox_exportObj"));
        checkBox_exportObj->setChecked(false);

        horizontalLayout_11->addWidget(checkBox_exportObj);

        checkBox_captureScreen = new QCheckBox(horizontalLayoutWidget_11);
        checkBox_captureScreen->setObjectName(QString::fromUtf8("checkBox_captureScreen"));
        checkBox_captureScreen->setChecked(false);

        horizontalLayout_11->addWidget(checkBox_captureScreen);

        checkBox_captureSubScreen = new QCheckBox(horizontalLayoutWidget_11);
        checkBox_captureSubScreen->setObjectName(QString::fromUtf8("checkBox_captureSubScreen"));
        checkBox_captureSubScreen->setChecked(false);

        horizontalLayout_11->addWidget(checkBox_captureSubScreen);

        horizontalLayoutWidget_9 = new QWidget(tab);
        horizontalLayoutWidget_9->setObjectName(QString::fromUtf8("horizontalLayoutWidget_9"));
        horizontalLayoutWidget_9->setGeometry(QRect(10, 290, 291, 31));
        horizontalLayout_9 = new QHBoxLayout(horizontalLayoutWidget_9);
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalLayout_9->setContentsMargins(0, 0, 0, 0);
        label_17 = new QLabel(horizontalLayoutWidget_9);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        horizontalLayout_9->addWidget(label_17);

        lineEdit_forceScalar = new QLineEdit(horizontalLayoutWidget_9);
        lineEdit_forceScalar->setObjectName(QString::fromUtf8("lineEdit_forceScalar"));

        horizontalLayout_9->addWidget(lineEdit_forceScalar);

        pushButton_setForceScalar = new QPushButton(horizontalLayoutWidget_9);
        pushButton_setForceScalar->setObjectName(QString::fromUtf8("pushButton_setForceScalar"));

        horizontalLayout_9->addWidget(pushButton_setForceScalar);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        horizontalLayoutWidget_8 = new QWidget(tab_2);
        horizontalLayoutWidget_8->setObjectName(QString::fromUtf8("horizontalLayoutWidget_8"));
        horizontalLayoutWidget_8->setGeometry(QRect(0, 70, 301, 31));
        horizontalLayout_8 = new QHBoxLayout(horizontalLayoutWidget_8);
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(0, 0, 0, 0);
        label_5 = new QLabel(horizontalLayoutWidget_8);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_8->addWidget(label_5);

        lineEdit_forceX = new QLineEdit(horizontalLayoutWidget_8);
        lineEdit_forceX->setObjectName(QString::fromUtf8("lineEdit_forceX"));

        horizontalLayout_8->addWidget(lineEdit_forceX);

        label_6 = new QLabel(horizontalLayoutWidget_8);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_8->addWidget(label_6);

        lineEdit_forceY = new QLineEdit(horizontalLayoutWidget_8);
        lineEdit_forceY->setObjectName(QString::fromUtf8("lineEdit_forceY"));

        horizontalLayout_8->addWidget(lineEdit_forceY);

        label_7 = new QLabel(horizontalLayoutWidget_8);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_8->addWidget(label_7);

        lineEdit_forceZ = new QLineEdit(horizontalLayoutWidget_8);
        lineEdit_forceZ->setObjectName(QString::fromUtf8("lineEdit_forceZ"));

        horizontalLayout_8->addWidget(lineEdit_forceZ);

        pushButton_setForce = new QPushButton(horizontalLayoutWidget_8);
        pushButton_setForce->setObjectName(QString::fromUtf8("pushButton_setForce"));

        horizontalLayout_8->addWidget(pushButton_setForce);

        horizontalLayoutWidget_15 = new QWidget(tab_2);
        horizontalLayoutWidget_15->setObjectName(QString::fromUtf8("horizontalLayoutWidget_15"));
        horizontalLayoutWidget_15->setGeometry(QRect(0, 0, 301, 31));
        horizontalLayout_16 = new QHBoxLayout(horizontalLayoutWidget_15);
        horizontalLayout_16->setSpacing(6);
        horizontalLayout_16->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_16->setObjectName(QString::fromUtf8("horizontalLayout_16"));
        horizontalLayout_16->setContentsMargins(0, 0, 0, 0);
        checkBox_setForce = new QCheckBox(horizontalLayoutWidget_15);
        checkBox_setForce->setObjectName(QString::fromUtf8("checkBox_setForce"));

        horizontalLayout_16->addWidget(checkBox_setForce);

        checkBox_exportTxt = new QCheckBox(horizontalLayoutWidget_15);
        checkBox_exportTxt->setObjectName(QString::fromUtf8("checkBox_exportTxt"));
        checkBox_exportTxt->setChecked(false);

        horizontalLayout_16->addWidget(checkBox_exportTxt);

        horizontalLayoutWidget_16 = new QWidget(tab_2);
        horizontalLayoutWidget_16->setObjectName(QString::fromUtf8("horizontalLayoutWidget_16"));
        horizontalLayoutWidget_16->setGeometry(QRect(0, 110, 301, 31));
        horizontalLayout_15 = new QHBoxLayout(horizontalLayoutWidget_16);
        horizontalLayout_15->setSpacing(6);
        horizontalLayout_15->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        horizontalLayout_15->setContentsMargins(0, 0, 0, 0);
        pushButton_loadCamConfig = new QPushButton(horizontalLayoutWidget_16);
        pushButton_loadCamConfig->setObjectName(QString::fromUtf8("pushButton_loadCamConfig"));

        horizontalLayout_15->addWidget(pushButton_loadCamConfig);

        pushButton_saveCameraConfig = new QPushButton(horizontalLayoutWidget_16);
        pushButton_saveCameraConfig->setObjectName(QString::fromUtf8("pushButton_saveCameraConfig"));

        horizontalLayout_15->addWidget(pushButton_saveCameraConfig);

        horizontalLayoutWidget_17 = new QWidget(tab_2);
        horizontalLayoutWidget_17->setObjectName(QString::fromUtf8("horizontalLayoutWidget_17"));
        horizontalLayoutWidget_17->setGeometry(QRect(0, 150, 301, 31));
        horizontalLayout_17 = new QHBoxLayout(horizontalLayoutWidget_17);
        horizontalLayout_17->setSpacing(6);
        horizontalLayout_17->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_17->setObjectName(QString::fromUtf8("horizontalLayout_17"));
        horizontalLayout_17->setContentsMargins(0, 0, 0, 0);
        pushButton_loadConstraint = new QPushButton(horizontalLayoutWidget_17);
        pushButton_loadConstraint->setObjectName(QString::fromUtf8("pushButton_loadConstraint"));

        horizontalLayout_17->addWidget(pushButton_loadConstraint);

        pushButton_saveConstraint = new QPushButton(horizontalLayoutWidget_17);
        pushButton_saveConstraint->setObjectName(QString::fromUtf8("pushButton_saveConstraint"));

        horizontalLayout_17->addWidget(pushButton_saveConstraint);

        horizontalLayoutWidget_19 = new QWidget(tab_2);
        horizontalLayoutWidget_19->setObjectName(QString::fromUtf8("horizontalLayoutWidget_19"));
        horizontalLayoutWidget_19->setGeometry(QRect(0, 30, 291, 31));
        horizontalLayout_19 = new QHBoxLayout(horizontalLayoutWidget_19);
        horizontalLayout_19->setSpacing(6);
        horizontalLayout_19->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_19->setObjectName(QString::fromUtf8("horizontalLayout_19"));
        horizontalLayout_19->setContentsMargins(0, 0, 0, 0);
        checkBox_setWindForce = new QCheckBox(horizontalLayoutWidget_19);
        checkBox_setWindForce->setObjectName(QString::fromUtf8("checkBox_setWindForce"));

        horizontalLayout_19->addWidget(checkBox_setWindForce);

        label_19 = new QLabel(horizontalLayoutWidget_19);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        horizontalLayout_19->addWidget(label_19);

        lineEdit_windMagnitude = new QLineEdit(horizontalLayoutWidget_19);
        lineEdit_windMagnitude->setObjectName(QString::fromUtf8("lineEdit_windMagnitude"));

        horizontalLayout_19->addWidget(lineEdit_windMagnitude);

        pushButton_setWindMagnitude = new QPushButton(horizontalLayoutWidget_19);
        pushButton_setWindMagnitude->setObjectName(QString::fromUtf8("pushButton_setWindMagnitude"));

        horizontalLayout_19->addWidget(pushButton_setWindMagnitude);

        horizontalLayoutWidget_20 = new QWidget(tab_2);
        horizontalLayoutWidget_20->setObjectName(QString::fromUtf8("horizontalLayoutWidget_20"));
        horizontalLayoutWidget_20->setGeometry(QRect(0, 190, 301, 31));
        horizontalLayout_20 = new QHBoxLayout(horizontalLayoutWidget_20);
        horizontalLayout_20->setSpacing(6);
        horizontalLayout_20->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_20->setObjectName(QString::fromUtf8("horizontalLayout_20"));
        horizontalLayout_20->setContentsMargins(0, 0, 0, 0);
        label_20 = new QLabel(horizontalLayoutWidget_20);
        label_20->setObjectName(QString::fromUtf8("label_20"));

        horizontalLayout_20->addWidget(label_20);

        lineEdit_mass = new QLineEdit(horizontalLayoutWidget_20);
        lineEdit_mass->setObjectName(QString::fromUtf8("lineEdit_mass"));

        horizontalLayout_20->addWidget(lineEdit_mass);

        pushButton_setMass = new QPushButton(horizontalLayoutWidget_20);
        pushButton_setMass->setObjectName(QString::fromUtf8("pushButton_setMass"));

        horizontalLayout_20->addWidget(pushButton_setMass);

        pushButton_resetMass = new QPushButton(horizontalLayoutWidget_20);
        pushButton_resetMass->setObjectName(QString::fromUtf8("pushButton_resetMass"));

        horizontalLayout_20->addWidget(pushButton_resetMass);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        horizontalLayoutWidget_12 = new QWidget(tab_3);
        horizontalLayoutWidget_12->setObjectName(QString::fromUtf8("horizontalLayoutWidget_12"));
        horizontalLayoutWidget_12->setGeometry(QRect(10, 10, 291, 31));
        horizontalLayout_12 = new QHBoxLayout(horizontalLayoutWidget_12);
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        horizontalLayout_12->setContentsMargins(0, 0, 0, 0);
        checkBox_Network = new QCheckBox(horizontalLayoutWidget_12);
        checkBox_Network->setObjectName(QString::fromUtf8("checkBox_Network"));

        horizontalLayout_12->addWidget(checkBox_Network);

        radioButton_Server = new QRadioButton(horizontalLayoutWidget_12);
        radioButton_Server->setObjectName(QString::fromUtf8("radioButton_Server"));

        horizontalLayout_12->addWidget(radioButton_Server);

        radioButton_Client = new QRadioButton(horizontalLayoutWidget_12);
        radioButton_Client->setObjectName(QString::fromUtf8("radioButton_Client"));

        horizontalLayout_12->addWidget(radioButton_Client);

        horizontalLayoutWidget_13 = new QWidget(tab_3);
        horizontalLayoutWidget_13->setObjectName(QString::fromUtf8("horizontalLayoutWidget_13"));
        horizontalLayoutWidget_13->setGeometry(QRect(10, 90, 291, 31));
        horizontalLayout_13 = new QHBoxLayout(horizontalLayoutWidget_13);
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        horizontalLayout_13->setContentsMargins(0, 0, 0, 0);
        spinBox_NumClients = new QSpinBox(horizontalLayoutWidget_13);
        spinBox_NumClients->setObjectName(QString::fromUtf8("spinBox_NumClients"));
        spinBox_NumClients->setMinimum(1);
        spinBox_NumClients->setValue(1);

        horizontalLayout_13->addWidget(spinBox_NumClients);

        pushButton_setNumOfClients = new QPushButton(horizontalLayoutWidget_13);
        pushButton_setNumOfClients->setObjectName(QString::fromUtf8("pushButton_setNumOfClients"));

        horizontalLayout_13->addWidget(pushButton_setNumOfClients);

        pushButton_configNetwork = new QPushButton(horizontalLayoutWidget_13);
        pushButton_configNetwork->setObjectName(QString::fromUtf8("pushButton_configNetwork"));

        horizontalLayout_13->addWidget(pushButton_configNetwork);

        listWidget_Status = new QListWidget(tab_3);
        listWidget_Status->setObjectName(QString::fromUtf8("listWidget_Status"));
        listWidget_Status->setGeometry(QRect(10, 210, 291, 141));
        horizontalLayoutWidget_14 = new QWidget(tab_3);
        horizontalLayoutWidget_14->setObjectName(QString::fromUtf8("horizontalLayoutWidget_14"));
        horizontalLayoutWidget_14->setGeometry(QRect(10, 50, 291, 31));
        horizontalLayout_14 = new QHBoxLayout(horizontalLayoutWidget_14);
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        horizontalLayout_14->setContentsMargins(0, 0, 0, 0);
        label_9 = new QLabel(horizontalLayoutWidget_14);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        horizontalLayout_14->addWidget(label_9);

        lineEdit_IP = new QLineEdit(horizontalLayoutWidget_14);
        lineEdit_IP->setObjectName(QString::fromUtf8("lineEdit_IP"));

        horizontalLayout_14->addWidget(lineEdit_IP);

        label_10 = new QLabel(horizontalLayoutWidget_14);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        horizontalLayout_14->addWidget(label_10);

        lineEdit_Port = new QLineEdit(horizontalLayoutWidget_14);
        lineEdit_Port->setObjectName(QString::fromUtf8("lineEdit_Port"));
        lineEdit_Port->setMaximumSize(QSize(50, 16777215));
        lineEdit_Port->setBaseSize(QSize(0, 0));

        horizontalLayout_14->addWidget(lineEdit_Port);

        horizontalLayoutWidget_18 = new QWidget(tab_3);
        horizontalLayoutWidget_18->setObjectName(QString::fromUtf8("horizontalLayoutWidget_18"));
        horizontalLayoutWidget_18->setGeometry(QRect(10, 130, 291, 31));
        horizontalLayout_18 = new QHBoxLayout(horizontalLayoutWidget_18);
        horizontalLayout_18->setSpacing(6);
        horizontalLayout_18->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_18->setObjectName(QString::fromUtf8("horizontalLayout_18"));
        horizontalLayout_18->setContentsMargins(0, 0, 0, 0);
        pushButton_dataFirst = new QPushButton(horizontalLayoutWidget_18);
        pushButton_dataFirst->setObjectName(QString::fromUtf8("pushButton_dataFirst"));

        horizontalLayout_18->addWidget(pushButton_dataFirst);

        horizontalLayoutWidget_21 = new QWidget(tab_3);
        horizontalLayoutWidget_21->setObjectName(QString::fromUtf8("horizontalLayoutWidget_21"));
        horizontalLayoutWidget_21->setGeometry(QRect(10, 170, 291, 31));
        horizontalLayout_21 = new QHBoxLayout(horizontalLayoutWidget_21);
        horizontalLayout_21->setSpacing(6);
        horizontalLayout_21->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_21->setObjectName(QString::fromUtf8("horizontalLayout_21"));
        horizontalLayout_21->setContentsMargins(0, 0, 0, 0);
        checkBox_isCompressed = new QCheckBox(horizontalLayoutWidget_21);
        checkBox_isCompressed->setObjectName(QString::fromUtf8("checkBox_isCompressed"));

        horizontalLayout_21->addWidget(checkBox_isCompressed);

        tabWidget->addTab(tab_3, QString());
        test = new QWidget();
        test->setObjectName(QString::fromUtf8("test"));
        groupBox_2 = new QGroupBox(test);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(0, 0, 301, 221));
        spinBox_gridDensity_2 = new QSpinBox(groupBox_2);
        spinBox_gridDensity_2->setObjectName(QString::fromUtf8("spinBox_gridDensity_2"));
        spinBox_gridDensity_2->setGeometry(QRect(20, 20, 47, 25));
        spinBox_gridDensity_2->setMinimumSize(QSize(0, 25));
        spinBox_gridDensity_2->setMaximumSize(QSize(16777215, 25));
        spinBox_gridDensity_2->setMinimum(1);
        spinBox_gridDensity_2->setMaximum(256);
        spinBox_gridDensity_2->setValue(1);
        spinBox_gridDensity_3 = new QSpinBox(groupBox_2);
        spinBox_gridDensity_3->setObjectName(QString::fromUtf8("spinBox_gridDensity_3"));
        spinBox_gridDensity_3->setGeometry(QRect(80, 20, 47, 25));
        spinBox_gridDensity_3->setMinimumSize(QSize(0, 25));
        spinBox_gridDensity_3->setMaximumSize(QSize(16777215, 25));
        spinBox_gridDensity_3->setMinimum(1);
        spinBox_gridDensity_3->setMaximum(1000);
        spinBox_gridDensity_3->setValue(128);
        pushButton_3 = new QPushButton(groupBox_2);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(140, 20, 75, 23));
        pushButton_neighbor = new QPushButton(groupBox_2);
        pushButton_neighbor->setObjectName(QString::fromUtf8("pushButton_neighbor"));
        pushButton_neighbor->setGeometry(QRect(220, 20, 75, 23));
        pushButton_parent_neighbor = new QPushButton(groupBox_2);
        pushButton_parent_neighbor->setObjectName(QString::fromUtf8("pushButton_parent_neighbor"));
        pushButton_parent_neighbor->setGeometry(QRect(140, 50, 111, 23));
        spinBox_gridDensity_vertex = new QSpinBox(groupBox_2);
        spinBox_gridDensity_vertex->setObjectName(QString::fromUtf8("spinBox_gridDensity_vertex"));
        spinBox_gridDensity_vertex->setGeometry(QRect(20, 140, 47, 25));
        spinBox_gridDensity_vertex->setMinimumSize(QSize(0, 25));
        spinBox_gridDensity_vertex->setMaximumSize(QSize(16777215, 25));
        spinBox_gridDensity_vertex->setMinimum(0);
        spinBox_gridDensity_vertex->setMaximum(10000);
        spinBox_gridDensity_vertex->setValue(1);
        pushButton_vox4vertex = new QPushButton(groupBox_2);
        pushButton_vox4vertex->setObjectName(QString::fromUtf8("pushButton_vox4vertex"));
        pushButton_vox4vertex->setGeometry(QRect(140, 80, 111, 23));
        spinBox_voxX = new QSpinBox(groupBox_2);
        spinBox_voxX->setObjectName(QString::fromUtf8("spinBox_voxX"));
        spinBox_voxX->setGeometry(QRect(20, 110, 47, 25));
        spinBox_voxX->setMinimumSize(QSize(0, 25));
        spinBox_voxX->setMaximumSize(QSize(16777215, 25));
        spinBox_voxX->setMinimum(0);
        spinBox_voxX->setMaximum(10000);
        spinBox_voxX->setValue(0);
        spinBox_voxY = new QSpinBox(groupBox_2);
        spinBox_voxY->setObjectName(QString::fromUtf8("spinBox_voxY"));
        spinBox_voxY->setGeometry(QRect(70, 110, 47, 25));
        spinBox_voxY->setMinimumSize(QSize(0, 25));
        spinBox_voxY->setMaximumSize(QSize(16777215, 25));
        spinBox_voxY->setMinimum(0);
        spinBox_voxY->setMaximum(10000);
        spinBox_voxY->setValue(0);
        spinBox_voxZ = new QSpinBox(groupBox_2);
        spinBox_voxZ->setObjectName(QString::fromUtf8("spinBox_voxZ"));
        spinBox_voxZ->setGeometry(QRect(120, 110, 47, 25));
        spinBox_voxZ->setMinimumSize(QSize(0, 25));
        spinBox_voxZ->setMaximumSize(QSize(16777215, 25));
        spinBox_voxZ->setMinimum(0);
        spinBox_voxZ->setMaximum(10000);
        spinBox_voxZ->setValue(0);
        pushButton_vox4idx = new QPushButton(groupBox_2);
        pushButton_vox4idx->setObjectName(QString::fromUtf8("pushButton_vox4idx"));
        pushButton_vox4idx->setGeometry(QRect(180, 110, 111, 23));
        pushButton_showLowerMesh = new QPushButton(groupBox_2);
        pushButton_showLowerMesh->setObjectName(QString::fromUtf8("pushButton_showLowerMesh"));
        pushButton_showLowerMesh->setGeometry(QRect(100, 150, 111, 23));
        horizontalLayoutWidget_22 = new QWidget(groupBox_2);
        horizontalLayoutWidget_22->setObjectName(QString::fromUtf8("horizontalLayoutWidget_22"));
        horizontalLayoutWidget_22->setGeometry(QRect(0, 180, 291, 31));
        horizontalLayout_22 = new QHBoxLayout(horizontalLayoutWidget_22);
        horizontalLayout_22->setSpacing(6);
        horizontalLayout_22->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_22->setObjectName(QString::fromUtf8("horizontalLayout_22"));
        horizontalLayout_22->setContentsMargins(0, 0, 0, 0);
        pushButton_testSleep = new QPushButton(horizontalLayoutWidget_22);
        pushButton_testSleep->setObjectName(QString::fromUtf8("pushButton_testSleep"));

        horizontalLayout_22->addWidget(pushButton_testSleep);

        lineEdit_sleep = new QLineEdit(horizontalLayoutWidget_22);
        lineEdit_sleep->setObjectName(QString::fromUtf8("lineEdit_sleep"));

        horizontalLayout_22->addWidget(lineEdit_sleep);

        spinBox_vox_vertex = new QSpinBox(groupBox_2);
        spinBox_vox_vertex->setObjectName(QString::fromUtf8("spinBox_vox_vertex"));
        spinBox_vox_vertex->setGeometry(QRect(20, 80, 47, 25));
        spinBox_vox_vertex->setMinimumSize(QSize(0, 25));
        spinBox_vox_vertex->setMaximumSize(QSize(16777215, 25));
        spinBox_vox_vertex->setMinimum(0);
        spinBox_vox_vertex->setMaximum(100000);
        spinBox_vox_vertex->setValue(1);
        pushButton_testFunction = new QPushButton(test);
        pushButton_testFunction->setObjectName(QString::fromUtf8("pushButton_testFunction"));
        pushButton_testFunction->setGeometry(QRect(0, 230, 75, 23));
        pushButton_testVoxConnection = new QPushButton(test);
        pushButton_testVoxConnection->setObjectName(QString::fromUtf8("pushButton_testVoxConnection"));
        pushButton_testVoxConnection->setGeometry(QRect(80, 230, 111, 23));
        tabWidget->addTab(test, QString());
        horizontalLayoutWidget_2 = new QWidget(centralWidget);
        horizontalLayoutWidget_2->setObjectName(QString::fromUtf8("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(1060, 710, 301, 51));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        pushButton_initSimulator = new QPushButton(horizontalLayoutWidget_2);
        pushButton_initSimulator->setObjectName(QString::fromUtf8("pushButton_initSimulator"));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/joint_deformation/Resources/gnome_run.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_initSimulator->setIcon(icon14);
        pushButton_initSimulator->setIconSize(QSize(24, 24));

        horizontalLayout_2->addWidget(pushButton_initSimulator);

        pushButton_startSimulation = new QPushButton(horizontalLayoutWidget_2);
        pushButton_startSimulation->setObjectName(QString::fromUtf8("pushButton_startSimulation"));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/joint_deformation/Resources/gtk_media_play_ltr.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_startSimulation->setIcon(icon15);
        pushButton_startSimulation->setIconSize(QSize(24, 24));

        horizontalLayout_2->addWidget(pushButton_startSimulation);

        pushButton_nextStep = new QPushButton(horizontalLayoutWidget_2);
        pushButton_nextStep->setObjectName(QString::fromUtf8("pushButton_nextStep"));
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/joint_deformation/Resources/gtk_media_next_ltr.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_nextStep->setIcon(icon16);
        pushButton_nextStep->setIconSize(QSize(24, 24));

        horizontalLayout_2->addWidget(pushButton_nextStep);

        pushButton_pauseSimulation = new QPushButton(horizontalLayoutWidget_2);
        pushButton_pauseSimulation->setObjectName(QString::fromUtf8("pushButton_pauseSimulation"));
        QIcon icon17;
        icon17.addFile(QString::fromUtf8(":/joint_deformation/Resources/gtk_media_pause.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_pauseSimulation->setIcon(icon17);
        pushButton_pauseSimulation->setIconSize(QSize(24, 24));

        horizontalLayout_2->addWidget(pushButton_pauseSimulation);

        pushButton_resetSimulation = new QPushButton(horizontalLayoutWidget_2);
        pushButton_resetSimulation->setObjectName(QString::fromUtf8("pushButton_resetSimulation"));
        pushButton_resetSimulation->setIcon(icon12);
        pushButton_resetSimulation->setIconSize(QSize(24, 24));

        horizontalLayout_2->addWidget(pushButton_resetSimulation);

        comboBox_chooseSimulator = new QComboBox(centralWidget);
        comboBox_chooseSimulator->setObjectName(QString::fromUtf8("comboBox_chooseSimulator"));
        comboBox_chooseSimulator->setGeometry(QRect(1050, 20, 331, 22));
        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(1050, 430, 341, 61));
        radioButton_setForceConstraint = new QRadioButton(groupBox_3);
        radioButton_setForceConstraint->setObjectName(QString::fromUtf8("radioButton_setForceConstraint"));
        radioButton_setForceConstraint->setGeometry(QRect(10, 30, 61, 17));
        radioButton_setForceConstraint->setChecked(true);
        radioButton_setPositionConstraint = new QRadioButton(groupBox_3);
        radioButton_setPositionConstraint->setObjectName(QString::fromUtf8("radioButton_setPositionConstraint"));
        radioButton_setPositionConstraint->setGeometry(QRect(80, 30, 111, 17));
        radioButton_setOrientationConstraint = new QRadioButton(groupBox_3);
        radioButton_setOrientationConstraint->setObjectName(QString::fromUtf8("radioButton_setOrientationConstraint"));
        radioButton_setOrientationConstraint->setGeometry(QRect(200, 30, 131, 17));
        joint_deformationClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(joint_deformationClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1400, 21));
        joint_deformationClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(joint_deformationClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        mainToolBar->setMovable(false);
        joint_deformationClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(joint_deformationClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        joint_deformationClass->setStatusBar(statusBar);

        mainToolBar->addAction(actionLoadMesh);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionScaleMesh);
        mainToolBar->addAction(actionShowMesh);
        mainToolBar->addAction(actionShowGrid);
        mainToolBar->addAction(actionShowVox);
        mainToolBar->addAction(actionShowGoalPosition);
        mainToolBar->addAction(actionSetClusterTree);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionShowSelection);
        mainToolBar->addAction(actionSetConstraint);
        mainToolBar->addAction(actionSetCubeStaticConstraint);
        mainToolBar->addAction(actionSetCubeActiveConstraint);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionCubeOperation);
        mainToolBar->addAction(actionSetMass);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionShowTest);

        retranslateUi(joint_deformationClass);
        QObject::connect(actionLoadMesh, SIGNAL(triggered()), joint_deformationClass, SLOT(loadMesh()));
        QObject::connect(actionScaleMesh, SIGNAL(triggered()), joint_deformationClass, SLOT(scaleMesh()));
        QObject::connect(actionShowGrid, SIGNAL(triggered()), joint_deformationClass, SLOT(showGrid()));
        QObject::connect(pushButton_setGridDensity, SIGNAL(clicked()), joint_deformationClass, SLOT(setGridDensity()));
        QObject::connect(actionShowMesh, SIGNAL(triggered(bool)), joint_deformationClass, SLOT(showMesh(bool)));
        QObject::connect(actionShowVox, SIGNAL(triggered(bool)), joint_deformationClass, SLOT(showVox(bool)));
        QObject::connect(actionShowSelection, SIGNAL(triggered(bool)), joint_deformationClass, SLOT(showSelection(bool)));
        QObject::connect(pushButton_startSimulation, SIGNAL(clicked()), joint_deformationClass, SLOT(startSimulation()));
        QObject::connect(pushButton_initSimulator, SIGNAL(clicked()), joint_deformationClass, SLOT(initializeSimulator()));
        QObject::connect(pushButton_pauseSimulation, SIGNAL(clicked()), joint_deformationClass, SLOT(pauseSimulation()));
        QObject::connect(actionSetClusterTree, SIGNAL(triggered(bool)), joint_deformationClass, SLOT(setRenderMode(bool)));
        QObject::connect(renderWidget, SIGNAL(appendCluster()), joint_deformationClass, SLOT(updateTreeDepth()));
        QObject::connect(pushButton_lastSameLevel, SIGNAL(clicked()), joint_deformationClass, SLOT(lastSameLevel()));
        QObject::connect(pushButton_nextSameLevel, SIGNAL(clicked()), joint_deformationClass, SLOT(nextSameLevel()));
        QObject::connect(comboBox_selectTreeDepth, SIGNAL(currentIndexChanged(int)), joint_deformationClass, SLOT(updateCurrentParentCluster(int)));
        QObject::connect(comboBox_chooseSimulator, SIGNAL(currentIndexChanged(int)), joint_deformationClass, SLOT(chooseSimulator(int)));
        QObject::connect(actionShowGoalPosition, SIGNAL(toggled(bool)), joint_deformationClass, SLOT(showGoalPosition(bool)));
        QObject::connect(pushButton_addLevel, SIGNAL(clicked()), joint_deformationClass, SLOT(addLevel()));
        QObject::connect(comboBox_level, SIGNAL(currentIndexChanged(int)), joint_deformationClass, SLOT(chooseLevelDisplay(int)));
        QObject::connect(pushButton_3, SIGNAL(clicked()), joint_deformationClass, SLOT(setShowNum()));
        QObject::connect(pushButton_neighbor, SIGNAL(clicked()), joint_deformationClass, SLOT(testNeighbor()));
        QObject::connect(pushButton_parent_neighbor, SIGNAL(clicked()), joint_deformationClass, SLOT(testParentNeighbor()));
        QObject::connect(actionShowTest, SIGNAL(triggered(bool)), joint_deformationClass, SLOT(showTest(bool)));
        QObject::connect(actionSetConstraint, SIGNAL(triggered(bool)), joint_deformationClass, SLOT(showConstraint(bool)));
        QObject::connect(radioButton_setPositionConstraint, SIGNAL(clicked(bool)), joint_deformationClass, SLOT(setPositionConstraint(bool)));
        QObject::connect(radioButton_setOrientationConstraint, SIGNAL(clicked(bool)), joint_deformationClass, SLOT(setOrientationConstraint(bool)));
        QObject::connect(pushButton_vox4vertex, SIGNAL(clicked()), joint_deformationClass, SLOT(testVox4Vertex()));
        QObject::connect(pushButton_setDamping, SIGNAL(clicked()), joint_deformationClass, SLOT(setDampParameter()));
        QObject::connect(checkBox_multigrid, SIGNAL(clicked(bool)), joint_deformationClass, SLOT(setMultigrid(bool)));
        QObject::connect(pushButton_setTimesSM, SIGNAL(clicked()), joint_deformationClass, SLOT(setTimesShapeMatching()));
        QObject::connect(pushButton_setForce, SIGNAL(clicked()), joint_deformationClass, SLOT(setForce()));
        QObject::connect(checkBox_setForce, SIGNAL(clicked(bool)), joint_deformationClass, SLOT(enableSetForce(bool)));
        QObject::connect(pushButton_vox4idx, SIGNAL(clicked()), joint_deformationClass, SLOT(testVox4Idx()));
        QObject::connect(checkBox_dynamics, SIGNAL(clicked(bool)), joint_deformationClass, SLOT(setDynamics(bool)));
        QObject::connect(checkBox_gravity, SIGNAL(clicked(bool)), joint_deformationClass, SLOT(setGravity(bool)));
        QObject::connect(pushButton_setGravityMagnitude, SIGNAL(clicked()), joint_deformationClass, SLOT(setGravityMagnitude()));
        QObject::connect(checkBox_exportObj, SIGNAL(clicked(bool)), joint_deformationClass, SLOT(setExportObj(bool)));
        QObject::connect(radioButton_setForceConstraint, SIGNAL(clicked(bool)), joint_deformationClass, SLOT(setForceConstraint(bool)));
        QObject::connect(checkBox_Network, SIGNAL(clicked(bool)), joint_deformationClass, SLOT(setNetwork(bool)));
        QObject::connect(radioButton_Server, SIGNAL(clicked(bool)), joint_deformationClass, SLOT(setServer(bool)));
        QObject::connect(radioButton_Client, SIGNAL(clicked(bool)), joint_deformationClass, SLOT(setClient(bool)));
        QObject::connect(pushButton_setNumOfClients, SIGNAL(clicked()), joint_deformationClass, SLOT(setNumOfClients()));
        QObject::connect(pushButton_configNetwork, SIGNAL(clicked()), joint_deformationClass, SLOT(configNetwork()));
        QObject::connect(checkBox_captureScreen, SIGNAL(clicked(bool)), joint_deformationClass, SLOT(setCaptureScreen(bool)));
        QObject::connect(checkBox_captureSubScreen, SIGNAL(clicked(bool)), joint_deformationClass, SLOT(setCaptureSubScreen(bool)));
        QObject::connect(checkBox_setWindForce, SIGNAL(clicked(bool)), joint_deformationClass, SLOT(enableSetWindForce(bool)));
        QObject::connect(pushButton_loadCamConfig, SIGNAL(clicked()), joint_deformationClass, SLOT(loadCamConfig()));
        QObject::connect(pushButton_saveCameraConfig, SIGNAL(clicked()), joint_deformationClass, SLOT(saveCamConfig()));
        QObject::connect(pushButton_dataFirst, SIGNAL(clicked()), joint_deformationClass, SLOT(sendInitData()));
        QObject::connect(pushButton_setWindMagnitude, SIGNAL(clicked()), joint_deformationClass, SLOT(setWindMagnitude()));
        QObject::connect(checkBox_exportTxt, SIGNAL(clicked(bool)), joint_deformationClass, SLOT(setExportTxt(bool)));
        QObject::connect(checkBox_isCompressed, SIGNAL(clicked(bool)), joint_deformationClass, SLOT(setCompressedData(bool)));
        QObject::connect(pushButton_testSleep, SIGNAL(clicked()), joint_deformationClass, SLOT(testSleepTime()));
        QObject::connect(pushButton_testFunction, SIGNAL(clicked()), joint_deformationClass, SLOT(testFunction()));
        QObject::connect(pushButton_deleteLevel, SIGNAL(clicked()), joint_deformationClass, SLOT(clearLevel()));
        QObject::connect(actionSetMass, SIGNAL(triggered(bool)), joint_deformationClass, SLOT(setEnableMass(bool)));
        QObject::connect(pushButton_setMass, SIGNAL(clicked()), joint_deformationClass, SLOT(setMass()));
        QObject::connect(pushButton_resetMass, SIGNAL(clicked()), joint_deformationClass, SLOT(resetMass()));
        QObject::connect(pushButton_testVoxConnection, SIGNAL(clicked()), joint_deformationClass, SLOT(testVoxConnection()));
        QObject::connect(actionCubeOperation, SIGNAL(triggered(bool)), joint_deformationClass, SLOT(setCubeOperation(bool)));
        QObject::connect(actionSetCubeStaticConstraint, SIGNAL(triggered(bool)), joint_deformationClass, SLOT(setCubeStaticConstraint(bool)));
        QObject::connect(actionSetCubeActiveConstraint, SIGNAL(triggered(bool)), joint_deformationClass, SLOT(setCubeActiveConstraint(bool)));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(joint_deformationClass);
    } // setupUi

    void retranslateUi(QMainWindow *joint_deformationClass)
    {
        joint_deformationClass->setWindowTitle(QApplication::translate("joint_deformationClass", "joint_deformation", 0, QApplication::UnicodeUTF8));
        actionLoadMesh->setText(QApplication::translate("joint_deformationClass", "loadMesh", 0, QApplication::UnicodeUTF8));
        actionLoadMesh->setShortcut(QApplication::translate("joint_deformationClass", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionScaleMesh->setText(QApplication::translate("joint_deformationClass", "scaleMesh", 0, QApplication::UnicodeUTF8));
        actionScaleMesh->setShortcut(QApplication::translate("joint_deformationClass", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        actionShowGrid->setText(QApplication::translate("joint_deformationClass", "showGrid", 0, QApplication::UnicodeUTF8));
        actionShowGrid->setShortcut(QApplication::translate("joint_deformationClass", "Ctrl+2", 0, QApplication::UnicodeUTF8));
        actionShowMesh->setText(QApplication::translate("joint_deformationClass", "showMesh", 0, QApplication::UnicodeUTF8));
        actionShowMesh->setShortcut(QApplication::translate("joint_deformationClass", "Ctrl+1", 0, QApplication::UnicodeUTF8));
        actionShowVox->setText(QApplication::translate("joint_deformationClass", "showVox", 0, QApplication::UnicodeUTF8));
        actionShowVox->setShortcut(QApplication::translate("joint_deformationClass", "Ctrl+3", 0, QApplication::UnicodeUTF8));
        actionShowSelection->setText(QApplication::translate("joint_deformationClass", "showSelection", 0, QApplication::UnicodeUTF8));
        actionShowSelection->setShortcut(QApplication::translate("joint_deformationClass", "Ctrl+4", 0, QApplication::UnicodeUTF8));
        actionSetClusterTree->setText(QApplication::translate("joint_deformationClass", "setClusterTree", 0, QApplication::UnicodeUTF8));
        actionShowGoalPosition->setText(QApplication::translate("joint_deformationClass", "showGoalPosition", 0, QApplication::UnicodeUTF8));
        actionShowGoalPosition->setShortcut(QApplication::translate("joint_deformationClass", "Ctrl+5", 0, QApplication::UnicodeUTF8));
        actionShowTest->setText(QApplication::translate("joint_deformationClass", "showTest", 0, QApplication::UnicodeUTF8));
        actionSetConstraint->setText(QApplication::translate("joint_deformationClass", "setConstraint", 0, QApplication::UnicodeUTF8));
        actionSetMass->setText(QApplication::translate("joint_deformationClass", "setMass", 0, QApplication::UnicodeUTF8));
        actionCubeOperation->setText(QApplication::translate("joint_deformationClass", "cubeOperation", 0, QApplication::UnicodeUTF8));
        actionSetCubeStaticConstraint->setText(QApplication::translate("joint_deformationClass", "setCubeStaticConstraint", 0, QApplication::UnicodeUTF8));
        actionSetCubeActiveConstraint->setText(QApplication::translate("joint_deformationClass", "setCubeActiveConstraint", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("joint_deformationClass", "Grid density", 0, QApplication::UnicodeUTF8));
        pushButton_setGridDensity->setText(QApplication::translate("joint_deformationClass", "Apply", 0, QApplication::UnicodeUTF8));
        comboBox_selectTreeDepth->clear();
        comboBox_selectTreeDepth->insertItems(0, QStringList()
         << QApplication::translate("joint_deformationClass", "Root", 0, QApplication::UnicodeUTF8)
        );
        pushButton_lastSameLevel->setText(QApplication::translate("joint_deformationClass", "<<", 0, QApplication::UnicodeUTF8));
        pushButton_nextSameLevel->setText(QApplication::translate("joint_deformationClass", ">>", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("joint_deformationClass", "Append", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("joint_deformationClass", "Clear", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("joint_deformationClass", "Level", 0, QApplication::UnicodeUTF8));
        pushButton_addLevel->setText(QApplication::translate("joint_deformationClass", "Add", 0, QApplication::UnicodeUTF8));
        pushButton_deleteLevel->setText(QApplication::translate("joint_deformationClass", "Delete", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("joint_deformationClass", "Damping", 0, QApplication::UnicodeUTF8));
        lineEdit_damping->setText(QApplication::translate("joint_deformationClass", "0.01", 0, QApplication::UnicodeUTF8));
        pushButton_setDamping->setText(QApplication::translate("joint_deformationClass", "SetDamp", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("joint_deformationClass", "Times Of Shape Matching", 0, QApplication::UnicodeUTF8));
        pushButton_setTimesSM->setText(QApplication::translate("joint_deformationClass", "SetTimes", 0, QApplication::UnicodeUTF8));
        checkBox_multigrid->setText(QApplication::translate("joint_deformationClass", "Multigrid", 0, QApplication::UnicodeUTF8));
        checkBox_dynamics->setText(QApplication::translate("joint_deformationClass", "Dynamics", 0, QApplication::UnicodeUTF8));
        checkBox_gravity->setText(QApplication::translate("joint_deformationClass", "Gravity", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("joint_deformationClass", "Gravity Magnitude", 0, QApplication::UnicodeUTF8));
        lineEdit_gravityMagnitude->setText(QApplication::translate("joint_deformationClass", "0.01", 0, QApplication::UnicodeUTF8));
        pushButton_setGravityMagnitude->setText(QApplication::translate("joint_deformationClass", "SetMagnitude", 0, QApplication::UnicodeUTF8));
        checkBox_exportObj->setText(QApplication::translate("joint_deformationClass", "ExportOBJ", 0, QApplication::UnicodeUTF8));
        checkBox_captureScreen->setText(QApplication::translate("joint_deformationClass", "CaptureScreen", 0, QApplication::UnicodeUTF8));
        checkBox_captureSubScreen->setText(QApplication::translate("joint_deformationClass", "CaptureSubScreen", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("joint_deformationClass", "Force Scalar", 0, QApplication::UnicodeUTF8));
        lineEdit_forceScalar->setText(QApplication::translate("joint_deformationClass", "10.0", 0, QApplication::UnicodeUTF8));
        pushButton_setForceScalar->setText(QApplication::translate("joint_deformationClass", "Set", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("joint_deformationClass", "Geometry", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("joint_deformationClass", "X:", 0, QApplication::UnicodeUTF8));
        lineEdit_forceX->setText(QApplication::translate("joint_deformationClass", "0.15", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("joint_deformationClass", "Y:", 0, QApplication::UnicodeUTF8));
        lineEdit_forceY->setText(QApplication::translate("joint_deformationClass", "0.5", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("joint_deformationClass", "Z:", 0, QApplication::UnicodeUTF8));
        lineEdit_forceZ->setText(QApplication::translate("joint_deformationClass", "0", 0, QApplication::UnicodeUTF8));
        pushButton_setForce->setText(QApplication::translate("joint_deformationClass", "Set", 0, QApplication::UnicodeUTF8));
        checkBox_setForce->setText(QApplication::translate("joint_deformationClass", "Manual Force", 0, QApplication::UnicodeUTF8));
        checkBox_exportTxt->setText(QApplication::translate("joint_deformationClass", "ExportTXT", 0, QApplication::UnicodeUTF8));
        pushButton_loadCamConfig->setText(QApplication::translate("joint_deformationClass", "LoadCameraConfig", 0, QApplication::UnicodeUTF8));
        pushButton_saveCameraConfig->setText(QApplication::translate("joint_deformationClass", "SaveCameraConfig", 0, QApplication::UnicodeUTF8));
        pushButton_loadConstraint->setText(QApplication::translate("joint_deformationClass", "LoadConstraints", 0, QApplication::UnicodeUTF8));
        pushButton_saveConstraint->setText(QApplication::translate("joint_deformationClass", "SaveConstraints", 0, QApplication::UnicodeUTF8));
        checkBox_setWindForce->setText(QApplication::translate("joint_deformationClass", "Wind", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("joint_deformationClass", "Wind Magnitude", 0, QApplication::UnicodeUTF8));
        lineEdit_windMagnitude->setText(QApplication::translate("joint_deformationClass", "10.0", 0, QApplication::UnicodeUTF8));
        pushButton_setWindMagnitude->setText(QApplication::translate("joint_deformationClass", "SetMagnitude", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("joint_deformationClass", "Mass", 0, QApplication::UnicodeUTF8));
        lineEdit_mass->setText(QApplication::translate("joint_deformationClass", "1.0", 0, QApplication::UnicodeUTF8));
        pushButton_setMass->setText(QApplication::translate("joint_deformationClass", "SetMass", 0, QApplication::UnicodeUTF8));
        pushButton_resetMass->setText(QApplication::translate("joint_deformationClass", "ResetMass", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("joint_deformationClass", "Shape matching", 0, QApplication::UnicodeUTF8));
        checkBox_Network->setText(QApplication::translate("joint_deformationClass", "Network", 0, QApplication::UnicodeUTF8));
        radioButton_Server->setText(QApplication::translate("joint_deformationClass", "Server", 0, QApplication::UnicodeUTF8));
        radioButton_Client->setText(QApplication::translate("joint_deformationClass", "Client", 0, QApplication::UnicodeUTF8));
        pushButton_setNumOfClients->setText(QApplication::translate("joint_deformationClass", "SetNumOfClients", 0, QApplication::UnicodeUTF8));
        pushButton_configNetwork->setText(QApplication::translate("joint_deformationClass", "Config", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("joint_deformationClass", "TextLabel", 0, QApplication::UnicodeUTF8));
        lineEdit_IP->setText(QApplication::translate("joint_deformationClass", "cs72283", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("joint_deformationClass", "TextLabel", 0, QApplication::UnicodeUTF8));
        lineEdit_Port->setText(QApplication::translate("joint_deformationClass", "37000", 0, QApplication::UnicodeUTF8));
        pushButton_dataFirst->setText(QApplication::translate("joint_deformationClass", "Initialize Data", 0, QApplication::UnicodeUTF8));
        checkBox_isCompressed->setText(QApplication::translate("joint_deformationClass", "CompressData", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("joint_deformationClass", "Network", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("joint_deformationClass", "Test Parameters", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("joint_deformationClass", "TestShowVox", 0, QApplication::UnicodeUTF8));
        pushButton_neighbor->setText(QApplication::translate("joint_deformationClass", "TestNeighbor", 0, QApplication::UnicodeUTF8));
        pushButton_parent_neighbor->setText(QApplication::translate("joint_deformationClass", "TestParentNeighbor", 0, QApplication::UnicodeUTF8));
        pushButton_vox4vertex->setText(QApplication::translate("joint_deformationClass", "TestVox4Vertex", 0, QApplication::UnicodeUTF8));
        pushButton_vox4idx->setText(QApplication::translate("joint_deformationClass", "TestVox4Idx", 0, QApplication::UnicodeUTF8));
        pushButton_showLowerMesh->setText(QApplication::translate("joint_deformationClass", "TestShowLowerMesh", 0, QApplication::UnicodeUTF8));
        pushButton_testSleep->setText(QApplication::translate("joint_deformationClass", "TestSleepTime", 0, QApplication::UnicodeUTF8));
        pushButton_testFunction->setText(QApplication::translate("joint_deformationClass", "testFunction", 0, QApplication::UnicodeUTF8));
        pushButton_testVoxConnection->setText(QApplication::translate("joint_deformationClass", "testVoxConnection", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(test), QApplication::translate("joint_deformationClass", "test", 0, QApplication::UnicodeUTF8));
        pushButton_initSimulator->setText(QString());
        pushButton_startSimulation->setText(QString());
        pushButton_nextStep->setText(QString());
        pushButton_pauseSimulation->setText(QString());
        pushButton_resetSimulation->setText(QString());
        comboBox_chooseSimulator->clear();
        comboBox_chooseSimulator->insertItems(0, QStringList()
         << QApplication::translate("joint_deformationClass", "NONE", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("joint_deformationClass", "Native Shape Matching", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("joint_deformationClass", "Velocity Matching", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("joint_deformationClass", "Single Grid", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("joint_deformationClass", "Hierarchical Shape Matching", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("joint_deformationClass", "Pair Matching", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("joint_deformationClass", "Multiple Velocity Matching", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("joint_deformationClass", "Simulation through Network", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("joint_deformationClass", "Simulation for Mobile", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("joint_deformationClass", "Experimental HSM", 0, QApplication::UnicodeUTF8)
        );
        groupBox_3->setTitle(QApplication::translate("joint_deformationClass", "Force | PosConstraint | OrientationConstraint", 0, QApplication::UnicodeUTF8));
        radioButton_setForceConstraint->setText(QApplication::translate("joint_deformationClass", "Force", 0, QApplication::UnicodeUTF8));
        radioButton_setPositionConstraint->setText(QApplication::translate("joint_deformationClass", "Position Constraint", 0, QApplication::UnicodeUTF8));
        radioButton_setOrientationConstraint->setText(QApplication::translate("joint_deformationClass", "Orientation Constraint", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class joint_deformationClass: public Ui_joint_deformationClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_JOINT_DEFORMATION_H
