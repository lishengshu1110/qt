#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QString>
#include <QStringList>

/**
 * @brief 文件管理类
 * 负责文件的导入导出、日志记录等功能
 */
class FileManager : public QObject
{
    Q_OBJECT

public:
    explicit FileManager(QObject* parent = nullptr);

    // 导出功能
    bool exportToCSV(const QString& filePath, const QList<QStringList>& data,
                     const QStringList& headers);
    bool exportToExcel(const QString& filePath, const QList<QStringList>& data,
                       const QStringList& headers);

    // 导入功能
    QList<QStringList> importFromCSV(const QString& filePath);

    // 日志功能
    void writeLog(const QString& message, const QString& level = "INFO");
    QStringList readLogs(int maxLines = 100);
    void clearLogs();

signals:
    void logMessage(const QString& message);

private:
    QString getLogFilePath() const;
    QString formatCSVLine(const QStringList& fields);
};

#endif // FILEMANAGER_H
