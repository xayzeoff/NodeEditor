#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QGraphicsEllipseItem>
#include <QObject>
#include <QPainter>
#include <QStyleOption>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneMouseEvent>

class Node;

class Connector : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    Connector(QGraphicsItem* parent = nullptr);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void Selectable(bool status);
    bool getRight() const;
    void setRight(bool newRight);
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    //void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    void setNode(Node *newNode);

    Node *getNode() const;

signals:
    void drawLineSignal(Connector* c,QPointF pos);
    void deleteNodeSignal(Connector* c);
    void reConnectSignal(Connector* c);
private:
    bool status = false;
    bool right;
    Node* node = nullptr;
};

#endif // CONNECTOR_H
