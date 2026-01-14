#include "databasemanager.h"
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>
#include <QSqlError>

DatabaseManager::DatabaseManager(QObject* parent)
    : QObject(parent)
{
    // 设置数据库路径为指定路径
    dbPath = "E:/sqLite/数据库/qiMo.db";

    // 确保数据库目录存在，如果不存在则创建
    QFileInfo fileInfo(dbPath);
    QDir dir = fileInfo.absoluteDir();
    if (!dir.exists()) {
        dir.mkpath(dir.absolutePath());
    }
}

DatabaseManager::~DatabaseManager()
{
    if (db.isOpen()) {
        db.close();
    }
}

DatabaseManager& DatabaseManager::getInstance()
{
    static DatabaseManager instance;
    return instance;
}

bool DatabaseManager::initializeDatabase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qDebug() << "数据库打开失败:" << db.lastError().text();
        return false;
    }

    return createTables();
}

bool DatabaseManager::createTables()
{
    QSqlQuery query(db);

    // 创建学生表
    QString createStudentTable = R"(
        CREATE TABLE IF NOT EXISTS students (
            student_id TEXT PRIMARY KEY,
            name TEXT NOT NULL,
            class_name TEXT NOT NULL,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    )";

    if (!query.exec(createStudentTable)) {
        qDebug() << "创建学生表失败:" << query.lastError().text();
        return false;
    }

    // 创建课程表
    QString createCourseTable = R"(
        CREATE TABLE IF NOT EXISTS courses (
            course_id TEXT PRIMARY KEY,
            course_name TEXT NOT NULL,
            credits INTEGER NOT NULL,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    )";

    if (!query.exec(createCourseTable)) {
        qDebug() << "创建课程表失败:" << query.lastError().text();
        return false;
    }

    // 创建成绩表
    QString createGradeTable = R"(
        CREATE TABLE IF NOT EXISTS grades (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            student_id TEXT NOT NULL,
            course_id TEXT NOT NULL,
            score REAL NOT NULL,
            semester TEXT NOT NULL,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (student_id) REFERENCES students(student_id),
            FOREIGN KEY (course_id) REFERENCES courses(course_id),
            UNIQUE(student_id, course_id, semester)
        )
    )";

    if (!query.exec(createGradeTable)) {
        qDebug() << "创建成绩表失败:" << query.lastError().text();
        return false;
    }

    return true;
}

// 学生相关操作
bool DatabaseManager::addStudent(const QString& studentId, const QString& name, const QString& className)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO students (student_id, name, class_name) VALUES (?, ?, ?)");
    query.addBindValue(studentId);
    query.addBindValue(name);
    query.addBindValue(className);

    if (!query.exec()) {
        qDebug() << "添加学生失败:" << query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::updateStudent(const QString& studentId, const QString& name, const QString& className)
{
    QSqlQuery query(db);
    query.prepare("UPDATE students SET name = ?, class_name = ? WHERE student_id = ?");
    query.addBindValue(name);
    query.addBindValue(className);
    query.addBindValue(studentId);

    return query.exec();
}

bool DatabaseManager::deleteStudent(const QString& studentId)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM students WHERE student_id = ?");
    query.addBindValue(studentId);

    return query.exec();
}

QList<QStringList> DatabaseManager::getAllStudents()
{
    QList<QStringList> result;
    QSqlQuery query("SELECT student_id, name, class_name FROM students ORDER BY student_id", db);

    while (query.next()) {
        QStringList row;
        row << query.value(0).toString();
        row << query.value(1).toString();
        row << query.value(2).toString();
        result.append(row);
    }

    return result;
}

// 课程相关操作
bool DatabaseManager::addCourse(const QString& courseId, const QString& courseName, int credits)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO courses (course_id, course_name, credits) VALUES (?, ?, ?)");
    query.addBindValue(courseId);
    query.addBindValue(courseName);
    query.addBindValue(credits);

    return query.exec();
}

bool DatabaseManager::updateCourse(const QString& courseId, const QString& courseName, int credits)
{
    QSqlQuery query(db);
    query.prepare("UPDATE courses SET course_name = ?, credits = ? WHERE course_id = ?");
    query.addBindValue(courseName);
    query.addBindValue(credits);
    query.addBindValue(courseId);

    return query.exec();
}

bool DatabaseManager::deleteCourse(const QString& courseId)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM courses WHERE course_id = ?");
    query.addBindValue(courseId);

    return query.exec();
}

QList<QStringList> DatabaseManager::getAllCourses()
{
    QList<QStringList> result;
    QSqlQuery query("SELECT course_id, course_name, credits FROM courses ORDER BY course_id", db);

    while (query.next()) {
        QStringList row;
        row << query.value(0).toString();
        row << query.value(1).toString();
        row << query.value(2).toString();
        result.append(row);
    }

    return result;
}

// 成绩相关操作
bool DatabaseManager::addGrade(const QString& studentId, const QString& courseId,
                               double score, const QString& semester)
{
    QSqlQuery query(db);
    query.prepare("INSERT OR REPLACE INTO grades (student_id, course_id, score, semester) VALUES (?, ?, ?, ?)");
    query.addBindValue(studentId);
    query.addBindValue(courseId);
    query.addBindValue(score);
    query.addBindValue(semester);

    return query.exec();
}

bool DatabaseManager::updateGrade(const QString& studentId, const QString& courseId,
                                  double score, const QString& semester)
{
    QSqlQuery query(db);
    query.prepare("UPDATE grades SET score = ? WHERE student_id = ? AND course_id = ? AND semester = ?");
    query.addBindValue(score);
    query.addBindValue(studentId);
    query.addBindValue(courseId);
    query.addBindValue(semester);

    return query.exec();
}

bool DatabaseManager::deleteGrade(const QString& studentId, const QString& courseId, const QString& semester)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM grades WHERE student_id = ? AND course_id = ? AND semester = ?");
    query.addBindValue(studentId);
    query.addBindValue(courseId);
    query.addBindValue(semester);

    return query.exec();
}

QList<QStringList> DatabaseManager::getAllGrades()
{
    QList<QStringList> result;
    QSqlQuery query(R"(
        SELECT g.student_id, s.name, g.course_id, c.course_name, g.score, g.semester
        FROM grades g
        JOIN students s ON g.student_id = s.student_id
        JOIN courses c ON g.course_id = c.course_id
        ORDER BY g.student_id, g.course_id, g.semester
    )", db);

    while (query.next()) {
        QStringList row;
        for (int i = 0; i < 6; i++) {
            row << query.value(i).toString();
        }
        result.append(row);
    }

    return result;
}

QList<QStringList> DatabaseManager::getGradesByStudent(const QString& studentId)
{
    QList<QStringList> result;
    QSqlQuery query(db);
    query.prepare(R"(
        SELECT g.student_id, s.name, g.course_id, c.course_name, g.score, g.semester
        FROM grades g
        JOIN students s ON g.student_id = s.student_id
        JOIN courses c ON g.course_id = c.course_id
        WHERE g.student_id = ?
        ORDER BY g.semester, g.course_id
    )");
    query.addBindValue(studentId);

    if (query.exec()) {
        while (query.next()) {
            QStringList row;
            for (int i = 0; i < 6; i++) {
                row << query.value(i).toString();
            }
            result.append(row);
        }
    }

    return result;
}

QList<QStringList> DatabaseManager::getGradesByCourse(const QString& courseId)
{
    QList<QStringList> result;
    QSqlQuery query(db);
    query.prepare(R"(
        SELECT g.student_id, s.name, g.course_id, c.course_name, g.score, g.semester
        FROM grades g
        JOIN students s ON g.student_id = s.student_id
        JOIN courses c ON g.course_id = c.course_id
        WHERE g.course_id = ?
        ORDER BY g.score DESC
    )");
    query.addBindValue(courseId);

    if (query.exec()) {
        while (query.next()) {
            QStringList row;
            for (int i = 0; i < 6; i++) {
                row << query.value(i).toString();
            }
            result.append(row);
        }
    }

    return result;
}

// 统计查询
double DatabaseManager::getAverageScore(const QString& studentId)
{
    QSqlQuery query(db);
    query.prepare("SELECT AVG(score) FROM grades WHERE student_id = ?");
    query.addBindValue(studentId);

    if (query.exec() && query.next()) {
        return query.value(0).toDouble();
    }
    return 0.0;
}

double DatabaseManager::getCourseAverageScore(const QString& courseId)
{
    QSqlQuery query(db);
    query.prepare("SELECT AVG(score) FROM grades WHERE course_id = ?");
    query.addBindValue(courseId);

    if (query.exec() && query.next()) {
        return query.value(0).toDouble();
    }
    return 0.0;
}

QList<QPair<QString, double>> DatabaseManager::getStudentRanking()
{
    QList<QPair<QString, double>> result;
    QSqlQuery query(R"(
        SELECT s.student_id, s.name, AVG(g.score) as avg_score
        FROM students s
        LEFT JOIN grades g ON s.student_id = g.student_id
        GROUP BY s.student_id, s.name
        ORDER BY avg_score DESC
    )", db);

    while (query.next()) {
        QString studentId = query.value(0).toString();
        double avgScore = query.value(2).toDouble();
        result.append(qMakePair(studentId, avgScore));
    }

    return result;
}

QList<QPair<QString, double>> DatabaseManager::getCourseStatistics()
{
    QList<QPair<QString, double>> result;
    QSqlQuery query(R"(
        SELECT c.course_id, AVG(g.score) as avg_score
        FROM courses c
        LEFT JOIN grades g ON c.course_id = g.course_id
        GROUP BY c.course_id
        ORDER BY avg_score DESC
    )", db);

    while (query.next()) {
        QString courseId = query.value(0).toString();
        double avgScore = query.value(1).toDouble();
        result.append(qMakePair(courseId, avgScore));
    }

    return result;
}
