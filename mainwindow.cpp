#include "MainWindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QHeaderView>
#include <QFormLayout>
#include <QGridLayout>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QApplication>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_studentModel(nullptr)
    , m_gradeModel(nullptr)
{
    // 初始化数据库
    m_dbManager = &DatabaseManager::getInstance();
    if (!m_dbManager->initializeDatabase()) {
        QMessageBox::critical(this, "错误", "数据库初始化失败！");
    }

    // 初始化文件管理
    m_fileManager = new FileManager(this);

    setupUI();
    setupMenuBar();
    setupStatusBar();
    connectSignals();
    refreshAllData();

    setWindowTitle("学生成绩管理系统");
    resize(1200, 800);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    m_tabWidget = new QTabWidget(this);
    setCentralWidget(m_tabWidget);

    setupStudentTab();
    setupCourseTab();
    setupGradeTab();
    setupStatisticsTab();
}

void MainWindow::setupStudentTab()
{
    m_studentTab = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(m_studentTab);

    // 表格视图
    m_studentTable = new QTableView();
    m_studentModel = new StudentModel(this);
    m_studentTable->setModel(m_studentModel);
    m_studentTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_studentTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_studentTable->horizontalHeader()->setStretchLastSection(true);
    m_studentTable->setAlternatingRowColors(true);

    mainLayout->addWidget(m_studentTable);

    // 操作区域
    QGroupBox* operationGroup = new QGroupBox("学生信息操作");
    QFormLayout* formLayout = new QFormLayout(operationGroup);

    m_studentIdEdit = new QLineEdit();
    m_studentNameEdit = new QLineEdit();
    m_classNameEdit = new QLineEdit();

    formLayout->addRow("学号:", m_studentIdEdit);
    formLayout->addRow("姓名:", m_studentNameEdit);
    formLayout->addRow("班级:", m_classNameEdit);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* addBtn = new QPushButton("添加");
    QPushButton* updateBtn = new QPushButton("更新");
    QPushButton* deleteBtn = new QPushButton("删除");
    QPushButton* refreshBtn = new QPushButton("刷新");

    connect(addBtn, &QPushButton::clicked, this, &MainWindow::onAddStudent);
    connect(updateBtn, &QPushButton::clicked, this, &MainWindow::onUpdateStudent);
    connect(deleteBtn, &QPushButton::clicked, this, &MainWindow::onDeleteStudent);
    connect(refreshBtn, &QPushButton::clicked, this, &MainWindow::onRefreshStudents);

    buttonLayout->addWidget(addBtn);
    buttonLayout->addWidget(updateBtn);
    buttonLayout->addWidget(deleteBtn);
    buttonLayout->addWidget(refreshBtn);
    buttonLayout->addStretch();

    formLayout->addRow(buttonLayout);
    mainLayout->addWidget(operationGroup);

    m_tabWidget->addTab(m_studentTab, "学生管理");
}

void MainWindow::setupCourseTab()
{
    m_courseTab = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(m_courseTab);

    // 表格视图（使用标准模型）
    m_courseTable = new QTableView();
    QStandardItemModel* courseModel = new QStandardItemModel(this);
    courseModel->setHorizontalHeaderLabels(QStringList() << "课程编号" << "课程名称" << "学分");
    m_courseTable->setModel(courseModel);
    m_courseTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_courseTable->horizontalHeader()->setStretchLastSection(true);
    m_courseTable->setAlternatingRowColors(true);

    mainLayout->addWidget(m_courseTable);

    // 操作区域
    QGroupBox* operationGroup = new QGroupBox("课程信息操作");
    QFormLayout* formLayout = new QFormLayout(operationGroup);

    m_courseIdEdit = new QLineEdit();
    m_courseNameEdit = new QLineEdit();
    m_creditsEdit = new QLineEdit();

    formLayout->addRow("课程编号:", m_courseIdEdit);
    formLayout->addRow("课程名称:", m_courseNameEdit);
    formLayout->addRow("学分:", m_creditsEdit);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* addBtn = new QPushButton("添加");
    QPushButton* updateBtn = new QPushButton("更新");
    QPushButton* deleteBtn = new QPushButton("删除");
    QPushButton* refreshBtn = new QPushButton("刷新");

    connect(addBtn, &QPushButton::clicked, this, &MainWindow::onAddCourse);
    connect(updateBtn, &QPushButton::clicked, this, &MainWindow::onUpdateCourse);
    connect(deleteBtn, &QPushButton::clicked, this, &MainWindow::onDeleteCourse);
    connect(refreshBtn, &QPushButton::clicked, this, &MainWindow::onRefreshCourses);

    buttonLayout->addWidget(addBtn);
    buttonLayout->addWidget(updateBtn);
    buttonLayout->addWidget(deleteBtn);
    buttonLayout->addWidget(refreshBtn);
    buttonLayout->addStretch();

    formLayout->addRow(buttonLayout);
    mainLayout->addWidget(operationGroup);

    m_tabWidget->addTab(m_courseTab, "课程管理");
}

void MainWindow::setupGradeTab()
{
    m_gradeTab = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(m_gradeTab);

    // 筛选区域
    QGroupBox* filterGroup = new QGroupBox("筛选条件");
    QHBoxLayout* filterLayout = new QHBoxLayout(filterGroup);

    filterLayout->addWidget(new QLabel("学生:"));
    m_filterStudentCombo = new QComboBox();
    m_filterStudentCombo->addItem("全部");
    m_filterStudentCombo->setEditable(false);

    filterLayout->addWidget(new QLabel("课程:"));
    m_filterCourseCombo = new QComboBox();
    m_filterCourseCombo->addItem("全部");
    m_filterCourseCombo->setEditable(false);

    QPushButton* filterBtn = new QPushButton("筛选");
    connect(filterBtn, &QPushButton::clicked, this, &MainWindow::onFilterGrades);

    filterLayout->addWidget(m_filterStudentCombo);
    filterLayout->addWidget(m_filterCourseCombo);
    filterLayout->addWidget(filterBtn);
    filterLayout->addStretch();

    mainLayout->addWidget(filterGroup);

    // 表格视图
    m_gradeTable = new QTableView();
    m_gradeModel = new GradeModel(this);
    m_gradeTable->setModel(m_gradeModel);
    m_gradeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_gradeTable->horizontalHeader()->setStretchLastSection(true);
    m_gradeTable->setAlternatingRowColors(true);

    mainLayout->addWidget(m_gradeTable);

    // 操作区域
    QGroupBox* operationGroup = new QGroupBox("成绩信息操作");
    QFormLayout* formLayout = new QFormLayout(operationGroup);

    m_gradeStudentCombo = new QComboBox();
    m_gradeCourseCombo = new QComboBox();
    m_scoreEdit = new QLineEdit();
    m_semesterEdit = new QLineEdit();

    formLayout->addRow("学生:", m_gradeStudentCombo);
    formLayout->addRow("课程:", m_gradeCourseCombo);
    formLayout->addRow("成绩:", m_scoreEdit);
    formLayout->addRow("学期:", m_semesterEdit);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* addBtn = new QPushButton("添加");
    QPushButton* updateBtn = new QPushButton("更新");
    QPushButton* deleteBtn = new QPushButton("删除");
    QPushButton* refreshBtn = new QPushButton("刷新");

    connect(addBtn, &QPushButton::clicked, this, &MainWindow::onAddGrade);
    connect(updateBtn, &QPushButton::clicked, this, &MainWindow::onUpdateGrade);
    connect(deleteBtn, &QPushButton::clicked, this, &MainWindow::onDeleteGrade);
    connect(refreshBtn, &QPushButton::clicked, this, &MainWindow::onRefreshGrades);

    buttonLayout->addWidget(addBtn);
    buttonLayout->addWidget(updateBtn);
    buttonLayout->addWidget(deleteBtn);
    buttonLayout->addWidget(refreshBtn);
    buttonLayout->addStretch();

    formLayout->addRow(buttonLayout);
    mainLayout->addWidget(operationGroup);

    m_tabWidget->addTab(m_gradeTab, "成绩管理");
}

void MainWindow::setupStatisticsTab()
{
    m_statisticsWidget = new StatisticsWidget();
    m_tabWidget->addTab(m_statisticsWidget, "统计分析");
}

void MainWindow::setupMenuBar()
{
    QMenu* fileMenu = menuBar()->addMenu("文件");

    QAction* importStudentsAction = fileMenu->addAction("导入学生数据");
    QAction* importCoursesAction = fileMenu->addAction("导入课程数据");
    QAction* importGradesAction = fileMenu->addAction("导入成绩数据");
    fileMenu->addSeparator();
    QAction* exportAction = fileMenu->addAction("导出数据");
    fileMenu->addSeparator();
    QAction* exitAction = fileMenu->addAction("退出");

    connect(importStudentsAction, &QAction::triggered, this, &MainWindow::onImportStudents);
    connect(importCoursesAction, &QAction::triggered, this, &MainWindow::onImportCourses);
    connect(importGradesAction, &QAction::triggered, this, &MainWindow::onImportGrades);
    connect(exportAction, &QAction::triggered, this, &MainWindow::onExportData);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    QMenu* networkMenu = menuBar()->addMenu("网络");
    QAction* syncAction = networkMenu->addAction("同步数据");
    connect(syncAction, &QAction::triggered, this, &MainWindow::onSyncData);

    QMenu* helpMenu = menuBar()->addMenu("帮助");
    QAction* aboutAction = helpMenu->addAction("关于");
    connect(aboutAction, &QAction::triggered, [this]() {
        QMessageBox::about(this, "关于", "学生成绩管理系统 v1.0\nQt课程设计项目");
    });
}

void MainWindow::setupStatusBar()
{
    m_statusLabel = new QLabel("就绪");
    statusBar()->addWidget(m_statusLabel);

    m_progressBar = new QProgressBar();
    m_progressBar->setVisible(false);
    statusBar()->addPermanentWidget(m_progressBar);
}

void MainWindow::connectSignals()
{
    // 信号槽连接（如需要）
}

void MainWindow::refreshAllData()
{
    onRefreshStudents();
    onRefreshCourses();
    onRefreshGrades();
    onUpdateStatistics();
}

