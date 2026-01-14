#include "GradeModel.h"
#include <QColor>

GradeModel::GradeModel(QObject* parent)
    : QAbstractTableModel(parent)
{
    m_headers << "学号" << "姓名" << "课程编号" << "课程名称" << "成绩" << "学期";
}

int GradeModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_grades.size();
}

int GradeModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_headers.size();
}

QVariant GradeModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= m_grades.size()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        const QStringList& grade = m_grades.at(index.row());
        if (index.column() < grade.size()) {
            // 成绩列特殊处理，显示为数字格式
            if (index.column() == 4) {
                bool ok;
                double score = grade.at(index.column()).toDouble(&ok);
                if (ok) {
                    return QString::number(score, 'f', 1);
                }
            }
            return grade.at(index.column());
        }
    }

    if (role == Qt::TextAlignmentRole) {
        return Qt::AlignCenter;
    }

    // 成绩列的颜色显示
    if (role == Qt::ForegroundRole && index.column() == 4) {
        const QStringList& grade = m_grades.at(index.row());
        bool ok;
        double score = grade.at(index.column()).toDouble(&ok);
        if (ok) {
            if (score >= 90) {
                return QColor(Qt::darkGreen);
            } else if (score >= 60) {
                return QColor(Qt::black);
            } else {
                return QColor(Qt::red);
            }
        }
    }

    return QVariant();
}

