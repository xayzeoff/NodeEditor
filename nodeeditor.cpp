#include "nodeeditor.h"
#include <QHBoxLayout>
#include <QListWidgetItem>
#include "dragwidget.h"
#include "rightframe.h"
#include <QFile>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>

NodeEditor::NodeEditor(QWidget *parent)
    : QWidget{parent}
{
    listW = new QListWidget;
    listW->setFocusPolicy(Qt::NoFocus);
    rightW = new RightFrame;
    graphicsView = new GraphicsView;
    graphicsView->setRframe(rightW);
    QHBoxLayout* layout = new QHBoxLayout;

    rightW->setMinimumWidth(50);

    QList<ItemData> itemsDataList {
        {":/res/icons/start.png", "Старт", "start"},
        //{":/res/icons/text.png", "Текст", "text"},
        {":/res/icons/if.png", "Условие", "radio"},
        //{":/res/icons/switch.png", "Выбор", "if"},
        {":/res/icons/page.png", "Страница", "textPage"},
        {":/res/icons/radio.png", "Один из...", "oneOfMany"},
        //{":/res/icons/start.png", "Сохранить...", "server"}
    };
    foreach (ItemData idata, itemsDataList) {
        DragWidget *drag = new DragWidget(QPixmap(idata.pix), idata.name, idata.text, idata.pix);
        QListWidgetItem *item = new QListWidgetItem;
        listW->addItem(item);
        listW->setItemWidget(item, drag);
        item->setSizeHint(drag->sizeHint());
    }

    listW->setDragDropMode(QAbstractItemView::DragOnly);

    layout->addWidget(listW);
    layout->addWidget(graphicsView);
    layout->addWidget(rightW);

    listW->setFixedWidth(150);
    rightW->setFixedWidth(260);

    this->setLayout(layout);
}

void NodeEditor::clearScene()
{
    graphicsView->clearScene();
}

void NodeEditor::generateForm()
{
    if(!graphicsView->getStartNode()){
        QMessageBox::warning(this,"Ошибка!","Нет стартового узла!\n Добавьте стартовый узел и соедините его.");
        return;
    }

    QString path = QFileDialog::getSaveFileName(this, "Сохранить форму",QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),"HTML Files (*.html *.htm)");
    if(path.isEmpty()){
        return;
    }
    graphicsView->generateForm(path);
}

GraphicsView *NodeEditor::getGraphicsView() const
{
    return graphicsView;
}

void NodeEditor::itemPressedSlot(QListWidgetItem *item)
{

}
