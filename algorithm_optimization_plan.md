# 学生成绩管理系统算法优化方案

## 当前问题分析

### 主要性能瓶颈
1. **频繁的线性搜索**：O(n)时间复杂度
2. **重复计算**：总分和平均分多次计算
3. **内存使用效率低**：缺乏索引结构
4. **统计计算复杂**：嵌套循环导致O(n×m)复杂度

## 优化方案

### 1. 数据结构优化

#### 当前结构
```cpp
QList<Student*> m_students;  // 线性存储
```

#### 优化后结构
```cpp
// 主存储
QList<Student*> m_students;

// 索引结构
QHash<QString, Student*> m_numberIndex;           // 学号索引
QHash<QString, QList<Student*>> m_classIndex;     // 班级索引  
QHash<QString, QList<Student*>> m_majorIndex;     // 专业索引
QHash<QString, QList<Student*>> m_subjectIndex;   // 科目成绩索引
```

### 2. 核心算法优化实现

<write_to_file>
<path>optimized_mainwindow.h</path>
<content>
#ifndef OPTIMIZED_MAINWINDOW_H
#define OPTIMIZED_MAINWINDOW_H

#include "mainwindow.h"
#include <QHash>

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
