#ifndef CAPACITORSERIAL_H
#define CAPACITORSERIAL_H

#include "componentgraphicsitem.h"

class CapacitorSerial : public ComponentGraphicsItem
{

public:
    enum { Type = UserType + ComponentGraphicsItemType::CapacitorSerial };

    CapacitorSerial(QGraphicsItem *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0) override;
    int type() const override;
};

#endif // CAPACITORSERIAL_H
