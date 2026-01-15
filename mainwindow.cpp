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
        QMessageBox::about(this, "关于", "学生成绩管理系统 1.0（制作人：李晟舒）");
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

// 学生管理槽函数
void MainWindow::onAddStudent()
{
    QString studentId = m_studentIdEdit->text().trimmed();
    QString name = m_studentNameEdit->text().trimmed();
    QString className = m_classNameEdit->text().trimmed();

    if (studentId.isEmpty() || name.isEmpty() || className.isEmpty()) {
        QMessageBox::warning(this, "警告", "请填写完整的学生信息！");
        return;
    }

    if (m_dbManager->addStudent(studentId, name, className)) {
        QMessageBox::information(this, "成功", "学生添加成功！");
        m_studentIdEdit->clear();
        m_studentNameEdit->clear();
        m_classNameEdit->clear();
        onRefreshStudents();
    } else {
        QMessageBox::critical(this, "错误", "学生添加失败！");
    }
}

void MainWindow::onUpdateStudent()
{
    QModelIndexList selected = m_studentTable->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "警告", "请选择要更新的学生！");
        return;
    }

    int row = selected.first().row();
    QStringList student = m_studentModel->getStudent(row);
    QString oldStudentId = student[0];

    QString studentId = m_studentIdEdit->text().trimmed();
    QString name = m_studentNameEdit->text().trimmed();
    QString className = m_classNameEdit->text().trimmed();

    if (studentId.isEmpty() || name.isEmpty() || className.isEmpty()) {
        QMessageBox::warning(this, "警告", "请填写完整的学生信息！");
        return;
    }

    if (m_dbManager->updateStudent(oldStudentId, name, className)) {
        QMessageBox::information(this, "成功", "学生更新成功！");
        onRefreshStudents();
    } else {
        QMessageBox::critical(this, "错误", "学生更新失败！");
    }
}

void MainWindow::onDeleteStudent()
{
    QModelIndexList selected = m_studentTable->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "警告", "请选择要删除的学生！");
        return;
    }

    int ret = QMessageBox::question(this, "确认", "确定要删除选中的学生吗？",
                                    QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        int row = selected.first().row();
        QStringList student = m_studentModel->getStudent(row);
        QString studentId = student[0];

        if (m_dbManager->deleteStudent(studentId)) {
            QMessageBox::information(this, "成功", "学生删除成功！");
            onRefreshStudents();
        } else {
            QMessageBox::critical(this, "错误", "学生删除失败！");
        }
    }
}

void MainWindow::onRefreshStudents()
{
    QList<QStringList> students = m_dbManager->getAllStudents();
    m_studentModel->setStudentData(students);
    m_statusLabel->setText(QString("已加载 %1 名学生").arg(students.size()));

    // 更新下拉框
    m_gradeStudentCombo->clear();
    m_gradeStudentCombo->addItem("请选择");
    m_filterStudentCombo->clear();
    m_filterStudentCombo->addItem("全部");

    for (const QStringList& student : students) {
        QString display = QString("%1 - %2").arg(student[0], student[1]);
        m_gradeStudentCombo->addItem(display, student[0]);
        m_filterStudentCombo->addItem(display, student[0]);
    }
}

// 课程管理槽函数
void MainWindow::onAddCourse()
{
    QString courseId = m_courseIdEdit->text().trimmed();
    QString courseName = m_courseNameEdit->text().trimmed();
    QString creditsStr = m_creditsEdit->text().trimmed();

    if (courseId.isEmpty() || courseName.isEmpty() || creditsStr.isEmpty()) {
        QMessageBox::warning(this, "警告", "请填写完整的课程信息！");
        return;
    }

    bool ok;
    int credits = creditsStr.toInt(&ok);
    if (!ok || credits <= 0) {
        QMessageBox::warning(this, "警告", "学分必须是正整数！");
        return;
    }

    if (m_dbManager->addCourse(courseId, courseName, credits)) {
        QMessageBox::information(this, "成功", "课程添加成功！");
        m_courseIdEdit->clear();
        m_courseNameEdit->clear();
        m_creditsEdit->clear();
        onRefreshCourses();
    } else {
        QMessageBox::critical(this, "错误", "课程添加失败！");
    }
}

void MainWindow::onUpdateCourse()
{
    QModelIndexList selected = m_courseTable->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "警告", "请选择要更新的课程！");
        return;
    }

    int row = selected.first().row();
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(m_courseTable->model());
    QString oldCourseId = model->item(row, 0)->text();

    QString courseId = m_courseIdEdit->text().trimmed();
    QString courseName = m_courseNameEdit->text().trimmed();
    QString creditsStr = m_creditsEdit->text().trimmed();

    if (courseId.isEmpty() || courseName.isEmpty() || creditsStr.isEmpty()) {
        QMessageBox::warning(this, "警告", "请填写完整的课程信息！");
        return;
    }

    bool ok;
    int credits = creditsStr.toInt(&ok);
    if (!ok || credits <= 0) {
        QMessageBox::warning(this, "警告", "学分必须是正整数！");
        return;
    }

    if (m_dbManager->updateCourse(oldCourseId, courseName, credits)) {
        QMessageBox::information(this, "成功", "课程更新成功！");
        onRefreshCourses();
    } else {
        QMessageBox::critical(this, "错误", "课程更新失败！");
    }
}

void MainWindow::onDeleteCourse()
{
    QModelIndexList selected = m_courseTable->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "警告", "请选择要删除的课程！");
        return;
    }

    int ret = QMessageBox::question(this, "确认", "确定要删除选中的课程吗？",
                                    QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        QStandardItemModel* model = qobject_cast<QStandardItemModel*>(m_courseTable->model());
        int row = selected.first().row();
        QString courseId = model->item(row, 0)->text();

        if (m_dbManager->deleteCourse(courseId)) {
            QMessageBox::information(this, "成功", "课程删除成功！");
            onRefreshCourses();
        } else {
            QMessageBox::critical(this, "错误", "课程删除失败！");
        }
    }
}

void MainWindow::onRefreshCourses()
{
    QList<QStringList> courses = m_dbManager->getAllCourses();
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(m_courseTable->model());
    model->setRowCount(0);

    for (const QStringList& course : courses) {
        QList<QStandardItem*> items;
        items << new QStandardItem(course[0]);
        items << new QStandardItem(course[1]);
        items << new QStandardItem(course[2]);
        model->appendRow(items);
    }

    m_statusLabel->setText(QString("已加载 %1 门课程").arg(courses.size()));

    // 更新下拉框
    m_gradeCourseCombo->clear();
    m_gradeCourseCombo->addItem("请选择");
    m_filterCourseCombo->clear();
    m_filterCourseCombo->addItem("全部");

    for (const QStringList& course : courses) {
        QString display = QString("%1 - %2").arg(course[0], course[1]);
        m_gradeCourseCombo->addItem(display, course[0]);
        m_filterCourseCombo->addItem(display, course[0]);
    }
}

// 成绩管理槽函数
void MainWindow::onAddGrade()
{
    if (m_gradeStudentCombo->currentIndex() == 0 || m_gradeCourseCombo->currentIndex() == 0) {
        QMessageBox::warning(this, "警告", "请选择学生和课程！");
        return;
    }

    QString studentId = m_gradeStudentCombo->currentData().toString();
    QString courseId = m_gradeCourseCombo->currentData().toString();
    QString scoreStr = m_scoreEdit->text().trimmed();
    QString semester = m_semesterEdit->text().trimmed();

    if (scoreStr.isEmpty() || semester.isEmpty()) {
        QMessageBox::warning(this, "警告", "请填写完整的成绩信息！");
        return;
    }

    bool ok;
    double score = scoreStr.toDouble(&ok);
    if (!ok || score < 0 || score > 100) {
        QMessageBox::warning(this, "警告", "成绩必须是0-100之间的数字！");
        return;
    }

    if (m_dbManager->addGrade(studentId, courseId, score, semester)) {
        QMessageBox::information(this, "成功", "成绩添加成功！");
        m_scoreEdit->clear();
        m_semesterEdit->clear();
        onRefreshGrades();
        onUpdateStatistics();
    } else {
        QMessageBox::critical(this, "错误", "成绩添加失败！");
    }
}

void MainWindow::onUpdateGrade()
{
    QModelIndexList selected = m_gradeTable->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "警告", "请选择要更新的成绩！");
        return;
    }

    int row = selected.first().row();
    QStringList grade = m_gradeModel->getGrade(row);

    QString studentId = grade[0];
    QString courseId = grade[2];
    QString semester = grade[5];
    QString scoreStr = m_scoreEdit->text().trimmed();

    if (scoreStr.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入成绩！");
        return;
    }

    bool ok;
    double score = scoreStr.toDouble(&ok);
    if (!ok || score < 0 || score > 100) {
        QMessageBox::warning(this, "警告", "成绩必须是0-100之间的数字！");
        return;
    }

    if (m_dbManager->updateGrade(studentId, courseId, score, semester)) {
        QMessageBox::information(this, "成功", "成绩更新成功！");
        onRefreshGrades();
        onUpdateStatistics();
    } else {
        QMessageBox::critical(this, "错误", "成绩更新失败！");
    }
}

void MainWindow::onDeleteGrade()
{
    QModelIndexList selected = m_gradeTable->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "警告", "请选择要删除的成绩！");
        return;
    }

    int ret = QMessageBox::question(this, "确认", "确定要删除选中的成绩吗？",
                                    QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        int row = selected.first().row();
        QStringList grade = m_gradeModel->getGrade(row);

        QString studentId = grade[0];
        QString courseId = grade[2];
        QString semester = grade[5];

        if (m_dbManager->deleteGrade(studentId, courseId, semester)) {
            QMessageBox::information(this, "成功", "成绩删除成功！");
            onRefreshGrades();
            onUpdateStatistics();
        } else {
            QMessageBox::critical(this, "错误", "成绩删除失败！");
        }
    }
}

void MainWindow::onRefreshGrades()
{
    QList<QStringList> grades = m_dbManager->getAllGrades();
    m_gradeModel->setGradeData(grades);
    m_statusLabel->setText(QString("已加载 %1 条成绩记录").arg(grades.size()));
}

void MainWindow::onFilterGrades()
{
    QString studentId;
    QString courseId;

    if (m_filterStudentCombo->currentIndex() > 0) {
        studentId = m_filterStudentCombo->currentData().toString();
    }

    if (m_filterCourseCombo->currentIndex() > 0) {
        courseId = m_filterCourseCombo->currentData().toString();
    }

    QList<QStringList> grades;

    if (!studentId.isEmpty() && !courseId.isEmpty()) {
        // 同时筛选学生和课程
        QList<QStringList> allGrades = m_dbManager->getAllGrades();
        for (const QStringList& grade : allGrades) {
            if (grade[0] == studentId && grade[2] == courseId) {
                grades.append(grade);
            }
        }
    } else if (!studentId.isEmpty()) {
        grades = m_dbManager->getGradesByStudent(studentId);
    } else if (!courseId.isEmpty()) {
        grades = m_dbManager->getGradesByCourse(courseId);
    } else {
        grades = m_dbManager->getAllGrades();
    }

    m_gradeModel->setGradeData(grades);
    m_statusLabel->setText(QString("已筛选 %1 条成绩记录").arg(grades.size()));
}

// 文件操作槽函数
void MainWindow::onImportStudents()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择CSV文件", "", "CSV文件 (*.csv)");
    if (fileName.isEmpty()) {
        return;
    }

    m_statusLabel->setText("正在导入学生数据...");
    QApplication::processEvents(); // 更新UI

    // 使用FileManager导入CSV文件
    QList<QStringList> data = m_fileManager->importFromCSV(fileName);

    if (data.isEmpty()) {
        QMessageBox::warning(this, "警告", "文件为空或格式错误！");
        m_statusLabel->setText("就绪");
        return;
    }

    // 将导入的数据写入数据库
    int count = 0;
    int total = data.size();

    for (int i = 0; i < total; i++) {
        const QStringList& row = data[i];
        if (row.size() >= 3) {
            // 跳过表头
            if (i == 0 && (row[0] == "学号" || row[0].contains("student"))) {
                continue;
            }
            if (m_dbManager->addStudent(row[0].trimmed(), row[1].trimmed(), row[2].trimmed())) {
                count++;
            }
        }
    }

    QMessageBox::information(this, "成功", QString("成功导入 %1 条学生记录").arg(count));
    refreshAllData();
    m_statusLabel->setText("就绪");
}

void MainWindow::onImportCourses()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择CSV文件", "", "CSV文件 (*.csv)");
    if (fileName.isEmpty()) {
        return;
    }

    m_statusLabel->setText("正在导入课程数据...");
    QApplication::processEvents(); // 更新UI

    // 使用FileManager导入CSV文件
    QList<QStringList> data = m_fileManager->importFromCSV(fileName);

    if (data.isEmpty()) {
        QMessageBox::warning(this, "警告", "文件为空或格式错误！");
        m_statusLabel->setText("就绪");
        return;
    }

    // 将导入的数据写入数据库
    int count = 0;
    int total = data.size();

    for (int i = 0; i < total; i++) {
        const QStringList& row = data[i];
        if (row.size() >= 3) {
            // 跳过表头
            if (i == 0 && (row[0] == "课程编号" || row[0].contains("course"))) {
                continue;
            }
            bool ok;
            int credits = row[2].toInt(&ok);
            if (ok && credits > 0 && m_dbManager->addCourse(row[0].trimmed(), row[1].trimmed(), credits)) {
                count++;
            }
        }
    }

    QMessageBox::information(this, "成功", QString("成功导入 %1 条课程记录").arg(count));
    refreshAllData();
    m_statusLabel->setText("就绪");
}

void MainWindow::onImportGrades()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择CSV文件", "", "CSV文件 (*.csv)");
    if (fileName.isEmpty()) {
        return;
    }

    m_statusLabel->setText("正在导入成绩数据...");
    QApplication::processEvents(); // 更新UI

    // 使用FileManager导入CSV文件
    QList<QStringList> data = m_fileManager->importFromCSV(fileName);

    if (data.isEmpty()) {
        QMessageBox::warning(this, "警告", "文件为空或格式错误！");
        m_statusLabel->setText("就绪");
        return;
    }

    // 将导入的数据写入数据库
    int count = 0;
    int total = data.size();

    for (int i = 0; i < total; i++) {
        const QStringList& row = data[i];
        if (row.size() >= 4) {
            // 跳过表头
            if (i == 0 && (row[0] == "学号" || row[0].contains("student"))) {
                continue;
            }
            bool ok;
            double score = row[2].toDouble(&ok);
            if (ok && score >= 0 && score <= 100 &&
                m_dbManager->addGrade(row[0].trimmed(), row[1].trimmed(), score, row[3].trimmed())) {
                count++;
            }
        }
    }

    QMessageBox::information(this, "成功", QString("成功导入 %1 条成绩记录").arg(count));
    refreshAllData();
    onUpdateStatistics();
    m_statusLabel->setText("就绪");
}

void MainWindow::onExportData()
{
    QString fileName = QFileDialog::getSaveFileName(this, "导出数据", "", "CSV文件 (*.csv)");
    if (fileName.isEmpty()) {
        return;
    }

    QList<QStringList> data = m_dbManager->getAllGrades();
    QStringList headers = QStringList() << "学号" << "姓名" << "课程编号" << "课程名称" << "成绩" << "学期";

    if (m_fileManager->exportToCSV(fileName, data, headers)) {
        QMessageBox::information(this, "成功", "数据导出成功！");
    } else {
        QMessageBox::critical(this, "错误", "数据导出失败！");
    }
}


// 统计槽函数
void MainWindow::onUpdateStatistics()
{
    QList<QPair<QString, double>> ranking = m_dbManager->getStudentRanking();
    m_statisticsWidget->updateStudentRankingChart(ranking);
}

// 网络同步槽函数
// 目前实现为本地数据刷新和状态提示，预留将来与服务器同步的扩展点
void MainWindow::onSyncData()
{
    m_statusLabel->setText("正在同步数据...");
    QApplication::processEvents();

    // 这里可以在将来加入真实的网络同步逻辑
    // 目前先刷新本地数据库视图，保证界面数据最新
    refreshAllData();

    m_statusLabel->setText("数据同步完成");
}
