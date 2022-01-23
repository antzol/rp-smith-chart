#ifndef CHARTCOMPONENT_H
#define CHARTCOMPONENT_H

#include <QObject>

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>

#include <complex>

#include "constants.h"

QT_CHARTS_USE_NAMESPACE

class ChartComponent : public QObject
{
    Q_OBJECT
public:
    ChartComponent(std::complex<double> imp, ConnectionType connectionType, QObject *parent = nullptr);
    ~ChartComponent();

    QSplineSeries *series();

    void setPoint(const QPointF &pt);
    QPointF point() const;

    ConnectionType type() const;

    void setImpedance(std::complex<double> imp);
    std::complex<double> impedance() const;

    void setTotalImpedance(std::complex<double> imp);
    std::complex<double> totalImpedance() const;

private:
    std::complex<double> m_impedance;
    std::complex<double> m_totalImpedance;
    ConnectionType m_connectionType;
    QPointF m_point;
    QSplineSeries *m_seriesToPoint;
};

#endif // CHARTCOMPONENT_H
