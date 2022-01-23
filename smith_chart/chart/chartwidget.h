#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <complex>

#include <QWidget>

#include <QBoxLayout>

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>

#include "constants.h"
#include "chartcomponent.h"

QT_CHARTS_USE_NAMESPACE

class QCheckBox;

class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChartWidget(QWidget *parent = nullptr);
    QChart* chart();

    void addComponent(int position, std::complex<double> impedance, ConnectionType type);
    void changeComponentImpedance(int position, std::complex<double> impedance);
    void deleteComponent(int position);


public slots:
    void updateSourceImpedance(const std::complex<double> &imp);
    void updateSourceConjugateImpedance(const std::complex<double> &imp);
    void updateLoadImpedance(const std::complex<double> &imp);

private slots:
    void processShowLinesCheckBox(bool state);

private:
    void createRCircles();
    void createXArcs();
    void createGCircles();
    void createBArcs();

    QList<QPointF> createRCircle(double r);
    QList<QPointF> createXArc(double x);
    QList<QPointF> createGCircle(double g);
    QList<QPointF> createBArc(double b);

    void repaintComponentLine(int position);

    bool inRange(const QPointF &point);

    QPointF calcPointFromImpedance(const std::complex<double> &imp);
    QPointF calcPointFromAdmitance(const std::complex<double> &adm);
    std::complex<double> calcImpedanceFromPoint(const QPointF &point);
    std::complex<double> calcAdmitanceFromPoint(const QPointF &point);

    QChart *m_chart;
    QChartView *m_chartView;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;

    QMap<QCheckBox*, QList<QSplineSeries*>*> linesMap;

    QScatterSeries m_sourceImpedancePoint;
    QScatterSeries m_sourceConjImpedancePoint;
    QScatterSeries m_loadImpedancePoint;

    std::complex<double> m_sourceImpedance;
    std::complex<double> m_loadImpedance;

    QList<ChartComponent*> m_components;

};

#endif // CHARTWIDGET_H
