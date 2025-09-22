// #include "scorepiechart.h"

// Scorepiechart::Scorepiechart() {}
#include "scorepiechart.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QtMath>
#include <QResizeEvent>

ScorePieChart::ScorePieChart(QWidget *parent)
    : QWidget(parent)
{
    // 设置颜色方案（5个分数段对应5种颜色）
    m_colors = {
        Qt::red,      // 0-59
        QColorConstants::Svg::orange,   // 60-69
        Qt::yellow,   // 70-79
        Qt::green,    // 80-89
        Qt::blue      // 90-100
    };
    // 设置分数段标签
    m_labels = {
        "0-59分",
        "60-69分",
        "70-79分",
        "80-89分",
        "90-100分"
    };
    setMinimumSize(300, 300); // 最小尺寸
}

// 重写大小变化事件，触发重绘
void ScorePieChart::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    update();
}

// 设置成绩数据并计算各分数段人数
void ScorePieChart::setScores(const QVector<double> &scores)
{
    m_scores = scores;
    m_counts.fill(0, 5); // 初始化5个分数段的计数为0

    // 统计各分数段人数
    for (double score : m_scores) {
        if (score < 60) {
            m_counts[0]++;
        } else if (score < 70) {
            m_counts[1]++;
        } else if (score < 80) {
            m_counts[2]++;
        } else if (score < 90) {
            m_counts[3]++;
        } else {
            m_counts[4]++;
        }
    }
    update(); // 触发重绘
}

// 绘制饼图和图例
void ScorePieChart::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // 抗锯齿

    // 无数据时显示提示
    if (m_scores.isEmpty()) {
        painter.drawText(rect(), Qt::AlignCenter, "无成绩数据");
        return;
    }

    // 1. 计算总人数和各部分角度
    int total = m_scores.size();
    QVector<int> angles(5); // 存储每个扇形的角度（1/16度为单位）
    for (int i = 0; i < 5; ++i) {
        angles[i] = qRound((double)m_counts[i] / total * 360 * 16); // 转换为1/16度
    }

    // 2. 绘制饼图（居中显示，直径为控件最小边的80%）
    int pieSize = qMin(width(), height()) * 0.8; // 饼图大小
    int pieX = (width() - pieSize) / 2;         // 饼图X坐标
    int pieY = (height() - pieSize) / 2;         // 饼图Y坐标
    QRect pieRect(pieX, pieY, pieSize, pieSize); // 饼图区域

    int startAngle = 0; // 起始角度
    for (int i = 0; i < 5; ++i) {
        if (angles[i] <= 0) continue; // 跳过无数据的分数段
        painter.setBrush(QBrush(m_colors[i]));
        painter.setPen(QPen(Qt::black, 1));
        painter.drawPie(pieRect, startAngle, angles[i]); // 绘制扇形
        startAngle += angles[i]; // 更新起始角度
    }

    // 3. 绘制图例（右侧或下方，根据控件宽高判断）
    drawLegend(&painter, pieSize);
}

// 绘制图例（显示分数段、人数和占比）
void ScorePieChart::drawLegend(QPainter *painter, int pieSize)
{
    int total = m_scores.size();
    if (total == 0) return;

    // 图例位置：宽>高则放右侧，否则放下侧
    bool drawOnRight = width() > height();
    int legendX = drawOnRight ? (width() - pieSize/2 - 100) : (width()/2 - 100);
    int legendY = drawOnRight ? (height()/2 - 80) : (pieSize + 30);

    // 绘制每个分数段的图例项
    for (int i = 0; i < 5; ++i) {
        if (m_counts[i] == 0) continue;

        // 1. 绘制颜色方块
        painter->setBrush(QBrush(m_colors[i]));
        painter->setPen(QPen(Qt::black, 1));
        painter->drawRect(legendX, legendY + i*30, 20, 20);

        // 2. 绘制标签和数据（人数和占比）
        QString text = QString("%1: %2人 (%3%)")
                           .arg(m_labels[i])
                           .arg(m_counts[i])
                           .arg(qRound((double)m_counts[i]/total*100));
        painter->setPen(Qt::black);
        painter->drawText(legendX + 30, legendY + i*30 + 15, text);
    }
}
