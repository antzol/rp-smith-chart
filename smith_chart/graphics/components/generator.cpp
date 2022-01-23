#include "generator.h"

#include <algorithm>

#include <QPainter>

#include <QStyleOptionGraphicsItem>
#include <QFontMetrics>

//---------------------------------------------------------------------------------------
Generator::Generator(QGraphicsItem *parent) : ComponentGraphicsItem(parent)
{
    m_labelText = QObject::tr("Source");
    m_valueText = "50 + j0";

    m_input = QPointF(0, 0);
}

//---------------------------------------------------------------------------------------
void Generator::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    const double radius = 14;
    const double centerY = 30;

    double centerX = radius;

    // top pin
    painter->drawLine(centerX, 0, centerX, centerY - radius);

    // circle
    painter->drawEllipse(0, centerY - radius, radius * 2, radius * 2);

    // wave in the circle
    QPainterPath path;
    path.moveTo(0.4 * radius, centerY - 0.3 * radius);
    path.cubicTo(1.0 * radius, centerY - 1.2 * radius,
                 1.0 * radius, centerY + 0.6 * radius,
                 1.6 * radius, centerY - 0.3 * radius);

    path.moveTo(0.4 * radius, centerY + 0.3 * radius);
    path.cubicTo(1.0 * radius, centerY - 0.6 * radius,
                 1.0 * radius, centerY + 1.2 * radius,
                 1.6 * radius, centerY + 0.3 * radius);
    painter->drawPath(path);

    // bottom pin
    painter->drawLine(centerX, centerY + radius, centerX, 2 * centerY);

    // GND symbol
    drawGndLine(painter, QPointF(centerX, 2 * centerY));

    // labels
    QPointF labelPoint(radius * 2 + option->fontMetrics.height() * 0.5, centerY);
    painter->drawText(labelPoint, m_labelText);
    double labelRightBound = labelPoint.x() + option->fontMetrics.horizontalAdvance(m_labelText);

    QPointF valuePoint(labelPoint.x(), labelPoint.y() + option->fontMetrics.lineSpacing() * 1.1);
    painter->drawText(valuePoint, m_valueText);
    double valueRightBound = valuePoint.x() + option->fontMetrics.horizontalAdvance(m_valueText);

    int rightestPoint = std::max(labelRightBound, valueRightBound);

    // output wire
    QPointF output = QPointF(rightestPoint, 0);

    if(output != m_output)
    {
        m_output = output;
    }

    painter->drawLine(QPointF(radius, m_output.y()), m_output);

    double width = rightestPoint;
    double height = centerY * 2 + ComponentSizes::GndSymbolWidth;

    if(width != m_width || height != m_heght)
    {
        prepareGeometryChange();
        m_width = width;
        m_heght = height;
    }

    checkYCoordinates();
}

//---------------------------------------------------------------------------------------
int Generator::type() const
{
    return Type;
}

//---------------------------------------------------------------------------------------
