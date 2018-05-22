#include "alphabetmodel.h"

AlphabetModel::AlphabetModel(const QStringList &data, QObject *parent)
    : QAbstractTableModel(parent),
      m_data(data)
{
}

QVariant AlphabetModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Vertical)
            return section;

        else if (orientation == Qt::Horizontal)
            return "Letter";
    }
    return {};
}

//bool AlphabetModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
//{
//    if (value != headerData(section, orientation, role)) {
//        // FIXME: Implement me!
//        emit headerDataChanged(orientation, section, section);
//        return true;
//    }
//    return false;
//}

int AlphabetModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_data.size();
}

int AlphabetModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 1;
}

QVariant AlphabetModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role)
    {
    case Qt::DisplayRole:
        return m_data.at(index.row());
    }
    return QVariant();
}

bool AlphabetModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        if (role == Qt::EditRole)
            m_data[index.row()] = value.toString();

        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags AlphabetModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

QStringList &AlphabetModel::data()
{
    return m_data;
}

//bool AlphabetModel::insertRows(int row, int count, const QModelIndex &parent)
//{
//    beginInsertRows(parent, row, row + count - 1);
//    // FIXME: Implement me!
//    endInsertRows();
//}

//bool AlphabetModel::insertColumns(int column, int count, const QModelIndex &parent)
//{
//    beginInsertColumns(parent, column, column + count - 1);
//    // FIXME: Implement me!
//    endInsertColumns();
//}

//bool AlphabetModel::removeRows(int row, int count, const QModelIndex &parent)
//{
//    beginRemoveRows(parent, row, row + count - 1);
//    // FIXME: Implement me!
//    endRemoveRows();
//}

//bool AlphabetModel::removeColumns(int column, int count, const QModelIndex &parent)
//{
//    beginRemoveColumns(parent, column, column + count - 1);
//    // FIXME: Implement me!
//    endRemoveColumns();
//}
