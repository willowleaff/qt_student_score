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
#include <QAxObject>
#include <limits>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_plot(nullptr)
    , m_pieChart(nullptr)
    , m_selectedMajor("")
{
    ui->setupUi(this);
    initStatPageLayout();
    initSubjectCombo();
    loadData("student.txt");
    calculateStudentScores();
    initScholarshipRules();

    // 关联按钮事件
    connect(ui->evalScholarshipBtn, &QPushButton::released, this, &MainWindow::evaluateScholarship);
    connect(ui->exportScholarshipBtn, &QPushButton::released, this, &MainWindow::exportScholarshipResult);
}

MainWindow::~MainWindow()
{
    // 释放学生对象内存
    for (auto s : m_students) {
        delete s;
    }
    delete ui;
}

// 学生类构造函数
Student::Student() : totalScore(0), averageScore(0), rankPercent(0)
{
    // 初始化6个科目，可根据需要扩展
    scores.resize(6);
}

// 显示学生信息
void Student::display()
{
    qDebug() << "学号:" << number
             << "姓名:" << name
             << "专业:" << major;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Escape)
    {
        // 回到菜单页面
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
    m_fields = stream.readLine().split("\t", Qt::SkipEmptyParts);

    // 清除现有数据
    for (auto s : m_students) {
        delete s;
    }
    m_students.clear();

    while (!stream.atEnd())
    {
        QString line = stream.readLine();
        if (line.isEmpty()) continue;

        auto lineData = line.split("\t", Qt::SkipEmptyParts);
        if (lineData.size() >= 11) // 确保有足够的数据
        {
            auto s = new Student;
            s->number   = lineData[0];
            s->name     = lineData[1];
            s->sex      = lineData[2];
            s->major    = lineData[3];
            s->classname= lineData[4];

            // 读取科目成绩到数组
            for (int i = 0; i < 6 && (5 + i) < lineData.size(); ++i) {
                s->scores[i] = lineData[5 + i].toDouble();
            }

            m_students.push_back(s);
        }
    }

    for(auto s : m_students) {
        s->display();
    }
}

void MainWindow::on_LookBtn_released()
{
    ui->stackedWidget->setCurrentWidget(ui->lookpage);
    ui->LookTW->setRowCount(0);
    ui->LookTW->setColumnCount(m_fields.size());
    ui->LookTW->setHorizontalHeaderLabels(m_fields);

    for(auto s : m_students)
    {
        int row = ui->LookTW->rowCount();
        ui->LookTW->insertRow(row);

        ui->LookTW->setItem(row, 0, new QTableWidgetItem(s->number));
        ui->LookTW->setItem(row, 1, new QTableWidgetItem(s->name));
        ui->LookTW->setItem(row, 2, new QTableWidgetItem(s->sex));
        ui->LookTW->setItem(row, 3, new QTableWidgetItem(s->major));
        ui->LookTW->setItem(row, 4, new QTableWidgetItem(s->classname));

        // 从数组中读取科目成绩
        for (int i = 0; i < s->scores.size(); ++i) {
            if (5 + i < ui->LookTW->columnCount()) {
                ui->LookTW->setItem(row, 5 + i, new QTableWidgetItem(QString::number(s->scores[i])));
            }
        }
    }
}

void MainWindow::saveData(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "错误", "无法打开文件保存");
        return;
    }

    QTextStream stream(&file);
    stream << m_fields.join("\t") << "\n";

    for (auto s : m_students) {
        stream << s->number << "\t"
               << s->name << "\t"
               << s->sex << "\t"
               << s->major << "\t"
               << s->classname;

        // 从数组中保存科目成绩
        for (qreal score : s->scores) {
            stream << "\t" << score;
        }
        stream << "\n";
    }

    QMessageBox::information(this, "提示", "数据已保存到文本文件");
}

void MainWindow::initStatPageLayout()
{
    QVBoxLayout *asLayout = qobject_cast<QVBoxLayout*>(ui->aspage->layout());
    if (!asLayout) {
        asLayout = new QVBoxLayout(ui->aspage);
        asLayout->setContentsMargins(10, 10, 10, 10);
        asLayout->setSpacing(10);
    }

    QHBoxLayout *chartsLayout = new QHBoxLayout();
    chartsLayout->setSpacing(20);

    m_plot = new NormalDistributionPlot();
    m_plot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chartsLayout->addWidget(m_plot);

    m_pieChart = new ScorePieChart();
    m_pieChart->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chartsLayout->addWidget(m_pieChart);

    asLayout->addLayout(chartsLayout);
}

void MainWindow::on_QuitBtn_released()
{
    close();
}

void MainWindow::on_addBtn_released()
{
    ui->LookTW->insertRow(ui->LookTW->rowCount());
    ui->LookTW->scrollToBottom();
    m_students.push_back(new Student());
}

void MainWindow::on_saveBtn_released()
{
    for(int i = 0; i < ui->LookTW->rowCount(); ++i)
    {
        if (i >= m_students.size()) break;

        Student* s = m_students[i];
        s->number = ui->LookTW->item(i, 0)->text();
        s->name = ui->LookTW->item(i, 1)->text();
        s->sex = ui->LookTW->item(i, 2)->text();
        s->major = ui->LookTW->item(i, 3)->text();
        s->classname = ui->LookTW->item(i, 4)->text();

        // 保存科目成绩到数组
        for (int j = 0; j < s->scores.size(); ++j) {
            if (5 + j < ui->LookTW->columnCount() && ui->LookTW->item(i, 5 + j)) {
                s->scores[j] = ui->LookTW->item(i, 5 + j)->text().toDouble();
            }
        }
    }

    QMessageBox::information(this, "保存提示", "保存成功");
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

    // 科目索引映射：0-高等数学，1-大学物理，2-程序设计，3-数据结构，4-线性代数，5-离散数学
    QMap<QString, int> subjectIndex = {
        {"高等数学", 0}, {"大学物理", 1}, {"程序设计", 2},
        {"数据结构", 3}, {"线性代数", 4}, {"离散数学", 5}
    };

    if (subject == "所有科目") {
        for (auto s : m_students) {
            double total = 0;
            for (qreal score : s->scores) {
                total += score;
            }
            scores.append(total);
        }
    } else if (subjectIndex.contains(subject)) {
        int idx = subjectIndex[subject];
        for (auto s : m_students) {
            if (idx < s->scores.size()) {
                scores.append(s->scores[idx]);
            }
        }
    }

    m_plot->setScores(scores);
    m_pieChart->setScores(scores);
    ui->stackedWidget->setCurrentWidget(ui->aspage);
}

void MainWindow::on_MoBtn_released()
{
    ui->stackedWidget->setCurrentWidget(ui->Asspage);
}

void MainWindow::on_LSBtn_released()
{
    ui->stackedWidget->setCurrentWidget(ui->choopage);
}

void MainWindow::initSubjectCombo()
{
    ui->subjectCombo->clear();
    ui->subjectCombo->addItem("所有科目");
    ui->subjectCombo->addItem("高等数学");
    ui->subjectCombo->addItem("大学物理");
    ui->subjectCombo->addItem("程序设计");
    ui->subjectCombo->addItem("数据结构");
    ui->subjectCombo->addItem("线性代数");
    ui->subjectCombo->addItem("离散数学");
    ui->subjectCombo->setCurrentIndex(0);

    ui->class_scoreCb->clear();
    ui->class_scoreCb->addItem("所有科目");
    ui->class_scoreCb->addItem("高等数学");
    ui->class_scoreCb->addItem("大学物理");
    ui->class_scoreCb->addItem("程序设计");
    ui->class_scoreCb->addItem("数据结构");
    ui->class_scoreCb->addItem("线性代数");
    ui->class_scoreCb->addItem("离散数学");
    ui->class_scoreCb->setCurrentIndex(0);

    ui->major_scoreCb->clear();
    ui->major_scoreCb->addItem("所有科目");
    ui->major_scoreCb->addItem("高等数学");
    ui->major_scoreCb->addItem("大学物理");
    ui->major_scoreCb->addItem("程序设计");
    ui->major_scoreCb->addItem("数据结构");
    ui->major_scoreCb->addItem("线性代数");
    ui->major_scoreCb->addItem("离散数学");
    ui->major_scoreCb->setCurrentIndex(0);

    ui->AnsCb->clear();
    ui->AnsCb->addItem("所有科目");
    ui->AnsCb->addItem("高等数学");
    ui->AnsCb->addItem("大学物理");
    ui->AnsCb->addItem("程序设计");
    ui->AnsCb->addItem("数据结构");
    ui->AnsCb->addItem("线性代数");
    ui->AnsCb->addItem("离散数学");
    ui->AnsCb->setCurrentIndex(0);

    ui->AssCb->clear();
    ui->AssCb->addItem("--请选择专业--");
    ui->AssCb->addItem("计算机科学与技术");
    ui->AssCb->addItem("软件工程");
    ui->AssCb->addItem("电子信息工程");
    ui->AssCb->addItem("通信工程");
    ui->AssCb->addItem("自动化");
    ui->AssCb->addItem("物联网工程");
    ui->AssCb->addItem("数据科学与大数据技术");
    ui->AssCb->addItem("人工智能");
    ui->AssCb->addItem("网络工程");
     ui->AssCb->addItem("信息安全");
    ui->AssCb->setCurrentIndex(0);
}

void MainWindow::on_classBtn_released()
{
    QString className = ui->classEdit->text().trimmed();
    if (className.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入班级名称");
        return;
    }

    QString subject = ui->subjectCombo->currentText();
    double minScore = ui->minScoreEdit->text().toDouble();
    double maxScore = ui->maxScoreEdit->text().toDouble();

    filterStudents("class", className, subject, minScore, maxScore);
    ui->stackedWidget->setCurrentWidget(ui->classpage);
}

void MainWindow::on_obBtn_released()
{
    QString major = ui->majorEdit->text().trimmed();
    if (major.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入专业名称");
        return;
    }

    QString subject = ui->subjectCombo->currentText();
    double minScore = ui->minScoreEdit->text().toDouble();
    double maxScore = ui->maxScoreEdit->text().toDouble();

    filterStudents("major", major, subject, minScore, maxScore);
    ui->stackedWidget->setCurrentWidget(ui->classpage);
}

void MainWindow::setFilterTWHeaders(const QString& subject)
{
    QStringList headers = {"学号", "姓名", "性别", "专业", "班级"};

    if (subject == "所有科目") {
        headers << "高等数学" << "大学物理" << "程序设计" << "数据结构" << "线性代数" << "离散数学";
    } else {
        headers << subject; // 单个科目时只添加选中的科目
    }

    // 先设置列数，再设置表头（关键修复）
    ui->filterTW->setColumnCount(headers.size());
    ui->filterTW->setHorizontalHeaderLabels(headers);
}

void MainWindow::filterStudents(const QString &filterType, const QString &value,
                                const QString &subject, double minScore, double maxScore)
{
    // 清除原有数据和表头
    ui->filterTW->setRowCount(0);
   // ui->filterTW->clearHeaders(); // 清除旧表头

    // 根据科目设置新表头（包含列数设置）
    setFilterTWHeaders(subject);

    QMap<QString, int> subjectIndex = {
        {"高等数学", 0}, {"大学物理", 1}, {"程序设计", 2},
        {"数据结构", 3}, {"线性代数", 4}, {"离散数学", 5}
    };

    int row = 0;
    for (auto s : m_students) {
        bool matchFilter = false;
        if (filterType == "class" && s->classname == value) {
            matchFilter = true;
        } else if (filterType == "major" && s->major == value) {
            matchFilter = true;
        }

        if (!matchFilter) continue;

        bool scoreInRange = false;
        if (subject == "所有科目") {
            scoreInRange = true;
            for (qreal score : s->scores) {
                if (!(score >= minScore && score <= maxScore)) {
                    scoreInRange = false;
                    break;
                }
            }
        } else if (subjectIndex.contains(subject)) {
            int idx = subjectIndex[subject];
            if (idx < s->scores.size()) {
                qreal score = s->scores[idx];
                scoreInRange = (score >= minScore && score <= maxScore);
            }
        }

        if (scoreInRange) {
            ui->filterTW->insertRow(row);

            // 基础信息列（0-4列）
            ui->filterTW->setItem(row, 0, new QTableWidgetItem(s->number));
            ui->filterTW->setItem(row, 1, new QTableWidgetItem(s->name));
            ui->filterTW->setItem(row, 2, new QTableWidgetItem(s->sex));
            ui->filterTW->setItem(row, 3, new QTableWidgetItem(s->major));
            ui->filterTW->setItem(row, 4, new QTableWidgetItem(s->classname));

            // 成绩列（从第5列开始）
            if (subject == "所有科目") {
                for (int i = 0; i < s->scores.size(); ++i) {
                    // 确保不超过表格实际列数（避免越界）
                    if (5 + i < ui->filterTW->columnCount()) {
                        ui->filterTW->setItem(row, 5 + i, new QTableWidgetItem(QString::number(s->scores[i])));
                    }
                }
            } else if (subjectIndex.contains(subject)) {
                int idx = subjectIndex[subject];
                ui->filterTW->setItem(row, 5, new QTableWidgetItem(QString::number(s->scores[idx])));
            }

            row++;
        }
    }

    // 自动调整列宽以显示完整表头和内容
    ui->filterTW->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    // 强制刷新表格
    ui->filterTW->updateGeometry();
    ui->filterTW->viewport()->repaint();
}

void MainWindow::calculateStudentScores()
{
    for (auto s : m_students) {
        s->totalScore = 0;
        for (qreal score : s->scores) {
            s->totalScore += score;
        }
        s->averageScore = s->scores.isEmpty() ? 0 : s->totalScore / s->scores.size();
    }
}

void MainWindow::rankStudentsByAverageScore(bool ascending)
{
    std::sort(m_students.begin(), m_students.end(), [ascending](const Student *a, const Student *b)
              {
                  return ascending ? a->averageScore < b->averageScore : a->averageScore > b->averageScore;
              });
}

void MainWindow::initRankTable()
{
    ui->rankTable->setColumnCount(8);
    QStringList headers = {"排名", "学号", "姓名", "性别", "专业", "班级", "总成绩", "平均分"};
    ui->rankTable->setHorizontalHeaderLabels(headers);
    ui->rankTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
SubjectStats MainWindow::calculateSubjectStats(const QString &classOrMajor, const QString &filterValue, const QString &subject)
{
    SubjectStats stats = {filterValue, 0, -std::numeric_limits<double>::max(),
                          std::numeric_limits<double>::max(), 0, 0};
    int count = 0;

    QMap<QString, int> subjectIndex = {
        {"高等数学", 0}, {"大学物理", 1}, {"程序设计", 2},
        {"数据结构", 3}, {"线性代数", 4}, {"离散数学", 5}
    };

    for (auto s : m_students) {
        if ((classOrMajor == "class" && s->classname == filterValue) ||
            (classOrMajor == "major" && s->major == filterValue)) {

            double score = 0;
            if (subject == "所有科目") {
                double total = 0;
                for (qreal s : s->scores) total += s;
                score = s->scores.isEmpty() ? 0 : total / s->scores.size();
            } else if (subjectIndex.contains(subject)) {
                int idx = subjectIndex[subject];
                if (idx < s->scores.size()) {
                    score = s->scores[idx];
                }
            }

            stats.average += score;
            stats.highest = std::max(stats.highest, score);
            stats.lowest = std::min(stats.lowest, score);
            if (score >= 60) stats.passCount++;
            if (score >= 90) stats.excellentCount++;
            count++;
        }
    }

    if (count > 0) {
        stats.average /= count;
    }
    return stats;
}

void MainWindow::updateRankTable()
{
    ui->rankTable->setRowCount(0);

    for (int i = 0; i < m_students.size(); ++i) {
        auto s = m_students[i];
        ui->rankTable->insertRow(i);

        ui->rankTable->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
        ui->rankTable->setItem(i, 1, new QTableWidgetItem(s->number));
        ui->rankTable->setItem(i, 2, new QTableWidgetItem(s->name));
        ui->rankTable->setItem(i, 3, new QTableWidgetItem(s->sex));
        ui->rankTable->setItem(i, 4, new QTableWidgetItem(s->major));
        ui->rankTable->setItem(i, 5, new QTableWidgetItem(s->classname));
        ui->rankTable->setItem(i, 6, new QTableWidgetItem(QString::number(s->totalScore, 'f', 1)));
        ui->rankTable->setItem(i, 7, new QTableWidgetItem(QString::number(s->averageScore, 'f', 1)));
    }
}

void MainWindow::on_StatBtn_released()
{
    ui->stackedWidget->setCurrentWidget(ui->statpage);
}

void MainWindow::on_rankBtn_released()
{
    ui->stackedWidget->setCurrentWidget(ui->rankpage);
    calculateStudentScores();
    rankStudentsByAverageScore(false);
    initRankTable();
    updateRankTable();
}

QVector<SubjectStats> MainWindow::calculateAllClassOrMajorSubjectStats(const QString& type, const QString& subject) {
    QVector<SubjectStats> stats;
    QMap<QString, QVector<double>> groupScores;

    QMap<QString, int> subjectIndex = {
        {"高等数学", 0}, {"大学物理", 1}, {"程序设计", 2},
        {"数据结构", 3}, {"线性代数", 4}, {"离散数学", 5}
    };

    for (auto s : m_students) {
        double score = 0;
        if (subject == "所有科目") {
            double total = 0;
            for (qreal s : s->scores) total += s;
            score = s->scores.isEmpty() ? 0 : total / s->scores.size();
        } else if (subjectIndex.contains(subject)) {
            int idx = subjectIndex[subject];
            if (idx < s->scores.size()) {
                score = s->scores[idx];
            }
        }

        QString groupKey = (type == "class") ? s->classname : s->major;
        groupScores[groupKey].append(score);
    }

    for (auto it = groupScores.begin(); it != groupScores.end(); ++it) {
        SubjectStats stat;
        stat.groupName = it.key();
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

    QString subject = ui->class_scoreCb->currentText();
    SubjectStats stats = calculateSubjectStats("class", classname, subject);

    ui->CM->setText(QString("班级：%1").arg(classname));
    ui->mathAverageLabel->setText(QString("平均分：%1").arg(stats.average));
    ui->mathHighestLabel->setText(QString("最高分：%1").arg(stats.highest));
    ui->mathLowestLabel->setText(QString("最低分：%1").arg(stats.lowest));

    int totalInGroup = 0;
    for (auto s : m_students) {
        if (s->classname == classname) totalInGroup++;
    }

    double passRate = totalInGroup > 0 ? (stats.passCount * 100.0) / totalInGroup : 0;
    double excellentRate = totalInGroup > 0 ? (stats.excellentCount * 100.0) / totalInGroup : 0;

    ui->mathPassRateLabel->setText(QString("及格率：%1%").arg(passRate));
    ui->mathExcellentRateLabel->setText(QString("优秀率：%1%").arg(excellentRate));

    QVector<SubjectStats> allStats = calculateAllClassOrMajorSubjectStats("class", subject);
    displayStatsToTable(allStats);
    ui->stackedWidget->setCurrentWidget(ui->scorepage);
}

void MainWindow::on_major_scoreBtn_released()
{
    QString majorname = ui->major_scoreEt->text().trimmed();
    if (majorname.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入专业名称");
        return;
    }

    QString subject = ui->major_scoreCb->currentText();
    SubjectStats stats = calculateSubjectStats("major", majorname, subject);

    ui->CM->setText(QString("专业：%1").arg(majorname));
    ui->mathAverageLabel->setText(QString("平均分：%1").arg(stats.average));
    ui->mathHighestLabel->setText(QString("最高分：%1").arg(stats.highest));
    ui->mathLowestLabel->setText(QString("最低分：%1").arg(stats.lowest));

    int totalInGroup = 0;
    for (auto s : m_students) {
        if (s->major == majorname) totalInGroup++;
    }

    double passRate = totalInGroup > 0 ? (stats.passCount * 100.0) / totalInGroup : 0;
    double excellentRate = totalInGroup > 0 ? (stats.excellentCount * 100.0) / totalInGroup : 0;

    ui->mathPassRateLabel->setText(QString("及格率：%1%").arg(passRate));
    ui->mathExcellentRateLabel->setText(QString("优秀率：%1%").arg(excellentRate));

    QVector<SubjectStats> allStats = calculateAllClassOrMajorSubjectStats("major", subject);
    displayStatsToTable(allStats);
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
        int totalInGroup = 0;
        for (auto s : m_students) {
            if ((type == "班级" && s->classname == stat.groupName) ||
                (type == "专业" && s->major == stat.groupName)) {
                totalInGroup++;
            }
        }

        double passRate = totalInGroup > 0 ? (stat.passCount * 100.0) / totalInGroup : 0;
        double excellentRate = totalInGroup > 0 ? (stat.excellentCount * 100.0) / totalInGroup : 0;

        out << QString("%1\t%2\t%3\t%4\t%5\t%6\n")
                   .arg(stat.groupName, -10)
                   .arg(stat.average, -8, 'f', 1)
                   .arg(stat.highest, -8, 'f', 1)
                   .arg(stat.lowest, -8, 'f', 1)
                   .arg(passRate, -10, 'f', 1)
                   .arg(excellentRate, -10, 'f', 1);
    }

    file.close();
    return true;
}

void MainWindow::displayStatsToTable(const QVector<SubjectStats>& stats) {
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

        int totalInGroup = 0;
        for (auto s : m_students) {
            if (s->classname == stat.groupName || s->major == stat.groupName) {
                totalInGroup++;
            }
        }

        double passRate = totalInGroup > 0 ? (stat.passCount * 100.0) / totalInGroup : 0;
        double excellentRate = totalInGroup > 0 ? (stat.excellentCount * 100.0) / totalInGroup : 0;

        ui->statsTable->setItem(i, 4, new QTableWidgetItem(QString::number(passRate, 'f', 1)));
        ui->statsTable->setItem(i, 5, new QTableWidgetItem(QString::number(excellentRate, 'f', 1)));
    }

    ui->statsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::on_downloadBtn_released()
{
    QString type = (ui->class_scoreCb->isVisible()) ? "班级" : "专业";
    QString subject = (ui->class_scoreCb->isVisible()) ? ui->class_scoreCb->currentText() : ui->major_scoreCb->currentText();
    QVector<SubjectStats> stats = calculateAllClassOrMajorSubjectStats(type == "班级" ? "class" : "major", subject);

    if (exportTableToExcel(ui->statsTable, "班级/专业成绩信息表.xlsx")) {
        QMessageBox::information(this, "成功", "统计数据已导出为文本文件");
    }
}

void MainWindow::on_AssPb_released()
{
    QString selectedMajor = ui->AssCb->currentText();
    if (selectedMajor == "--请选择专业--" || selectedMajor.isEmpty()) {
        QMessageBox::warning(this, "提示", "请先选择一个专业");
        return;
    }

    m_selectedMajor = selectedMajor;
    ui->stackedWidget->setCurrentWidget(ui->mopage);
    ui->scholarshipTable->setRowCount(0);
    QMessageBox::information(this, "提示", QString("已选择专业: %1\n请点击'评定奖学金'按钮进行评定").arg(m_selectedMajor));
}

void MainWindow::rankStudentsByAverageScore2(bool ascending, QVector<Student*>& students)
{
    std::sort(students.begin(), students.end(), [ascending](const Student *a, const Student *b)
              {
                  return ascending ? a->averageScore < b->averageScore : a->averageScore > b->averageScore;
              });
}

void MainWindow::initScholarshipRules()
{
    m_scholarshipRules.clear();
    m_scholarshipRules.append({"一等奖学金", 80.0, 0.05});
    m_scholarshipRules.append({"二等奖学金", 75.0, 0.10});
    m_scholarshipRules.append({"三等奖学金", 70.0, 0.20});
}

bool MainWindow::hasFailed(const Student* s)
{
    for (qreal score : s->scores) {
        if (score < 60) return true;
    }
    return false;
}

void MainWindow::evaluateScholarship()
{
    if (m_selectedMajor.isEmpty() || m_selectedMajor == "--请选择专业--") {
        QMessageBox::warning(this, "错误", "请先从专业下拉框选择专业");
        return;
    }

    QVector<Student*> majorStudents;
    for (auto s : m_students) {
        if (s->major == m_selectedMajor) {
            majorStudents.push_back(s);
        }
    }

    if (majorStudents.isEmpty()) {
        QMessageBox::information(this, "结果", QString("专业 '%1' 没有找到学生数据").arg(m_selectedMajor));
        ui->scholarshipTable->setRowCount(0);
        return;
    }

    rankStudentsByAverageScore2(false, majorStudents);

    int totalCount = majorStudents.size();
    for (int i = 0; i < totalCount; ++i) {
        Student* s = majorStudents[i];
        s->rankPercent = (double)(i + 1) / totalCount;
        s->scholarshipLevel = "";
    }

    QVector<Student*> eligibleStudents;
    for (Student* s : majorStudents) {
        if (hasFailed(s)) continue;

        for (const Scholarship& rule : m_scholarshipRules) {
            if (s->averageScore >= rule.minAvgScore && s->rankPercent <= rule.rankPercent) {
                s->scholarshipLevel = rule.level;
                eligibleStudents.append(s);
                break;
            }
        }
    }

    displayScholarshipResults(eligibleStudents);
}

void MainWindow::displayScholarshipResults(const QVector<Student*>& students)
{
    ui->scholarshipTable->clearContents();
    ui->scholarshipTable->setRowCount(0);

    ui->scholarshipTable->setColumnCount(6);
    QStringList headers = {"排名", "学号", "姓名", "班级", "平均分", "奖学金等级"};
    ui->scholarshipTable->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < students.size(); ++i) {
        Student* s = students[i];
        ui->scholarshipTable->insertRow(i);

        ui->scholarshipTable->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
        ui->scholarshipTable->setItem(i, 1, new QTableWidgetItem(s->number));
        ui->scholarshipTable->setItem(i, 2, new QTableWidgetItem(s->name));
        ui->scholarshipTable->setItem(i, 3, new QTableWidgetItem(s->classname));
        ui->scholarshipTable->setItem(i, 4, new QTableWidgetItem(QString::number(s->averageScore, 'f', 1)));
        ui->scholarshipTable->setItem(i, 5, new QTableWidgetItem(s->scholarshipLevel));
    }

    ui->scholarshipTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QVector<Student*> majorStudents;
    for (auto s : m_students) {
        if (s->major == m_selectedMajor) {
            majorStudents.push_back(s);
        }
    }

    QMessageBox::information(this, "评定完成",
                             QString("专业: %1\n专业总人数: %2\n专业获奖人数: %3")
                                 .arg(m_selectedMajor)
                                 .arg(majorStudents.size())
                                 .arg(students.size()));
}

void MainWindow::exportScholarshipResult()
{
    if (ui->scholarshipTable->rowCount() == 0) {
        QMessageBox::warning(this, "错误", "没有可导出的奖学金评定结果");
        return;
    }

    QString defaultFileName = QString("%1_奖学金评定结果.xlsx").arg(m_selectedMajor);
    QString filePath = QFileDialog::getSaveFileName(
        this, "导出奖学金结果", defaultFileName, "Excel文件 (*.xlsx);;Excel 97-2003文件 (*.xls);;所有文件 (*)"
        );

    if (filePath.isEmpty()) return;

    QAxObject excel("Excel.Application");
    if (excel.isNull()) {
        QMessageBox::critical(this, "错误", "未检测到Excel，请先安装Microsoft Excel");
        return;
    }

    excel.setProperty("Visible", false);
    excel.setProperty("DisplayAlerts", false);

    QAxObject *workBooks = excel.querySubObject("Workbooks");
    QAxObject *workBook = workBooks->querySubObject("Add");
    QAxObject *workSheet = workBook->querySubObject("Worksheets(int)", 1);
    workSheet->setProperty("Name", "奖学金评定结果");

    int row = 1;
    QAxObject *cell = workSheet->querySubObject("Cells(int, int)", row, 1);
    cell->setProperty("Value", QString("专业: %1 奖学金评定结果").arg(m_selectedMajor));
    cell->querySubObject("Font")->setProperty("Bold", true);
    cell->querySubObject("Font")->setProperty("Size", 14);
    workSheet->querySubObject("Range(QString)", "A1:F1")->setProperty("MergeCells", true);

    row++;
    cell = workSheet->querySubObject("Cells(int, int)", row, 1);
    cell->setProperty("Value", QString("导出时间: %1").arg(QDateTime::currentDateTime().toString()));
    workSheet->querySubObject("Range(QString)", "A2:F2")->setProperty("MergeCells", true);

    row++;
    row++;
    QStringList headers = {"排名", "学号", "姓名", "班级", "平均分", "奖学金等级"};
    for (int col = 0; col < headers.size(); ++col) {
        cell = workSheet->querySubObject("Cells(int, int)", row, col + 1);
        cell->setProperty("Value", headers[col]);
        cell->querySubObject("Font")->setProperty("Bold", true);
        cell->querySubObject("Interior")->setProperty("Color", QColor(200, 200, 200));
    }

    for (int i = 0; i < ui->scholarshipTable->rowCount(); ++i) {
        row++;
        for (int j = 0; j < ui->scholarshipTable->columnCount(); ++j) {
            cell = workSheet->querySubObject("Cells(int, int)", row, j + 1);
            cell->setProperty("Value", ui->scholarshipTable->item(i, j)->text());

            if (j == 4) {
                cell->setProperty("NumberFormat", "0.00");
            }
        }
    }

    workSheet->querySubObject("UsedRange")->dynamicCall("Columns.AutoFit()");

    bool saveSuccess = workBook->dynamicCall("SaveAs(const QString&)",
                                             QDir::toNativeSeparators(filePath)).toBool();
    if (!saveSuccess) {
        QMessageBox::critical(this, "错误", "保存文件失败");
    }

    workBook->dynamicCall("Close()");
    excel.dynamicCall("Quit()");

    delete workSheet;
    delete workBook;
    delete workBooks;

    if (saveSuccess) {
        QMessageBox::information(this, "成功", QString("奖学金结果已导出至:\n%1").arg(filePath));
    }
}

void MainWindow::on_numBtn_released()
{
    // 获取输入的学号
    QString targetNumber = ui->numEt->text().trimmed();

    // 检查输入是否为空
    if (targetNumber.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请输入要查询的学号");
        ui->numTw->setRowCount(0); // 清空表格
        return;
    }

    // 定义科目名称列表（与scores数组顺序对应）
    QStringList subjectNames = {
        "高等数学", "大学物理", "程序设计",
        "数据结构", "线性代数", "离散数学"
    };

    // 查找对应的学生
    Student* foundStudent = nullptr;
    for (auto s : m_students) {
        if (s->number == targetNumber) {
            foundStudent = s;
            break;
        }
    }

    // 清空表格之前的数据
    ui->numTw->clearContents();
    ui->numTw->setRowCount(0);

    // 处理查询结果
    if (foundStudent) {
        // 设置表格列数和表头
        ui->numTw->setColumnCount(2);
        QStringList headers = {"信息项", "内容"};
        ui->numTw->setHorizontalHeaderLabels(headers);

        // 添加基本信息行
        int row = 0;

        // 学号
        ui->numTw->insertRow(row);
        ui->numTw->setItem(row, 0, new QTableWidgetItem("学号"));
        ui->numTw->setItem(row, 1, new QTableWidgetItem(foundStudent->number));
        row++;

        // 姓名
        ui->numTw->insertRow(row);
        ui->numTw->setItem(row, 0, new QTableWidgetItem("姓名"));
        ui->numTw->setItem(row, 1, new QTableWidgetItem(foundStudent->name));
        row++;

        // 性别
        ui->numTw->insertRow(row);
        ui->numTw->setItem(row, 0, new QTableWidgetItem("性别"));
        ui->numTw->setItem(row, 1, new QTableWidgetItem(foundStudent->sex));
        row++;

        // 专业
        ui->numTw->insertRow(row);
        ui->numTw->setItem(row, 0, new QTableWidgetItem("专业"));
        ui->numTw->setItem(row, 1, new QTableWidgetItem(foundStudent->major));
        row++;

        // 班级
        ui->numTw->insertRow(row);
        ui->numTw->setItem(row, 0, new QTableWidgetItem("班级"));
        ui->numTw->setItem(row, 1, new QTableWidgetItem(foundStudent->classname));
        row++;

        // 空行分隔
        ui->numTw->insertRow(row);
        ui->numTw->setItem(row, 0, new QTableWidgetItem(""));
        ui->numTw->setItem(row, 1, new QTableWidgetItem(""));
        row++;

        // 添加各科目成绩
        for (int i = 0; i < foundStudent->scores.size() && i < subjectNames.size(); ++i) {
            ui->numTw->insertRow(row);
            ui->numTw->setItem(row, 0, new QTableWidgetItem(subjectNames[i]));
            ui->numTw->setItem(row, 1, new QTableWidgetItem(QString::number(foundStudent->scores[i])));
            row++;
        }

        // 空行分隔
        ui->numTw->insertRow(row);
        ui->numTw->setItem(row, 0, new QTableWidgetItem(""));
        ui->numTw->setItem(row, 1, new QTableWidgetItem(""));
        row++;

        // 总分
        ui->numTw->insertRow(row);
        ui->numTw->setItem(row, 0, new QTableWidgetItem("总分"));
        ui->numTw->setItem(row, 1, new QTableWidgetItem(QString::number(foundStudent->totalScore)));
        row++;

        // 平均分
        ui->numTw->insertRow(row);
        ui->numTw->setItem(row, 0, new QTableWidgetItem("平均分"));
        ui->numTw->setItem(row, 1, new QTableWidgetItem(QString::number(foundStudent->averageScore, 'f', 1)));
        row++;

        // 自动调整列宽
        ui->numTw->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    } else {
        // 未找到学生，显示错误信息
        QMessageBox::critical(this, "查询错误", QString("未找到学号为 %1 的学生").arg(targetNumber));
    }
   ui->stackedWidget->setCurrentWidget(ui->numpage);
}
bool MainWindow::exportTableToExcel(QTableWidget *table, const QString &defaultFileName)
{
    if (!table || table->rowCount() == 0) {
        QMessageBox::warning(this, "导出失败", "表格中没有数据可导出");
        return false;
    }

    // 选择保存路径
    QString filePath = QFileDialog::getSaveFileName(
        this, "导出Excel文件", defaultFileName,
        "Excel文件 (*.xlsx);;Excel 97-2003文件 (*.xls);;所有文件 (*)"
        );

    if (filePath.isEmpty()) return false;

    // 检查Excel是否可用
    QAxObject excel("Excel.Application");
    if (excel.isNull()) {
        QMessageBox::critical(this, "错误", "未检测到Excel，请先安装Microsoft Excel");
        return false;
    }

    // 隐藏Excel窗口
    excel.setProperty("Visible", false);
    excel.setProperty("DisplayAlerts", false);

    // 创建工作簿和工作表
    QAxObject *workBooks = excel.querySubObject("Workbooks");
    QAxObject *workBook = workBooks->querySubObject("Add");
    QAxObject *workSheet = workBook->querySubObject("Worksheets(int)", 1);
    workSheet->setProperty("Name", "表格数据");

    // 写入表头
    for (int col = 0; col < table->columnCount(); ++col) {
        QTableWidgetItem *headerItem = table->horizontalHeaderItem(col);
        QString headerText = headerItem ? headerItem->text() : QString("列 %1").arg(col + 1);

        QAxObject *cell = workSheet->querySubObject("Cells(int, int)", 1, col + 1);
        cell->setProperty("Value", headerText);
        cell->querySubObject("Font")->setProperty("Bold", true);
    }

    // 写入表格数据
    for (int row = 0; row < table->rowCount(); ++row) {
        for (int col = 0; col < table->columnCount(); ++col) {
            QTableWidgetItem *item = table->item(row, col);
            QString cellText = item ? item->text() : "";

            QAxObject *cell = workSheet->querySubObject("Cells(int, int)", row + 2, col + 1);
            cell->setProperty("Value", cellText);

            // 尝试将数字列设置为数字格式
            bool isNumber;
            cellText.toDouble(&isNumber);
            if (isNumber) {
                cell->setProperty("NumberFormat", "0.00");
            }
        }
    }

    // 自动调整列宽
    workSheet->querySubObject("UsedRange")->dynamicCall("Columns.AutoFit()");

    // 保存文件
    bool saveSuccess = workBook->dynamicCall("SaveAs(const QString&)",
                                             QDir::toNativeSeparators(filePath)).toBool();
    if (!saveSuccess) {
        QMessageBox::critical(this, "错误", "保存文件失败");
    }

    // 清理资源
    workBook->dynamicCall("Close()");
    excel.dynamicCall("Quit()");

    delete workSheet;
    delete workBook;
    delete workBooks;

    if (saveSuccess) {
        QMessageBox::information(this, "成功", QString("数据已导出至:\n%1").arg(filePath));
    }

    return saveSuccess;
}

// LookTW表格导出
void MainWindow::on_lookDL_released()
{
    exportTableToExcel(ui->LookTW, "学生信息表.xlsx");
}

// numTw表格导出（学号查询结果）
void MainWindow::on_numDL_released()
{
    exportTableToExcel(ui->numTw, "学生成绩查询结果.xlsx");
}

// filterTW表格导出（筛选结果）
void MainWindow::on_classDL_released()
{
    exportTableToExcel(ui->filterTW, "筛选结果表.xlsx");
}

// rankTable表格导出（排名表）
void MainWindow::on_rankDL_released()
{
    exportTableToExcel(ui->rankTable, "成绩排名表.xlsx");
}

