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

