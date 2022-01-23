#include "doublelistmodel.h"

//---------------------------------------------------------------------------------------
DoubleListModel::DoubleListModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

//---------------------------------------------------------------------------------------
void DoubleListModel::fillModel(const QVector<double> &vect)
{
    beginInsertRows(QModelIndex(), 0, vect.size() - 1);
    m_numbers = vect;
    endInsertRows();
    emit dataChanged(index(0,0), index(vect.size()-1, 0), {Qt::DisplayRole});
}

//---------------------------------------------------------------------------------------
QVector<double> DoubleListModel::getAllData()
{
    return m_numbers;
}

//---------------------------------------------------------------------------------------
int DoubleListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_numbers.size();
}

//---------------------------------------------------------------------------------------
Qt::ItemFlags DoubleListModel::flags(const QModelIndex &index) const
{
    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

//---------------------------------------------------------------------------------------
QVariant DoubleListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    int row = index.row();

    if (row >= rowCount())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole)
        return m_numbers.at(row);

    return QVariant();
}

//---------------------------------------------------------------------------------------
bool DoubleListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    int row = index.row();

    if (row >= rowCount())
        return false;

    if (role == Qt::EditRole)
        m_numbers[row] = value.toDouble();

    emit dataChanged(this->index(row, 0), this->index(row, 0), {Qt::DisplayRole});
    return true;
}

//---------------------------------------------------------------------------------------
QModelIndex DoubleListModel::addItem()
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_numbers.append(0.0);
    endInsertRows();
    QModelIndex lastItemIndex = index(rowCount()-1, 0);
    emit dataChanged(lastItemIndex, lastItemIndex, {Qt::DisplayRole});
    return lastItemIndex;
}

//---------------------------------------------------------------------------------------
void DoubleListModel::removeItem(const QModelIndex &index)
{
    int row = index.row();
    if (row >= m_numbers.count())
        return;

    beginRemoveRows(QModelIndex(), row, row);
    m_numbers.removeAt(row);
    endRemoveRows();
    QModelIndex lastItemIndex = this->index(row, 0);
    emit dataChanged(lastItemIndex, lastItemIndex, {Qt::DisplayRole});
}

//---------------------------------------------------------------------------------------
