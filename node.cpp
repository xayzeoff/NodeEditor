#include "node.h"
#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include "connector.h"


Node::Node(Type type, QPixmap pixmap, QGraphicsItem *parent)
    : QObject(nullptr), QGraphicsRectItem(parent), m_type(type)
{
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);

    if (m_type == Random){
        setRect(0, 0, 260, 110);
    }else if (m_type == Action){
        setRect(0, 0, 260, 100);
    }else if (m_type == Note){
        setRect(0, 0, 200, 60);
    }else if(m_type == Start){
        setRect(0, 0, 80, 40);
    }else if(m_type == TextPage){
        setRect(0, 0, 200, 60);
    }else if(m_type == oneOfMany){
        setRect(0, 0, 200, 60);
    }else if(m_type == Server){
        setRect(0, 0, 120, 40);
    }
    else {
        setRect(0, 0, 260, 40);
    }

    icon = new QGraphicsPixmapItem(this);
    if (!pixmap.isNull()) {
        icon->setPixmap(pixmap.scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    icon->setPos(10, 10);

    text = new QGraphicsTextItem(this);
    text->setPos(35, 8);
    QFont f = text->font();
    f.setBold(true);
    f.setPointSize(10);
    text->setFont(f);


    if (m_type == Note) {
        text->setPlainText("Note");
        text->setDefaultTextColor(Qt::white);
        text->setPos(10, 5);
        icon->hide();
    }
    else if (m_type == Start) {
        text->setPlainText("Старт");
        text->setDefaultTextColor(Qt::white);
        text->setPos(10, 5);
        icon->hide();

    }else if(m_type == TextPage){
        text->setPlainText("Текстовая страница");
        text->setDefaultTextColor(QColor("#3E86AD"));
    }
    else if (m_type == Random) {
        text->setPlainText("Random");
        text->setDefaultTextColor(QColor("#3E86AD"));
    } else if (m_type == Action) {
        text->setPlainText("Условие");
        text->setDefaultTextColor(QColor("#3E86AD"));
    }else if(m_type == oneOfMany){
        text->setPlainText("Один из...");
        text->setDefaultTextColor(QColor("#3E86AD"));
    }else if(m_type == Server){
        text->setPlainText("Сохранить...");
        text->setDefaultTextColor(Qt::white);
        text->setPos(10, 5);
        icon->hide();
    }else {
        text->setPlainText("Node");
        text->setDefaultTextColor(QColor("#2C2C2C"));
    }
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //QPointF pos = event->pos();
    pressed = true;
    qDebug() << "press Node";
    if (m_type == TextPage){
        emit pressSignal("TextPage", this);
    }else if(m_type == Action){
        emit pressSignal("Action", this);

    }else if(m_type == oneOfMany){
        emit pressSignal("oneOfMany", this);
    }else if(m_type == Server){
        emit pressSignal("Server", this);
    }else{
        emit pressSignal("",this);
    }


    QGraphicsRectItem::mousePressEvent(event);
}

void Node::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(pressed){
        qDebug() << "move";
        emit moveSignal(this->getConnectors());
    }
    QGraphicsRectItem::mouseMoveEvent(event);
}

void Node::addConnector(Connector *connector)
{
    connectors.append(connector);
}

QVector<Connector*> Node::getConnectors()
{
    return connectors;
}



void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    painter->setRenderHint(QPainter::Antialiasing);
    QRectF r = rect();
    double radius = 6.0;

    if (m_type == Start) {
        painter->setBrush(QColor("#5DADE2"));
        painter->setPen(Qt::NoPen);
        bool selected = option->state & QStyle::State_Selected;
        if(selected){
            foreach (Connector* c, connectors){
                c->Selectable(true);
            }
        }else{
            foreach (Connector* c, connectors) {
                c->Selectable(false);
            }
        }
    }else if(m_type == Server){
        painter->setBrush(QColor("#5DADE2"));
        painter->setPen(Qt::NoPen);
    }else {
        painter->setBrush(Qt::white);
        bool selected = option->state & QStyle::State_Selected;
        if (selected) {
            QPen selPen(QColor("#3B82F6"), 2.5);
            selPen.setJoinStyle(Qt::RoundJoin);
            painter->setPen(selPen);
        } else {
            painter->setPen(QPen(QColor("#D9DEE7"), 1));
        }
        if(selected){
            foreach (Connector* c, connectors){
                c->Selectable(true);
            }
        }else{
            foreach (Connector* c, connectors) {
                c->Selectable(false);
            }
        }
    }
    painter->drawRoundedRect(r, radius, radius);

    if (m_type == HeaderBlue || m_type == Random || m_type == Action || m_type == TextPage || m_type == oneOfMany) {
        double hHeight = 35.0;
        QPainterPath hPath;
        hPath.addRoundedRect(r.x(), r.y(), r.width(), hHeight, radius, radius);
        hPath.addRect(r.x(), r.y() + hHeight - 5, r.width(), 5);
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor("#E9F2F8"));
        painter->drawPath(hPath.simplified());
        painter->setPen(QPen(QColor("#D9DEE7"), 1));
        painter->drawLine(r.left(), r.top() + hHeight, r.right(), r.top() + hHeight);
    }

    if (m_type == Action) {
        //     painter->setPen(QPen(QColor("#D9DEE7"), 1));
        //     painter->setBrush(Qt::white);
        //     painter->drawRoundedRect(10, 45, 110, 25, 4, 4);
        //     painter->setPen(QColor("#4A5568"));
        //     painter->drawText(QRectF(15, 45, 100, 25), Qt::AlignVCenter, "Else");

        //     painter->setPen(Qt::NoPen);
        //     painter->setBrush(QColor("#6388BD"));
        //     painter->drawRoundedRect(130, 45, 120, 25, 4, 4);
        //     painter->setPen(Qt::white);
        //     painter->drawText(QRectF(130, 45, 120, 25), Qt::AlignCenter, "If");
        //
    }

    if (m_type == Random) {
        painter->setPen(QPen(QColor("#3E86AD"), 1));
        painter->setBrush(Qt::white);
        painter->drawRoundedRect(10, 45, r.width() - 20, 25, 4, 4);
        painter->drawRoundedRect(10, 75, r.width() - 20, 25, 4, 4);
        painter->setPen(QColor("#3E86AD"));
        painter->drawText(QRectF(15, 45, 200, 25), Qt::AlignVCenter, "A (50%)");
        painter->drawText(QRectF(15, 75, 200, 25), Qt::AlignVCenter, "B (50%)");
    }

    if (option->state & QStyle::State_Selected) {
        painter->setPen(QPen(QColor("#3B82F6"), 2));
        painter->setBrush(Qt::NoBrush);
        painter->drawRoundedRect(r, radius, radius);
    }
}

Node::Type Node::getNodetype() const
{
    return m_type;
}

IfElseWidget *Node::getWdg() const
{
    return wdg;
}

void Node::setWdg(IfElseWidget *newWdg)
{
    wdg = newWdg;
}

void Node::setInputBox(QStringList lst)
{
    wdg->setInputBoxList(lst);

}




Node::Settings Node::settings() const
{
    return m_settings;
}

void Node::setSettings(const Settings &newSettings)
{
    m_settings = newSettings;
}

void Node::setName(QString name)
{
    m_settings.name = name;
}

QString Node::getName()
{
    return m_settings.name;
}