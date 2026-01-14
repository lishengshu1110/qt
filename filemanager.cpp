#include "FileManager.h"
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QStandardPaths>
#include <QDateTime>
#include <QDebug>
#include <QStringConverter>

FileManager::FileManager(QObject* parent)
    : QObject(parent)
{
}

bool FileManager::exportToCSV(const QString& filePath, const QList<QStringList>& data,
                              const QStringList& headers)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        writeLog(QString("导出CSV失败: 无法创建文件 %1").arg(filePath), "ERROR");
        return false;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);

    // 写入BOM以支持Excel正确识别UTF-8
    out << "\xEF\xBB\xBF";

    // 写入表头
    if (!headers.isEmpty()) {
        out << formatCSVLine(headers) << "\n";
    }

    // 写入数据
    for (const QStringList& row : data) {
        out << formatCSVLine(row) << "\n";
    }

    file.close();
    writeLog(QString("成功导出CSV文件: %1，共 %2 条记录").arg(filePath).arg(data.size()));
    return true;
}

bool FileManager::exportToExcel(const QString& filePath, const QList<QStringList>& data,
                                const QStringList& headers)
{
    // 注意：这里实际上导出为CSV格式，Excel可以打开
    // 真正的Excel格式(.xlsx)需要使用第三方库如QXlsx
    // 为了简化，这里使用CSV格式，Excel可以正常打开
    return exportToCSV(filePath, data, headers);
}

QList<QStringList> FileManager::importFromCSV(const QString& filePath)
{
    QList<QStringList> result;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        writeLog(QString("导入CSV失败: 无法打开文件 %1").arg(filePath), "ERROR");
        return result;
    }

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) {
            continue;
        }

        QStringList fields;
        QString currentField;
        bool inQuotes = false;

        for (int i = 0; i < line.length(); i++) {
            QChar ch = line[i];

            if (ch == '"') {
                inQuotes = !inQuotes;
            } else if (ch == ',' && !inQuotes) {
                fields.append(currentField.trimmed());
                currentField.clear();
            } else {
                currentField.append(ch);
            }
        }

        fields.append(currentField.trimmed());

        if (!fields.isEmpty()) {
            result.append(fields);
        }
    }

    file.close();
    writeLog(QString("成功导入CSV文件: %1，共 %2 条记录").arg(filePath).arg(result.size()));
    return result;
}
void FileManager::writeLog(const QString& message, const QString& level)
{
    QString logFilePath = getLogFilePath();
    QFile file(logFilePath);

    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out.setEncoding(QStringConverter::Utf8);

        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        QString logLine = QString("[%1] [%2] %3\n").arg(timestamp, level, message);

        out << logLine;
        file.close();

        emit logMessage(logLine.trimmed());
    }
}

QStringList FileManager::readLogs(int maxLines)
{
    QStringList result;
    QString logFilePath = getLogFilePath();

    QFile file(logFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return result;
    }

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);

    QStringList allLines;
    while (!in.atEnd()) {
        allLines.append(in.readLine());
    }

    file.close();

    // 返回最后maxLines行
    int start = qMax(0, allLines.size() - maxLines);
    for (int i = start; i < allLines.size(); i++) {
        result.append(allLines[i]);
    }

    return result;
}

void FileManager::clearLogs()
{
    QString logFilePath = getLogFilePath();
    QFile file(logFilePath);
    if (file.exists()) {
        file.remove();
    }
    writeLog("日志文件已清空");
}

QString FileManager::getLogFilePath() const
{
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir;
    if (!dir.exists(dataPath)) {
        dir.mkpath(dataPath);
    }
    return dataPath + "/application.log";
}

QString FileManager::formatCSVLine(const QStringList& fields)
{
    QStringList formattedFields;

    for (const QString& field : fields) {
        QString formatted = field;

        // 如果字段包含逗号、引号或换行符，需要用引号包围
        if (formatted.contains(',') || formatted.contains('"') || formatted.contains('\n')) {
            formatted.replace("\"", "\"\""); // 转义引号
            formatted = "\"" + formatted + "\"";
        }

        formattedFields.append(formatted);
    }

    return formattedFields.join(",");
}
