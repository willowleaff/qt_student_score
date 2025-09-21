// // #ifndef NORMALDISTRIBUTIONPLOT_H
// // #define NORMALDISTRIBUTIONPLOT_H

// // class NormalDistributionPlot
// // {
// // public:
// //     NormalDistributionPlot();
// // };

// // #endif // NORMALDISTRIBUTIONPLOT_H
// #ifndef NORMALDISTRIBUTIONPLOT_H
// #define NORMALDISTRIBUTIONPLOT_H

// #include <QWidget>
// #include <QVector>
// #include <algorithm> // 用于min_element和max_element

// class NormalDistributionPlot : public QWidget
// {
//     Q_OBJECT
// public:
//     explicit NormalDistributionPlot(QWidget *parent = nullptr)
//         : QWidget(parent), m_binCount(10), m_mean(0), m_stdDev(0) {
//         setMinimumSize(600, 400); // 设置最小尺寸
//     }

//     // 设置成绩数据并触发重绘
//     void setScores(const QVector<double> &scores) {
//         m_scores = scores;
//         calculateStats(); // 计算均值和标准差
//         update(); // 触发paintEvent
//     }

// protected:
//     void paintEvent(QPaintEvent *event) override;

// private:
//     QVector<double> m_scores; // 数学成绩数据
//     int m_binCount; // 直方图的柱形数量
//     double m_mean; // 均值
//     double m_stdDev; // 标准差

//     // 计算均值和标准差
//     void calculateStats() {
//         if (m_scores.isEmpty()) {
//             m_mean = 0;
//             m_stdDev = 0;
//             return;
//         }

//         // 计算均值
//         double sum = 0;
//         for (double s : m_scores) sum += s;
//         m_mean = sum / m_scores.size();

//         // 计算标准差
//         double sumSq = 0;
//         for (double s : m_scores) sumSq += (s - m_mean) * (s - m_mean);
//         m_stdDev = qSqrt(sumSq / (m_scores.size() - 1));
//     }

//     // 正态分布概率密度函数
//     double normalDensity(double x) const {
//         if (m_stdDev == 0) return 0;
//         double exponent = -((x - m_mean) * (x - m_mean)) / (2 * m_stdDev * m_stdDev);
//         return (1 / (m_stdDev * qSqrt(2 * M_PI))) * qExp(exponent);
//     }

//     // 成绩值转控件X坐标
//     int scoreToX(double score) const {
//         if (m_scores.isEmpty()) return 0;
//         double min = *std::min_element(m_scores.begin(), m_scores.end()) - 5;
//         double max = *std::max_element(m_scores.begin(), m_scores.end()) + 5;
//         return 50 + (score - min) / (max - min) * (width() - 100); // 左右边距各50
//     }

//     // 频率/密度值转控件Y坐标
//     int valueToY(double value) const {
//         if (m_scores.isEmpty()) return 0;
//         double maxValue = 0;

//         // 计算直方图最大频率
//         double min = *std::min_element(m_scores.begin(), m_scores.end()) - 5;
//         double max = *std::max_element(m_scores.begin(), m_scores.end()) + 5;
//         double binWidth = (max - min) / m_binCount;
//         for (int i = 0; i < m_binCount; ++i) {
//             double binMin = min + i * binWidth;
//             double binMax = binMin + binWidth;
//             int count = 0;
//             for (double s : m_scores) if (s >= binMin && s < binMax) count++;
//             maxValue = qMax(maxValue, (double)count / m_scores.size());
//         }

//         // 对比正态分布最大值
//         maxValue = qMax(maxValue, normalDensity(m_mean)) * 1.2;

//         // 转换为Y坐标（上下边距各50）
//         return height() - 50 - (value / maxValue) * (height() - 100);
//     }
// };

// #endif // NORMALDISTRIBUTIONPLOT_H
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
