/********************************************************************************
** Form generated from reading UI file 'medicalsurgery.ui'
**
** Created: Mon May 26 16:59:16 2014
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEDICALSURGERY_H
#define UI_MEDICALSURGERY_H

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
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>
#include "renderer.h"

QT_BEGIN_NAMESPACE

class Ui_MedicalSurgeryClass
{
public:
    QAction *actionLoadMesh;
    QAction *actionScaleMesh;
    QAction *actionShowMesh;
    QAction *actionShowVox;
    QAction *actionShowGoalPosition;
    QAction *actionSetAnchor;
    QAction *actionSetConstraint;
    QWidget *centralWidget;
    Renderer *renderWidget;
    QComboBox *comboBox_chooseSimulator;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpinBox *spinBox_gridDensity;
    QPushButton *pushButton_setGridDensity;
    QWidget *horizontalLayoutWidget_5;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_3;
    QLineEdit *lineEdit_damping;
    QPushButton *pushButton_setDamping;
    QGroupBox *groupBox_3;
    QRadioButton *radioButton_setForceConstraint;
    QRadioButton *radioButton_setPositionConstraint;
    QRadioButton *radioButton_setOrientationConstraint;
    QWidget *horizontalLayoutWidget_7;
    QHBoxLayout *horizontalLayout_7;
    QCheckBox *checkBox_dynamics;
    QWidget *horizontalLayoutWidget_10;
    QHBoxLayout *horizontalLayout_10;
    QCheckBox *checkBox_gravity;
    QLabel *label_18;
    QLineEdit *lineEdit_gravityMagnitude;
    QPushButton *pushButton_setGravityMagnitude;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_initSimulator;
    QPushButton *pushButton_startSimulation;
    QPushButton *pushButton_nextStep;
    QPushButton *pushButton_pauseSimulation;
    QPushButton *pushButton_resetSimulation;
    QWidget *horizontalLayoutWidget_20;
    QHBoxLayout *horizontalLayout_20;
    QLabel *label_20;
    QLineEdit *lineEdit_mass;
    QPushButton *pushButton_setMass;
    QPushButton *pushButton_resetMass;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MedicalSurgeryClass)
    {
        if (MedicalSurgeryClass->objectName().isEmpty())
            MedicalSurgeryClass->setObjectName(QString::fromUtf8("MedicalSurgeryClass"));
        MedicalSurgeryClass->resize(1128, 689);
        actionLoadMesh = new QAction(MedicalSurgeryClass);
        actionLoadMesh->setObjectName(QString::fromUtf8("actionLoadMesh"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/MedicalSurgery/Resources/folder_drag_accept.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLoadMesh->setIcon(icon);
        actionScaleMesh = new QAction(MedicalSurgeryClass);
        actionScaleMesh->setObjectName(QString::fromUtf8("actionScaleMesh"));
        actionScaleMesh->setCheckable(true);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/MedicalSurgery/Resources/scale_ruler.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionScaleMesh->setIcon(icon1);
        actionShowMesh = new QAction(MedicalSurgeryClass);
        actionShowMesh->setObjectName(QString::fromUtf8("actionShowMesh"));
        actionShowMesh->setCheckable(true);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/MedicalSurgery/Resources/mesh.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionShowMesh->setIcon(icon2);
        actionShowVox = new QAction(MedicalSurgeryClass);
        actionShowVox->setObjectName(QString::fromUtf8("actionShowVox"));
        actionShowVox->setCheckable(true);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/MedicalSurgery/Resources/CMS.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionShowVox->setIcon(icon3);
        actionShowGoalPosition = new QAction(MedicalSurgeryClass);
        actionShowGoalPosition->setObjectName(QString::fromUtf8("actionShowGoalPosition"));
        actionShowGoalPosition->setCheckable(true);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/MedicalSurgery/Resources/target.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionShowGoalPosition->setIcon(icon4);
        actionSetAnchor = new QAction(MedicalSurgeryClass);
        actionSetAnchor->setObjectName(QString::fromUtf8("actionSetAnchor"));
        actionSetAnchor->setCheckable(true);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/MedicalSurgery/Resources/Selection.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSetAnchor->setIcon(icon5);
        actionSetConstraint = new QAction(MedicalSurgeryClass);
        actionSetConstraint->setObjectName(QString::fromUtf8("actionSetConstraint"));
        actionSetConstraint->setCheckable(true);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/MedicalSurgery/Resources/Selection4Constraint.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSetConstraint->setIcon(icon6);
        centralWidget = new QWidget(MedicalSurgeryClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        renderWidget = new Renderer(centralWidget);
        renderWidget->setObjectName(QString::fromUtf8("renderWidget"));
        renderWidget->setGeometry(QRect(10, 10, 900, 600));
        renderWidget->setMinimumSize(QSize(900, 600));
        renderWidget->setMaximumSize(QSize(900, 600));
        comboBox_chooseSimulator = new QComboBox(centralWidget);
        comboBox_chooseSimulator->setObjectName(QString::fromUtf8("comboBox_chooseSimulator"));
        comboBox_chooseSimulator->setGeometry(QRect(920, 10, 201, 22));
        horizontalLayoutWidget = new QWidget(centralWidget);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(920, 40, 201, 31));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(horizontalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(50, 25));
        label->setMaximumSize(QSize(150, 25));
        label->setLineWidth(1);

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
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/joint_deformation/Resources/dialog_apply.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_setGridDensity->setIcon(icon7);

        horizontalLayout->addWidget(pushButton_setGridDensity);

        horizontalLayoutWidget_5 = new QWidget(centralWidget);
        horizontalLayoutWidget_5->setObjectName(QString::fromUtf8("horizontalLayoutWidget_5"));
        horizontalLayoutWidget_5->setGeometry(QRect(920, 80, 284, 31));
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
        pushButton_setDamping->setIcon(icon7);

        horizontalLayout_5->addWidget(pushButton_setDamping);

        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(920, 120, 201, 91));
        radioButton_setForceConstraint = new QRadioButton(groupBox_3);
        radioButton_setForceConstraint->setObjectName(QString::fromUtf8("radioButton_setForceConstraint"));
        radioButton_setForceConstraint->setGeometry(QRect(10, 30, 61, 17));
        radioButton_setForceConstraint->setChecked(true);
        radioButton_setPositionConstraint = new QRadioButton(groupBox_3);
        radioButton_setPositionConstraint->setObjectName(QString::fromUtf8("radioButton_setPositionConstraint"));
        radioButton_setPositionConstraint->setGeometry(QRect(70, 30, 111, 17));
        radioButton_setOrientationConstraint = new QRadioButton(groupBox_3);
        radioButton_setOrientationConstraint->setObjectName(QString::fromUtf8("radioButton_setOrientationConstraint"));
        radioButton_setOrientationConstraint->setGeometry(QRect(10, 60, 131, 17));
        horizontalLayoutWidget_7 = new QWidget(centralWidget);
        horizontalLayoutWidget_7->setObjectName(QString::fromUtf8("horizontalLayoutWidget_7"));
        horizontalLayoutWidget_7->setGeometry(QRect(920, 220, 201, 31));
        horizontalLayout_7 = new QHBoxLayout(horizontalLayoutWidget_7);
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(0, 0, 0, 0);
        checkBox_dynamics = new QCheckBox(horizontalLayoutWidget_7);
        checkBox_dynamics->setObjectName(QString::fromUtf8("checkBox_dynamics"));
        checkBox_dynamics->setChecked(true);

        horizontalLayout_7->addWidget(checkBox_dynamics);

        horizontalLayoutWidget_10 = new QWidget(centralWidget);
        horizontalLayoutWidget_10->setObjectName(QString::fromUtf8("horizontalLayoutWidget_10"));
        horizontalLayoutWidget_10->setGeometry(QRect(920, 260, 256, 31));
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

        horizontalLayoutWidget_2 = new QWidget(centralWidget);
        horizontalLayoutWidget_2->setObjectName(QString::fromUtf8("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(920, 440, 206, 51));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        pushButton_initSimulator = new QPushButton(horizontalLayoutWidget_2);
        pushButton_initSimulator->setObjectName(QString::fromUtf8("pushButton_initSimulator"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/joint_deformation/Resources/gnome_run.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_initSimulator->setIcon(icon8);
        pushButton_initSimulator->setIconSize(QSize(24, 24));

        horizontalLayout_2->addWidget(pushButton_initSimulator);

        pushButton_startSimulation = new QPushButton(horizontalLayoutWidget_2);
        pushButton_startSimulation->setObjectName(QString::fromUtf8("pushButton_startSimulation"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/joint_deformation/Resources/gtk_media_play_ltr.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_startSimulation->setIcon(icon9);
        pushButton_startSimulation->setIconSize(QSize(24, 24));

        horizontalLayout_2->addWidget(pushButton_startSimulation);

        pushButton_nextStep = new QPushButton(horizontalLayoutWidget_2);
        pushButton_nextStep->setObjectName(QString::fromUtf8("pushButton_nextStep"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/joint_deformation/Resources/gtk_media_next_ltr.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_nextStep->setIcon(icon10);
        pushButton_nextStep->setIconSize(QSize(24, 24));

        horizontalLayout_2->addWidget(pushButton_nextStep);

        pushButton_pauseSimulation = new QPushButton(horizontalLayoutWidget_2);
        pushButton_pauseSimulation->setObjectName(QString::fromUtf8("pushButton_pauseSimulation"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/joint_deformation/Resources/gtk_media_pause.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_pauseSimulation->setIcon(icon11);
        pushButton_pauseSimulation->setIconSize(QSize(24, 24));

        horizontalLayout_2->addWidget(pushButton_pauseSimulation);

        pushButton_resetSimulation = new QPushButton(horizontalLayoutWidget_2);
        pushButton_resetSimulation->setObjectName(QString::fromUtf8("pushButton_resetSimulation"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/joint_deformation/Resources/gtk_media_stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_resetSimulation->setIcon(icon12);
        pushButton_resetSimulation->setIconSize(QSize(24, 24));

        horizontalLayout_2->addWidget(pushButton_resetSimulation);

        horizontalLayoutWidget_20 = new QWidget(centralWidget);
        horizontalLayoutWidget_20->setObjectName(QString::fromUtf8("horizontalLayoutWidget_20"));
        horizontalLayoutWidget_20->setGeometry(QRect(920, 300, 244, 31));
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

        MedicalSurgeryClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MedicalSurgeryClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1128, 21));
        MedicalSurgeryClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MedicalSurgeryClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MedicalSurgeryClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MedicalSurgeryClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MedicalSurgeryClass->setStatusBar(statusBar);

        mainToolBar->addAction(actionLoadMesh);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionScaleMesh);
        mainToolBar->addAction(actionShowMesh);
        mainToolBar->addAction(actionShowVox);
        mainToolBar->addAction(actionShowGoalPosition);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionSetAnchor);
        mainToolBar->addAction(actionSetConstraint);

        retranslateUi(MedicalSurgeryClass);
        QObject::connect(actionLoadMesh, SIGNAL(triggered()), MedicalSurgeryClass, SLOT(loadMesh()));
        QObject::connect(comboBox_chooseSimulator, SIGNAL(currentIndexChanged(int)), MedicalSurgeryClass, SLOT(chooseSimulator(int)));
        QObject::connect(actionScaleMesh, SIGNAL(triggered()), MedicalSurgeryClass, SLOT(scaleMesh()));
        QObject::connect(actionSetAnchor, SIGNAL(triggered(bool)), MedicalSurgeryClass, SLOT(setAnchor(bool)));
        QObject::connect(actionSetConstraint, SIGNAL(triggered(bool)), MedicalSurgeryClass, SLOT(setConstraint(bool)));
        QObject::connect(actionShowMesh, SIGNAL(triggered(bool)), MedicalSurgeryClass, SLOT(showMesh(bool)));
        QObject::connect(actionShowVox, SIGNAL(triggered(bool)), MedicalSurgeryClass, SLOT(showVox(bool)));
        QObject::connect(actionShowGoalPosition, SIGNAL(triggered(bool)), MedicalSurgeryClass, SLOT(showGoalPosition(bool)));

        QMetaObject::connectSlotsByName(MedicalSurgeryClass);
    } // setupUi

    void retranslateUi(QMainWindow *MedicalSurgeryClass)
    {
        MedicalSurgeryClass->setWindowTitle(QApplication::translate("MedicalSurgeryClass", "MedicalSurgery", 0, QApplication::UnicodeUTF8));
        actionLoadMesh->setText(QApplication::translate("MedicalSurgeryClass", "LoadMesh", 0, QApplication::UnicodeUTF8));
        actionLoadMesh->setShortcut(QApplication::translate("MedicalSurgeryClass", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionScaleMesh->setText(QApplication::translate("MedicalSurgeryClass", "scaleMesh", 0, QApplication::UnicodeUTF8));
        actionShowMesh->setText(QApplication::translate("MedicalSurgeryClass", "showMesh", 0, QApplication::UnicodeUTF8));
        actionShowVox->setText(QApplication::translate("MedicalSurgeryClass", "showVox", 0, QApplication::UnicodeUTF8));
        actionShowGoalPosition->setText(QApplication::translate("MedicalSurgeryClass", "showGoalPosition", 0, QApplication::UnicodeUTF8));
        actionSetAnchor->setText(QApplication::translate("MedicalSurgeryClass", "setAnchor", 0, QApplication::UnicodeUTF8));
        actionSetConstraint->setText(QApplication::translate("MedicalSurgeryClass", "setConstraint", 0, QApplication::UnicodeUTF8));
        comboBox_chooseSimulator->clear();
        comboBox_chooseSimulator->insertItems(0, QStringList()
         << QApplication::translate("MedicalSurgeryClass", "New Item", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MedicalSurgeryClass", "ShapeMatching", 0, QApplication::UnicodeUTF8)
        );
        label->setText(QApplication::translate("MedicalSurgeryClass", "Grid density", 0, QApplication::UnicodeUTF8));
        pushButton_setGridDensity->setText(QApplication::translate("MedicalSurgeryClass", "Apply", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MedicalSurgeryClass", "Damping", 0, QApplication::UnicodeUTF8));
        lineEdit_damping->setText(QApplication::translate("MedicalSurgeryClass", "0.01", 0, QApplication::UnicodeUTF8));
        pushButton_setDamping->setText(QApplication::translate("MedicalSurgeryClass", "SetDamp", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("MedicalSurgeryClass", "Force | PosConstraint | Orientation", 0, QApplication::UnicodeUTF8));
        radioButton_setForceConstraint->setText(QApplication::translate("MedicalSurgeryClass", "Force", 0, QApplication::UnicodeUTF8));
        radioButton_setPositionConstraint->setText(QApplication::translate("MedicalSurgeryClass", "Position Constraint", 0, QApplication::UnicodeUTF8));
        radioButton_setOrientationConstraint->setText(QApplication::translate("MedicalSurgeryClass", "Orientation Constraint", 0, QApplication::UnicodeUTF8));
        checkBox_dynamics->setText(QApplication::translate("MedicalSurgeryClass", "Dynamics", 0, QApplication::UnicodeUTF8));
        checkBox_gravity->setText(QApplication::translate("MedicalSurgeryClass", "Gravity", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("MedicalSurgeryClass", " Magnitude", 0, QApplication::UnicodeUTF8));
        lineEdit_gravityMagnitude->setText(QApplication::translate("MedicalSurgeryClass", "0.01", 0, QApplication::UnicodeUTF8));
        pushButton_setGravityMagnitude->setText(QApplication::translate("MedicalSurgeryClass", "SetMagnitude", 0, QApplication::UnicodeUTF8));
        pushButton_initSimulator->setText(QString());
        pushButton_startSimulation->setText(QString());
        pushButton_nextStep->setText(QString());
        pushButton_pauseSimulation->setText(QString());
        pushButton_resetSimulation->setText(QString());
        label_20->setText(QApplication::translate("MedicalSurgeryClass", "Mass", 0, QApplication::UnicodeUTF8));
        lineEdit_mass->setText(QApplication::translate("MedicalSurgeryClass", "1.0", 0, QApplication::UnicodeUTF8));
        pushButton_setMass->setText(QApplication::translate("MedicalSurgeryClass", "SetMass", 0, QApplication::UnicodeUTF8));
        pushButton_resetMass->setText(QApplication::translate("MedicalSurgeryClass", "ResetMass", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MedicalSurgeryClass: public Ui_MedicalSurgeryClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEDICALSURGERY_H
