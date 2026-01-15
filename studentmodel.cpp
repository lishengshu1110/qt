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

QVariant StudentModel::headerData(int section, Qt::Orientation orientation, int role) const
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

bool StudentModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || role != Qt::EditRole) {
        return false;
    }
    
    if (index.row() < m_students.size() && index.column() < m_students[index.row()].size()) {
        m_students[index.row()][index.column()] = value.toString();
        emit dataChanged(index, index, {role});
        return true;
    }
    
    return false;
}

Qt::ItemFlags StudentModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }
    
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

void StudentModel::setStudentData(const QList<QStringList>& data)
{
    beginResetModel();
    m_students = data;
    endResetModel();
}

void StudentModel::addStudent(const QStringList& student)
{
    beginInsertRows(QModelIndex(), m_students.size(), m_students.size());
    m_students.append(student);
    endInsertRows();
}

void StudentModel::removeStudent(int row)
{
    if (row >= 0 && row < m_students.size()) {
        beginRemoveRows(QModelIndex(), row, row);
        m_students.removeAt(row);
        endRemoveRows();
    }
}

QStringList StudentModel::getStudent(int row) const
{
    if (row >= 0 && row < m_students.size()) {
        return m_students.at(row);
    }
    return QStringList();
}

void StudentModel::clear()
{
    beginResetModel();
    m_students.clear();
    endResetModel();
}
