#ifndef RELATIONTABLEVIEW_H
#define RELATIONTABLEVIEW_H

#include <QAbstractTableModel>

class RelationTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit RelationTableModel(QVector<QPair<QString, QString>> inputData, QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QVector<QPair<QString, QString>> &getData();

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    QVector<QPair<QString, QString>> m_data;
};

#endif // RELATIONTABLEVIEW_H
