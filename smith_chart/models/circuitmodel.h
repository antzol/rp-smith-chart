#ifndef CIRCUITMODEL_H
#define CIRCUITMODEL_H

#include <QAbstractTableModel>

#include "constants.h"

class CircuitComponent;
class ComponentGraphicsItem;

class CircuitModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum Columns {
        ColumnSymbol,
        ColumnConvertedValue,
        ColumnUnit,
        ColumnBaseUnit,
        ColumnPowerOf10,
        NumberOfColumns
    };

    explicit CircuitModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

public slots:
    void addCircuitComponent(int pos, ComponentType componentType, ConnectionType connectionType);
    void deleteCircuitComponent(int pos);

signals:
    void componentAdded(int position, const CircuitComponent *component);
    void componentChanged(int position, const CircuitComponent *component);
    void componentDeleted(int position);

private:
    QList<CircuitComponent*> m_components;

    QStringList m_headers;
};

#endif // CIRCUITMODEL_H
