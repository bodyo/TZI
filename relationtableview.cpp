#include "relationtableview.h"

#include <QDebug>

RelationTableModel::RelationTableModel(QVector<QPair<QString, QString> > inputData, QObject *parent)
    : QAbstractTableModel(parent),
      m_data(inputData)
{
}

QVariant RelationTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Vertical)
            return section+1;

        else if (orientation == Qt::Horizontal)
        {
            if (section == 0)
                return "Original";
            else if (section == 1)
                return "Conformity";
        }
    }
    return {};
}

int RelationTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_data.size();
}

int RelationTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 2;
}

QVariant RelationTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role)
    {
    case Qt::DisplayRole:
        if (index.column() == 0)
            return m_data.at(index.row()).first;
        else
            return m_data.at(index.row()).second;

    }
    return QVariant();
}

QVector<QPair<QString, QString> > &RelationTableModel::getData()
{
    return m_data;
}

bool RelationTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value)
    {
        if (role == Qt::EditRole)
        {
            if (index.column() == 0)
                m_data[index.row()].first = value.toString();
            else if (index.column() == 1)
                m_data[index.row()].second = value.toString();
        }

        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags RelationTableModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return Qt::ItemIsEditable | Qt::ItemIsEnabled;
}
