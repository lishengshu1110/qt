#ifndef GRADEMODEL_H
#define GRADEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QStringList>

/**
 * @brief 成绩数据模型类
 * 用于在QTableView中显示成绩信息
 */
class GradeModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit GradeModel(QObject* parent = nullptr);

    // QAbstractItemModel接口实现
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // 自定义方法
    void setGradeData(const QList<QStringList>& data);
    void addGrade(const QStringList& grade);
    void removeGrade(int row);
    QStringList getGrade(int row) const;
    void clear();

private:
    QList<QStringList> m_grades;
    QStringList m_headers;
};

#endif // GRADEMODEL_H
