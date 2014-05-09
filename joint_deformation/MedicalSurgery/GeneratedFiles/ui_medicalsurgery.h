/********************************************************************************
** Form generated from reading UI file 'medicalsurgery.ui'
**
** Created: Thu May 8 19:59:43 2014
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
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MedicalSurgeryClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MedicalSurgeryClass)
    {
        if (MedicalSurgeryClass->objectName().isEmpty())
            MedicalSurgeryClass->setObjectName(QString::fromUtf8("MedicalSurgeryClass"));
        MedicalSurgeryClass->resize(600, 400);
        menuBar = new QMenuBar(MedicalSurgeryClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        MedicalSurgeryClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MedicalSurgeryClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MedicalSurgeryClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(MedicalSurgeryClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        MedicalSurgeryClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MedicalSurgeryClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MedicalSurgeryClass->setStatusBar(statusBar);

        retranslateUi(MedicalSurgeryClass);

        QMetaObject::connectSlotsByName(MedicalSurgeryClass);
    } // setupUi

    void retranslateUi(QMainWindow *MedicalSurgeryClass)
    {
        MedicalSurgeryClass->setWindowTitle(QApplication::translate("MedicalSurgeryClass", "MedicalSurgery", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MedicalSurgeryClass: public Ui_MedicalSurgeryClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEDICALSURGERY_H
