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
