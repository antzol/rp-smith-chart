#ifndef LOAD_H
#define LOAD_H

#include "componentgraphicsitem.h"

class Load : public ComponentGraphicsItem
{

public:
    enum { Type = UserType + ComponentGraphicsItemType::Load };

    Load(QGraphicsItem *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0) override;
    int type() const override;
};

#endif // LOAD_H
