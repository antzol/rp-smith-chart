#include "chartcontroller.h"

#include "constants.h"
#include "circuitcomponent.h"

//---------------------------------------------------------------------------------------
ChartController::ChartController(ChartWidget *chart, QObject *parent) : QObject(parent)
  , m_chartWidget(chart)
{

}

//---------------------------------------------------------------------------------------
void ChartController::updateSourceImpedance(const std::complex<double> &imp)
{
    m_sourceImpedance.real(imp.real());
    m_sourceImpedance.imag(imp.imag());

    m_sourceScaledImpedance.real(m_sourceImpedance.real() / m_scalingValue);
    m_sourceScaledImpedance.imag(m_sourceImpedance.imag() / m_scalingValue);

    m_chartWidget->updateSourceImpedance(m_sourceScaledImpedance);
    m_chartWidget->updateSourceConjugateImpedance(std::conj(m_sourceScaledImpedance));
}

//---------------------------------------------------------------------------------------
void ChartController::updateLoadImpedance(const std::complex<double> &imp)
{
    m_loadImpedance.real(imp.real());
    m_loadImpedance.imag(imp.imag());

    m_loadScaledImpedance.real(imp.real() / m_scalingValue);
    m_loadScaledImpedance.imag(imp.imag() / m_scalingValue);

    m_chartWidget->updateLoadImpedance(m_loadScaledImpedance);
}

//---------------------------------------------------------------------------------------
void ChartController::updateScalingValue(double val)
{
    m_scalingValue = val;
    updateSourceImpedance(m_sourceImpedance);
    updateLoadImpedance(m_loadImpedance);
}

//---------------------------------------------------------------------------------------
void ChartController::updateCenterFrequency(double val)
{
    m_centerFrequency = val;
}

//---------------------------------------------------------------------------------------
void ChartController::addComponent(int position, const CircuitComponent *component)
{
    std::complex<double> impedance = calcComponentScaledImpedance(component);
    ConnectionType connType = component->connectionType();
    m_chartWidget->addComponent(position, impedance, connType);
}

//---------------------------------------------------------------------------------------
void ChartController::updateComponent(int position, const CircuitComponent *component)
{
    m_chartWidget->changeComponentImpedance(position, calcComponentScaledImpedance(component));
}

//---------------------------------------------------------------------------------------
void ChartController::deleteComponent(int position)
{
    m_chartWidget->deleteComponent(position);
}

//---------------------------------------------------------------------------------------
std::complex<double> ChartController::calcComponentScaledImpedance(const CircuitComponent *component)
{
    std::complex<double> impedance;
    if (component->componentType() == ComponentType::Inductance)
    {
        impedance.imag(2 * PI * m_centerFrequency * component->value() / m_scalingValue);
    }
    else if (component->componentType() == ComponentType::Capacitor)
    {
        impedance.imag((-1 / (2 * PI * m_centerFrequency * component->value()))/m_scalingValue);
    }
    return impedance;
}

//---------------------------------------------------------------------------------------
