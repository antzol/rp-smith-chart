#ifndef INDUCTANCESERIAL_H
#define INDUCTANCESERIAL_H

#include "componentgraphicsitem.h"

class InductanceSerial : public ComponentGraphicsItem
{
public:
    enum { Type = UserType + ComponentGraphicsItemType::InductanceSerial };

    InductanceSerial(QGraphicsItem *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0) override;
    int type() const override;
};

#endif // INDUCTANCESERIAL_H
