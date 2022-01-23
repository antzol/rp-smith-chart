#ifndef COMPONENTGRAPHICSITEM_H
#define COMPONENTGRAPHICSITEM_H

#include <QGraphicsItem>

class CircuitScene;

namespace ComponentSizes
{
    // GND
    inline constexpr double GndSymbolWidth{ 22.0 };
    // connection dot
    inline constexpr double ConnectionDotRadius{ 2.0 };

    // capacitor
    inline constexpr double CapacitorPlateWidth{ 22.0 };
    inline constexpr double CapacitorPlateSpace{ 4.0 };

    // inductance
    inline constexpr double InductanceTurnRadius{ 4.5 };

    inline constexpr double VertComponentWireLength{ 20.0 };
    inline constexpr double VertComponentCenterY{ 30.0 };

    inline constexpr double HorizComponentWireLength{ 20.0 };
}

namespace ComponentGraphicsItemType
{
    enum {
        Generator,
        Load,
        CapacitorSerial,
        CapacitorParallel,
        InductanceSerial,
        InductanceParallel,
        ResistorSerial,
        ResistorParallel
    };
}

class ComponentGraphicsItem : public  QGraphicsItem
{
public:

    explicit ComponentGraphicsItem(QGraphicsItem *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0) override = 0;
    QRectF boundingRect() const override;
    int type() const override;

    QPointF inputPosition() const {return m_input;}
    QPointF outputPosition() const  {return m_output;}

    void setLabelText(const QString &txt);
    void setValueText(const QString &txt);

protected:
    QString labelText() const;
    virtual QString getValueText() const;

    void adjustPositonByPreviousComponent();
    void checkYCoordinates() const;

    void drawConnectionDot(QPainter *painter, const QPointF &center);
    void drawGndLine(QPainter *painter, const QPointF &center);

    void drawSelectionRect(QPainter *painter);

    CircuitScene* getCircuitScene() const;

    QString m_labelText;
    QString m_valueText;

    double m_width;
    double m_heght;

    QPointF m_input;
    QPointF m_output;
};

#endif // COMPONENTGRAPHICSITEM_H
