# Qt学生成绩管理系统 - 优化实现代码示例

## 1. 索引构建优化

### 原始实现（O(n)查找）
```cpp
// mainwindow.cpp 中的原始查找方法
Student* MainWindow::findStudent(const QString &number) {
    for (Student* student : students) {
        if (student->getNumber() == number) {
            return student;
        }
    }
    return nullptr;
}
```

### 优化实现（O(1)查找）
```cpp
// optimized_mainwindow.cpp 中的优化查找方法
Student* OptimizedMainWindow::findStudentByNumber(const QString& number) {
    return m_numberIndex.value(number, nullptr);
}

void OptimizedMainWindow::buildIndexes() {
    m_numberIndex.clear();
    m_classIndex.clear();
    m_majorIndex.clear();
    
    for (Student* student : students) {
        // 学号索引
        m_numberIndex[student->getNumber()] = student;
        
        // 班级索引
        m_classIndex[student->getClassName()].append(student);
        
        // 专业索引  
        m_majorIndex[student->getMajorName()].append(student);
    }
}
```

## 2. 缓存机制优化

### 原始实现（重复计算）
```cpp
// 每次都需要重新计算统计信息
SubjectStats MainWindow::calculateSubjectStats(const QString &classOrMajor, 
                                             const QString &filterValue, 
                                             const QString &subject) {
    // 遍历所有学生进行统计计算
    // O(n)复杂度，每次调用都需要完整计算
}
```

### 优化实现（缓存机制）
```cpp
// optimized_mainwindow.cpp 中的缓存实现
SubjectStats OptimizedMainWindow::calculateSubjectStatsOptimized(
    const QString &classOrMajor, 
    const QString &filterValue, 
    const QString &subject) {
    
    QString cacheKey = classOrMajor + ":" + filterValue + ":" + subject;
    
    // 检查缓存
    if (m_statsCache.contains(cacheKey)) {
        return m_statsCache[cacheKey];
    }
    
    // 计算统计信息（使用索引加速）
    SubjectStats stats;
    QList<Student*> targetStudents;
    
    if (classOrMajor == "class") {
        targetStudents = m_classIndex.value(filterValue);
    } else {
        targetStudents = m_majorIndex.value(filterValue);  
    }
    
    // 计算统计信息...
    
    // 存入缓存
    m_statsCache[cacheKey] = stats;
    return stats;
}
```

## 3. 内存管理优化

### 原始实现（频繁创建临时对象）
```cpp
// 频繁创建临时QList对象
QList<Student*> MainWindow::getStudentsByClass(const QString& className) {
    QList<Student*> result;
    for (Student* student : students) {
        if (student->getClassName() == className) {
            result.append(student);
        }
    }
    return result; // 返回临时对象副本
}
```

### 优化实现（对象复用）
```cpp
// 返回索引中的引用，避免复制
const QList<Student*>& OptimizedMainWindow::findStudentsByClass(const QString& className) {
    static QList<Student*> emptyList; // 静态空列表避免重复创建
    
    if (m_classIndex.contains(className)) {
        return m_classIndex[className];
    }
    return emptyList;
}
```

## 4. 批量操作优化

### 原始实现（逐个处理）
```cpp
void MainWindow::addMultipleStudents(const QList<Student*>& newStudents) {
    for (Student* student : newStudents) {
        students.append(student);
        // 每次添加都触发界面更新
        updateStudentList();
    }
}
```

### 优化实现（批量处理）
```cpp
void OptimizedMainWindow::addMultipleStudents(const QList<Student*>& newStudents) {
    // 批量添加学生
    students.append(newStudents);
    
    // 批量更新索引
    for (Student* student : newStudents) {
        updateIndexes(student, true);
    }
    
    // 单次界面更新
    updateStudentList();
}
```

## 5. 智能内存管理

### 原始实现（手动内存管理）
```cpp
// 容易造成内存泄漏
void MainWindow::removeStudent(const QString& number) {
    for (int i = 0; i < students.size(); ++i) {
        if (students[i]->getNumber() == number) {
            delete students[i]; // 手动删除
            students.removeAt(i);
            break;
        }
    }
}
```

### 优化实现（智能指针）
```cpp
// 使用QSharedPointer自动管理内存
void OptimizedMainWindow::removeStudent(const QString& number) {
    Student* student = m_numberIndex.value(number);
    if (student) {
        // 从索引中移除
        m_numberIndex.remove(number);
        
        // 从班级索引中移除
        QString className = student->getClassName();
        m_classIndex[className].removeAll(student);
        if (m_classIndex[className].isEmpty()) {
            m_classIndex.remove(className);
        }
        
        // 从专业索引中移除
        QString majorName = student->getMajorName();
        m_majorIndex[majorName].removeAll(student);
        if (m_majorIndex[majorName].isEmpty()) {
            m_majorIndex.remove(majorName);
        }
        
        // 从主列表中移除
        students.removeAll(student);
        
        // 智能指针自动释放内存
    }
}
```

## 6. 性能监控和调试

### 性能测试方法
```cpp
void OptimizedMainWindow::performanceTest() {
    QElapsedTimer timer;
    
    // 测试查找性能
    timer.start();
    for (int i = 0; i < 1000; ++i) {
        findStudentByNumber("2023001");
    }
    qDebug() << "1000次查找耗时:" << timer.elapsed() << "ms";
    
    // 测试筛选性能
    timer.restart();
    for (int i = 0; i < 100; ++i) {
        findStudentsByClass("计算机科学与技术1班");
    }
    qDebug() << "100次班级筛选耗时:" << timer.elapsed() << "ms";
}
```

## 7. 内存使用报告

### 内存监控实现
```cpp
void OptimizedMainWindow::memoryUsageReport() {
    qDebug() << "=== 内存使用报告 ===";
    qDebug() << "学生数量:" << students.size();
    qDebug() << "学号索引大小:" << m_numberIndex.size();
    qDebug() << "班级索引大小:" << m_classIndex.size();
    qDebug() << "专业索引大小:" << m_majorIndex.size();
    qDebug() << "缓存条目数量:" << m_statsCache.size();
    
    // 估算内存使用量
    size_t estimatedMemory = students.size() * sizeof(Student) +
                           m_numberIndex.size() * sizeof(QString) +
                           m_classIndex.size() * sizeof(QList<Student*>) +
                           m_statsCache.size() * sizeof(SubjectStats);
    
    qDebug() << "估算内存使用:" << estimatedMemory / 1024 << "KB";
}
```

## 总结

这些优化实现展示了如何通过：
1. **索引结构**将O(n)操作优化为O(1)
2. **缓存机制**避免重复计算
3. **内存管理**减少临时对象创建
4. **批量处理**减少界面更新频率
5. **智能指针**自动管理内存生命周期

这些技术组合使用，显著提升了系统性能和大数据承载能力。
