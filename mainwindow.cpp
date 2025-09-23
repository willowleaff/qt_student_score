#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QKeyEvent>
#include<QFile>
#include<QTextStream>
#include<QList>
#include<QTableWidgetItem>
#include<QMessageBox>
#include "scorepiechart.h"
#include <QFileDialog>
#include <algorithm>
#include <numeric>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_plot(nullptr) // 初始化绘图控件指针
    , m_pieChart(nullptr)
    , m_selectedMajor("") // 初始化选中的专业为空
{
    ui->setupUi(this);
    initStatPageLayout();
    // 初始化科目下拉框（假设下拉框对象名为subjectCombo）
    initSubjectCombo(); // 调用初始化函数
    loadData("D:\\qt\\student\\student.txt");
    calculateStudentScores();
    initScholarshipRules(); // 初始化奖学金规则

    // 关联按钮事件
    connect(ui->evalScholarshipBtn, &QPushButton::released, this, &MainWindow::evaluateScholarship);
    connect(ui->exportScholarshipBtn, &QPushButton::released, this, &MainWindow::exportScholarshipResult);
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
        if (lineData.size() >= 11) // 确保有足够的数据进行赋值
        {
            auto s=new Student;
            s->number   =lineData[0];
            s->name     =lineData[1];
            s->sex   =lineData[2];
            s->major     =lineData[3];
            s->classname=lineData[4];
            s->math     =lineData[5].toDouble();
            s->cp  =lineData[6].toDouble();
            s->pd  =lineData[7].toDouble();
            s->ds  =lineData[8].toDouble();
            s->la  =lineData[9].toDouble();
            s->dm =lineData[10].toDouble();
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
        ui->LookTW->setItem(row,2,new QTableWidgetItem(s->sex));
        ui->LookTW->setItem(row,3,new QTableWidgetItem(s->major));
        ui->LookTW->setItem(row,4,new QTableWidgetItem(s->classname));
        ui->LookTW->setItem(row,5,new QTableWidgetItem(QString::number(s->math)));
        ui->LookTW->setItem(row,6,new QTableWidgetItem(QString::number(s->cp)));
        ui->LookTW->setItem(row,7,new QTableWidgetItem(QString::number(s->pd)));
        ui->LookTW->setItem(row,8,new QTableWidgetItem(QString::number(s->ds)));
        ui->LookTW->setItem(row,9,new QTableWidgetItem(QString::number(s->la)));
        ui->LookTW->setItem(row,10,new QTableWidgetItem(QString::number(s->dm)));
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
               << s->sex << "\t"
               << s->major << "\t"
               << s->classname << "\t"
               << s->math << "\t"
               << s->cp << "\t"
               << s->pd << "\t"
               << s->ds << "\t"
               << s->la << "\t"
               << s->dm << "\n";
    }
    QMessageBox::information(this, "提示", "数据已保存到文本文件");
}
void MainWindow::initStatPageLayout()
{
    // 1. 获取statpage的布局（之前设置的垂直布局）
    QVBoxLayout *asLayout = qobject_cast<QVBoxLayout*>(ui->aspage->layout());
    if (!asLayout) {
        // 如果没有布局，新建一个垂直布局
        asLayout = new QVBoxLayout(ui->aspage);
        asLayout->setContentsMargins(10, 10, 10, 10);
        asLayout->setSpacing(10);
    }

    // 2. 创建一个水平布局，用于并排显示两个图表
    QHBoxLayout *chartsLayout = new QHBoxLayout();
    chartsLayout->setSpacing(20);

    // 3. 初始化正态分布图
    m_plot = new NormalDistributionPlot();
    m_plot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chartsLayout->addWidget(m_plot);

    // 4. 初始化饼状图
    m_pieChart = new ScorePieChart();
    m_pieChart->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chartsLayout->addWidget(m_pieChart);

    // 5. 将图表布局添加到统计页面
    asLayout->addLayout(chartsLayout);
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
        m_students[i]->sex=ui->LookTW->item(i,2)->text();
        m_students[i]->major=ui->LookTW->item(i,3)->text();
        m_students[i]->classname=ui->LookTW->item(i,4)->text();
        m_students[i]->math=ui->LookTW->item(i,5)->text().toDouble();
        m_students[i]->cp=ui->LookTW->item(i,6)->text().toDouble();
        m_students[i]->pd=ui->LookTW->item(i,7)->text().toDouble();
        m_students[i]->ds=ui->LookTW->item(i,8)->text().toDouble();
        m_students[i]->la=ui->LookTW->item(i,9)->text().toDouble();
        m_students[i]->dm=ui->LookTW->item(i,10)->text().toDouble();
    }
    QMessageBox::information(this,"保存提示","保存成功");
    // 保存到文件（可替换为数据库保存）
    saveData("D:\\qt\\student\\student.txt");
}
void MainWindow::on_AsBtn_released()
{
   ui->stackedWidget->setCurrentWidget(ui->Anspage);
}

void MainWindow::on_AnsBtn_released()
{
    QString subject = ui->AnsCb->currentText();
    m_selectedSubject = subject;
    QVector<double> scores;
    if (subject == "高等数学") {
        for (auto s : m_students) {
            scores.append(s->math);
        }
    } else if (subject == "大学物理") {
        for (auto s : m_students) {
            scores.append(s->cp);
        }
    } else if (subject == "程序设计") {
        for (auto s : m_students) {
            scores.append(s->pd);
        }
    } else if (subject == "数据结构") {
        for (auto s : m_students) {
            scores.append(s->ds);
        }
    } else if (subject == "线性代数") {
        for (auto s : m_students) {
            scores.append(s->la);
        }
    } else if (subject == "离散数学") {
        for (auto s : m_students) {
            scores.append(s->dm);
        }
    } else if (subject == "所有科目") {
        for (auto s : m_students) {
            scores.append(s->math + s->cp + s->pd + s->ds + s->la + s->dm);
        }
    }
    m_plot->setScores(scores);
    m_pieChart->setScores(scores);
    ui->stackedWidget->setCurrentWidget(ui->aspage);
    //ui->stackedWidget->setCurrentWidget(ui->aspage);
}

void MainWindow::on_MoBtn_released()
{
    //切换到页面
    ui->stackedWidget->setCurrentWidget(ui->Asspage);
}

void MainWindow::on_LSBtn_released()
{
    ui->stackedWidget->setCurrentWidget(ui->choopage);
}
// 添加一个初始化下拉框的函数
void MainWindow::initSubjectCombo()
{
    // 清空现有选项（可选）
    ui->subjectCombo->clear();

    // 添加科目选项
    ui->subjectCombo->addItem("所有科目");  // 默认选项
    ui->subjectCombo->addItem("高等数学");
    ui->subjectCombo->addItem("大学物理");
    ui->subjectCombo->addItem("程序设计");
    ui->subjectCombo->addItem("数据结构");
    ui->subjectCombo->addItem("线性代数");
    ui->subjectCombo->addItem("离散数学");

    // 可选：设置默认选中项（第0项为"所有科目"）
    ui->subjectCombo->setCurrentIndex(0);
    ui->class_scoreCb->clear();

    // 添加科目选项
    ui->class_scoreCb->addItem("所有科目");  // 默认选项
    ui->class_scoreCb->addItem("高等数学");
    ui->class_scoreCb->addItem("大学物理");
    ui->class_scoreCb->addItem("程序设计");
    ui->class_scoreCb->addItem("数据结构");
    ui->class_scoreCb->addItem("线性代数");
    ui->class_scoreCb->addItem("离散数学");

    // 可选：设置默认选中项（第0项为"所有科目"）
    ui->class_scoreCb->setCurrentIndex(0);
    ui->major_scoreCb->clear();

    // 添加科目选项
    ui->major_scoreCb->addItem("所有科目");  // 默认选项
    ui->major_scoreCb->addItem("高等数学");
    ui->major_scoreCb->addItem("大学物理");
    ui->major_scoreCb->addItem("程序设计");
    ui->major_scoreCb->addItem("数据结构");
    ui->major_scoreCb->addItem("线性代数");
    ui->major_scoreCb->addItem("离散数学");

    // 可选：设置默认选中项（第0项为"所有科目"）
    ui->AnsCb->clear();

    // 添加科目选项
    ui->AnsCb->addItem("所有科目");  // 默认选项
    ui->AnsCb->addItem("高等数学");
    ui->AnsCb->addItem("大学物理");
    ui->AnsCb->addItem("程序设计");
    ui->AnsCb->addItem("数据结构");
    ui->AnsCb->addItem("线性代数");
    ui->AnsCb->addItem("离散数学");

    // 可选：设置默认选中项（第0项为"所有科目"）
    ui->class_scoreCb->setCurrentIndex(0);
    // ui->major_scoreCb->clear();

    // // 添加科目选项
    // ui->AssCb->addItem("计算机科学");
    // ui->AssCb->addItem("人工智能");
    // ui->AssCb->addItem("汉语言文学");
    ui->AssCb->clear();

    // 添加专业选项
    ui->AssCb->addItem("--请选择专业--");
    ui->AssCb->addItem("计算机科学");
    ui->AssCb->addItem("人工智能");
    ui->AssCb->addItem("汉语言文学");

    // 设置默认选中项
    ui->AssCb->setCurrentIndex(0);
}
void MainWindow::on_classBtn_released()
{
    // 获取班级输入（假设你有一个名为classEdit的QLineEdit用于输入班级）
    QString className = ui->classEdit->text().trimmed();
    if (className.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入班级名称");
        return;
    }

    // 获取科目选择（假设你有一个名为subjectCombo的QComboBox）
    QString subject = ui->subjectCombo->currentText();

    // 获取分数段（假设你有minScoreEdit和maxScoreEdit两个QLineEdit）
    double minScore = ui->minScoreEdit->text().toDouble();
    double maxScore = ui->maxScoreEdit->text().toDouble();

    // 执行筛选
    filterStudents("class", className, subject, minScore, maxScore);
    ui->stackedWidget->setCurrentWidget(ui->classpage);
}
// 专业查询按钮点击事件（假设专业信息在Student类中有major成员）
void MainWindow::on_obBtn_released()
{
    // 获取专业输入（假设你有一个名为majorEdit的QLineEdit）
    QString major = ui->majorEdit->text().trimmed();
    if (major.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入专业名称");
        return;
    }

    // 获取科目选择
    QString subject = ui->subjectCombo->currentText();

    // 获取分数段
    double minScore = ui->minScoreEdit->text().toDouble();
    double maxScore = ui->maxScoreEdit->text().toDouble();

    // 执行筛选
    filterStudents("major", major, subject, minScore, maxScore);
    ui->stackedWidget->setCurrentWidget(ui->classpage);
}
void MainWindow::setFilterTWHeaders(const QString& subject)
{
    QStringList headers;
    if (subject == "所有科目")
    {
        headers << "学号" << "姓名" << "性别" << "专业" << "班级" << "高等数学" << "大学物理" << "程序设计" << "数据结构" << "线性代数" << "离散数学";
    }
    else if (subject == "高等数学")
    {
        headers << "学号" << "姓名" << "性别" << "专业" << "班级" << "高等数学";
    }
    else if (subject == "大学物理")
    {
        headers << "学号" << "姓名" << "性别" << "专业" << "班级" << "大学物理";
    }
    else if (subject == "程序设计")
    {
        headers << "学号" << "姓名" << "性别" << "专业" << "班级" << "程序设计";
    }
    else if (subject == "数据结构")
    {
        headers << "学号" << "姓名" << "性别" << "专业" << "班级" << "数据结构";
    }
    else if (subject == "线性代数")
    {
        headers << "学号" << "姓名" << "性别" << "专业" << "班级" << "线性代数";
    }
    else if (subject == "离散数学")
    {
        headers << "学号" << "姓名" << "性别" << "专业" << "班级" << "离散数学";
    }
    // 其他科目同理
    ui->filterTW->setHorizontalHeaderLabels(headers);
}
void MainWindow::filterStudents(const QString &filterType, const QString &value,
                                const QString &subject, double minScore, double maxScore)
{
    // 清空筛选结果表格（假设筛选结果显示在filterTW表格中）
    ui->filterTW->setRowCount(0);
    ui->filterTW->setColumnCount(m_fields.size());
    //ui->filterTW->setHorizontalHeaderLabels(m_fields);

    int row = 0;
    for (auto s : m_students) {
        // 检查班级或专业是否匹配
        bool matchFilter = false;
        if (filterType == "class" && s->classname == value) {
            matchFilter = true;
        } else if (filterType == "major" && s->major == value) { // 假设Student有major成员
            matchFilter = true;
        }

        if (!matchFilter) continue;

        // 检查分数是否在指定范围内
        bool scoreInRange = false;
        if (subject == "所有科目" || subject.isEmpty()) {
            // 所有科目都要满足分数范围
            scoreInRange = (s->math >= minScore && s->math <= maxScore) &&
                           (s->cp >= minScore && s->cp <= maxScore) &&
                           (s->pd >= minScore && s->pd <= maxScore) &&
                           (s->ds >= minScore && s->ds <= maxScore) &&
                           (s->la >= minScore && s->la <= maxScore) &&
                           (s->dm >= minScore && s->dm <= maxScore);
        } else if (subject == "高等数学") {
            scoreInRange = (s->math >= minScore && s->math <= maxScore);
        } else if (subject == "大学物理") {
            scoreInRange = (s->cp >= minScore && s->cp <= maxScore);
        } else if (subject == "程序设计") {
            scoreInRange = (s->pd >= minScore && s->pd <= maxScore);
        } else if (subject == "数据结构") {
            scoreInRange = (s->ds >= minScore && s->ds <= maxScore);
        } else if (subject == "线性代数") {
            scoreInRange = (s->la >= minScore && s->la <= maxScore);
        } else if (subject == "离散数学") {
            scoreInRange = (s->dm >= minScore && s->ds <= maxScore);
        }
        setFilterTWHeaders(subject);
        if (scoreInRange&&subject == "所有科目") {
            // 添加到筛选结果表格
            ui->filterTW->insertRow(row);
            ui->filterTW->setItem(row, 0, new QTableWidgetItem(s->number));
            ui->filterTW->setItem(row, 1, new QTableWidgetItem(s->name));
            ui->filterTW->setItem(row, 2, new QTableWidgetItem(s->sex));
            ui->filterTW->setItem(row, 3, new QTableWidgetItem(s->major));
            ui->filterTW->setItem(row, 4, new QTableWidgetItem(s->classname));
            ui->filterTW->setItem(row, 5, new QTableWidgetItem(QString::number(s->math)));
            ui->filterTW->setItem(row, 6, new QTableWidgetItem(QString::number(s->cp)));
            ui->filterTW->setItem(row, 7, new QTableWidgetItem(QString::number(s->pd)));
            ui->filterTW->setItem(row, 8, new QTableWidgetItem(QString::number(s->ds)));
            ui->filterTW->setItem(row, 9, new QTableWidgetItem(QString::number(s->la)));
            ui->filterTW->setItem(row, 10, new QTableWidgetItem(QString::number(s->dm)));
            row++;
        } else if (scoreInRange&&subject == "高等数学") {
            ui->filterTW->insertRow(row);
            ui->filterTW->setItem(row, 0, new QTableWidgetItem(s->number));
            ui->filterTW->setItem(row, 1, new QTableWidgetItem(s->name));
            ui->filterTW->setItem(row, 2, new QTableWidgetItem(s->sex));
            ui->filterTW->setItem(row, 3, new QTableWidgetItem(s->major));
            ui->filterTW->setItem(row, 4, new QTableWidgetItem(s->classname));
            ui->filterTW->setItem(row, 5, new QTableWidgetItem(QString::number(s->math)));
        } else if (scoreInRange&&subject == "大学物理") {
            ui->filterTW->insertRow(row);
            ui->filterTW->setItem(row, 0, new QTableWidgetItem(s->number));
            ui->filterTW->setItem(row, 1, new QTableWidgetItem(s->name));
            ui->filterTW->setItem(row, 2, new QTableWidgetItem(s->sex));
            ui->filterTW->setItem(row, 3, new QTableWidgetItem(s->major));
            ui->filterTW->setItem(row, 4, new QTableWidgetItem(s->classname));
            ui->filterTW->setItem(row, 5, new QTableWidgetItem(QString::number(s->cp)));
        } else if (scoreInRange&&subject == "程序设计") {
            ui->filterTW->insertRow(row);
            ui->filterTW->setItem(row, 0, new QTableWidgetItem(s->number));
            ui->filterTW->setItem(row, 1, new QTableWidgetItem(s->name));
            ui->filterTW->setItem(row, 2, new QTableWidgetItem(s->sex));
            ui->filterTW->setItem(row, 3, new QTableWidgetItem(s->major));
            ui->filterTW->setItem(row, 4, new QTableWidgetItem(s->classname));
            ui->filterTW->setItem(row, 5, new QTableWidgetItem(QString::number(s->pd)));
        } else if (scoreInRange&&subject == "数据结构") {
            ui->filterTW->insertRow(row);
            ui->filterTW->setItem(row, 0, new QTableWidgetItem(s->number));
            ui->filterTW->setItem(row, 1, new QTableWidgetItem(s->name));
            ui->filterTW->setItem(row, 2, new QTableWidgetItem(s->sex));
            ui->filterTW->setItem(row, 3, new QTableWidgetItem(s->major));
            ui->filterTW->setItem(row, 4, new QTableWidgetItem(s->classname));
            ui->filterTW->setItem(row, 5, new QTableWidgetItem(QString::number(s->ds)));
        } else if (scoreInRange&&subject == "线性代数") {
            ui->filterTW->insertRow(row);
            ui->filterTW->setItem(row, 0, new QTableWidgetItem(s->number));
            ui->filterTW->setItem(row, 1, new QTableWidgetItem(s->name));
            ui->filterTW->setItem(row, 2, new QTableWidgetItem(s->sex));
            ui->filterTW->setItem(row, 3, new QTableWidgetItem(s->major));
            ui->filterTW->setItem(row, 4, new QTableWidgetItem(s->classname));
            ui->filterTW->setItem(row, 5, new QTableWidgetItem(QString::number(s->la)));
        } else if (scoreInRange&&subject == "离散数学") {
            ui->filterTW->insertRow(row);
            ui->filterTW->setItem(row, 0, new QTableWidgetItem(s->number));
            ui->filterTW->setItem(row, 1, new QTableWidgetItem(s->name));
            ui->filterTW->setItem(row, 2, new QTableWidgetItem(s->sex));
            ui->filterTW->setItem(row, 3, new QTableWidgetItem(s->major));
            ui->filterTW->setItem(row, 4, new QTableWidgetItem(s->classname));
            ui->filterTW->setItem(row, 5, new QTableWidgetItem(QString::number(s->dm)));
        }
    }

    // // 显示筛选结果数量
    // QMessageBox::information(this, "筛选结果",
    //                          QString("共找到 %1 条符合条件的记录").arg(row));
}
//------------------------------统计
void MainWindow::calculateStudentScores()
{
    for (auto s : m_students)
    {
        s->totalScore = s->math + s->cp + s->pd + s->ds + s->la + s->dm;
        s->averageScore = s->totalScore / 6.0;
    }
}
void MainWindow::rankStudentsByAverageScore(bool ascending)
{
    std::sort(m_students.begin(), m_students.end(), [ascending](const Student *a, const Student *b)
              {
                  if (ascending)
                      return a->averageScore < b->averageScore;//升
                  else
                      return a->averageScore > b->averageScore;
              });
}
void MainWindow::initRankTable()
{
    // 设置表格列数（排名、学号、姓名、班级、总成绩、平均分）
    ui->rankTable->setColumnCount(8);
    // 设置表头
    QStringList headers;
    headers << "排名" << "学号" << "姓名"<<"性别" <<"专业"<<"班级" << "总成绩" << "平均分";
    ui->rankTable->setHorizontalHeaderLabels(headers);
    // 自动调整列宽
    ui->rankTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
SubjectStats MainWindow::calculateSubjectStats(const QString &classOrMajor, const QString &filterValue, const QString &subject)
{
    SubjectStats stats = {0, -std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), 0, 0};
    int count = 0;
    for (auto s : m_students)
    {
        if ((classOrMajor == "class" && s->classname == filterValue) || (classOrMajor == "major" && s->major == filterValue))
        {
            double score = 0;
            if (subject == "高等数学") score = s->math;
            else if (subject == "大学物理") score = s->cp;
            else if (subject == "程序设计") score = s->pd;
            else if (subject == "数据结构") score = s->ds;
            else if (subject == "线性代数") score = s->la;
            else if (subject == "离散数学") score = s->dm;
            else if (subject == "所有科目")
            {
                score = s->math + s->cp + s->pd + s->ds + s->la + s->dm;
                score /= 6;
            }
            stats.average += score;
            stats.highest = std::max(stats.highest, score);
            stats.lowest = std::min(stats.lowest, score);
            if (score >= 60) stats.passCount++;
            if (score >= 90) stats.excellentCount++;
            count++;
        }
    }
    if (count > 0)
    {
        stats.average /= count;
    }
    return stats;
}
void MainWindow::updateRankTable()
{
    // 清空现有数据
    ui->rankTable->setRowCount(0);

    // 填充排序后的学生数据
    for (int i = 0; i < m_students.size(); ++i) {
        auto s = m_students[i];
        // 添加新行
        ui->rankTable->insertRow(i);

        // 设置排名（第i+1名）
        ui->rankTable->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
        // 学号
        ui->rankTable->setItem(i, 1, new QTableWidgetItem(s->number));
        // 姓名
        ui->rankTable->setItem(i, 2, new QTableWidgetItem(s->name));
        // 性别
        ui->rankTable->setItem(i, 3, new QTableWidgetItem(s->sex));
        // 专业
        ui->rankTable->setItem(i, 4, new QTableWidgetItem(s->major));
        // 班级
        ui->rankTable->setItem(i, 5, new QTableWidgetItem(s->classname));
        // 总成绩（保留1位小数）
        ui->rankTable->setItem(i, 6, new QTableWidgetItem(QString::number(s->totalScore, 'f', 1)));
        // 平均分（保留1位小数）
        ui->rankTable->setItem(i, 7, new QTableWidgetItem(QString::number(s->averageScore, 'f', 1)));
    }
}
void MainWindow::on_StatBtn_released()
{
    //切换到统计页面
    ui->stackedWidget->setCurrentWidget(ui->statpage);

   // rankStudentsByAverageScore(false);
}

void MainWindow::on_rankBtn_released()
{
    // 切换到页面
    ui->stackedWidget->setCurrentWidget(ui->rankpage);
    // 计算成绩并排名（降序，从高到低）
    calculateStudentScores();
    rankStudentsByAverageScore(false);  // false表示降序

    // 初始化表格并显示排名
    initRankTable();
    updateRankTable();
}

QVector<SubjectStats> MainWindow::calculateAllClassOrMajorSubjectStats(const QString& type, const QString& subject) {
    QVector<SubjectStats> stats;
    QMap<QString, QVector<double>> groupScores;
    // 根据类型（班级或专业）收集成绩
    for (auto s : m_students) {
        double score = 0;
        if (subject == "高等数学") score = s->math;
        else if (subject == "大学物理") score = s->cp;
        else if (subject == "程序设计") score = s->pd;
        else if (subject == "数据结构") score = s->ds;
        else if (subject == "线性代数") score = s->la;
        else if (subject == "离散数学") score = s->dm;
        else if (subject == "所有科目") {
            score = s->math + s->cp + s->pd + s->ds + s->la + s->dm;
            score /= 6;
        }
        QString groupKey = (type == "class") ? s->classname : s->major;
        groupScores[groupKey].append(score);
    }
    // 计算每个分组的统计信息
    for (auto it = groupScores.begin(); it != groupScores.end(); ++it) {
        SubjectStats stat;
        stat.groupName = it.key();  // 保存分组名称（班级/专业）
        stat.average = 0;
        stat.highest = -std::numeric_limits<double>::max();
        stat.lowest = std::numeric_limits<double>::max();
        stat.passCount = 0;
        stat.excellentCount = 0;
        int count = it.value().size();
        for (double score : it.value()) {
            stat.average += score;
            stat.highest = std::max(stat.highest, score);
            stat.lowest = std::min(stat.lowest, score);
            if (score >= 60) stat.passCount++;
            if (score >= 90) stat.excellentCount++;
        }
        if (count > 0) stat.average /= count;
        stats.append(stat);
    }
    return stats;
}

void MainWindow::on_class_scoreBtn_released()
{
    QString classname = ui->class_scoreEt->text().trimmed();
    if (classname.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入班级名称");
        return;
    }
    // // 获取科目选择
    QString subject = ui->class_scoreCb->currentText();

    SubjectStats Stats = calculateSubjectStats("class",classname, subject );
    ui->CM->setText(QString("班级：%1").arg(classname));
    ui->mathAverageLabel->setText(QString("平均分：%1").arg(Stats.average));
    ui->mathHighestLabel->setText(QString("最高分：%1").arg(Stats.highest));
    ui->mathLowestLabel->setText(QString("最低分：%1").arg(Stats.lowest));
    ui->mathPassRateLabel->setText(QString("及格率：%1%").arg((Stats.passCount / (double)m_students.size()) * 100));
    ui->mathExcellentRateLabel->setText(QString("优秀率：%1%").arg((Stats.excellentCount / (double)m_students.size()) * 100));
    //QString subject = ui->class_scoreCb->currentText();
    QVector<SubjectStats> stats = calculateAllClassOrMajorSubjectStats("class", subject);
    displayStatsToTable(stats);
    ui->stackedWidget->setCurrentWidget(ui->scorepage);
}


void MainWindow::on_major_scoreBtn_released()
{
    QString majorname = ui->major_scoreEt->text().trimmed();
    if (majorname.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入专业名称");
        return;
    }
    // 获取科目选择
    QString subject = ui->major_scoreCb->currentText();

    SubjectStats Stats = calculateSubjectStats("major",majorname, subject );
    ui->CM->setText(QString("班级：%1").arg(majorname));
    ui->mathAverageLabel->setText(QString("平均分：%1").arg(Stats.average));
    ui->mathHighestLabel->setText(QString("最高分：%1").arg(Stats.highest));
    ui->mathLowestLabel->setText(QString("最低分：%1").arg(Stats.lowest));
    ui->mathPassRateLabel->setText(QString("及格率：%1%").arg((Stats.passCount / (double)m_students.size()) * 100));
    ui->mathExcellentRateLabel->setText(QString("优秀率：%1%").arg((Stats.excellentCount / (double)m_students.size()) * 100));
    //QString subject = ui->major_scoreCb->currentText();
    QVector<SubjectStats> stats = calculateAllClassOrMajorSubjectStats("major", subject);
    displayStatsToTable(stats);
    ui->stackedWidget->setCurrentWidget(ui->scorepage);
}
bool MainWindow::exportStatsToText(const QVector<SubjectStats>& stats, const QString& type, const QString& subject) {
    QString defaultName = QString("%1_%2统计.txt").arg(subject).arg(type);
    QString filePath = QFileDialog::getSaveFileName(this, "导出统计结果", defaultName, "文本文件 (*.txt)");
    if (filePath.isEmpty()) return false;
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "错误", "无法打开文件进行写入");
        return false;
    }
    QTextStream out(&file);
    out << "===== " << subject << type << "成绩统计 =====" << "\n";
    out << "统计时间: " << QDateTime::currentDateTime().toString() << "\n\n";
    out << QString("%1\t%2\t%3\t%4\t%5\t%6\n")
               .arg("班级/专业", -10)
               .arg("平均分", -8)
               .arg("最高分", -8)
               .arg("最低分", -8)
               .arg("及格率(%)", -10)
               .arg("优秀率(%)", -10);
    for (const auto& stat : stats) {
        out << QString("%1\t%2\t%3\t%4\t%5\t%6\n")
        .arg(stat.groupName, -10)
            .arg(stat.average, -8, 'f', 1)
            .arg(stat.highest, -8, 'f', 1)
            .arg(stat.lowest, -8, 'f', 1)
            .arg(stat.passCount * 100.0 / m_students.size(), -10, 'f', 1)
            .arg(stat.excellentCount * 100.0 / m_students.size(), -10, 'f', 1);
    }
    file.close();
    return true;
}
void MainWindow::displayStatsToTable(const QVector<SubjectStats>& stats) {
    // 假设在Scorepage页面有一个QTableWidget用于显示统计结果，对象名为statsTable
    ui->statsTable->clearContents();
    ui->statsTable->setRowCount(0);
    QStringList headers = {"班级/专业", "平均分", "最高分", "最低分", "及格率(%)", "优秀率(%)"};
    ui->statsTable->setColumnCount(headers.size());
    ui->statsTable->setHorizontalHeaderLabels(headers);
    for (int i = 0; i < stats.size(); ++i) {
        const SubjectStats& stat = stats[i];
        ui->statsTable->insertRow(i);
        ui->statsTable->setItem(i, 0, new QTableWidgetItem(stat.groupName));
        ui->statsTable->setItem(i, 1, new QTableWidgetItem(QString::number(stat.average, 'f', 1)));
        ui->statsTable->setItem(i, 2, new QTableWidgetItem(QString::number(stat.highest, 'f', 1)));
        ui->statsTable->setItem(i, 3, new QTableWidgetItem(QString::number(stat.lowest, 'f', 1)));
        ui->statsTable->setItem(i, 4, new QTableWidgetItem(QString::number(stat.passCount * 100.0 / m_students.size(), 'f', 1)));
        ui->statsTable->setItem(i, 5, new QTableWidgetItem(QString::number(stat.excellentCount * 100.0 / m_students.size(), 'f', 1)));
    }
    ui->statsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::on_downloadBtn_released()
{
    QString type = (ui->class_scoreCb->isVisible()) ? "班级" : "专业";
    QString subject = (ui->class_scoreCb->isVisible()) ? ui->class_scoreCb->currentText() : ui->major_scoreCb->currentText();
    QVector<SubjectStats> stats = calculateAllClassOrMajorSubjectStats(type, subject);
    if (exportStatsToText(stats, type, subject)) {
        QMessageBox::information(this, "成功", "统计数据已导出为文本文件");
    }
}


void MainWindow::on_AssPb_released()
{
    // 获取选中的专业
    QString selectedMajor = ui->AssCb->currentText();

    // 验证选择
    if (selectedMajor == "--请选择专业--" || selectedMajor.isEmpty()) {
        QMessageBox::warning(this, "提示", "请先选择一个专业");}
        // 保存选中的专业
        m_selectedMajor = selectedMajor;

        // 跳转到mopage页面
        ui->stackedWidget->setCurrentWidget(ui->mopage);

        // 清空之前的奖学金表格数据
        ui->scholarshipTable->setRowCount(0);

        // 显示提示信息
        QMessageBox::information(this, "提示", QString("已选择专业: %1\n请点击'评定奖学金'按钮进行评定").arg(m_selectedMajor));
}
void MainWindow::rankStudentsByAverageScore2(bool ascending, QVector<Student*>& students)
    {
        std::sort(students.begin(), students.end(), [ascending](const Student *a, const Student *b)
                  {
                      if (ascending)
                          return a->averageScore < b->averageScore;
                      else
                          return a->averageScore > b->averageScore;
                  });
    }
void MainWindow::initScholarshipRules()
{
    m_scholarshipRules.clear();
    // 一等奖学金：平均分90+，专业前5%，无挂科
    m_scholarshipRules.append({"一等奖学金", 90.0, 0.05});
    // 二等奖学金：平均分85+，专业前10%，无挂科
    m_scholarshipRules.append({"二等奖学金", 85.0, 0.10});
    // 三等奖学金：平均分80+，专业前20%，无挂科
    m_scholarshipRules.append({"三等奖学金", 80.0, 0.20});
}

// 判断学生是否挂科（某科目<60分）
bool MainWindow::hasFailed(const Student* s)
{
    return s->math < 60 || s->cp < 60 || s->pd < 60 ||
           s->ds < 60 || s->la < 60 || s->dm < 60;
}


// 评定奖学金
void MainWindow::evaluateScholarship()
{
    // 检查是否选择了专业
    if (m_selectedMajor.isEmpty() || m_selectedMajor == "--请选择专业--") {
        QMessageBox::warning(this, "错误", "请先从专业下拉框选择专业");
        return;
    }

    // 1. 根据选择的专业筛选学生
    QVector<Student*> majorStudents;
    for (auto s : m_students) {
        if (s->major == m_selectedMajor) {
            majorStudents.push_back(s);
        }
    }

    // 检查该专业是否有学生
    if (majorStudents.isEmpty()) {
        QMessageBox::information(this, "结果", QString("专业 '%1' 没有找到学生数据").arg(m_selectedMajor));
        ui->scholarshipTable->setRowCount(0);
        return;
    }

    // 2. 按平均分降序排序
    rankStudentsByAverageScore2(false, majorStudents);

    // 3. 计算每个学生的排名百分比
    int totalCount = majorStudents.size();
    for (int i = 0; i < totalCount; ++i) {
        Student* s = majorStudents[i];
        s->rankPercent = (double)(i + 1) / totalCount; // 排名百分比
        s->scholarshipLevel = ""; // 清空之前的评定结果
    }

    // 4. 筛选符合奖学金条件的学生
    QVector<Student*> eligibleStudents;
    for (Student* s : majorStudents) {
        // 跳过挂科的学生
        if (hasFailed(s)) continue;

        // 检查是否符合奖学金条件
        for (const Scholarship& rule : m_scholarshipRules) {
            if (s->averageScore >= rule.minAvgScore && s->rankPercent <= rule.rankPercent) {
                s->scholarshipLevel = rule.level;
                eligibleStudents.append(s);
                break; // 符合高等级则不再检查低等级
            }
        }
    }
    // qDebug() << "专业学生数量：" << majorStudents.size();
    // for (auto s : majorStudents) {
    //     qDebug() << s->name << "平均分：" << s->averageScore;
    // }
    // 5. 显示结果到表格
    displayScholarshipResults(eligibleStudents);
}

// 在表格中显示奖学金评定结果
void MainWindow::displayScholarshipResults(const QVector<Student*>& students)
{
    // 清空现有数据
    ui->scholarshipTable->clearContents();
    ui->scholarshipTable->setRowCount(0);

    // 设置表格列数和表头
    ui->scholarshipTable->setColumnCount(6);
    QStringList headers = {"排名", "学号", "姓名", "班级", "平均分", "奖学金等级"};
    ui->scholarshipTable->setHorizontalHeaderLabels(headers);

    // 填充数据
    for (int i = 0; i < students.size(); ++i) {
        Student* s = students[i];
        ui->scholarshipTable->insertRow(i);

        // 排名
        ui->scholarshipTable->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
        // 学号
        ui->scholarshipTable->setItem(i, 1, new QTableWidgetItem(s->number));
        // 姓名
        ui->scholarshipTable->setItem(i, 2, new QTableWidgetItem(s->name));
        // 班级
        ui->scholarshipTable->setItem(i, 3, new QTableWidgetItem(s->classname));
        // 平均分（保留1位小数）
        ui->scholarshipTable->setItem(i, 4, new QTableWidgetItem(QString::number(s->averageScore, 'f', 1)));
        // 奖学金等级
        ui->scholarshipTable->setItem(i, 5, new QTableWidgetItem(s->scholarshipLevel));
    }

    // 设置列宽自适应
    ui->scholarshipTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QVector<Student*> majorStudents;
    for (auto s : m_students) {
        if (s->major == m_selectedMajor) {
            majorStudents.push_back(s);
        }
    }

    // 显示统计信息
    QMessageBox::information(this, "评定完成",
                             QString("专业: %1\n专业总人数: %2\n专业获奖人数: %3")
                                 .arg(m_selectedMajor)
                                 .arg(majorStudents.size())
                                 .arg(students.size()));
}

// 导出奖学金评定结果
void MainWindow::exportScholarshipResult()
{
    // 检查是否有数据可导出
    if (ui->scholarshipTable->rowCount() == 0) {
        QMessageBox::warning(this, "错误", "没有可导出的奖学金评定结果");
        return;
    }

    // 选择保存路径
    QString defaultFileName = QString("%1_奖学金评定结果.txt").arg(m_selectedMajor);
    QString filePath = QFileDialog::getSaveFileName(
        this, "导出奖学金结果", defaultFileName, "文本文件 (*.txt);;Excel文件 (*.csv)"
        );

    if (filePath.isEmpty()) return;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "错误", "无法打开文件进行写入");
        return;
    }

    QTextStream out(&file);
    // 写入文件头
    out << QString("专业: %1 奖学金评定结果\n").arg(m_selectedMajor);
    out << QString("导出时间: %1\n\n").arg(QDateTime::currentDateTime().toString());

    // 写入表头
    out << "排名\t学号\t姓名\t班级\t平均分\t奖学金等级\n";

    // 写入数据
    for (int i = 0; i < ui->scholarshipTable->rowCount(); ++i) {
        out << QString("%1\t%2\t%3\t%4\t%5\t%6\n")
        .arg(ui->scholarshipTable->item(i, 0)->text())
            .arg(ui->scholarshipTable->item(i, 1)->text())
            .arg(ui->scholarshipTable->item(i, 2)->text())
            .arg(ui->scholarshipTable->item(i, 3)->text())
            .arg(ui->scholarshipTable->item(i, 4)->text())
            .arg(ui->scholarshipTable->item(i, 5)->text());
    }

    file.close();
    QMessageBox::information(this, "成功", QString("奖学金结果已导出至:\n%1").arg(filePath));
}

// void MainWindow::on_AnsBtn_released()
// {

// }

