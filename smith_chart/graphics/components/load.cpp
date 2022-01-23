#include "load.h"

#include <algorithm>

#include <QPainter>

#include <QStyleOptionGraphicsItem>
#include <QFontMetrics>

#include <QString>

//---------------------------------------------------------------------------------------
Load::Load(QGraphicsItem *parent) : ComponentGraphicsItem(parent)
{
    m_labelText =  QObject::tr("Load");
    m_valueText = "75 + j0";

    m_input = QPointF(0, 0);
    m_output = QPointF(0, 0);
}

//---------------------------------------------------------------------------------------
void Load::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    const double resistorWideEdgeSize = 28;
    const double resistorNarrowEdgeSize = resistorWideEdgeSize / 2.5;

    const double centerY = 30;

    double centerX = ComponentSizes::VertComponentWireLength;

    // input wire
    painter->drawLine(0, 0, centerX, 0);

    // top pin
    painter->drawLine(centerX, 0, centerX, centerY - resistorWideEdgeSize / 2);

    // rectangle
    painter->drawRect(centerX - resistorNarrowEdgeSize / 2, centerY - resistorWideEdgeSize / 2,
                      resistorNarrowEdgeSize, resistorWideEdgeSize);


    // bottom pin
    painter->drawLine(centerX, centerY + resistorWideEdgeSize / 2, centerX, 2 * centerY);

    // GND symbol
    drawGndLine(painter, QPointF(centerX, 2 * centerY));

    // labels
    QPointF labelPoint(centerX + resistorNarrowEdgeSize,
                       centerY - option->fontMetrics.height() * 0.5);
    painter->drawText(labelPoint, m_labelText);
    double labelRightBound = labelPoint.x() + option->fontMetrics.horizontalAdvance(m_labelText);

    QPointF valuePoint(labelPoint.x(), labelPoint.y() + option->fontMetrics.lineSpacing() * 1.1);
    painter->drawText(valuePoint, m_valueText);
    double valueRightBound = valuePoint.x() + option->fontMetrics.horizontalAdvance(m_valueText);

    int rightestPoint = std::max(labelRightBound, valueRightBound);

    double width = rightestPoint;
    double height = centerY * 2 + ComponentSizes::GndSymbolWidth;

    if(width != m_width || height != m_heght)
    {
        prepareGeometryChange();
        m_width = width;
        m_heght = height;
    }

    adjustPositonByPreviousComponent();
    checkYCoordinates();
}


//---------------------------------------------------------------------------------------
int Load::type() const
{
    return Type;
}

//---------------------------------------------------------------------------------------
