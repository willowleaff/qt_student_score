#ifndef STUDENT_H
#define STUDENT_H
#include<QString>
class Student
{
public:
    Student();
    void display();
    QString number;
    QString name;
    QString sex;
    QString major;
    QString classname;
    qreal math;
    qreal cp;
    qreal pd;
    qreal ds;
    qreal la;
    qreal dm;
    qreal totalScore;
    qreal averageScore;
    double rankPercent;
    QString scholarshipLevel;
};

#endif // STUDENT_H
