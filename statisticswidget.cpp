#include "StatisticsWidget.h"
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>
#include <QPainter>
#include <QDebug>

StatisticsWidget::StatisticsWidget(QWidget* parent)
    : QWidget(parent)
    , m_chart(nullptr)
{
    setupUI();
}

void StatisticsWidget::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    
    // 控制栏
    m_controlLayout = new QHBoxLayout();
    m_controlLayout->addWidget(new QLabel("图表类型:"));
    
    m_chartTypeCombo = new QComboBox(this);
    m_chartTypeCombo->addItem("柱状图");
    m_chartTypeCombo->addItem("折线图");
    connect(m_chartTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &StatisticsWidget::onChartTypeChanged);
    
    m_controlLayout->addWidget(m_chartTypeCombo);
    m_controlLayout->addStretch();
    
    m_mainLayout->addLayout(m_controlLayout);
    
    // 图表视图
    m_chartView = new QChartView(this);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_mainLayout->addWidget(m_chartView);
}

void StatisticsWidget::updateStudentRankingChart(const QList<QPair<QString, double>>& ranking)
{
    m_currentData = ranking;
    
    if (m_chartTypeCombo->currentIndex() == 0) {
        // 柱状图
        clearChart();
        m_chart = new QChart();
        m_chart->setTitle("学生成绩排名");
        
        QBarSeries* series = new QBarSeries();
        QBarSet* set = new QBarSet("平均成绩");
        
        QStringList categories;
        for (const auto& pair : ranking) {
            *set << pair.second;
            categories << pair.first;
        }
        
        series->append(set);
        m_chart->addSeries(series);
        
        QBarCategoryAxis* axisX = new QBarCategoryAxis();
        axisX->append(categories);
        m_chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);
        
        QValueAxis* axisY = new QValueAxis();
        axisY->setRange(0, 100);
        axisY->setTitleText("成绩");
        m_chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);
        
        m_chartView->setChart(m_chart);
    } else {
        // 折线图
        clearChart();
        m_chart = new QChart();
        m_chart->setTitle("学生成绩排名趋势");
        
        QLineSeries* series = new QLineSeries();
        series->setName("平均成绩");
        
        int index = 0;
        for (const auto& pair : ranking) {
            series->append(index, pair.second);
            index++;
        }
        
        m_chart->addSeries(series);
        
        QValueAxis* axisX = new QValueAxis();
        axisX->setRange(0, ranking.size() - 1);
        axisX->setTitleText("排名");
        m_chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);
        
        QValueAxis* axisY = new QValueAxis();
        axisY->setRange(0, 100);
        axisY->setTitleText("成绩");
        m_chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);
        
        m_chartView->setChart(m_chart);
    }
}

void StatisticsWidget::updateCourseStatisticsChart(const QList<QPair<QString, double>>& statistics)
{
    m_currentData = statistics;
    
    if (m_chartTypeCombo->currentIndex() == 0) {
        // 柱状图
        clearChart();
        m_chart = new QChart();
        m_chart->setTitle("课程平均成绩统计");
        
        QBarSeries* series = new QBarSeries();
        QBarSet* set = new QBarSet("平均成绩");
        
        QStringList categories;
        for (const auto& pair : statistics) {
            *set << pair.second;
            categories << pair.first;
        }
        
        series->append(set);
        m_chart->addSeries(series);
        
        QBarCategoryAxis* axisX = new QBarCategoryAxis();
        axisX->append(categories);
        m_chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);
        
        QValueAxis* axisY = new QValueAxis();
        axisY->setRange(0, 100);
        axisY->setTitleText("成绩");
        m_chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);
        
        m_chartView->setChart(m_chart);
    } else {
        // 折线图
        clearChart();
        m_chart = new QChart();
        m_chart->setTitle("课程平均成绩趋势");
        
        QLineSeries* series = new QLineSeries();
        series->setName("平均成绩");
        
        int index = 0;
        for (const auto& pair : statistics) {
            series->append(index, pair.second);
            index++;
        }
        
        m_chart->addSeries(series);
        
        QValueAxis* axisX = new QValueAxis();
        axisX->setRange(0, statistics.size() - 1);
        axisX->setTitleText("课程");
        m_chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);
        
        QValueAxis* axisY = new QValueAxis();
        axisY->setRange(0, 100);
        axisY->setTitleText("成绩");
        m_chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);
        
        m_chartView->setChart(m_chart);
    }
}

void StatisticsWidget::updateGradeTrendChart(const QList<QPair<QString, double>>& trend)
{
    m_currentData = trend;
    
    clearChart();
    m_chart = new QChart();
    m_chart->setTitle("成绩变化趋势");
    
    QLineSeries* series = new QLineSeries();
    series->setName("成绩");
    
    int index = 0;
    for (const auto& pair : trend) {
        series->append(index, pair.second);
        index++;
    }
    
    m_chart->addSeries(series);
    
    QValueAxis* axisX = new QValueAxis();
    axisX->setRange(0, trend.size() - 1);
    axisX->setTitleText("学期");
    m_chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    
    QValueAxis* axisY = new QValueAxis();
    axisY->setRange(0, 100);
    axisY->setTitleText("成绩");
    m_chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    
    m_chartView->setChart(m_chart);
}

void StatisticsWidget::onChartTypeChanged(int index)
{
    // 根据当前数据类型重新绘制图表
    if (!m_currentData.isEmpty()) {
        // 简单处理：如果是学生排名数据，重新绘制
        updateStudentRankingChart(m_currentData);
    }
}

void StatisticsWidget::clearChart()
{
    if (m_chart) {
        m_chartView->setChart(nullptr);
        delete m_chart;
        m_chart = nullptr;
    }
}
