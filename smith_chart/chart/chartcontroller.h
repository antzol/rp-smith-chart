#ifndef CHARTCONTROLLER_H
#define CHARTCONTROLLER_H

#include <QObject>

#include <complex>

#include "constants.h"
#include "chartwidget.h"
#include "circuitcomponent.h"

class ChartController : public QObject
{
    Q_OBJECT
public:
    explicit ChartController(ChartWidget *chart, QObject *parent = nullptr);

public slots:
    void updateSourceImpedance(const std::complex<double> &imp);
    void updateLoadImpedance(const std::complex<double> &imp);
    void updateScalingValue(double val);

    void updateCenterFrequency(double val);

    void addComponent(int position, const CircuitComponent *component);
    void updateComponent(int position, const CircuitComponent *component);
    void deleteComponent(int position);

private:
    std::complex<double> calcComponentScaledImpedance(const CircuitComponent *component);

    ChartWidget *m_chartWidget;

    std::complex<double> m_sourceImpedance;
    std::complex<double> m_loadImpedance;

    double m_scalingValue{50};
    std::complex<double> m_sourceScaledImpedance;
    std::complex<double> m_loadScaledImpedance;

    double m_centerFrequency;

};

#endif // CHARTCONTROLLER_H
