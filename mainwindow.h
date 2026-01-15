#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QTableView>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QProgressBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QFileDialog>
#include <QSplitter>

#include "DatabaseManager.h"
#include "StudentModel.h"
#include "GradeModel.h"
#include "FileManager.h"
#include "StatisticsWidget.h"

/**
 * @brief 主窗口类
 * 学生成绩管理系统的主界面
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    // 学生管理
    void onAddStudent();
    void onUpdateStudent();
    void onDeleteStudent();
    void onRefreshStudents();
    
    // 课程管理
    void onAddCourse();
    void onUpdateCourse();
    void onDeleteCourse();
    void onRefreshCourses();
    
    // 成绩管理
    void onAddGrade();
    void onUpdateGrade();
    void onDeleteGrade();
    void onRefreshGrades();
    void onFilterGrades();
    
    // 文件操作
    void onImportStudents();
    void onImportCourses();
    void onImportGrades();
    void onExportData();
    
    // 统计与同步
    void onUpdateStatistics();
    void onSyncData();

private:
    void setupUI();
    void setupStudentTab();
    void setupCourseTab();
    void setupGradeTab();
    void setupStatisticsTab();
    void setupMenuBar();
    void setupStatusBar();
    void connectSignals();
    void refreshAllData();
    
    // UI组件
    QTabWidget* m_tabWidget;
    
    // 学生管理标签页
    QWidget* m_studentTab;
    QTableView* m_studentTable;
    StudentModel* m_studentModel;
    QLineEdit* m_studentIdEdit;
    QLineEdit* m_studentNameEdit;
    QLineEdit* m_classNameEdit;
    
    // 课程管理标签页
    QWidget* m_courseTab;
    QTableView* m_courseTable;
    QLineEdit* m_courseIdEdit;
    QLineEdit* m_courseNameEdit;
    QLineEdit* m_creditsEdit;
    
    // 成绩管理标签页
    QWidget* m_gradeTab;
    QTableView* m_gradeTable;
    GradeModel* m_gradeModel;
    QComboBox* m_gradeStudentCombo;
    QComboBox* m_gradeCourseCombo;
    QLineEdit* m_scoreEdit;
    QLineEdit* m_semesterEdit;
    QComboBox* m_filterStudentCombo;
    QComboBox* m_filterCourseCombo;
    
    // 统计标签页
    StatisticsWidget* m_statisticsWidget;
    
    // 工具栏
    QProgressBar* m_progressBar;
    QLabel* m_statusLabel;
    
    // 数据管理
    DatabaseManager* m_dbManager;
    FileManager* m_fileManager;
};

#endif // MAINWINDOW_H
