#include "inductanceserial.h"

#include <algorithm>
#include <initializer_list>

#include <QPainter>

#include <QStyleOptionGraphicsItem>
#include <QFontMetrics>

#include <QDebug>

//---------------------------------------------------------------------------------------
InductanceSerial::InductanceSerial(QGraphicsItem *parent) : ComponentGraphicsItem(parent)
{
    setFlag(GraphicsItemFlag::ItemIsSelectable);
}

//---------------------------------------------------------------------------------------
void InductanceSerial::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QPointF labelPoint(ComponentSizes::HorizComponentWireLength + option->fontMetrics.horizontalAdvance(m_labelText),
                       option->fontMetrics.height());
    painter->drawText(labelPoint, m_labelText);
    double labelRightBound = labelPoint.x() + option->fontMetrics.horizontalAdvance(m_labelText);

    QPointF valuePoint(labelPoint.x(), labelPoint.y() + option->fontMetrics.lineSpacing() * 1.1);
    painter->drawText(valuePoint, m_valueText);
    double valueRightBound = valuePoint.x() + option->fontMetrics.horizontalAdvance(m_valueText);

    m_input = QPointF(0, valuePoint.y() + option->fontMetrics.height() * 0.5 + ComponentSizes::InductanceTurnRadius);

    QPointF inductanceLeftPoint = m_input + QPointF(ComponentSizes::HorizComponentWireLength, 0);

    // left pin
    painter->drawLine(m_input, inductanceLeftPoint);

    double rectX = inductanceLeftPoint.x();
    double rectY = inductanceLeftPoint.y() - ComponentSizes::InductanceTurnRadius;
    double rectEdge = 2 * ComponentSizes::InductanceTurnRadius;
    int startAngle = 0;
    int spanAngle = 180 * 16; // an angle is specified in 1/16th of a degree

    for (int i = 0; i < 4; ++i)
    {
        QRectF rect(rectX, rectY, rectEdge, rectEdge);
        painter->drawArc(rect, startAngle, spanAngle);
        rectX += rectEdge;
    }

    QPointF inductanceRightPoint = QPointF(rectX, m_input.y());

    int rightestPoint = std::max({ labelRightBound,
                                   valueRightBound,
                                   inductanceRightPoint.x() + ComponentSizes::HorizComponentWireLength });

    m_output = QPointF(rightestPoint, inductanceRightPoint.y());

    // right pin
    painter->drawLine(inductanceRightPoint, m_output);


    double width = rightestPoint;
    double height = m_input.y();

    if(width != m_width || height != m_heght)
    {
        prepareGeometryChange();
        m_width = width;
        m_heght = height;
    }
    drawSelectionRect(painter);
    adjustPositonByPreviousComponent();
    checkYCoordinates();

}

//---------------------------------------------------------------------------------------
int InductanceSerial::type() const
{
    return Type;
}

//---------------------------------------------------------------------------------------
