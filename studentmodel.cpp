#include "StudentModel.h"

StudentModel::StudentModel(QObject* parent)
    : QAbstractTableModel(parent)
{
    m_headers << "学号" << "姓名" << "班级";
}

int StudentModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_students.size();
}

int StudentModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_headers.size();
}

QVariant StudentModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= m_students.size()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        const QStringList& student = m_students.at(index.row());
        if (index.column() < student.size()) {
            return student.at(index.column());
        }
    }

    if (role == Qt::TextAlignmentRole) {
        return Qt::AlignCenter;
    }

    return QVariant();
}

