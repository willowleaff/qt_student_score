#ifndef STUDENT_H
#define STUDENT_H
#include<QString>
#include<QVector>
class Student
{
public:
    // Student();
    // void display();
    // QString number;
    // QString name;
    // QString sex;
    // QString major;
    // QString classname;
    // qreal math;
    // qreal cp;
    // qreal pd;
    // qreal ds;
    // qreal la;
    // qreal dm;
    // qreal totalScore;
    // qreal averageScore;
    // double rankPercent;
    // QString scholarshipLevel;
    Student();
    void display();
    QString number;
    QString name;
    QString sex;
    QString major;
    QString classname;
    // 使用QVector来存储科目成绩，方便动态添加
    QVector<qreal> scores;
    qreal totalScore;
    qreal averageScore;
    double rankPercent;
    QString scholarshipLevel;
};

#endif // STUDENT_H
