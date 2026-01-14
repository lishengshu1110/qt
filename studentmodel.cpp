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
