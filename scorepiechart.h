// #ifndef SCOREPIECHART_H
// #define SCOREPIECHART_H

// class Scorepiechart
// {
// public:
//     Scorepiechart();
// };

// #endif // SCOREPIECHART_H
#ifndef SCOREPIECHART_H
#define SCOREPIECHART_H

#include <QWidget>
#include <QVector>
#include <QColor>

class ScorePieChart : public QWidget
{
    Q_OBJECT

public:
    explicit ScorePieChart(QWidget *parent = nullptr);
    // 设置成绩数据，自动统计分数段
    void setScores(const QVector<double> &scores);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QVector<double> m_scores;       // 成绩数据
    QVector<int> m_counts{5, 0};    // 各分数段人数（0-59到90-100）
    QVector<QColor> m_colors;       // 各分数段颜色
    QVector<QString> m_labels;      // 各分数段标签
    // 绘制图例
    void drawLegend(QPainter *painter, int pieSize);
};

#endif // SCOREPIECHART_H
