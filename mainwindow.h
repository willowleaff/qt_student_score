#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QWidget>
#include"student.h"
#include"normaldistributionplot.h"
#include"scorepiechart.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE
struct SubjectStats
{
    QString groupName;
    double average;
    double highest;
    double lowest;
    int passCount;
    int excellentCount;
};
struct GroupStat {
    QString groupName;  // 存储班级名或专业名
    SubjectStats stats; // 存储该分组的统计数据
};
struct Scholarship {
    QString level;       // 等级（一等、二等、三等）
    double minAvgScore;  // 最低平均分
    double rankPercent;  // 排名百分比（如前5%为0.05）
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //键盘操作
    void keyPressEvent(QKeyEvent *event) override;
    //加载数据（从文件中读取数据）
    void loadData(const QString &filename);
    //保存数据
    void saveData(const QString &filename);
    void initStatPageLayout();
    void initSubjectCombo();
    void setFilterTWHeaders(const QString& subject);
    void calculateStudentScores();
    void rankStudentsByAverageScore(bool ascending);
    void rankStudentsByAverageScore2(bool ascending, QVector<Student*>& students);
    SubjectStats calculateSubjectStats(const QString &classOrMajor, const QString &filterValue,const QString &subject);
    void initRankTable();  // 初始化排名表格
    void updateRankTable();  // 更新排名表格数据
    void displayStatsToTable(const QVector<SubjectStats>& stats);
    bool exportStatsToText(const QVector<SubjectStats>& stats, const QString& type, const QString& subject);
    QVector<SubjectStats> calculateAllClassOrMajorSubjectStats(const QString& type, const QString& subject);
    void displayScholarshipResults(const QVector<Student*>& students);
    void initScholarshipRules();
    void exportScholarshipResult();
private slots:

    void on_LookBtn_released();

    void on_StatBtn_released();

    void on_QuitBtn_released();

    void on_addBtn_released();

    void on_saveBtn_released();

    void on_AsBtn_released();

    void on_MoBtn_released();

    void on_LSBtn_released();
    void on_classBtn_released();
    void on_obBtn_released();

    void on_rankBtn_released();

    void on_class_scoreBtn_released();

    void on_major_scoreBtn_released();

    void on_downloadBtn_released();

    void on_AssPb_released();

    //void on_evalScholarshipBtn_released();

    void on_AnsBtn_released();

private:
    Ui::MainWindow *ui;
    QStringList m_fields;//表头字段（封装）
    QList<Student*> m_students;
    NormalDistributionPlot *m_plot; //绘图控件指针
    ScorePieChart *m_pieChart; //绘图控件指针
    bool hasFailed(const Student* s);  // 判断学生是否挂科
    void evaluateScholarship();        // 评定奖学金
    QVector<Scholarship> m_scholarshipRules; // 奖学金规则
    QString m_selectedSubject;
    QString m_selectedMajor;
    void filterStudents(const QString &filterType, const QString &value,
                        const QString &subject, double minScore, double maxScore);//函数
};
#endif // MAINWINDOW_H
