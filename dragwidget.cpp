#include "dragwidget.h"
#include <QMimeData>
#include <QDrag>
#include <QApplication>
#include <QHBoxLayout>

DragWidget::DragWidget(QWidget *parent) : QWidget{parent} {
    setup();
}

DragWidget::DragWidget(QPixmap p, QString name, QString mimeText, QString pixPath, QWidget *parent)
    : QWidget{parent}
{
    setup();
    this->setData(p, name, mimeText, pixPath);
}

void DragWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton){
        dragStartPosition = event->pos();
    }
}

void DragWidget::mouseMoveEvent(QMouseEvent *event) {
    if (!(event->buttons() & Qt::LeftButton)){
        return;
    }

    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;


    mimeData->setText(mimeText); 

    mimeData->setData("application/x-icon-path", pixPath.toUtf8()); 

    drag->setMimeData(mimeData);
    drag->setPixmap(pixLabel->pixmap().scaled(32, 32, Qt::KeepAspectRatio));

    drag->exec(Qt::CopyAction | Qt::MoveAction);
}

void DragWidget::setData(QPixmap p, QString name, QString mimeText, QString pixPath) {

    pixLabel->setPixmap(p);
    nameLabel->setText(name);

    this->mimeText = mimeText;
    this->pixPath = pixPath;
}

void DragWidget::setup(){

    QHBoxLayout* box = new QHBoxLayout(this);

    nameLabel = new QLabel(this);
    pixLabel = new QLabel(this);

    box->addWidget(pixLabel, 0);
    box->addWidget(nameLabel, 1);

    this->setLayout(box);
}
