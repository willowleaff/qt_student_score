#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QKeyEvent>
#include<QFile>
#include<QTextStream>
#include<QList>
#include<QTableWidgetItem>
#include<QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_plot(nullptr) // 初始化绘图控件指针
{
    ui->setupUi(this);
    // 初始化绘图控件，放在statpage页面（假设UI中有一个QFrame名为plotFrame）
    // m_plot = new NormalDistributionPlot(ui->plotFrame);
    // m_plot->setGeometry(ui->plotFrame->rect()); // 填充整个frame
    m_plot = new NormalDistributionPlot(ui->plotFrame);
    m_plot->setGeometry(ui->plotFrame->rect()); // 填充整个QFrame
    // 关键：让绘图控件随QFrame缩放（若QFrame大小变化，绘图控件也变化）
    m_plot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    loadData("D:\\qt\\student\\student.txt");
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Escape)
    {
    //回到菜单页面
       ui->stackedWidget->setCurrentWidget(ui->memupage);
    }
}
void MainWindow::loadData(const QString &filename)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qInfo()<<"open file failed";
        return;
    }
    QTextStream stream(&file);
    //stream.setEncoding(QStringConverter::Utf8);
    m_fields=stream.readLine().split("\t", Qt::SkipEmptyParts);
    while (!stream.atEnd())
    {
        QString line = stream.readLine();
        if (line.isEmpty()) continue; // 跳过空行
        auto lineData =line.split("\t", Qt::SkipEmptyParts);
        if (lineData.size() >= 6) // 确保有足够的数据进行赋值
        {
            auto s=new Student;
            s->number   =lineData[0];
            s->name     =lineData[1];
            s->classname=lineData[2];
            s->math     =lineData[3].toDouble();
            s->chinese  =lineData[4].toDouble();
            s->english  =lineData[5].toDouble();
            m_students.push_back(s);
        }
    }
    for(auto s:m_students)
    {
        s->display();
    }
    qDebug() << "Read line: " << stream.readLine();
}

void MainWindow::on_LookBtn_released()
{
    //切换到查看页面
    ui->stackedWidget->setCurrentWidget(ui->lookpage);
    //把list中的数据显示到表格中
    qDebug() << "Initial row count: " << ui->LookTW->rowCount();
    ui->LookTW->setRowCount(0);//清空
    ui->LookTW->setColumnCount(m_fields.size());
    ui->LookTW->setHorizontalHeaderLabels(m_fields);
    for(auto s:m_students)
    {
        //获取新行行号
        int row=(ui->LookTW->rowCount());
        //1.添加一个空行
        ui->LookTW->insertRow(row);
        //2.把数据设置到空行中
        ui->LookTW->setItem(row,0,new QTableWidgetItem(s->number));
        ui->LookTW->setItem(row,1,new QTableWidgetItem(s->name));
        ui->LookTW->setItem(row,2,new QTableWidgetItem(s->classname));
        ui->LookTW->setItem(row,3,new QTableWidgetItem(QString::number(s->math)));
        ui->LookTW->setItem(row,4,new QTableWidgetItem(QString::number(s->english)));
        ui->LookTW->setItem(row,5,new QTableWidgetItem(QString::number(s->chinese)));
    }
}
// void MainWindow::saveData(const QString &filename)
// {
//     //流操作<< >>
//     //通过文件操作数据（处理麻烦，格式规范）
//     //数据库存储 学生表 成绩表 班级表 选课表...
//     //大数据分析 大数据处理...数据库
//     //c、c++、数据结构，Qt,MFC,win32,企业开发框架，数据库，网络编程，多线程

// }
void MainWindow::saveData(const QString &filename)
{
    // 这里可以保留你的数据库实现计划，或先实现文本保存
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "错误", "无法打开文件保存");
        return;
    }
    QTextStream stream(&file);
    // 写入表头
    stream << m_fields.join("\t") << "\n";
    // 写入数据
    for (auto s : m_students) {
        stream << s->number << "\t"
               << s->name << "\t"
               << s->classname << "\t"
               << s->math << "\t"
               << s->chinese << "\t"
               << s->english << "\n";
    }
    QMessageBox::information(this, "提示", "数据已保存到文本文件");
}
void MainWindow::on_StatBtn_released()
{
    //切换到统计页面
    ui->stackedWidget->setCurrentWidget(ui->statpage);
    // 提取数学成绩并绘制正态分布
    QVector<double> mathScores;
    for (auto s : m_students) {
        mathScores.append(s->math);
    }
    m_plot->setScores(mathScores);
}


void MainWindow::on_QuitBtn_released()
{
    close();
}


void MainWindow::on_addBtn_released()
{
    //添加一个新行在最下面
    ui->LookTW->insertRow(ui->LookTW->rowCount());
    //跳转到最下面
    ui->LookTW->scrollToBottom();
    //创建一个学生
    m_students.push_back(new Student);
}


void MainWindow::on_saveBtn_released()
{
    for(int i=0;i<ui->LookTW->rowCount();i++)
    {
        m_students[i]->number=ui->LookTW->item(i,0)->text();
        m_students[i]->name=ui->LookTW->item(i,1)->text();
        m_students[i]->classname=ui->LookTW->item(i,2)->text();
        m_students[i]->math=ui->LookTW->item(i,3)->text().toDouble();
        m_students[i]->english=ui->LookTW->item(i,4)->text().toDouble();
        m_students[i]->chinese=ui->LookTW->item(i,5)->text().toDouble();
    }
    QMessageBox::information(this,"保存提示","保存成功");
    // 保存到文件（可替换为数据库保存）
    saveData("D:\\qt\\student\\student.txt");
}

void MainWindow::on_AsBtn_released()
{
    //切换到页面
    ui->stackedWidget->setCurrentWidget(ui->aspage);
}

void MainWindow::on_MoBtn_released()
{
    //切换到页面
    ui->stackedWidget->setCurrentWidget(ui->mopage);
}
void MainWindow::on_pushButton_released(){}
