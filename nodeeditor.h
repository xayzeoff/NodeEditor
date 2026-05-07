#ifndef NODEEDITOR_H
#define NODEEDITOR_H

#include <QWidget>
#include <QListWidget>
#include "graphicsview.h"
#include <QList>
#include "rightframe.h"
class QListWidgetItem;
class NodeEditor : public QWidget
{
    Q_OBJECT
public:
    struct ItemData{
        QString pix;
        QString name;
        QString text;
    };
    explicit NodeEditor(QWidget *parent = nullptr);
    void clearScene();
    void generateForm();

    GraphicsView *getGraphicsView() const;

private:
    QListWidget* listW;
    RightFrame* rightW;
    GraphicsView* graphicsView;

private slots:
    void itemPressedSlot(QListWidgetItem * item);
signals:
};

#endif // NODEEDITOR_H
