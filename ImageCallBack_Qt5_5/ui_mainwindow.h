/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_takePhoto;
    QLabel *label_2;
    QLabel *label;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QPushButton *pushButton_OpenCom;
    QLabel *label_3;
    QPushButton *pushButton_ClearRecv;
    QPushButton *pushButton_3;
    QTextBrowser *textBrowser;
    QLabel *label_4;
    QLabel *label_ComStatus;
    QComboBox *comboBox_com;
    QGraphicsView *graphicsView_ComStatus;
    QGraphicsView *graphicsView_Grabingstate;
    QLabel *label_5;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1072, 655);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(10, 10, 89, 31));
        pushButton->setMinimumSize(QSize(89, 25));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(130, 10, 89, 31));
        pushButton_2->setMinimumSize(QSize(89, 25));
        pushButton_takePhoto = new QPushButton(centralWidget);
        pushButton_takePhoto->setObjectName(QStringLiteral("pushButton_takePhoto"));
        pushButton_takePhoto->setGeometry(QRect(250, 10, 89, 31));
        pushButton_takePhoto->setMinimumSize(QSize(89, 25));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(412, 9, 651, 271));
        label_2->setMinimumSize(QSize(651, 271));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(412, 286, 651, 271));
        label->setMinimumSize(QSize(651, 271));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(20, 250, 321, 171));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        pushButton_OpenCom = new QPushButton(groupBox);
        pushButton_OpenCom->setObjectName(QStringLiteral("pushButton_OpenCom"));

        gridLayout->addWidget(pushButton_OpenCom, 2, 0, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 0, 0, 1, 1);

        pushButton_ClearRecv = new QPushButton(groupBox);
        pushButton_ClearRecv->setObjectName(QStringLiteral("pushButton_ClearRecv"));

        gridLayout->addWidget(pushButton_ClearRecv, 3, 0, 1, 1);

        pushButton_3 = new QPushButton(groupBox);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        gridLayout->addWidget(pushButton_3, 1, 0, 1, 1);

        textBrowser = new QTextBrowser(groupBox);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));

        gridLayout->addWidget(textBrowser, 1, 1, 3, 1);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 1, 2, 1, 1);

        label_ComStatus = new QLabel(groupBox);
        label_ComStatus->setObjectName(QStringLiteral("label_ComStatus"));

        gridLayout->addWidget(label_ComStatus, 2, 2, 1, 1);

        comboBox_com = new QComboBox(groupBox);
        comboBox_com->setObjectName(QStringLiteral("comboBox_com"));

        gridLayout->addWidget(comboBox_com, 0, 1, 1, 2);

        graphicsView_ComStatus = new QGraphicsView(groupBox);
        graphicsView_ComStatus->setObjectName(QStringLiteral("graphicsView_ComStatus"));

        gridLayout->addWidget(graphicsView_ComStatus, 3, 2, 1, 1);

        graphicsView_Grabingstate = new QGraphicsView(centralWidget);
        graphicsView_Grabingstate->setObjectName(QStringLiteral("graphicsView_Grabingstate"));
        graphicsView_Grabingstate->setGeometry(QRect(160, 110, 103, 28));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(50, 110, 111, 20));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1072, 28));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButton->setText(QApplication::translate("MainWindow", "\350\277\236\346\216\245\346\221\204\345\203\217\345\244\264", nullptr));
        pushButton_2->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\346\221\204\345\203\217\345\244\264", nullptr));
        pushButton_takePhoto->setText(QApplication::translate("MainWindow", "\346\213\215\347\205\247", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        label->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        groupBox->setTitle(QApplication::translate("MainWindow", "\346\265\213\350\267\235\346\250\241\345\235\227", nullptr));
        pushButton_OpenCom->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\344\270\262\345\217\243", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "   \351\200\211\346\213\251\344\270\262\345\217\243", nullptr));
        pushButton_ClearRecv->setText(QApplication::translate("MainWindow", "\346\270\205\347\251\272", nullptr));
        pushButton_3->setText(QApplication::translate("MainWindow", "\346\220\234\347\264\242\344\270\262\345\217\243", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "\344\270\262\345\217\243\347\212\266\346\200\201", nullptr));
        label_ComStatus->setText(QApplication::translate("MainWindow", "\346\226\255\345\274\200", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "\345\217\226\346\265\201\347\212\266\346\200\201\346\217\220\351\206\222\357\274\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
