#ifndef STUDENTMODEL_H
#define STUDENTMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QStringList>

/**
 * @brief 学生数据模型类
 * 用于在QTableView中显示学生信息
 */
class StudentModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit StudentModel(QObject* parent = nullptr);
    
    // QAbstractItemModel接口实现
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, 
                        int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    
    // 自定义方法
    void setStudentData(const QList<QStringList>& data);
    void addStudent(const QStringList& student);
    void removeStudent(int row);
    QStringList getStudent(int row) const;
    void clear();

private:
    QList<QStringList> m_students;
    QStringList m_headers;
};

#endif // STUDENTMODEL_H
