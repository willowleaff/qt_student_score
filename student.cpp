#include "student.h"
#include<QDebug>
Student::Student() {}
void Student::display()
{
    qInfo()<<number<<name<<sex<<major<<classname<<math<<cp<<pd<<ds<<la<<dm;
}
