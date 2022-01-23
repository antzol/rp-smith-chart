#ifndef GENERATOR_H
#define GENERATOR_H

#include "componentgraphicsitem.h"

class Generator : public ComponentGraphicsItem
{
public:
    enum { Type = UserType + ComponentGraphicsItemType::Generator };

    Generator(QGraphicsItem *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0) override;
    int type() const override;
};

#endif // GENERATOR_H
