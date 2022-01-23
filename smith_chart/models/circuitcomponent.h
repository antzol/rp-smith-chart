#ifndef CIRCUITCOMPONENT_H
#define CIRCUITCOMPONENT_H

#include <QObject>

#include "constants.h"
#include "componentgraphicsitem.h"
#include "metricnotion.h"


class CircuitComponent : public QObject
{
    Q_OBJECT
public:
    explicit CircuitComponent(ComponentType type, ConnectionType connectionType, QObject *parent = nullptr);

    QString symbol() const;
    double value() const;
    QString baseMeasureUnit() const;
    MetricNotion metricNotion() const;

    ConnectionType connectionType() const;
    ComponentType componentType() const;

    void setValue(double val);

    static const QMap<ComponentType, const char*> measureUnits;
    static const QMap<ComponentType, QString> baseSymbols;

private:
    ComponentGraphicsItem *m_graphicsItem;

    QString m_baseSymbol;
    int m_positionNumber;

    double m_value{1e-12};
    QString m_baseMeasureUnit;
    MetricNotion m_metricNotion;

    ComponentType m_type;
    ConnectionType m_connectionType;
};

#endif // CIRCUITCOMPONENT_H
