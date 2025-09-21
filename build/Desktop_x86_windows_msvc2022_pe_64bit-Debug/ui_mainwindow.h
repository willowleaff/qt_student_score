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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QStackedWidget *stackedWidget;
    QWidget *memupage;
    QPushButton *LookBtn;
    QPushButton *StatBtn;
    QPushButton *QuitBtn;
    QLabel *StuLa;
    QPushButton *AsBtn;
    QPushButton *MoBtn;
    QWidget *aspage;
    QFrame *plotFrame;
    QWidget *fqpage;
    QPushButton *pushButton;
    QWidget *mopage;
    QWidget *lookpage;
    QLabel *label_2;
    QTableWidget *LookTW;
    QPushButton *addBtn;
    QPushButton *saveBtn;
    QPushButton *LSBtn;
    QWidget *statpage;
    QTableWidget *StTW;
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
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
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
        QuitBtn->setGeometry(QRect(30, 20, 71, 31));
        StuLa = new QLabel(memupage);
        StuLa->setObjectName("StuLa");
        StuLa->setGeometry(QRect(310, 40, 191, 61));
        AsBtn = new QPushButton(memupage);
        AsBtn->setObjectName("AsBtn");
        AsBtn->setGeometry(QRect(300, 240, 81, 31));
        MoBtn = new QPushButton(memupage);
        MoBtn->setObjectName("MoBtn");
        MoBtn->setGeometry(QRect(430, 240, 81, 31));
        stackedWidget->addWidget(memupage);
        aspage = new QWidget();
        aspage->setObjectName("aspage");
        plotFrame = new QFrame(aspage);
        plotFrame->setObjectName("plotFrame");
        plotFrame->setGeometry(QRect(80, 120, 201, 181));
        plotFrame->setFrameShape(QFrame::Shape::StyledPanel);
        plotFrame->setFrameShadow(QFrame::Shadow::Raised);
        stackedWidget->addWidget(aspage);
        fqpage = new QWidget();
        fqpage->setObjectName("fqpage");
        pushButton = new QPushButton(fqpage);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(260, 170, 56, 18));
        stackedWidget->addWidget(fqpage);
        mopage = new QWidget();
        mopage->setObjectName("mopage");
        stackedWidget->addWidget(mopage);
        lookpage = new QWidget();
        lookpage->setObjectName("lookpage");
        label_2 = new QLabel(lookpage);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(0, -10, 181, 61));
        LookTW = new QTableWidget(lookpage);
        if (LookTW->rowCount() < 1)
            LookTW->setRowCount(1);
        LookTW->setObjectName("LookTW");
        LookTW->setGeometry(QRect(0, 40, 701, 341));
        addBtn = new QPushButton(lookpage);
        addBtn->setObjectName("addBtn");
        addBtn->setGeometry(QRect(720, 230, 71, 31));
        saveBtn = new QPushButton(lookpage);
        saveBtn->setObjectName("saveBtn");
        saveBtn->setGeometry(QRect(720, 280, 71, 31));
        LSBtn = new QPushButton(lookpage);
        LSBtn->setObjectName("LSBtn");
        LSBtn->setGeometry(QRect(720, 90, 71, 31));
        stackedWidget->addWidget(lookpage);
        statpage = new QWidget();
        statpage->setObjectName("statpage");
        StTW = new QTableWidget(statpage);
        StTW->setObjectName("StTW");
        StTW->setGeometry(QRect(50, 0, 721, 431));
        stackedWidget->addWidget(statpage);

        verticalLayout->addWidget(stackedWidget);

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
        AsBtn->setText(QCoreApplication::translate("MainWindow", "\346\210\220\347\273\251\345\210\206\346\236\220", nullptr));
        MoBtn->setText(QCoreApplication::translate("MainWindow", "\345\245\226\345\255\246\351\207\221\350\257\204\345\256\232", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "PushButton", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\345\255\246\347\224\237\346\210\220\347\273\251\346\237\245\350\257\242", nullptr));
        addBtn->setText(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240\346\225\260\346\215\256", nullptr));
        saveBtn->setText(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230\344\277\256\346\224\271", nullptr));
        LSBtn->setText(QCoreApplication::translate("MainWindow", "\346\210\220\347\273\251\347\255\233\351\200\211", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
