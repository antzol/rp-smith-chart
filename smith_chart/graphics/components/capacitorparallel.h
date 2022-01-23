#ifndef CAPACITORPARALLEL_H
#define CAPACITORPARALLEL_H

#include "componentgraphicsitem.h"

class CapacitorParallel : public ComponentGraphicsItem
{
public:
    enum { Type = UserType + ComponentGraphicsItemType::CapacitorParallel };

    CapacitorParallel(QGraphicsItem *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0) override;
    int type() const override;
};

#endif // CAPACITORPARALLEL_H
