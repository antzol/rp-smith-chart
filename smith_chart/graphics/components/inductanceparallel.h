#ifndef INDUCTANCEPARALLEL_H
#define INDUCTANCEPARALLEL_H

#include "componentgraphicsitem.h"

class InductanceParallel : public ComponentGraphicsItem
{
public:
    enum { Type = UserType + ComponentGraphicsItemType::InductanceParallel };

    InductanceParallel(QGraphicsItem *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0) override;
    int type() const override;
};

#endif // INDUCTANCEPARALLEL_H
