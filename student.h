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
    QString classname;
    qreal math;
    qreal chinese;
    qreal english;
};

#endif // STUDENT_H
