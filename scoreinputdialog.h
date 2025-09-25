// #ifndef SCOREINPUTDIALOG_H
// #define SCOREINPUTDIALOG_H

// class ScoreInputDialog
// {
// public:
//     ScoreInputDialog();
// };

// #endif // SCOREINPUTDIALOG_H
#ifndef SCOREINPUTDIALOG_H
#define SCOREINPUTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include "mainwindow.h" // 引入Student类定义

class ScoreInputDialog : public QDialog
{
    Q_OBJECT

public:
    // 构造函数：isModify=true为修改模式（需传Student指针），false为新增模式
    explicit ScoreInputDialog(Student* student = nullptr, QWidget *parent = nullptr);
    ~ScoreInputDialog() = default;

    // 获取录入的学生数据（新增时调用）
    Student* getInputStudent() const { return m_inputStudent; }

private slots:
    // 确认按钮点击事件（校验+数据封装）
    void onConfirmClicked();
    // 取消按钮点击事件（关闭弹窗）
    void onCancelClicked();

private:
    // 初始化弹窗UI布局
    void initUI();
    // 输入合法性校验（返回空字符串表示校验通过，否则返回错误信息）
    QString validateInput();
    // 填充已有学生数据到表单（修改模式时调用）
    void fillStudentData(Student* student);

private:
    Student* m_inputStudent; // 录入/修改的学生对象
    bool m_isModify;         // 是否为修改模式

    // 基础信息输入框
    QLineEdit* m_leNumber;   // 学号
    QLineEdit* m_leName;     // 姓名
    QLineEdit* m_leSex;      // 性别
    QLineEdit* m_leMajor;    // 专业
    QLineEdit* m_leClass;    // 班级

    // 6科成绩输入框（对应：高数、物理、程序设计、数据结构、线性代数、离散数学）
    QLineEdit* m_leScores[6];
    // 科目名称数组（与输入框顺序对应）
    QString m_subjectNames[6] = {
        "高等数学", "大学物理", "程序设计",
        "数据结构", "线性代数", "离散数学"
    };
};

#endif // SCOREINPUTDIALOG_H
