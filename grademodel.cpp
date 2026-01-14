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

QVariant GradeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            if (section < m_headers.size()) {
                return m_headers.at(section);
            }
        } else {
            return section + 1;
        }
    }

    return QVariant();
}

bool GradeModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || role != Qt::EditRole) {
        return false;
    }

    if (index.row() < m_grades.size() && index.column() < m_grades[index.row()].size()) {
        m_grades[index.row()][index.column()] = value.toString();
        emit dataChanged(index, index, {role});
        return true;
    }

    return false;
}

Qt::ItemFlags GradeModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

void GradeModel::setGradeData(const QList<QStringList>& data)
{
    beginResetModel();
    m_grades = data;
    endResetModel();
}

void GradeModel::addGrade(const QStringList& grade)
{
    beginInsertRows(QModelIndex(), m_grades.size(), m_grades.size());
    m_grades.append(grade);
    endInsertRows();
}

void GradeModel::removeGrade(int row)
{
    if (row >= 0 && row < m_grades.size()) {
        beginRemoveRows(QModelIndex(), row, row);
        m_grades.removeAt(row);
        endRemoveRows();
    }
}

QStringList GradeModel::getGrade(int row) const
{
    if (row >= 0 && row < m_grades.size()) {
        return m_grades.at(row);
    }
    return QStringList();
}

void GradeModel::clear()
{
    beginResetModel();
    m_grades.clear();
    endResetModel();
}
