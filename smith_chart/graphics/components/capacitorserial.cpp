#include "capacitorserial.h"


#include <algorithm>
#include <initializer_list>

#include <QPainter>

#include <QStyleOptionGraphicsItem>
#include <QFontMetrics>

//---------------------------------------------------------------------------------------
CapacitorSerial::CapacitorSerial(QGraphicsItem *parent) : ComponentGraphicsItem(parent)
{
    setFlag(GraphicsItemFlag::ItemIsSelectable);
}

//---------------------------------------------------------------------------------------
void CapacitorSerial::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QPointF labelPoint(ComponentSizes::HorizComponentWireLength - option->fontMetrics.horizontalAdvance(m_labelText),
                       option->fontMetrics.height());
    painter->drawText(labelPoint, m_labelText);
    double labelRightBound = labelPoint.x() + option->fontMetrics.horizontalAdvance(m_labelText);

    QPointF valuePoint(labelPoint.x(), labelPoint.y() + option->fontMetrics.lineSpacing() * 1.1);
    painter->drawText(valuePoint, m_valueText);
    double valueRightBound = valuePoint.x() + option->fontMetrics.horizontalAdvance(m_valueText);

    m_input = QPointF(0, valuePoint.y() + option->fontMetrics.height() * 0.5 + ComponentSizes::CapacitorPlateWidth / 2);

    QPointF leftPlateCenter(m_input.x() + ComponentSizes::HorizComponentWireLength, m_input.y());
    QPointF rightPlateCenter(leftPlateCenter.x() + ComponentSizes::CapacitorPlateSpace, leftPlateCenter.y());


    int rightestPoint = std::max({ labelRightBound,
                                   valueRightBound,
                                   rightPlateCenter.x() + ComponentSizes::HorizComponentWireLength });

    m_output = QPointF(rightestPoint, rightPlateCenter.y());

    painter->drawLine(m_input, leftPlateCenter);

    painter->drawLine(QPointF(leftPlateCenter.x(), leftPlateCenter.y() - ComponentSizes::CapacitorPlateWidth/2),
                      QPointF(leftPlateCenter.x(), leftPlateCenter.y() + ComponentSizes::CapacitorPlateWidth/2));

    painter->drawLine(QPointF(rightPlateCenter.x(), rightPlateCenter.y() - ComponentSizes::CapacitorPlateWidth/2),
                      QPointF(rightPlateCenter.x(), rightPlateCenter.y() + ComponentSizes::CapacitorPlateWidth/2));

    painter->drawLine(rightPlateCenter, m_output);


    double width = rightestPoint;
    double height = rightPlateCenter.y() + ComponentSizes::CapacitorPlateWidth/2;

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
int CapacitorSerial::type() const
{
    return Type;
}

//---------------------------------------------------------------------------------------
