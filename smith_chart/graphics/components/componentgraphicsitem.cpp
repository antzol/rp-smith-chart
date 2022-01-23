#include "componentgraphicsitem.h"
#include "circuitscene.h"

#include <QPointF>

#include <QPainter>

//---------------------------------------------------------------------------------------
ComponentGraphicsItem::ComponentGraphicsItem(QGraphicsItem *parent) : QGraphicsItem(parent)
  , m_width(0), m_heght(0)

{

}

//---------------------------------------------------------------------------------------
QRectF ComponentGraphicsItem::boundingRect() const
{
    return QRectF(0, -10, m_width, m_heght + 20);
}

//---------------------------------------------------------------------------------------
int ComponentGraphicsItem::type() const
{
    return Type;
}

//---------------------------------------------------------------------------------------
void ComponentGraphicsItem::setLabelText(const QString &txt)
{
    m_labelText = txt;
}

//---------------------------------------------------------------------------------------
void ComponentGraphicsItem::setValueText(const QString &txt)
{
    m_valueText = txt;
}

//---------------------------------------------------------------------------------------
QString ComponentGraphicsItem::labelText() const
{
    return m_labelText;
}

//---------------------------------------------------------------------------------------
QString ComponentGraphicsItem::getValueText() const
{
    return m_valueText;
}

//---------------------------------------------------------------------------------------
void ComponentGraphicsItem::adjustPositonByPreviousComponent()
{
    CircuitScene *circuitScene = getCircuitScene();
    if (!circuitScene)
        return;

    const ComponentGraphicsItem *prevComp = circuitScene->previousComponent(this);
    if (!prevComp || circuitScene->isFirstInShiftedGroup(this))
        return;

    QPointF prevCompOutput = prevComp->mapToScene(prevComp->outputPosition());
    if (m_input == prevCompOutput)
        return;

    double adjustedX = prevCompOutput.x();
    double adjustedY = prevCompOutput.y() - inputPosition().y();

    setPos(adjustedX, adjustedY);
}

//---------------------------------------------------------------------------------------
// To update the coordinates and height of the placeholder rectangle created
// when a new element is added
void ComponentGraphicsItem::checkYCoordinates() const
{
    double yTop = scenePos().y();
    double yBottom = yTop + boundingRect().height();

    CircuitScene *circuitScene = getCircuitScene();
    if (circuitScene)
        circuitScene->updateCircuitYCoordinates(yTop, yBottom);
}

//---------------------------------------------------------------------------------------
void ComponentGraphicsItem::drawConnectionDot(QPainter *painter, const QPointF &center)
{
    // save current brush
    QBrush tmpBrush = painter->brush();
    painter->setBrush(Qt::black);
    painter->drawEllipse(center.x() - ComponentSizes::ConnectionDotRadius,
                         center.y() - ComponentSizes::ConnectionDotRadius,
                         ComponentSizes::ConnectionDotRadius * 2,
                         ComponentSizes::ConnectionDotRadius * 2);
    // restore brush
    painter->setBrush(tmpBrush);
}

//---------------------------------------------------------------------------------------
void ComponentGraphicsItem::drawGndLine(QPainter *painter, const QPointF &center)
{
    // save current pen
    QPen tmpPen = painter->pen();
    // pen for GND symbol
    QPen gndPen(tmpPen);
    gndPen.setWidth(2 * tmpPen.width());
    // draw GND symbol
    painter->setPen(gndPen);
    painter->drawLine(center.x() - ComponentSizes::GndSymbolWidth/2, center.y(),
                      center.x() + ComponentSizes::GndSymbolWidth/2, center.y());
    // restore normal pen
    painter->setPen(tmpPen);
}

//---------------------------------------------------------------------------------------
void ComponentGraphicsItem::drawSelectionRect(QPainter *painter)
{
    if (isSelected())
    {
        // save current pen
        QPen tmpPen = painter->pen();
        // pen for GND symbol
        QPen selectionPen(tmpPen);
        selectionPen.setStyle(Qt::DashLine);
        selectionPen.setWidth(0.1 * tmpPen.width());
        selectionPen.setColor(Qt::red);

        painter->setPen(selectionPen);
        painter->drawRect(boundingRect());

        // restore normal pen
        painter->setPen(tmpPen);
    }
}

//---------------------------------------------------------------------------------------
CircuitScene* ComponentGraphicsItem::getCircuitScene() const
{
    CircuitScene *circuitScene = nullptr;
    QGraphicsScene *sc = scene();
    if (!sc)
        return circuitScene;

    circuitScene = qobject_cast<CircuitScene*>(sc);
    return circuitScene;
}

//---------------------------------------------------------------------------------------
