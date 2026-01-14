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
