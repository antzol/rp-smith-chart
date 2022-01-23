#include "circuitscene.h"

#include "generator.h"
#include "load.h"
#include "capacitorserial.h"
#include "capacitorparallel.h"
#include "inductanceserial.h"
#include "inductanceparallel.h"

#include "componentgraphicsitem.h"

#include "circuitmodel.h"

#include <QMimeData>
#include <QKeyEvent>

#include <QGraphicsSceneDragDropEvent>

const QString CircuitScene::MimeTypeComponent{"type/component"};

//---------------------------------------------------------------------------------------
CircuitScene::CircuitScene(CircuitModel *circuitModel, QObject *parent) : QGraphicsScene(parent)
  , m_shiftedGroup(nullptr)
  , m_xShiftValue(0)
  , m_rectangularFiller(nullptr)
  , m_rectangularFillerTopY(100), m_rectangularFillerBottomY(0)
  , m_circuitModel(circuitModel)
{
    Generator *gen = new Generator();
    addItem(gen);
    m_circuitComponents.append(gen);

    Load *load = new Load();
    addItem(load);
    m_circuitComponents.append(load);

    connect(m_circuitModel, &CircuitModel::dataChanged, this, &CircuitScene::processComponentChange);
}

//---------------------------------------------------------------------------------------
const ComponentGraphicsItem* CircuitScene::previousComponent(ComponentGraphicsItem *const comp)
{
    int idx = m_circuitComponents.indexOf(comp);

    if (idx < 1)
        return nullptr;

    return m_circuitComponents.at(idx - 1);
}

//---------------------------------------------------------------------------------------
bool CircuitScene::isFirstInShiftedGroup(ComponentGraphicsItem* const comp)
{
    if (m_shiftedComponents.empty())
        return false;

    return (m_shiftedComponents.first() == comp);
}

//---------------------------------------------------------------------------------------
void CircuitScene::updateCircuitYCoordinates(double yTop, double yBottom)
{
    if (yTop < m_rectangularFillerTopY)
        m_rectangularFillerTopY = yTop;

    if (yBottom > m_rectangularFillerBottomY)
        m_rectangularFillerBottomY = yBottom;
}

//---------------------------------------------------------------------------------------
void CircuitScene::processComponentChange(const QModelIndex &topLeft,
                                          const QModelIndex &bottomRight,
                                          const QVector<int> &roles)
{
    if (roles.indexOf(Qt::DisplayRole) == -1)
        return;

    int firstRow = topLeft.row();
    int lastRow = bottomRight.row();

    advance();
    for(int i = firstRow; i <= lastRow && i < m_circuitComponents.count() - 1; ++i)
    {
        if (i < 0)
            continue;

        QString label = m_circuitModel->index(i, CircuitModel::ColumnSymbol).data().toString();
        double val = m_circuitModel->index(i, CircuitModel::ColumnConvertedValue).data().toDouble();
        QString units = m_circuitModel->index(i, CircuitModel::ColumnUnit).data().toString();

        ComponentGraphicsItem *comp = m_circuitComponents.at(i + 1);
        comp->setLabelText(label);
        comp->setValueText(QString("%1 %2").arg(val).arg(units));
        comp->update(comp->boundingRect());
    }
}

//---------------------------------------------------------------------------------------
void CircuitScene::processSourceImpedanceChange(std::complex<double> imp)
{
    QString str = QString("%1 %2 j%3")
            .arg(imp.real())
            .arg(imp.imag() < 0 ? '-' : '+')
            .arg(std::abs(imp.imag()));
    advance();
    m_circuitComponents.at(0)->setValueText(str);
}

//---------------------------------------------------------------------------------------
void CircuitScene::processLoadImpedanceChange(std::complex<double> imp)
{
    QString str = QString("%1 %2 j%3")
            .arg(imp.real())
            .arg(imp.imag() < 0 ? '-' : '+')
            .arg(std::abs(imp.imag()));
    advance();
    m_circuitComponents.at(m_circuitComponents.size() - 1)->setValueText(str);
}

//---------------------------------------------------------------------------------------
void CircuitScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    QGraphicsScene::dragMoveEvent(event);
    QPointF cursorPosition = event->scenePos();

    QGraphicsItem *hoveredItem = itemAt(cursorPosition, QTransform());

    // The cursor is over none of the components - remove the offset, if any, and exit
    if (!hoveredItem)
    {
        if (isComponentsShifted())
        {
            resetComponentsShift();
        }
        return;
    }

    // The cursor is over the rectangular placeholder for the new component.
    if (hoveredItem == m_rectangularFiller)
        return;

    ComponentGraphicsItem *hoveredComponent = static_cast<ComponentGraphicsItem*>(hoveredItem);
    if (!hoveredComponent)
        return;

    double itemCenter = hoveredComponent->x() + hoveredComponent->boundingRect().width() / 2;

    int componentIndex = m_circuitComponents.indexOf(hoveredComponent);

    // it's impossible, but just in case...
    if (componentIndex == -1)
        return;

    if (isComponentsShifted())
    {
        if (hoveredComponent != m_shiftedComponents.first())
        {
            resetComponentsShift();
        }
        else if (hoveredComponent == m_circuitComponents.last() && cursorPosition.x() > itemCenter)
        {
            resetComponentsShift();
            return;
        }
        else
        {
            return;
        }
    }


    if (cursorPosition.x() < itemCenter)
    {
        if (hoveredComponent != m_circuitComponents.first())
            performComponentsShift(hoveredComponent);
    }
    else
    {
        if (hoveredComponent != m_circuitComponents.last())
            performComponentsShift(m_circuitComponents.at(componentIndex + 1));
    }

}

//---------------------------------------------------------------------------------------
void CircuitScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    static const QMap<int, QPair<ComponentType, ConnectionType>> componentDescriptions{
        {ComponentGraphicsItemType::CapacitorSerial, {ComponentType::Capacitor, ConnectionType::Series}},
        {ComponentGraphicsItemType::CapacitorParallel, {ComponentType::Capacitor, ConnectionType::Parallel}},
        {ComponentGraphicsItemType::InductanceSerial, {ComponentType::Inductance, ConnectionType::Series}},
        {ComponentGraphicsItemType::InductanceParallel, {ComponentType::Inductance, ConnectionType::Parallel}}
    };

    QGraphicsScene::dropEvent(event);
    QPointF cursorPosition = event->scenePos();

    QGraphicsItem *hoveredItem = itemAt(cursorPosition, QTransform());
    if (hoveredItem != m_rectangularFiller)
    {
        resetComponentsShift();
        return;
    }

    const QMimeData *mimeData = event->mimeData();

    if(mimeData->hasFormat(MimeTypeComponent))
    {
        bool ok;
        int componentType = mimeData->data(MimeTypeComponent).toInt(&ok);

        ComponentGraphicsItem *component = nullptr;
        switch (componentType)
        {
        case ComponentGraphicsItemType::CapacitorSerial:
            component = new CapacitorSerial();
            break;
        case ComponentGraphicsItemType::CapacitorParallel:
            component = new CapacitorParallel();
            break;
        case ComponentGraphicsItemType::InductanceSerial:
            component = new InductanceSerial();
            break;
        case ComponentGraphicsItemType::InductanceParallel:
            component = new InductanceParallel();
            break;
        default:
            break;
        }

        if (!component)
            return;

        int nextComponentIndex = m_circuitComponents.indexOf(m_shiftedComponents.first());
        m_circuitComponents.insert(nextComponentIndex, component);


        // The index of the new component in the CircuitModel.
        // The offset is -1 because the model does not use the signal source component,
        // which comes first in the list here.
        int modelComponentIndex = nextComponentIndex - 1;
        m_circuitModel->addCircuitComponent(modelComponentIndex,
                                            componentDescriptions.value(componentType).first,
                                            componentDescriptions.value(componentType).second);

        advance();

        addItem(component);

        ComponentGraphicsItem *prevComponent = m_circuitComponents.at(nextComponentIndex - 1);
        QPointF prevComponentOutput = prevComponent->mapToScene(prevComponent->outputPosition());
        QPointF newComponentInput = component->inputPosition();
        QPointF newComponentPos = QPointF(prevComponentOutput.x(),
                                          prevComponentOutput.y() - newComponentInput.y());
        component->setPos(newComponentPos);

    }

    resetComponentsShift();

}

//---------------------------------------------------------------------------------------
void CircuitScene::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    Q_UNUSED(event);
    resetComponentsShift();
}

//---------------------------------------------------------------------------------------
void CircuitScene::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete && m_circuitComponents.size() > 2)
    {
        for(int i = 1; i < m_circuitComponents.size() - 1; ++i)
        {
            if (m_circuitComponents.at(i)->isSelected())
            {
                ComponentGraphicsItem *comp = m_circuitComponents.takeAt(i);
                delete comp;
                m_circuitModel->deleteCircuitComponent(i-1);
            }
        }
    }
}

//---------------------------------------------------------------------------------------
void CircuitScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    for(int i = 0; i < m_circuitComponents.size(); ++i)
        m_circuitComponents.at(i)->setSelected(false);

    QGraphicsScene::mousePressEvent(event);
    QPointF cursorPosition = event->scenePos();
    QGraphicsItem *hoveredItem = itemAt(cursorPosition, QTransform());

    if (hoveredItem)
    {
        ComponentGraphicsItem *hoveredComponent = static_cast<ComponentGraphicsItem*>(hoveredItem);
        if(hoveredComponent
                && hoveredComponent != m_circuitComponents.first()
                && hoveredComponent != m_circuitComponents.last())
        {
            hoveredComponent->setSelected(true);
        }
    }
    QGraphicsScene::mousePressEvent(event);
}

//---------------------------------------------------------------------------------------
bool CircuitScene::isComponentsShifted() const
{
    return m_rectangularFiller;
}

//---------------------------------------------------------------------------------------
void CircuitScene::resetComponentsShift()
{
    advance();
    if (m_rectangularFiller)
    {
        delete m_rectangularFiller;
        m_rectangularFiller = nullptr;
    }

    if (m_shiftedGroup)
    {
        m_shiftedGroup->setX(m_shiftedGroup->x() - m_xShiftValue);
        destroyItemGroup(m_shiftedGroup);
        m_shiftedGroup = nullptr;
        m_xShiftValue = 0;
    }

    m_shiftedComponents.clear();
}

//---------------------------------------------------------------------------------------
void CircuitScene::performComponentsShift(ComponentGraphicsItem *firstComponent)
{
    int idx = m_circuitComponents.indexOf(firstComponent);

    if (idx == -1)
        return;

    QPointF rectPoint = QPointF(firstComponent->pos().x(), m_rectangularFillerTopY);

    m_shiftedComponents = m_circuitComponents.mid(idx);
    QList<QGraphicsItem *> graphicsItems;
    for (auto &comp : m_shiftedComponents)
        graphicsItems.append(qgraphicsitem_cast<QGraphicsItem*>(comp));
    m_shiftedGroup = createItemGroup(graphicsItems);
    m_xShiftValue = firstComponent->boundingRect().width() / 2 + 5;

    advance();
    firstComponent->moveBy(m_xShiftValue, 0);

    double rectHeight = m_rectangularFillerBottomY - m_rectangularFillerTopY;
    m_rectangularFiller = new QGraphicsRectItem(0, 0, m_xShiftValue, rectHeight);
    m_rectangularFiller->setPen(Qt::NoPen);
    m_rectangularFiller->setBrush(QBrush(QColor::fromRgb(230, 230, 230, 255)));
    m_rectangularFiller->setAcceptDrops(true);
    addItem(m_rectangularFiller);
    m_rectangularFiller->setPos(rectPoint);
}

//---------------------------------------------------------------------------------------
