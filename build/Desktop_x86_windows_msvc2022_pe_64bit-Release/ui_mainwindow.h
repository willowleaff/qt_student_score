/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QStackedWidget *stackedWidget;
    QWidget *memupage;
    QPushButton *LookBtn;
    QPushButton *StatBtn;
    QPushButton *QuitBtn;
    QLabel *StuLa;
    QWidget *lookpage;
    QLabel *label_2;
    QTableWidget *LookTW;
    QPushButton *addBtn;
    QPushButton *saveBtn;
    QWidget *statpage;
    QLabel *label_3;
    QFrame *plotFrame;
    QMenuBar *menubar;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(820, 488);
        MainWindow->setStyleSheet(QString::fromUtf8("QStackedWidget#stackedWidget\n"
"{\n"
"	background-image: url(:/images/background.png);\n"
"}\n"
"QStackedWidget#stackedWidget QPushButton\n"
"{\n"
"	border:none;\n"
"	background-color: rgb(255, 255, 255);\n"
"}\n"
""));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName("horizontalLayout");
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setStyleSheet(QString::fromUtf8("QStackedWidget#stackedWidget\n"
"{\n"
"	\n"
"	border-image: url(:/images/bg1.png);\n"
"}\n"
"QStackedWidget#stackedWidget QPushButton\n"
"{\n"
"	border:none;\n"
"	background-color: rgb(255, 255, 255);\n"
"	font: 12pt \"\345\256\213\344\275\223\";\n"
"}\n"
"QStackedWidget#stackedWidget QPushButton:hover\n"
"{\n"
"	border:none;\n"
"	background-color: rgb(170, 255, 255)\n"
"	\n"
"}\n"
"QLabel\n"
"{\n"
"	border:none;\n"
"	font: 18pt \"\345\256\213\344\275\223\";\n"
"\n"
"}"));
        memupage = new QWidget();
        memupage->setObjectName("memupage");
        LookBtn = new QPushButton(memupage);
        LookBtn->setObjectName("LookBtn");
        LookBtn->setGeometry(QRect(350, 130, 111, 31));
        LookBtn->setMinimumSize(QSize(61, 0));
        StatBtn = new QPushButton(memupage);
        StatBtn->setObjectName("StatBtn");
        StatBtn->setGeometry(QRect(350, 180, 111, 31));
        QuitBtn = new QPushButton(memupage);
        QuitBtn->setObjectName("QuitBtn");
        QuitBtn->setGeometry(QRect(370, 230, 71, 31));
        StuLa = new QLabel(memupage);
        StuLa->setObjectName("StuLa");
        StuLa->setGeometry(QRect(310, 40, 191, 61));
        stackedWidget->addWidget(memupage);
        lookpage = new QWidget();
        lookpage->setObjectName("lookpage");
        label_2 = new QLabel(lookpage);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(0, -10, 81, 61));
        LookTW = new QTableWidget(lookpage);
        if (LookTW->rowCount() < 1)
            LookTW->setRowCount(1);
        LookTW->setObjectName("LookTW");
        LookTW->setGeometry(QRect(0, 40, 701, 341));
        addBtn = new QPushButton(lookpage);
        addBtn->setObjectName("addBtn");
        addBtn->setGeometry(QRect(720, 90, 71, 31));
        saveBtn = new QPushButton(lookpage);
        saveBtn->setObjectName("saveBtn");
        saveBtn->setGeometry(QRect(720, 140, 71, 31));
        stackedWidget->addWidget(lookpage);
        statpage = new QWidget();
        statpage->setObjectName("statpage");
        label_3 = new QLabel(statpage);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(0, -10, 81, 51));
        plotFrame = new QFrame(statpage);
        plotFrame->setObjectName("plotFrame");
        plotFrame->setGeometry(QRect(50, 10, 741, 441));
        plotFrame->setFrameShape(QFrame::Shape::StyledPanel);
        plotFrame->setFrameShadow(QFrame::Shadow::Raised);
        stackedWidget->addWidget(statpage);

        horizontalLayout->addWidget(stackedWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 820, 18));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName("toolBar");
        MainWindow->addToolBar(Qt::ToolBarArea::TopToolBarArea, toolBar);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        LookBtn->setText(QCoreApplication::translate("MainWindow", "\345\255\246\347\224\237\346\210\220\347\273\251\346\237\245\350\257\242", nullptr));
        StatBtn->setText(QCoreApplication::translate("MainWindow", "\345\255\246\347\224\237\346\210\220\347\273\251\347\273\237\350\256\241", nullptr));
        QuitBtn->setText(QCoreApplication::translate("MainWindow", "\351\200\200\345\207\272\347\263\273\347\273\237", nullptr));
        StuLa->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\"><span style=\" font-size:18pt; color:#000000;\">\345\255\246\347\224\237\346\210\220\347\273\251\347\256\241\347\220\206\347\263\273\347\273\237</span></p></body></html>", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "look", nullptr));
        addBtn->setText(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240\346\225\260\346\215\256", nullptr));
        saveBtn->setText(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230\344\277\256\346\224\271", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "stat", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
