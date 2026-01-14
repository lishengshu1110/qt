#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QList>
#include <QDebug>

/**
 * @brief 数据库管理类
 * 负责SQLite数据库的初始化、连接管理和基本CRUD操作
 */
class DatabaseManager : public QObject
{
    Q_OBJECT

public:
    static DatabaseManager& getInstance();

    // 初始化数据库和表结构
    bool initializeDatabase();

    // 学生相关操作
    bool addStudent(const QString& studentId, const QString& name, const QString& className);
    bool updateStudent(const QString& studentId, const QString& name, const QString& className);
    bool deleteStudent(const QString& studentId);
    QList<QStringList> getAllStudents();

    // 课程相关操作
    bool addCourse(const QString& courseId, const QString& courseName, int credits);
    bool updateCourse(const QString& courseId, const QString& courseName, int credits);
    bool deleteCourse(const QString& courseId);
    QList<QStringList> getAllCourses();

    // 成绩相关操作
    bool addGrade(const QString& studentId, const QString& courseId,
                  double score, const QString& semester);
    bool updateGrade(const QString& studentId, const QString& courseId,
                     double score, const QString& semester);
    bool deleteGrade(const QString& studentId, const QString& courseId, const QString& semester);
    QList<QStringList> getAllGrades();
    QList<QStringList> getGradesByStudent(const QString& studentId);
    QList<QStringList> getGradesByCourse(const QString& courseId);

    // 统计查询
    double getAverageScore(const QString& studentId);
    double getCourseAverageScore(const QString& courseId);
    QList<QPair<QString, double>> getStudentRanking();
    QList<QPair<QString, double>> getCourseStatistics();

    // 获取数据库连接
    QSqlDatabase getDatabase() const { return db; }

private:
    DatabaseManager(QObject* parent = nullptr);
    ~DatabaseManager();
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    bool createTables();
    QSqlDatabase db;
    QString dbPath;
};

#endif // DATABASEMANAGER_H
