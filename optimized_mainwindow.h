#ifndef OPTIMIZED_MAINWINDOW_H
#define OPTIMIZED_MAINWINDOW_H

#include "mainwindow.h"
#include <QHash>
#include <QList>
#include <QString>

class OptimizedMainWindow : public MainWindow
{
    Q_OBJECT

public:
    using MainWindow::MainWindow;
    
    // 重写关键方法进行优化
    void loadData(const QString &filename) override;
    void addStudent(Student* student);
    void removeStudent(const QString& number);
    Student* findStudentByNumber(const QString& number);
    QList<Student*> findStudentsByClass(const QString& className);
    QList<Student*> findStudentsByMajor(const QString& majorName);
    SubjectStats calculateSubjectStatsOptimized(const QString &classOrMajor, 
                                               const QString &filterValue, 
                                               const QString &subject);

private:
    void buildIndexes();  // 构建索引
    void updateIndexes(Student* student, bool isAdd);  // 更新索引
    
    // 索引结构
    QHash<QString, Student*> m_numberIndex;
    QHash<QString, QList<Student*>> m_classIndex;
    QHash<QString, QList<Student*>> m_majorIndex;
};

#endif // OPTIMIZED_MAINWINDOW_H
