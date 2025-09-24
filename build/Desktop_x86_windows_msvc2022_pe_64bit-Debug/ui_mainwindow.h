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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
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
    QWidget *Anspage;
    QLabel *label_7;
    QPushButton *AnsBtn;
    QComboBox *AnsCb;
    QWidget *lookpage;
    QLabel *label_2;
    QTableWidget *LookTW;
    QPushButton *addBtn;
    QPushButton *saveBtn;
    QPushButton *LSBtn;
    QWidget *choopage;
    QPushButton *classBtn;
    QPushButton *obBtn;
    QLineEdit *classEdit;
    QLineEdit *majorEdit;
    QComboBox *subjectCombo;
    QLabel *label;
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *minScoreEdit;
    QLineEdit *maxScoreEdit;
    QLabel *label_5;
    QWidget *classpage;
    QTableWidget *filterTW;
    QWidget *statpage;
    QPushButton *rankBtn;
    QPushButton *class_scoreBtn;
    QPushButton *major_scoreBtn;
    QLineEdit *class_scoreEt;
    QLineEdit *major_scoreEt;
    QComboBox *class_scoreCb;
    QComboBox *major_scoreCb;
    QWidget *rankpage;
    QTableWidget *rankTable;
    QWidget *scorepage;
    QLabel *mathAverageLabel;
    QLabel *mathHighestLabel;
    QLabel *mathLowestLabel;
    QLabel *mathPassRateLabel;
    QLabel *mathExcellentRateLabel;
    QTableWidget *statsTable;
    QPushButton *downloadBtn;
    QLabel *CM;
    QWidget *major_scorepage;
    QWidget *Asspage;
    QPushButton *AssPb;
    QComboBox *AssCb;
    QLabel *label_6;
    QWidget *aspage;
    QFrame *plotFrame;
    QWidget *mopage;
    QTableWidget *scholarshipTable;
    QPushButton *evalScholarshipBtn;
    QPushButton *exportScholarshipBtn;
    QWidget *fqpage;
    QPushButton *pushButton;
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
        Anspage = new QWidget();
        Anspage->setObjectName("Anspage");
        label_7 = new QLabel(Anspage);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(310, 90, 161, 81));
        AnsBtn = new QPushButton(Anspage);
        AnsBtn->setObjectName("AnsBtn");
        AnsBtn->setGeometry(QRect(340, 270, 81, 31));
        AnsCb = new QComboBox(Anspage);
        AnsCb->setObjectName("AnsCb");
        AnsCb->setGeometry(QRect(310, 200, 131, 31));
        stackedWidget->addWidget(Anspage);
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
        choopage = new QWidget();
        choopage->setObjectName("choopage");
        classBtn = new QPushButton(choopage);
        classBtn->setObjectName("classBtn");
        classBtn->setGeometry(QRect(200, 330, 91, 31));
        obBtn = new QPushButton(choopage);
        obBtn->setObjectName("obBtn");
        obBtn->setGeometry(QRect(560, 330, 91, 31));
        classEdit = new QLineEdit(choopage);
        classEdit->setObjectName("classEdit");
        classEdit->setGeometry(QRect(150, 130, 113, 20));
        majorEdit = new QLineEdit(choopage);
        majorEdit->setObjectName("majorEdit");
        majorEdit->setGeometry(QRect(560, 130, 113, 20));
        subjectCombo = new QComboBox(choopage);
        subjectCombo->setObjectName("subjectCombo");
        subjectCombo->setGeometry(QRect(150, 200, 111, 21));
        label = new QLabel(choopage);
        label->setObjectName("label");
        label->setGeometry(QRect(70, 130, 51, 21));
        label_3 = new QLabel(choopage);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(70, 200, 61, 21));
        label_4 = new QLabel(choopage);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(70, 240, 81, 41));
        minScoreEdit = new QLineEdit(choopage);
        minScoreEdit->setObjectName("minScoreEdit");
        minScoreEdit->setGeometry(QRect(150, 250, 41, 16));
        maxScoreEdit = new QLineEdit(choopage);
        maxScoreEdit->setObjectName("maxScoreEdit");
        maxScoreEdit->setGeometry(QRect(220, 250, 41, 16));
        label_5 = new QLabel(choopage);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(490, 130, 41, 21));
        stackedWidget->addWidget(choopage);
        classpage = new QWidget();
        classpage->setObjectName("classpage");
        filterTW = new QTableWidget(classpage);
        filterTW->setObjectName("filterTW");
        filterTW->setGeometry(QRect(40, 10, 751, 431));
        stackedWidget->addWidget(classpage);
        statpage = new QWidget();
        statpage->setObjectName("statpage");
        rankBtn = new QPushButton(statpage);
        rankBtn->setObjectName("rankBtn");
        rankBtn->setGeometry(QRect(310, 80, 111, 31));
        class_scoreBtn = new QPushButton(statpage);
        class_scoreBtn->setObjectName("class_scoreBtn");
        class_scoreBtn->setGeometry(QRect(180, 160, 111, 31));
        major_scoreBtn = new QPushButton(statpage);
        major_scoreBtn->setObjectName("major_scoreBtn");
        major_scoreBtn->setGeometry(QRect(180, 250, 111, 31));
        class_scoreEt = new QLineEdit(statpage);
        class_scoreEt->setObjectName("class_scoreEt");
        class_scoreEt->setGeometry(QRect(310, 170, 121, 21));
        major_scoreEt = new QLineEdit(statpage);
        major_scoreEt->setObjectName("major_scoreEt");
        major_scoreEt->setGeometry(QRect(320, 260, 113, 20));
        class_scoreCb = new QComboBox(statpage);
        class_scoreCb->setObjectName("class_scoreCb");
        class_scoreCb->setGeometry(QRect(470, 170, 101, 21));
        major_scoreCb = new QComboBox(statpage);
        major_scoreCb->setObjectName("major_scoreCb");
        major_scoreCb->setGeometry(QRect(470, 260, 101, 21));
        stackedWidget->addWidget(statpage);
        rankpage = new QWidget();
        rankpage->setObjectName("rankpage");
        rankTable = new QTableWidget(rankpage);
        rankTable->setObjectName("rankTable");
        rankTable->setGeometry(QRect(-10, 10, 821, 421));
        rankTable->verticalHeader()->setStretchLastSection(true);
        stackedWidget->addWidget(rankpage);
        scorepage = new QWidget();
        scorepage->setObjectName("scorepage");
        mathAverageLabel = new QLabel(scorepage);
        mathAverageLabel->setObjectName("mathAverageLabel");
        mathAverageLabel->setGeometry(QRect(560, 40, 221, 51));
        mathHighestLabel = new QLabel(scorepage);
        mathHighestLabel->setObjectName("mathHighestLabel");
        mathHighestLabel->setGeometry(QRect(560, 250, 201, 71));
        mathLowestLabel = new QLabel(scorepage);
        mathLowestLabel->setObjectName("mathLowestLabel");
        mathLowestLabel->setGeometry(QRect(560, 200, 171, 51));
        mathPassRateLabel = new QLabel(scorepage);
        mathPassRateLabel->setObjectName("mathPassRateLabel");
        mathPassRateLabel->setGeometry(QRect(560, 150, 181, 41));
        mathExcellentRateLabel = new QLabel(scorepage);
        mathExcellentRateLabel->setObjectName("mathExcellentRateLabel");
        mathExcellentRateLabel->setGeometry(QRect(560, 100, 181, 41));
        statsTable = new QTableWidget(scorepage);
        statsTable->setObjectName("statsTable");
        statsTable->setGeometry(QRect(30, 30, 521, 351));
        downloadBtn = new QPushButton(scorepage);
        downloadBtn->setObjectName("downloadBtn");
        downloadBtn->setGeometry(QRect(580, 360, 56, 18));
        CM = new QLabel(scorepage);
        CM->setObjectName("CM");
        CM->setGeometry(QRect(560, 30, 241, 21));
        stackedWidget->addWidget(scorepage);
        major_scorepage = new QWidget();
        major_scorepage->setObjectName("major_scorepage");
        stackedWidget->addWidget(major_scorepage);
        Asspage = new QWidget();
        Asspage->setObjectName("Asspage");
        AssPb = new QPushButton(Asspage);
        AssPb->setObjectName("AssPb");
        AssPb->setGeometry(QRect(330, 260, 121, 21));
        AssCb = new QComboBox(Asspage);
        AssCb->setObjectName("AssCb");
        AssCb->setGeometry(QRect(320, 200, 131, 31));
        label_6 = new QLabel(Asspage);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(330, 140, 191, 41));
        stackedWidget->addWidget(Asspage);
        aspage = new QWidget();
        aspage->setObjectName("aspage");
        plotFrame = new QFrame(aspage);
        plotFrame->setObjectName("plotFrame");
        plotFrame->setGeometry(QRect(80, 120, 201, 181));
        plotFrame->setFrameShape(QFrame::Shape::StyledPanel);
        plotFrame->setFrameShadow(QFrame::Shadow::Raised);
        stackedWidget->addWidget(aspage);
        mopage = new QWidget();
        mopage->setObjectName("mopage");
        scholarshipTable = new QTableWidget(mopage);
        scholarshipTable->setObjectName("scholarshipTable");
        scholarshipTable->setGeometry(QRect(130, 40, 521, 381));
        evalScholarshipBtn = new QPushButton(mopage);
        evalScholarshipBtn->setObjectName("evalScholarshipBtn");
        evalScholarshipBtn->setGeometry(QRect(690, 150, 81, 41));
        exportScholarshipBtn = new QPushButton(mopage);
        exportScholarshipBtn->setObjectName("exportScholarshipBtn");
        exportScholarshipBtn->setGeometry(QRect(680, 200, 91, 31));
        stackedWidget->addWidget(mopage);
        fqpage = new QWidget();
        fqpage->setObjectName("fqpage");
        pushButton = new QPushButton(fqpage);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(260, 170, 56, 18));
        stackedWidget->addWidget(fqpage);

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

        stackedWidget->setCurrentIndex(7);


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
        label_7->setText(QCoreApplication::translate("MainWindow", "\351\200\211\346\213\251\347\247\221\347\233\256\345\210\206\346\236\220", nullptr));
        AnsBtn->setText(QCoreApplication::translate("MainWindow", "\345\210\206\346\236\220", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\345\255\246\347\224\237\346\210\220\347\273\251\346\237\245\350\257\242", nullptr));
        addBtn->setText(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240\346\225\260\346\215\256", nullptr));
        saveBtn->setText(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230\344\277\256\346\224\271", nullptr));
        LSBtn->setText(QCoreApplication::translate("MainWindow", "\346\210\220\347\273\251\347\255\233\351\200\211", nullptr));
        classBtn->setText(QCoreApplication::translate("MainWindow", "\347\217\255\347\272\247\346\237\245\350\257\242", nullptr));
        obBtn->setText(QCoreApplication::translate("MainWindow", "\344\270\223\344\270\232\346\237\245\350\257\242", nullptr));
        subjectCombo->setCurrentText(QString());
        label->setText(QCoreApplication::translate("MainWindow", "\347\217\255\347\272\247\357\274\232", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\347\247\221\347\233\256\357\274\232", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\345\210\206\346\225\260\346\256\265\357\274\232", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "\344\270\223\344\270\232\357\274\232", nullptr));
        rankBtn->setText(QCoreApplication::translate("MainWindow", "\345\255\246\347\224\237\346\216\222\345\220\215", nullptr));
        class_scoreBtn->setText(QCoreApplication::translate("MainWindow", "\347\217\255\347\272\247\346\210\220\347\273\251", nullptr));
        major_scoreBtn->setText(QCoreApplication::translate("MainWindow", "\344\270\223\344\270\232\346\210\220\347\273\251", nullptr));
        mathAverageLabel->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        mathHighestLabel->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        mathLowestLabel->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        mathPassRateLabel->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        mathExcellentRateLabel->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        downloadBtn->setText(QCoreApplication::translate("MainWindow", "\344\270\213\350\275\275\346\226\207\344\273\266", nullptr));
        CM->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        AssPb->setText(QCoreApplication::translate("MainWindow", "\346\237\245\350\257\242", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "\351\200\211\346\213\251\347\247\221\347\233\256\345\210\206\346\236\220", nullptr));
        evalScholarshipBtn->setText(QCoreApplication::translate("MainWindow", "\350\257\204\345\256\232\345\245\226\345\255\246\351\207\221", nullptr));
        exportScholarshipBtn->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\207\272\347\273\223\346\236\234", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "PushButton", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
