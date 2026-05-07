#ifndef NODE_H
#define NODE_H

#include <QGraphicsRectItem>
#include <QObject>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QGraphicsProxyWidget>
#include "ifelsewidget.h"
#include "connector.h"

class GraphicsView;

class Node : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    enum Type { Simple,
                HeaderBlue,
                Note, Random,
                Action, Start,
                TextPage, oneOfMany,
                Server
    };

    struct Settings
    {
        QString name = "Страница";
        QString text;
        bool nextButton = true;
        bool returnButton = false;
        QString returnButtonText = "Назад";
        QString nextButtonText = "Дальше";

        // Один из многих
        bool test = false;
        QString q1;
        QString q2;
        QString q3;
        QString q4;

        QString correctAnswer;
        int idAnswer;


        // Сервер
        bool localSave;
        bool serverSave;
        QString path;
        QString url;

        QString format = "CSV";

        // -------------

    };

    Node(Type type = Simple, QPixmap pixmap = QPixmap(), QGraphicsItem *parent = nullptr);

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    // void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void addConnector(Connector* connector);
    QVector <Connector*> getConnectors();



    Settings settings() const;
    void setSettings(const Settings &newSettings);
    void setName(QString name);
    QString getName();


    IfElseWidget *getWdg() const;
    void setWdg(IfElseWidget *newWdg);


    void setInputBox(QStringList lst);

    Type getNodetype() const;

signals:
    void releaseSignal(QPointF pos);
    void moveSignal(QVector <Connector*> connectors);
    void pressSignal(QString p, Node* n);


protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private slots:

private:
    QVector <Connector*> connectors;
    bool pressed = false;
    Type m_type;
    Settings m_settings;
    QGraphicsTextItem* text;
    QGraphicsPixmapItem* icon;
    IfElseWidget* wdg;


};

#endif // NODE_H
