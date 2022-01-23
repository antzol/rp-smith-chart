#ifndef INITIALDATADOCKWIDGET_H
#define INITIALDATADOCKWIDGET_H

#include <QDockWidget>

#include <complex>

#include "metricnotion.h"

namespace Ui {
class InitialDataDockWidget;
}

class InitialDataDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit InitialDataDockWidget(QWidget *parent = nullptr);
    ~InitialDataDockWidget();

    void fillValues();

public slots:
    void configureImpedance();
    void configureFrequency();

    void updateSourceImpedance(std::complex<double> imp);
    void updateLoadImpedance(std::complex<double> imp);

    void updateCenterFrequency(const MetricNotion &notion);
    void updateFrequencySpan(const MetricNotion &notion);
    void updateFrequencyStep(const MetricNotion &notion);

signals:
    void sourceImpedanceChanged(std::complex<double> imp);
    void loadImpedanceChanged(std::complex<double> imp);
    void scalingValueChanged(double val);

    void centerFrequencyChanged(double freq);
    void frequencySpanChanged(double span);
    void frequencyStepChanged(double step);


private:
    Ui::InitialDataDockWidget *ui;

    std::complex<double> m_sourceImpedance;
    std::complex<double> m_loadImpedance;
    double m_manualScalingValue;

    MetricNotion m_centerFrequency;
    MetricNotion m_frequencySpan;
    MetricNotion m_frequencyStep;
};

#endif // INITIALDATADOCKWIDGET_H
