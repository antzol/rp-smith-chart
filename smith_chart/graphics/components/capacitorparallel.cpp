#include "capacitorparallel.h"

#include <algorithm>
#include <initializer_list>

#include <QPainter>

#include <QStyleOptionGraphicsItem>
#include <QFontMetrics>

//---------------------------------------------------------------------------------------
CapacitorParallel::CapacitorParallel(QGraphicsItem *parent) : ComponentGraphicsItem(parent)
{
    setFlag(GraphicsItemFlag::ItemIsSelectable);
}

//---------------------------------------------------------------------------------------
void CapacitorParallel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);


    double compCenterX = ComponentSizes::VertComponentWireLength;

    double topPlateY = ComponentSizes::VertComponentCenterY - ComponentSizes::CapacitorPlateSpace/2;
    double bottomPlateY = topPlateY + ComponentSizes::CapacitorPlateSpace;
    double gndY = ComponentSizes::VertComponentCenterY * 2;

    m_input = QPointF(0, 0);

    // input wire
    painter->drawLine(0, 0, compCenterX, 0);

    // top pin
    painter->drawLine(compCenterX, 0, compCenterX, topPlateY);

    // top plate
    painter->drawLine(compCenterX - ComponentSizes::CapacitorPlateWidth/2, topPlateY,
                      compCenterX + ComponentSizes::CapacitorPlateWidth/2, topPlateY);

    // bottom plate
    painter->drawLine(compCenterX - ComponentSizes::CapacitorPlateWidth/2, bottomPlateY,
                      compCenterX + ComponentSizes::CapacitorPlateWidth/2, bottomPlateY);

    // bottom pin
    painter->drawLine(compCenterX, bottomPlateY, compCenterX, gndY);

    // GND symbol
    drawGndLine(painter, QPointF(compCenterX, gndY));

    // labels
    QPointF labelPoint(compCenterX + ComponentSizes::CapacitorPlateWidth/2 + option->fontMetrics.height() * 0.5,
                       ComponentSizes::VertComponentCenterY - option->fontMetrics.height() * 0.5);
    painter->drawText(labelPoint, m_labelText);
    double labelRightBound = labelPoint.x() + option->fontMetrics.horizontalAdvance(m_labelText);

    QPointF valuePoint(labelPoint.x(), labelPoint.y() + option->fontMetrics.lineSpacing() * 1.1);
    painter->drawText(valuePoint, m_valueText);
    double valueRightBound = valuePoint.x() + option->fontMetrics.horizontalAdvance(m_valueText);

    int rightestPoint = std::max({labelRightBound,
                                  valueRightBound,
                                  compCenterX + ComponentSizes::VertComponentWireLength});

    // output wire
    painter->drawLine(compCenterX, 0, rightestPoint, 0);

    m_output = QPointF(rightestPoint, 0);


    drawConnectionDot(painter, QPointF(compCenterX, 0));

    double width = rightestPoint;
    double height = gndY + ComponentSizes::GndSymbolWidth;

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
int CapacitorParallel::type() const
{
    return Type;
}

//---------------------------------------------------------------------------------------
