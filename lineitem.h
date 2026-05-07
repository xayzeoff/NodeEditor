#ifndef LINEITEM_H
#define LINEITEM_H

#include <QGraphicsLineItem>

class LineItem : public QGraphicsLineItem
{
public:
    LineItem();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

#endif // LINEITEM_H
