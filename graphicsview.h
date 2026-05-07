#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsLineItem>
#include "connector.h"
#include <QVector>
#include <QKeyEvent>
#include "node.h"
#include "rightframe.h"
#include "lineitem.h"
#include <QFile>
#include <QXmlStreamAttribute>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QMessageBox>
#include <QScrollBar>


class TreeNode;
struct ConnectorLine
{
    Connector* first;
    Connector* second;
    LineItem*  line;
};

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(QWidget *parent = nullptr);
    void dropEvent(QDropEvent* event) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

    void clearScene();
    void generateForm(QString path);
    void setFrame(RightFrame *newFrame);
    void setRframe(RightFrame *newRframe);
    void removeNodeFromFrame(Node* n);

    void addHtmlCode(TreeNode* tn);

    Node *getStartNode() const;


    //----------------- Сохранить, Открыть Сохранить как и тп ------------


    bool saveProject(const QString& filePath);
    bool loadProject(const QString& filePath);


    QString currentFilePath() const { return m_currentFilePath; }
    void    setCurrentFilePath(const QString& p) { m_currentFilePath = p; }

    bool writeXmlFile(QFile& file);



private slots:
    void drawLineSlot(Connector* c, QPointF pos);
    void reDrawLineSlot(QVector<Connector*> connectors);
    void changeCurrentPageSlot(QString p, Node* n);
    void removeNodeFromConnectorSlot(Connector* c);
    void reConnectFromConnectorSlot(Connector* c);

private:
    QGraphicsScene* scene;
    Connector*  activeConnector = nullptr;
    LineItem*   tempLine        = nullptr;
    QVector<ConnectorLine> cLines;
    QVector<Node*>         nodes;

    RightFrame* Rframe;

    void removeNode(Node* n);
    Node* getPrevNode(Node* n);
    QList<Node*> getNextNode(Node* n);
    void expandSceneRectIfNeeded();


    Node* createNodeFromXml(QXmlStreamReader& xml,
                            qreal x, qreal y,
                            Node::Type ntype,
                            QString& ifElseInput,
                            QString& ifElseOp,
                            QString& ifElseValue);

    int    numQuestion = 0;
    Node*  startNode   = nullptr;
    QString htmlCode;
    QString m_currentFilePath;  // путь к текущему файлу проекта

    QString generateCSS();
    QString generateJS(const QList<Node*>& ordered,
                       const QMap<Node*,int>& nodeId,
                       const QMap<Node*,int>& nextTrue,
                       const QMap<Node*,int>& nextFalse);
    QString generateBody(const QList<Node*>& ordered,
                         const QMap<Node*,int>& nodeId);
    QString typeToString(Node::Type t);

    // Ввделение --------------------------------
    bool               m_selecting    = false;
    QPointF            m_selectOrigin;
    QGraphicsRectItem* m_selectRect   = nullptr;

    //  -----------------------------------------

    QPoint lastMousePos;
    QRectF lastSceneRect;
    bool   spacePressed = false;

    // Multi-node drag
    bool               m_nodeDragging   = false;
    QPointF            m_dragStartScene;
    QMap<Node*, QPointF> m_dragStartPositions;
};

#endif
