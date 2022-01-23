#include "chartwidget.h"

#include <array>
#include <cmath>

#include <QCheckBox>
#include <QGroupBox>

#include <QtCharts/QSplineSeries>

#include "aspectratiowidget.h"
#include "config.h"

static QVector<double> defaultCirclesValues{0.0, 0.2, 0.5, 1.0, 2.0, 5.0};
static QVector<double> defaultArcsValues{0.0, 0.2, 0.5, 1.0, 2.0, 5.0};

//---------------------------------------------------------------------------------------
ChartWidget::ChartWidget(QWidget *parent) : QWidget(parent)
{
    Config *config = Config::instance();

    m_chart = new QChart();

    m_axisX = new QValueAxis();
    m_axisX->setRange(-1, 1);
    m_chart->addAxis(m_axisX, Qt::AlignBottom);

    m_axisY = new QValueAxis();
    m_axisY->setRange(-1, 1);
    m_chart->addAxis(m_axisY, Qt::AlignLeft);

    // impedances
    createRCircles();
    createXArcs();

    // admitances
    createGCircles();
    createBArcs();

    // impedance points
    int pointSize = config->getIntValue(Config::ImpedancePointSize);

    m_sourceImpedancePoint.setMarkerShape(QScatterSeries::MarkerShapeRectangle);
    m_sourceImpedancePoint.setMarkerSize(pointSize);
    m_sourceImpedancePoint.setColor(config->getColor(Config::ColorSourceImpedancePoint));
    m_chart->addSeries(&m_sourceImpedancePoint);
    m_sourceImpedancePoint.attachAxis(m_axisX);
    m_sourceImpedancePoint.attachAxis(m_axisY);

    m_sourceConjImpedancePoint.setMarkerShape(QScatterSeries::MarkerShapeCircle);
    m_sourceConjImpedancePoint.setMarkerSize(pointSize);
    m_sourceConjImpedancePoint.setColor(config->getColor(Config::ColorSourceImpedanceConjugatePoint));
    m_chart->addSeries(&m_sourceConjImpedancePoint);
    m_sourceConjImpedancePoint.attachAxis(m_axisX);
    m_sourceConjImpedancePoint.attachAxis(m_axisY);

    m_loadImpedancePoint.setMarkerShape(QScatterSeries::MarkerShapeCircle);
    m_loadImpedancePoint.setMarkerSize(pointSize);
    m_loadImpedancePoint.setColor(config->getColor(Config::ColorLoadImpedancePoint));
    m_chart->addSeries(&m_loadImpedancePoint);
    m_loadImpedancePoint.attachAxis(m_axisX);
    m_loadImpedancePoint.attachAxis(m_axisY);

    m_chart->legend()->hide();

    m_chartView = new QChartView();
    m_chartView->setChart(m_chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);

    AspectRatioWidget *chartWidget = new AspectRatioWidget(m_chartView, 200, 200);

    // add checkboxes in the legend for hiding/showing circles and arcs
    QBoxLayout *legendLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    QMapIterator<QCheckBox*, QList<QSplineSeries*>*> map(linesMap);
    legendLayout->addStretch(1);
    while (map.hasNext())
    {
        map.next();
        legendLayout->addWidget(map.key());
    }
    legendLayout->addStretch(1);

    QWidget *legendWidget = new QWidget();
    legendWidget->setLayout(legendLayout);

    QBoxLayout *wndLayout = new QBoxLayout(QBoxLayout::TopToBottom);

    wndLayout->addWidget(chartWidget);
    wndLayout->addWidget(legendWidget);

    setLayout(wndLayout);
}

//---------------------------------------------------------------------------------------
QChart* ChartWidget::chart()
{
    return m_chart;
}

//---------------------------------------------------------------------------------------
void ChartWidget::addComponent(int position, std::complex<double> impedance, ConnectionType type)
{
    ChartComponent *comp = new ChartComponent(impedance, type);
    m_components.insert(position, comp);

    changeComponentImpedance(position, impedance);

    // set color for the series
    Config *config = Config::instance();
    int colorKey;
    if (impedance.imag() < 0) // capacitor
    {
        if (type == ConnectionType::Series)
            colorKey = Config::ColorSeriesCapacitor;
        else
            colorKey = Config::ColorParallelCapacitor;
    }
    else // inductance
    {
        if (type == ConnectionType::Series)
            colorKey = Config::ColorSeriesInductance;
        else
            colorKey = Config::ColorParallelInductance;
    }

    QPen pen;
    pen.setColor(config->getColor(colorKey));
    pen.setWidth(config->getIntValue(Config::WidthComponentLine));
    comp->series()->setPen(pen);

    m_chart->addSeries(comp->series());
    comp->series()->attachAxis(m_axisX);
    comp->series()->attachAxis(m_axisY);
}

//---------------------------------------------------------------------------------------
void ChartWidget::changeComponentImpedance(int position, std::complex<double> impedance)
{
    ChartComponent *comp = m_components.at(position);
    comp->setImpedance(impedance);

    // update the lines for the current component and all subsequent ones
    // in the direction of the source
    for (int i = position; i >= 0; --i)
        repaintComponentLine(i);

}

//---------------------------------------------------------------------------------------
void ChartWidget::deleteComponent(int position)
{
    if (position < 0 || position >= m_components.size())
        return;

    ChartComponent *comp = m_components.takeAt(position);
    m_chart->removeSeries(comp->series());
    delete comp;

    for (int i = position - 1; i >= 0 && i <= m_components.size() - 1; --i)
        repaintComponentLine(i);
}

//---------------------------------------------------------------------------------------
void ChartWidget::updateSourceImpedance(const std::complex<double> &imp)
{
    m_sourceImpedance = imp;
    m_sourceImpedancePoint.clear();
    m_sourceImpedancePoint.append(calcPointFromImpedance(imp));
}

//---------------------------------------------------------------------------------------
void ChartWidget::updateSourceConjugateImpedance(const std::complex<double> &imp)
{
    m_sourceConjImpedancePoint.clear();
    m_sourceConjImpedancePoint.append(calcPointFromImpedance(imp));
}

//---------------------------------------------------------------------------------------
void ChartWidget::updateLoadImpedance(const std::complex<double> &imp)
{
    m_loadImpedance = imp;
    m_loadImpedancePoint.clear();
    m_loadImpedancePoint.append(calcPointFromImpedance(imp));

    for (int i = m_components.size() - 1; i >= 0; --i)
        repaintComponentLine(i);
}

//---------------------------------------------------------------------------------------
void ChartWidget::processShowLinesCheckBox(bool state)
{
    QCheckBox *chbox = qobject_cast<QCheckBox*>(sender());
    if (!chbox)
        return;

    QList<QSplineSeries*> *lines(linesMap.value(chbox));
    for (int i = 0; i < lines->size(); ++i)
        lines->at(i)->setVisible(state);
}

//---------------------------------------------------------------------------------------
void ChartWidget::createRCircles()
{
    Config *config = Config::instance();
    QColor color = config->getColor(Config::ColorResistanceCircle);
    int lineWidth = config->getIntValue(Config::WidthChartLine);
    QVector<double> circlesValues = config->getDoubleVector(Config::ChartCirclesValues);

    if (circlesValues.isEmpty())
        circlesValues = defaultCirclesValues;

    QList<QSplineSeries*> *rCircles = new QList<QSplineSeries*>();
    for (auto& r : circlesValues)
    {
        QSplineSeries *series = new QSplineSeries();
        series->append(createRCircle(r));
        QPen pen = series->pen();
        pen.setWidth(lineWidth);
        pen.setColor(color);
        series->setPen(pen);
        m_chart->addSeries(series);
        series->attachAxis(m_axisX);
        series->attachAxis(m_axisY);

        rCircles->append(series);
    }

    QCheckBox *checkBoxRLines = new QCheckBox(tr("R (resistance)"));
    checkBoxRLines->setChecked(true);
    linesMap.insert(checkBoxRLines, rCircles);
    connect(checkBoxRLines, &QCheckBox::stateChanged,
            this, &ChartWidget::processShowLinesCheckBox);
}

//---------------------------------------------------------------------------------------
void ChartWidget::createXArcs()
{
    Config *config = Config::instance();
    QColor color = config->getColor(Config::ColorReactanceArc);
    int lineWidth = config->getIntValue(Config::WidthChartLine);
    QVector<double> arcsValues = config->getDoubleVector(Config::ChartArcsValues);

    if (arcsValues.isEmpty())
        arcsValues = defaultArcsValues;

    QList<QSplineSeries*> *xArcs = new QList<QSplineSeries*>();
    for (auto& x : arcsValues)
    {
        QSplineSeries *pseries = new QSplineSeries();
        pseries->append(createXArc(x));
        QPen pen = pseries->pen();
        pen.setWidth(lineWidth);
        pen.setColor(color);
        pseries->setPen(pen);
        m_chart->addSeries(pseries);
        pseries->attachAxis(m_axisX);
        pseries->attachAxis(m_axisY);

        QSplineSeries *nseries = new QSplineSeries();
        nseries->append(createXArc(-x));
        nseries->setPen(pen);
        m_chart->addSeries(nseries);
        nseries->attachAxis(m_axisX);
        nseries->attachAxis(m_axisY);

        xArcs->append(pseries);
        xArcs->append(nseries);
    }

    QCheckBox *checkBoxXLines = new QCheckBox(tr("X (reactance)"));
    checkBoxXLines->setChecked(true);
    linesMap.insert(checkBoxXLines, xArcs);
    connect(checkBoxXLines, &QCheckBox::stateChanged,
            this, &ChartWidget::processShowLinesCheckBox);
}

//---------------------------------------------------------------------------------------
void ChartWidget::createGCircles()
{
    Config *config = Config::instance();
    QColor color = config->getColor(Config::ColorConductanceCircle);
    int lineWidth = config->getIntValue(Config::WidthChartLine);
    QVector<double> circlesValues = config->getDoubleVector(Config::ChartCirclesValues);

    if (circlesValues.isEmpty())
        circlesValues = defaultCirclesValues;

    QList<QSplineSeries*> *gCircles = new QList<QSplineSeries*>();
    for (auto& g : circlesValues)
    {
        QSplineSeries *series = new QSplineSeries();
        series->append(createGCircle(g));
        QPen pen = series->pen();
        pen.setWidth(lineWidth);
        pen.setColor(color);
        series->setPen(pen);
        m_chart->addSeries(series);
        series->attachAxis(m_axisX);
        series->attachAxis(m_axisY);

        gCircles->append(series);
    }

    QCheckBox *checkBoxGLines = new QCheckBox(tr("G (conductance)"));
    checkBoxGLines->setChecked(true);
    linesMap.insert(checkBoxGLines, gCircles);
    connect(checkBoxGLines, &QCheckBox::stateChanged,
            this, &ChartWidget::processShowLinesCheckBox);
}

//---------------------------------------------------------------------------------------
void ChartWidget::createBArcs()
{
    Config *config = Config::instance();
    QColor color = config->getColor(Config::ColorSusceptanceArc);
    int lineWidth = config->getIntValue(Config::WidthChartLine);
    QVector<double> arcsValues = config->getDoubleVector(Config::ChartArcsValues);

    if (arcsValues.isEmpty())
        arcsValues = defaultArcsValues;

    QList<QSplineSeries*> *bArcs = new QList<QSplineSeries*>();
    for (auto& b : arcsValues)
    {
        QSplineSeries *pseries = new QSplineSeries();
        pseries->append(createBArc(b));
        QPen pen = pseries->pen();
        pen.setWidth(lineWidth);
        pen.setColor(color);
        pseries->setPen(pen);
        m_chart->addSeries(pseries);
        pseries->attachAxis(m_axisX);
        pseries->attachAxis(m_axisY);

        QSplineSeries *nseries = new QSplineSeries();
        nseries->append(createBArc(-b));
        nseries->setPen(pen);
        m_chart->addSeries(nseries);
        nseries->attachAxis(m_axisX);
        nseries->attachAxis(m_axisY);

        bArcs->append(pseries);
        bArcs->append(nseries);

    }

    QCheckBox *checkBoxBLines = new QCheckBox(tr("B (susceptance)"));
    checkBoxBLines->setChecked(true);
    linesMap.insert(checkBoxBLines, bArcs);
    connect(checkBoxBLines, &QCheckBox::stateChanged,
            this, &ChartWidget::processShowLinesCheckBox);
}

//---------------------------------------------------------------------------------------
QList<QPointF> ChartWidget::createRCircle(double r)
{
    QList<QPointF> points;

    for (int degree = 0; degree <= 360; degree += 5)
    {
        double radius = 1.0 / (r + 1);
        double xCenter = r / (r + 1);
        double x = radius * cos(degree * PI / 180) + xCenter;
        double y = radius * sin(degree * PI / 180);
        points.append(QPointF(x, y));
    }

    return points;
}

//---------------------------------------------------------------------------------------
QList<QPointF> ChartWidget::createXArc(double x)
{
    QList<QPointF> points;

    // when x = 0, the line is the same as the x-axis
    if (std::abs(x) < epsilon)
    {
        points.append(QPoint(-1, 0));
        points.append(QPoint(1, 0));
        return points;
    }


    double degree = 90;
    while(1)
    {
        double radius = 1.0 / x;
        double xCenter = 1.0;
        double yCenter = 1.0 / x;
        double x = radius * cos(degree * PI / 180) + xCenter;
        double y = radius * sin(degree * PI / 180) + yCenter;
        QPointF p = QPointF(x, y);
        if (inRange(p))
            points.append(p);

        degree += 0.1;
        if(degree > 450)
            break;
    }

    return points;
}

//---------------------------------------------------------------------------------------
QList<QPointF> ChartWidget::createGCircle(double g)
{
    QList<QPointF> points;

    for (int degree = 0; degree <= 360; degree += 5)
    {
        double radius = 1.0 / (g + 1);
        double xCenter = -g / (g + 1);
        double x = radius * cos(degree * PI / 180) + xCenter;
        double y = radius * sin(degree * PI / 180);
        points.append(QPointF(x, y));
    }

    return points;
}

//---------------------------------------------------------------------------------------
QList<QPointF> ChartWidget::createBArc(double b)
{
    QList<QPointF> points;

    // when b = 0, the line is the same as the x-axis
    if (std::abs(b) < epsilon)
    {
        points.append(QPoint(-1, 0));
        points.append(QPoint(1, 0));
        return points;
    }

    double degree = 90;
    while(1)
    {
        double radius = 1.0 / b;
        double xCenter = -1.0;
        double yCenter = 1.0 / b;
        double x = radius * cos(degree * PI / 180) + xCenter;
        double y = radius * sin(degree * PI / 180) + yCenter;
        QPointF p = QPointF(x, y);
        if (inRange(p))
            points.append(p);

        degree += 0.1;
        if(degree > 450)
            break;
    }

    return points;
}

//---------------------------------------------------------------------------------------
void ChartWidget::repaintComponentLine(int position)
{
    if (position >= m_components.size())
        return;

    ChartComponent *comp = m_components.at(position);
    QList<QPointF> points;

    std::complex<double> prevTotalImpedance;
    QPointF prevPoint;
    if (position == m_components.size() - 1 )
    {
        prevTotalImpedance = m_loadImpedance;
        prevPoint = m_loadImpedancePoint.points().at(0);
    }
    else
    {
        prevTotalImpedance = m_components.at(position + 1)->totalImpedance();
        prevPoint = m_components.at(position + 1)->point();
    }


    std::complex<double> curTotalImpedance;
    if (comp->type() == ConnectionType::Series)
    {
        curTotalImpedance = prevTotalImpedance + comp->impedance();

        comp->setTotalImpedance(curTotalImpedance);
        comp->setPoint(calcPointFromImpedance(curTotalImpedance));

        double rCircleRadius = 1.0 / (prevTotalImpedance.real() + 1);
        double rCircleCenterX = prevTotalImpedance.real() / (prevTotalImpedance.real() + 1);

        double alpha = asin(prevPoint.y()/rCircleRadius);
        if (prevPoint.x() > rCircleCenterX) // если угол тупой
            alpha = PI - alpha;

        double beta = asin(comp->point().y()/rCircleRadius);
        if (comp->point().x() > rCircleCenterX) // если угол тупой
            beta = PI - beta;

        double mn = std::min(alpha, beta);
        double mx = std::max(alpha, beta);

        double rad = mn;
        while (rad <= mx)
        {
            double x = rCircleCenterX - rCircleRadius * cos(rad);
            double y = rCircleRadius * sin(rad);
            points.append(QPointF(x, y));
            rad += 0.01;
        }
    }
    else
    {
        // switch the work with the diagram to the admitance mode
        QPointF prevTotalAdmitancePoint(-prevPoint);
        std::complex<double> prevTotalAdmitance(calcImpedanceFromPoint(prevTotalAdmitancePoint));

        // shift the total admitance by the value of the imaginary part
        // of the admitance of the added component
        // TODO: add the influence of the real part of the component's admitance
        double susceptance = -comp->impedance().imag() / std::pow(std::abs(comp->impedance()), 2);
        std::complex<double> curTotalAdmitance(prevTotalAdmitance.real(), prevTotalAdmitance.imag() + susceptance);
        QPointF curTotalAdmitancePoint(calcPointFromImpedance(curTotalAdmitance));

        // switch back to impedance mode
        QPointF curTotalImpedancePoint(-curTotalAdmitancePoint);
        comp->setPoint(curTotalImpedancePoint);
        comp->setTotalImpedance(calcImpedanceFromPoint(curTotalImpedancePoint));


        double rCircleRadius = 1.0 / (prevTotalAdmitance.real() + 1);
        double rCircleCenterX = prevTotalAdmitance.real() / (prevTotalAdmitance.real() + 1);

        double alpha = asin(prevTotalAdmitancePoint.y()/rCircleRadius);
        if (prevTotalAdmitancePoint.x() > rCircleCenterX) // if the angle is obtuse
            alpha = PI - alpha;

        double beta = asin(curTotalAdmitancePoint.y()/rCircleRadius);
        if (curTotalAdmitancePoint.x() > rCircleCenterX)  // if the angle is obtuse
            beta = PI - beta;

        double mn = std::min(alpha, beta);
        double mx = std::max(alpha, beta);

        double rad = mn;
        while (rad <= mx)
        {
            double x = rCircleCenterX - rCircleRadius * cos(rad);
            double y = rCircleRadius * sin(rad);
            points.append(QPointF(-x, -y));
            rad += 0.01;
        }
    }
    comp->series()->clear();
    comp->series()->append(points);
}

//---------------------------------------------------------------------------------------
bool ChartWidget::inRange(const QPointF &point)
{
    double radius = 1.0;
    double centerX = 0;
    double centerY = 0;

    return (pow(point.x() - centerX, 2) + pow(point.y() - centerY, 2)) <= pow(radius, 2);
}

//---------------------------------------------------------------------------------------
QPointF ChartWidget::calcPointFromImpedance(const std::complex<double> &imp)
{
    double rCircleRadius = 1.0 / (imp.real() + 1);
    double rCircleCenterX = imp.real() / (imp.real() + 1);

    if (std::abs(imp.imag()) < epsilon)
        return QPointF(1 - 2*rCircleRadius, 0);


    double xArcCenterY = 1.0 / imp.imag();

    double alpha = atan(xArcCenterY/(1.0 - rCircleCenterX));

    double y = rCircleRadius * sin(2*alpha);
    double x = rCircleRadius * cos(2*alpha) + rCircleCenterX;

    return QPointF(x, y);
}

//---------------------------------------------------------------------------------------
QPointF ChartWidget::calcPointFromAdmitance(const std::complex<double> &adm)
{
    double gCircleRadius = 1.0 / (adm.real() + 1);
    double gCircleCenterX = - adm.real() / (adm.real() + 1);

    if (std::abs(adm.imag()) < epsilon)
        return QPointF(-1 + 2*gCircleRadius, 0);

    double bArcCenterY = 1.0 / adm.imag();

    double alpha = atan(bArcCenterY/gCircleRadius);

    double y = gCircleRadius * sin(PI - 2*alpha);
    double x = gCircleCenterX + gCircleRadius * cos(PI - 2*alpha);

    return QPointF(x, y);
}

//---------------------------------------------------------------------------------------
std::complex<double> ChartWidget::calcImpedanceFromPoint(const QPointF &point)
{
    double denominator = 1 + std::pow(point.x(), 2) - 2 * point.x() + std::pow(point.y(), 2);

    double r = (1 - std::pow(point.x(), 2) - std::pow(point.y(), 2)) /denominator;
    double x = 2 * point.y() / denominator;

    return std::complex<double>(r, x);
}

//---------------------------------------------------------------------------------------
std::complex<double> ChartWidget::calcAdmitanceFromPoint(const QPointF &point)
{
    double denominator = 1 + std::pow(point.x(), 2) + 2 * point.x() + std::pow(point.y(), 2);

    double g = (1 - std::pow(point.x(), 2) - std::pow(point.y(), 2)) /denominator;
    double b = -2 * point.y() / denominator;

    return std::complex<double>(g, b);
}

//---------------------------------------------------------------------------------------
