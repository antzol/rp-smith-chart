#include "circuitcomponent.h"

#include <QMap>

#include "circuitmodel.h"
#include "config.h"

const QMap<ComponentType, const char*> CircuitComponent::measureUnits{
    {ComponentType::Capacitor, QT_TR_NOOP("F")},
    {ComponentType::Inductance, QT_TR_NOOP("H")}
};

const QMap<ComponentType, QString> CircuitComponent::baseSymbols {
    {ComponentType::Capacitor, "C"},
    {ComponentType::Inductance, "L"}
};

//---------------------------------------------------------------------------------------
CircuitComponent::CircuitComponent(ComponentType type, ConnectionType connectionType, QObject *parent)
    : QObject(parent)
    , m_type(type)
    , m_connectionType(connectionType)
{
    m_baseSymbol = baseSymbols.value(m_type, "??");
    m_baseMeasureUnit = tr(measureUnits.value(m_type, "??"));

    m_value = Config::instance()->getComponentDefaultValue(m_baseSymbol);
    m_metricNotion.update(m_value);
}

//---------------------------------------------------------------------------------------
QString CircuitComponent::symbol() const
{
    return m_baseSymbol;
}

//---------------------------------------------------------------------------------------
double CircuitComponent::value() const
{
    return m_value;
}

//---------------------------------------------------------------------------------------
QString CircuitComponent::baseMeasureUnit() const
{
    return m_baseMeasureUnit;
}

//---------------------------------------------------------------------------------------
MetricNotion CircuitComponent::metricNotion() const
{
    return  m_metricNotion;
}

//---------------------------------------------------------------------------------------
ConnectionType CircuitComponent::connectionType() const
{
    return m_connectionType;
}

//---------------------------------------------------------------------------------------
ComponentType CircuitComponent::componentType() const
{
    return m_type;
}

//---------------------------------------------------------------------------------------
void CircuitComponent::setValue(double val)
{
    m_value = val;
    m_metricNotion.update(m_value);
}

//---------------------------------------------------------------------------------------
