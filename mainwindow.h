#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QWidget>
#include"student.h"
#include"normaldistributionplot.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

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

private slots:

    void on_LookBtn_released();

    void on_StatBtn_released();

    void on_QuitBtn_released();

    void on_addBtn_released();

    void on_saveBtn_released();

    void on_pushButton_released();

    void on_AsBtn_released();

    void on_MoBtn_released();

private:
    Ui::MainWindow *ui;
    QStringList m_fields;//表头字段（封装）
    QList<Student*> m_students;
    NormalDistributionPlot *m_plot; //绘图控件指针
};
#endif // MAINWINDOW_H
