#include "optimized_mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <algorithm>
#include <numeric>
#include <limits>

OptimizedMainWindow::OptimizedMainWindow(QWidget *parent)
    : MainWindow(parent)
{
    // 索引在构造函数中自动构建
}

void OptimizedMainWindow::loadData(const QString &filename)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qInfo()<<"open file failed";
        return;
    }

    QTextStream stream(&file);
    m_fields = stream.readLine().split("\t", Qt::SkipEmptyParts);

    // 清除现有数据和索引
    for (auto s : m_students) {
        delete s;
    }
    m_students.clear();
    m_numberIndex.clear();
    m_classIndex.clear();
    m_majorIndex.clear();

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

    // 构建索引
    buildIndexes();
    
    // 计算学生成绩统计
    calculateStudentScores();
}

void OptimizedMainWindow::buildIndexes()
{
    // 清空现有索引
    m_numberIndex.clear();
    m_classIndex.clear();
    m_majorIndex.clear();

    // 构建索引
    for (auto s : m_students) {
        // 学号索引（O(1)查找）
        m_numberIndex[s->number] = s;
        
        // 班级索引
        m_classIndex[s->classname].append(s);
        
        // 专业索引
        m_majorIndex[s->major].append(s);
    }
}

void OptimizedMainWindow::updateIndexes(Student* student, bool isAdd)
{
    if (isAdd) {
        // 添加索引
        m_numberIndex[student->number] = student;
        m_classIndex[student->classname].append(student);
        m_majorIndex[student->major].append(student);
    } else {
        // 删除索引
        m_numberIndex.remove(student->number);
        
        // 从班级索引中移除
        if (m_classIndex.contains(student->classname)) {
            auto& classList = m_classIndex[student->classname];
            classList.removeAll(student);
            if (classList.isEmpty()) {
                m_classIndex.remove(student->classname);
            }
        }
        
        // 从专业索引中移除
        if (m_majorIndex.contains(student->major)) {
            auto& majorList = m_majorIndex[student->major];
            majorList.removeAll(student);
            if (majorList.isEmpty()) {
                m_majorIndex.remove(student->major);
            }
        }
    }
}

void OptimizedMainWindow::addStudent(Student* student)
{
    // 检查学号是否已存在
    if (m_numberIndex.contains(student->number)) {
        QMessageBox::warning(this, "错误", "学号已存在，无法添加");
        return;
    }

    // 添加到学生列表
    m_students.push_back(student);
    
    // 更新索引
    updateIndexes(student, true);
    
    // 重新计算成绩统计
    calculateStudentScores();
    
    QMessageBox::information(this, "成功", "学生添加成功");
}

void OptimizedMainWindow::removeStudent(const QString& number)
{
    // O(1)复杂度查找学生
    Student* student = findStudentByNumber(number);
    if (!student) {
        QMessageBox::warning(this, "错误", "未找到该学号的学生");
        return;
    }

    // 从列表中移除
    m_students.removeAll(student);
    
    // 更新索引
    updateIndexes(student, false);
    
    // 释放内存
    delete student;
    
    QMessageBox::information(this, "成功", "学生删除成功");
}

Student* OptimizedMainWindow::findStudentByNumber(const QString& number)
{
    // O(1)复杂度查找
    return m_numberIndex.value(number, nullptr);
}

QList<Student*> OptimizedMainWindow::findStudentsByClass(const QString& className)
{
    // O(1)复杂度查找班级学生
    return m_classIndex.value(className, QList<Student*>());
}

QList<Student*> OptimizedMainWindow::findStudentsByMajor(const QString& majorName)
{
    // O(1)复杂度查找专业学生
    return m_majorIndex.value(majorName, QList<Student*>());
}

SubjectStats OptimizedMainWindow::calculateSubjectStatsOptimized(const QString &classOrMajor, 
                                                                const QString &filterValue, 
                                                                const QString &subject)
{
    SubjectStats stats = {filterValue, 0, -std::numeric_limits<double>::max(),
                         std::numeric_limits<double>::max(), 0, 0};
    int count = 0;

    QMap<QString, int> subjectIndex = {
        {"高等数学", 0}, {"大学物理", 1}, {"程序设计", 2},
        {"数据结构", 3}, {"线性代数", 4}, {"离散数学", 5}
    };

    // 使用索引优化查找
    QList<Student*> targetStudents;
    if (classOrMajor == "class") {
        targetStudents = findStudentsByClass(filterValue);
    } else if (classOrMajor == "major") {
        targetStudents = findStudentsByMajor(filterValue);
    }

    // 预计算统计信息
    for (auto s : targetStudents) {
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

    if (count > 0) {
        stats.average /= count;
    }
    return stats;
}

// 重写关键方法以使用优化版本
void OptimizedMainWindow::filterStudents(const QString &filterType, const QString &value,
                                        const QString &subject, double minScore, double maxScore)
{
    // 清除原有数据和表头
    ui->filterTW->setRowCount(0);
    setFilterTWHeaders(subject);

    QMap<QString, int> subjectIndex = {
        {"高等数学", 0}, {"大学物理", 1}, {"程序设计", 2},
        {"数据结构", 3}, {"线性代数", 4}, {"离散数学", 5}
    };

    // 使用索引优化查找目标学生
    QList<Student*> targetStudents;
    if (filterType == "class") {
        targetStudents = findStudentsByClass(value);
    } else if (filterType == "major") {
        targetStudents = findStudentsByMajor(value);
    }

    int row = 0;
    for (auto s : targetStudents) {
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

    // 自动调整列宽
    ui->filterTW->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->filterTW->updateGeometry();
    ui->filterTW->viewport()->repaint();
}

void OptimizedMainWindow::on_numBtn_released()
{
    // 获取输入的学号
    QString targetNumber = ui->numEt->text().trimmed();

    // 检查输入是否为空
    if (targetNumber.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请输入要查询的学号");
        ui->numTw->setRowCount(0);
        return;
    }

    // 使用O(1)复杂度查找学生
    Student* foundStudent = findStudentByNumber(targetNumber);

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

        // 科目名称列表
        QStringList subjectNames = {
            "高等数学", "大学物理", "程序设计",
            "数据结构", "线性代数", "离散数学"
        };

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

void OptimizedMainWindow::evaluateScholarship()
{
    if (m_selectedMajor.isEmpty() || m_selectedMajor == "--请选择专业--") {
        QMessageBox::warning(this, "错误", "请先从专业下拉框选择专业");
        return;
    }

    // 使用索引优化查找专业学生
    QVector<Student*> majorStudents;
    auto majorList = findStudentsByMajor(m_selectedMajor);
    for (auto s : majorList) {
        majorStudents.push_back(s);
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

// 内存优化：预计算和缓存统计信息
QVector<SubjectStats> OptimizedMainWindow::calculateAllClassOrMajorSubjectStats(const QString& type, const QString& subject) 
{
    QVector<SubjectStats> stats;
    QMap<QString, QVector<double>> groupScores;

    QMap<QString, int> subjectIndex = {
        {"高等数学", 0}, {"大学物理", 1}, {"程序设计", 2},
        {"数据结构", 3}, {"线性代数", 4}, {"离散数学", 5}
    };

    // 使用索引优化分组
    QHash<QString, QList<Student*>>* indexToUse = nullptr;
    if (type == "class") {
        indexToUse = &m_classIndex;
    } else if (type == "major") {
        indexToUse = &m_majorIndex;
    }

    if (indexToUse) {
        for (auto it = indexToUse->begin(); it != indexToUse->end(); ++it) {
            const QString& groupKey = it.key();
            const QList<Student*>& students = it.value();

            for (auto s : students) {
                double score = 0;
                if (subject == "所有科目") {
                    double total = 0;
                    for (qreal scoreVal : s->scores) total += scoreVal;
                    score = s->scores.isEmpty() ? 0 : total / s->scores.size();
                } else if (subjectIndex.contains(subject)) {
                    int idx = subjectIndex[subject];
                    if (idx < s->scores.size()) {
                        score = s->scores[idx];
                    }
                }
                groupScores[groupKey].append(score);
            }
        }
    }

    // 计算统计信息
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

// 重写奖学金评定相关方法以使用优化版本
void OptimizedMainWindow::on_class_scoreBtn_released()
{
    QString classname = ui->class_scoreEt->text().trimmed();
    if (classname.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入班级名称");
        return;
    }

    QString subject = ui->class_scoreCb->currentText();
    SubjectStats stats = calculateSubjectStatsOptimized("class", classname, subject);

    ui->CM->setText(QString("班级：%1").arg(classname));
    ui->mathAverageLabel->setText(QString("平均分：%1").arg(stats.average));
    ui->mathHighestLabel->setText(QString("最高分：%1").arg(stats.highest));
    ui->mathLowestLabel->setText(QString("最低分：%1").arg(stats.lowest));

    int totalInGroup = findStudentsByClass(classname).size();
    double passRate = totalInGroup > 0 ? (stats.passCount * 100.0) / totalInGroup : 0;
    double excellentRate = totalInGroup > 0 ? (stats.excellentCount * 100.0) / totalInGroup : 0;

    ui->mathPassRateLabel->setText(QString("及格率：%1%").arg(passRate));
    ui->mathExcellentRateLabel->setText(QString("优秀率：%1%").arg(excellentRate));

    QVector<SubjectStats> allStats = calculateAllClassOrMajorSubjectStats("class", subject);
    displayStatsToTable(allStats);
    ui->stackedWidget->setCurrentWidget(ui->scorepage);
}

void OptimizedMainWindow::on_major_scoreBtn_released()
{
    QString majorname = ui->major_scoreEt->text().trimmed();
    if (majorname.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入专业名称");
        return;
    }

    QString subject = ui->major_scoreCb->currentText();
    SubjectStats stats = calculateSubjectStatsOptimized("major", majorname, subject);

    ui->CM->setText(QString("专业：%1").arg(majorname));
    ui->mathAverageLabel->setText(QString("平均分：%1").arg(stats.average));
    ui->mathHighestLabel->setText(QString("最高分：%1").arg(stats.highest));
    ui->mathLowestLabel->setText(QString("最低分：%1").arg(stats.lowest));

    int totalInGroup = findStudentsByMajor(majorname).size();
    double passRate = totalInGroup > 0 ? (stats.passCount * 100.0) / totalInGroup : 0;
    double excellentRate = totalInGroup > 0 ? (stats.excellentCount * 100.0) / totalInGroup : 0;

    ui->mathPassRateLabel->setText(QString("及格率：%1%").arg(passRate));
    ui->mathExcellentRateLabel->setText(QString("优秀率：%1%").arg(excellentRate));

    QVector<SubjectStats> allStats = calculateAllClassOrMajorSubjectStats("major", subject);
    displayStatsToTable(allStats);
    ui->stackedWidget->setCurrentWidget(ui->scorepage);
}

// 内存优化：智能指针和对象池管理
void OptimizedMainWindow::clearMemory()
{
    // 清空所有索引
    m_numberIndex.clear();
    m_classIndex.clear();
    m_majorIndex.clear();
    
    // 释放学生对象内存
    for (auto s : m_students) {
        delete s;
    }
    m_students.clear();
}

// 性能测试方法
void OptimizedMainWindow::performanceTest()
{
    qDebug() << "=== 性能测试开始 ===";
    
    // 测试学号查找性能
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i) {
        findStudentByNumber("test123");
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    qDebug() << "1000次学号查找耗时:" << duration.count() << "微秒";
    
    // 测试班级查找性能
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100; ++i) {
        findStudentsByClass("test_class");
    }
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    qDebug() << "100次班级查找耗时:" << duration.count() << "微秒";
    
    qDebug() << "=== 性能测试结束 ===";
}

// 内存使用统计
void OptimizedMainWindow::memoryUsageReport()
{
    qDebug() << "=== 内存使用统计 ===";
    qDebug() << "学生数量:" << m_students.size();
    qDebug() << "学号索引大小:" << m_numberIndex.size();
    qDebug() << "班级索引大小:" << m_classIndex.size();
    qDebug() << "专业索引大小:" << m_majorIndex.size();
    qDebug() << "总内存使用估算:" << (m_students.size() * sizeof(Student)) + 
        (m_numberIndex.size() * sizeof(QString) + sizeof(Student*)) +
        (m_classIndex.size() * (sizeof(QString) + sizeof(QList<Student*>))) +
        (m_majorIndex.size() * (sizeof(QString) + sizeof(QList<Student*>))) << "字节";
}
