#include "connector.h"
#include <QLine>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QMenu>
#include "node.h"

Connector::Connector(QGraphicsItem *parent) : QObject(nullptr), QGraphicsEllipseItem(parent) {
    this->setRect(-6,-6,12,12);
    this->setZValue(10);
}

QRectF Connector::boundingRect() const {
    return QRectF(-7,-7,14,14);
}

void Connector::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setRenderHint(QPainter::Antialiasing);
    QPen pen(QColor("#D9DEE7"));
    pen.setWidth(2);
    painter->setPen(pen);
    painter->setBrush(QColor("#ffffff"));
    painter->drawEllipse(-6,-6,12,12);
    if(status){
        pen.setColor(QColor("#3B82F6"));
        painter->setPen(pen);
        if(right){
            painter->drawArc(-6,-6,12,12,-96*16,180*16);
        }else{
            painter->drawArc(-6,-6,12,12,96*16,180*16);
        }
    }
}

void Connector::Selectable(bool status) {
    this->status = status;
    this->update();
}

bool Connector::getRight() const { return right; }
void Connector::setRight(bool newRight) { right = newRight; }

void Connector::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if(event->button() == Qt::LeftButton){
      emit drawLineSignal(this, event->scenePos());
    }else if(event->button() == Qt::RightButton){

    }

    //QGraphicsEllipseItem::mousePressEvent(event);
}

void Connector::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsEllipseItem::mouseReleaseEvent(event);
}

void Connector::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    QAction* reConnectAction = menu.addAction("Отсоеденить");
    QAction* deleteNodeAction = menu.addAction("Удалить узел");
    QAction *selectedAction = menu.exec(event->screenPos());
    if (selectedAction == deleteNodeAction ) {
        emit deleteNodeSignal(this);
    } else if (selectedAction == reConnectAction ) {
        emit reConnectSignal(this);
    }
}

void Connector::setNode(Node *newNode)
{
    node = newNode;
}

Node *Connector::getNode() const
{
    return node;
}
