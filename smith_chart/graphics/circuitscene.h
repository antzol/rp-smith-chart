#ifndef CIRCUITSCENE_H
#define CIRCUITSCENE_H

#include <QGraphicsScene>

#include <complex>

#include "constants.h"

class ComponentGraphicsItem;

class CircuitModel;

class CircuitScene : public QGraphicsScene
{
    Q_OBJECT
public:
    static const QString MimeTypeComponent;

    explicit CircuitScene(CircuitModel *circuitModel, QObject *parent = nullptr);

    const ComponentGraphicsItem* previousComponent(ComponentGraphicsItem* const comp);
    bool isFirstInShiftedGroup(ComponentGraphicsItem* const comp);

    void updateCircuitYCoordinates(double yTop, double yBottom);

public slots:
    void processComponentChange(const QModelIndex &topLeft, const QModelIndex &bottomRight,
                                    const QVector<int> &roles);
    void processSourceImpedanceChange(std::complex<double> imp);
    void processLoadImpedanceChange(std::complex<double> imp);

protected:
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    void circuitComponentCreated(int pos, ComponentGraphicsItem *componentItem);

private:
    bool isComponentsShifted() const;
    void resetComponentsShift();
    void performComponentsShift(ComponentGraphicsItem *firstComponent);

    QList<ComponentGraphicsItem*> m_circuitComponents;

    QList<ComponentGraphicsItem*> m_shiftedComponents;
    QGraphicsItemGroup *m_shiftedGroup;
    double m_xShiftValue;


    QGraphicsRectItem *m_rectangularFiller;
    double m_rectangularFillerTopY;
    double m_rectangularFillerBottomY;

    CircuitModel *m_circuitModel;
};

#endif // CIRCUITSCENE_H
