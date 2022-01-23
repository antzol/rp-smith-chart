#include "chartcomponent.h"

//---------------------------------------------------------------------------------------
ChartComponent::ChartComponent(std::complex<double> imp, ConnectionType connectionType, QObject *parent)
    : QObject(parent)
    , m_impedance(imp)
    , m_connectionType(connectionType)
{
    m_seriesToPoint = new QSplineSeries(this);
}

//---------------------------------------------------------------------------------------
ChartComponent::~ChartComponent()
{
    delete m_seriesToPoint;
}

//---------------------------------------------------------------------------------------
QSplineSeries *ChartComponent::series()
{
    return m_seriesToPoint;
}
//---------------------------------------------------------------------------------------
void ChartComponent::setPoint(const QPointF &pt)
{
    m_point = pt;
}

//---------------------------------------------------------------------------------------
QPointF ChartComponent::point() const
{
    return m_point;
}

//---------------------------------------------------------------------------------------
ConnectionType ChartComponent::type() const
{
    return m_connectionType;
}

//---------------------------------------------------------------------------------------
void ChartComponent::setImpedance(std::complex<double> imp)
{
    m_impedance = imp;
}

//---------------------------------------------------------------------------------------
std::complex<double> ChartComponent::impedance() const
{
    return m_impedance;
}

//---------------------------------------------------------------------------------------
void ChartComponent::setTotalImpedance(std::complex<double> imp)
{
    m_totalImpedance = imp;
}

//---------------------------------------------------------------------------------------
std::complex<double> ChartComponent::totalImpedance() const
{
    return m_totalImpedance;
}

//---------------------------------------------------------------------------------------
