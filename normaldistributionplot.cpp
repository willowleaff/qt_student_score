
#include "normaldistributionplot.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QtMath>
#include <QResizeEvent>

NormalDistributionPlot::NormalDistributionPlot(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(400, 300); // 设置最小尺寸，避免过小导致图形错乱
}

// 1. 监听画布大小变化，触发重绘
void NormalDistributionPlot::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    update(); // 尺寸变化时自动重绘
}

// 2. 计算有效绘图区域（扣除边距）：边距为画布宽高的5%
QRectF NormalDistributionPlot::getPlotRect() const
{
    int marginW = width() * 0.05;  // 宽度边距（5%）
    int marginH = height() * 0.05; // 高度边距（5%）
    // 有效绘图区域：左上角(marginW, marginH)，右下角(width()-marginW, height()-marginH)
    return QRectF(marginW, marginH,
                  width() - 2 * marginW,
                  height() - 2 * marginH);
}

// 3. 设置成绩数据时，重新计算均值和标准差
void NormalDistributionPlot::setScores(const QVector<double> &scores)
{
    m_scores = scores;
    m_mean = calculateMean();
    m_stdDev = calculateStdDev();
    update(); // 数据变化触发重绘
}

// 计算均值
double NormalDistributionPlot::calculateMean() const
{
    if (m_scores.isEmpty()) return 0;
    double sum = 0;
    for (double s : m_scores) sum += s;
    return sum / m_scores.size();
}

// 计算标准差
double NormalDistributionPlot::calculateStdDev() const
{
    if (m_scores.size() < 2) return 0;
    double sum = 0;
    for (double s : m_scores) sum += qPow(s - m_mean, 2);
    return qSqrt(sum / (m_scores.size() - 1));
}

// 正态分布概率密度函数
double NormalDistributionPlot::normalDensity(double x) const
{
    if (m_stdDev == 0) return 0;
    double exponent = -qPow(x - m_mean, 2) / (2 * qPow(m_stdDev, 2));
    return (1 / (m_stdDev * qSqrt(2 * M_PI))) * qExp(exponent);
}

// 4. 成绩→X坐标：动态关联有效绘图区域宽度
int NormalDistributionPlot::scoreToX(double score) const
{
    if (m_scores.isEmpty()) return 0;
    QRectF plotRect = getPlotRect(); // 实时获取有效区域

    // 成绩范围：min-5到max+5（留余量）
    double minScore = *std::min_element(m_scores.begin(), m_scores.end()) - 5;
    double maxScore = *std::max_element(m_scores.begin(), m_scores.end()) + 5;
    double scoreRange = maxScore - minScore;
    if (scoreRange < 0.1) scoreRange = 0.1; // 避免除以0

    // 映射公式：X = 有效区域左边界 + (成绩-最小成绩)/成绩范围 * 有效区域宽度
    return plotRect.left() + (score - minScore) / scoreRange * plotRect.width();
}

// 5. 频率/密度→Y坐标：动态关联有效绘图区域高度
int NormalDistributionPlot::valueToY(double value) const
{
    if (m_scores.isEmpty()) return 0;
    QRectF plotRect = getPlotRect(); // 实时获取有效区域

    // 计算Y轴最大值（实际频率和理论密度的最大值，留20%余量）
    double maxValue = 0;
    // 步骤1：计算直方图的最大频率
    double minScore = *std::min_element(m_scores.begin(), m_scores.end()) - 5;
    double maxScore = *std::max_element(m_scores.begin(), m_scores.end()) + 5;
    double binWidth = (maxScore - minScore) / m_binCount;
    for (int i = 0; i < m_binCount; ++i) {
        double binMin = minScore + i * binWidth;
        double binMax = binMin + binWidth;
        int count = 0;
        for (double s : m_scores) if (s >= binMin && s < binMax) count++;
        double freq = (double)count / m_scores.size();
        if (freq > maxValue) maxValue = freq;
    }
    // 步骤2：计算理论正态分布的最大密度（均值处密度最大）
    double maxDensity = normalDensity(m_mean);
    maxValue = qMax(maxValue, maxDensity) * 1.2; // 留20%余量，避免图形超出
    if (maxValue < 0.01) maxValue = 0.01; // 避免除以0

    // 映射公式：Y = 有效区域下边界 - (值/最大值) * 有效区域高度（Y轴向上为正）
    return plotRect.bottom() - (value / maxValue) * plotRect.height();
}

// 6. 核心绘图逻辑（全动态关联画布尺寸）
void NormalDistributionPlot::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // 抗锯齿
    painter.fillRect(rect(), Qt::white); // 白色背景

    // 无数据时显示提示
    if (m_scores.isEmpty()) {
        painter.drawText(rect(), Qt::AlignCenter, "无数学成绩数据");
        return;
    }

    QRectF plotRect = getPlotRect(); // 实时获取有效绘图区域
    int marginW = plotRect.left();   // 实际边距（宽度方向）
    int marginH = plotRect.top();    // 实际边距（高度方向）

    // 1. 绘制坐标轴（关联有效区域）
    QPen axisPen(Qt::black, 2);
    painter.setPen(axisPen);
    // X轴：有效区域下边界（从左到右）
    painter.drawLine(plotRect.bottomLeft(), plotRect.bottomRight());
    // Y轴：有效区域左边界（从上到下）
    painter.drawLine(plotRect.topLeft(), plotRect.bottomLeft());

    // 2. 绘制坐标轴标签（位置随边距动态变化）
    painter.drawText(plotRect.bottomRight() + QPointF(-30, -10), "成绩"); // X轴标签
    painter.save();
    painter.rotate(-90);
    // Y轴标签：垂直居中，位置随边距变化
    painter.drawText(-(plotRect.top() + plotRect.height()/2), marginW - 10, "频率/概率密度");
    painter.restore();

    // 3. 绘制直方图（柱宽随画布宽度自适应）
    double minScore = *std::min_element(m_scores.begin(), m_scores.end()) - 5;
    double maxScore = *std::max_element(m_scores.begin(), m_scores.end()) + 5;
    double binWidth = (maxScore - minScore) / m_binCount;
    painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
    painter.setPen(QPen(Qt::black, 1));

    for (int i = 0; i < m_binCount; ++i) {
        double binMin = minScore + i * binWidth;
        double binMax = binMin + binWidth;
        // 统计区间内成绩数量
        int count = 0;
        for (double s : m_scores) if (s >= binMin && s < binMax) count++;
        double frequency = (double)count / m_scores.size();

        // 动态计算柱形坐标和尺寸
        int x = scoreToX(binMin);
        int y = valueToY(frequency);
        int w = scoreToX(binMax) - x - 2; // 减2避免柱形重叠（随画布宽度自适应）
        int h = plotRect.bottom() - y;    // 柱高关联有效区域下边界

        if (w > 0) painter.drawRect(x, y, w, h); // 确保宽度为正才绘制
    }

    // 4. 绘制正态分布曲线（100个点，随画布宽度自适应）
    QPen curvePen(Qt::blue, 2);
    painter.setPen(curvePen);
    painter.setBrush(Qt::NoBrush);

    QVector<QPointF> points;
    for (int i = 0; i <= 100; ++i) {
        double x = minScore + (maxScore - minScore) * i / 100;
        double density = normalDensity(x);
        points.append(QPointF(scoreToX(x), valueToY(density)));
    }
    painter.drawPolyline(points.data(), points.size());

    // 5. 绘制均值线和标注（位置随画布自适应）
    QPen meanPen(Qt::green, 1, Qt::DashLine);
    painter.setPen(meanPen);
    int meanX = scoreToX(m_mean);
    // 均值线：从有效区域顶部到底部
    painter.drawLine(meanX, plotRect.top(), meanX, plotRect.bottom());
    // 均值标注：位置随均值线和边距动态调整
    painter.drawText(meanX + 5, plotRect.bottom() + marginH/2,
                     QString("均值: %.1f  标准差: %.1f").arg(m_mean).arg(m_stdDev));
}
