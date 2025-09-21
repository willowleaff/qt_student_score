#include "student.h"
#include<QDebug>
Student::Student() {}
void Student::display()
{
    qInfo()<<number<<name<<classname<<math<<chinese<<english;
}
