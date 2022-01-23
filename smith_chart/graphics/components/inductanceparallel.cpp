#include "inductanceparallel.h"

#include <algorithm>
#include <initializer_list>

#include <QPainter>

#include <QStyleOptionGraphicsItem>
#include <QFontMetrics>

//---------------------------------------------------------------------------------------
InductanceParallel::InductanceParallel(QGraphicsItem *parent) : ComponentGraphicsItem(parent)
{
    setFlag(GraphicsItemFlag::ItemIsSelectable);
}

//---------------------------------------------------------------------------------------
void InductanceParallel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    double compCenterX = ComponentSizes::VertComponentWireLength;

    double compTopY = ComponentSizes::VertComponentCenterY - ComponentSizes::InductanceTurnRadius * 4;
    double compBottomY = ComponentSizes::VertComponentCenterY + ComponentSizes::InductanceTurnRadius * 4;
    double gndY = ComponentSizes::VertComponentCenterY * 2;

    m_input = QPointF(0, 0);

    // input wire
    painter->drawLine(0, 0, compCenterX, 0);

    // top pin
    painter->drawLine(compCenterX, 0, compCenterX, compTopY);

    double rectX = compCenterX - ComponentSizes::InductanceTurnRadius;
    double rectY = compTopY;
    double rectEdge = 2 * ComponentSizes::InductanceTurnRadius;
    int startAngle = 270 * 16;
    int spanAngle = 180 * 16; // an angle is specified in 1/16th of a degree

    for (int i = 0; i < 4; ++i)
    {
        QRectF rect(rectX, rectY, rectEdge, rectEdge);
        painter->drawArc(rect, startAngle, spanAngle);
        rectY += rectEdge;
    }

    // bottom pin
    painter->drawLine(compCenterX, compBottomY, compCenterX, gndY);

    // GND symbol
    drawGndLine(painter, QPointF(compCenterX, gndY));

    // labels
    QPointF labelPoint(compCenterX + ComponentSizes::InductanceTurnRadius + option->fontMetrics.height() * 0.5,
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
int InductanceParallel::type() const
{
    return Type;
}

//---------------------------------------------------------------------------------------
