#include "circuitmodel.h"

#include <math.h>

#include "circuitcomponent.h"
#include "componentgraphicsitem.h"

//---------------------------------------------------------------------------------------
CircuitModel::CircuitModel(QObject *parent) : QAbstractTableModel(parent)
{
    m_headers << tr("Symbol") << tr("Value") << tr("Unit") << tr("Base unit")
              << tr("Power of 10");
}

//---------------------------------------------------------------------------------------
int CircuitModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_components.count();
}

//---------------------------------------------------------------------------------------
int CircuitModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return NumberOfColumns;
}

//---------------------------------------------------------------------------------------
Qt::ItemFlags CircuitModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags baseFlags = QAbstractTableModel::flags(index);
    if (!index.isValid())
        return baseFlags;

    Qt::ItemFlags myFlags = baseFlags;
    switch (index.column())
    {
    case ColumnConvertedValue:
    case ColumnUnit:
        myFlags |= Qt::ItemIsEditable;
    }

    return myFlags;
}

//---------------------------------------------------------------------------------------
QVariant CircuitModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        return m_headers.at(section);

    return QVariant();
}

//---------------------------------------------------------------------------------------
QVariant CircuitModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    int row = index.row();
    int column = index.column();

    if (row >= rowCount() || column >= columnCount())
        return QVariant();

    const CircuitComponent *comp = m_components.at(row);
    if (role == Qt::DisplayRole)
    {
        switch (column)
        {
        case ColumnSymbol:
            return QString("%1%2").arg(comp->symbol()).arg(row + 1);
        case ColumnConvertedValue:
            return comp->metricNotion().convertedValue();
        case ColumnUnit:
            return QString("%1%2").arg(comp->metricNotion().prefix()).arg(comp->baseMeasureUnit());
        case ColumnBaseUnit:
            return comp->baseMeasureUnit();
        case ColumnPowerOf10:
            return comp->metricNotion().powerOf10();
        }
    }
    else if (role == Qt::EditRole)
    {
        switch (column)
        {
        case ColumnUnit:
            return comp->metricNotion().powerOf10();
        }
    }

    return QVariant();
}

//---------------------------------------------------------------------------------------
bool CircuitModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    int row = index.row();
    int col = index.column();

    if (role == Qt::EditRole)
    {

        CircuitComponent *comp = m_components.at(row);

        if (col == ColumnConvertedValue)
        {
            double val = value.toDouble() * pow(10, comp->metricNotion().powerOf10());
            comp->setValue(val);
        }
        else if (col == ColumnUnit)
        {
            double curConvertedValue = comp->metricNotion().convertedValue();
            double newValue = curConvertedValue * pow(10, value.toInt());
            comp->setValue(newValue);
        }

        emit componentChanged(row, comp);
    }

    emit dataChanged(this->index(row, 0), this->index(row, NumberOfColumns-1), {Qt::DisplayRole});
    return true;
}

//---------------------------------------------------------------------------------------
void CircuitModel::addCircuitComponent(int pos, ComponentType componentType, ConnectionType connectionType)
{
    beginInsertRows(QModelIndex(), pos, pos);
    CircuitComponent *component = new CircuitComponent(componentType, connectionType, this);
    if (pos > m_components.count())
        pos = m_components.count();
    m_components.insert(pos, component);
    endInsertRows();

    // All components coming after the new one are changed,
    // as their sequence numbers are changed.
    emit dataChanged(index(pos, 0), index(rowCount()-1, NumberOfColumns-1), {Qt::DisplayRole});
    emit componentAdded(pos, component);
}

//---------------------------------------------------------------------------------------
void CircuitModel::deleteCircuitComponent(int pos)
{
    if (pos >= m_components.count())
        return;

    beginRemoveRows(QModelIndex(), pos, pos);
    CircuitComponent *component = m_components.takeAt(pos);
    delete component;
    endRemoveRows();

    emit dataChanged(index(pos, 0), index(rowCount()-1, NumberOfColumns-1), {Qt::DisplayRole});
    emit componentDeleted(pos);
}

//---------------------------------------------------------------------------------------
