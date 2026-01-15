#ifndef STATISTICSWIDGET_H
#define STATISTICSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QList>
#include <QPair>
#include <QString>

QT_BEGIN_NAMESPACE
class QChart;
class QChartView;
QT_END_NAMESPACE

/**
 * @brief 统计图表组件
 * 使用Qt Charts显示成绩统计图表
 */
class StatisticsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticsWidget(QWidget* parent = nullptr);
    
    // 更新图表数据
    void updateStudentRankingChart(const QList<QPair<QString, double>>& ranking);
    void updateCourseStatisticsChart(const QList<QPair<QString, double>>& statistics);
    void updateGradeTrendChart(const QList<QPair<QString, double>>& trend);

private slots:
    void onChartTypeChanged(int index);

private:
    void setupUI();
    void clearChart();
    
    QVBoxLayout* m_mainLayout;
    QHBoxLayout* m_controlLayout;
    QComboBox* m_chartTypeCombo;
    QChartView* m_chartView;
    QChart* m_chart;
    
    QList<QPair<QString, double>> m_currentData;
};

#endif // STATISTICSWIDGET_H
