
#ifndef NORMALDISTRIBUTIONPLOT_H
#define NORMALDISTRIBUTIONPLOT_H

#include <QWidget>
#include <QVector>
#include <algorithm> // 用于std::min_element/std::max_element

class NormalDistributionPlot : public QWidget
{
    Q_OBJECT

public:
    NormalDistributionPlot(QWidget *parent = nullptr);
    void setScores(const QVector<double> &scores); // 设置成绩数据

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override; // 监听大小变化

private:
    QVector<double> m_scores;   // 数学成绩数据
    double m_mean = 0;          // 均值
    double m_stdDev = 0;        // 标准差
    const int m_binCount = 10;  // 直方图区间数量（可调整）

    // 辅助函数：计算均值
    double calculateMean() const;
    // 辅助函数：计算标准差
    double calculateStdDev() const;
    // 辅助函数：正态分布概率密度
    double normalDensity(double x) const;
    // 辅助函数：成绩→X坐标（动态关联画布宽度）
    int scoreToX(double score) const;
    // 辅助函数：频率/密度→Y坐标（动态关联画布高度）
    int valueToY(double value) const;
    // 辅助函数：计算当前画布的有效绘图区域（扣除边距）
    QRectF getPlotRect() const;
};

#endif // NORMALDISTRIBUTIONPLOT_H
