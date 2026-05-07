#include "lineitem.h"
#include <QPainter>
LineItem::LineItem() {

}

void LineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // Включаем сглаживание
    painter->setRenderHint(QPainter::Antialiasing);
    QLineF myLine = line();
    QPen pen(QColor("#3B82F6"), 2);
    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);
    painter->drawLine(myLine);
}
