#include "ifelsewidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
IfElseWidget::IfElseWidget(QWidget *parent)
    : QWidget{parent}
{
    QHBoxLayout* layout = new QHBoxLayout;
    this->setLayout(layout);
    operatorBox = new QComboBox;
    valueEdit = new QLineEdit;
    inputBox = new QComboBox;
    falseLb = new QLabel("False");
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(inputBox);
    layout->addWidget(operatorBox, 0);
    QVBoxLayout* vbox = new QVBoxLayout;
    vbox->addWidget(valueEdit);
    vbox->addWidget(falseLb);
    layout->addLayout(vbox);
    layout->itemAt(0)->setAlignment(Qt::AlignTop);
    layout->itemAt(1)->setAlignment(Qt::AlignTop);
    vbox->itemAt(1)->setAlignment(Qt::AlignRight);

    operatorBox->addItems({"==", "!=", ">", "<", ">=", "<="});
    inputBox->addItems(inputBoxList);
    //valueBox->addItems({"1", "346"});

    QPalette p = this->palette();
    this->setAutoFillBackground(true);
    p.setColor(QPalette::Window, Qt::white);
    p.setColor(QPalette::Base, qRgba(219,219,219,255));
    this->setPalette(p);
    valueEdit->setStyleSheet("QLineEdit{"
                             "background:#f5f5f5;"
                              "border:1px solid #dddddd;"
                              "border-radius:5px;"
                             "color:#333333;"
                             "min-height:23px;"
                             "}"
                             "QLineEdit::focus{"
                             "background:#ffffff;"
                             "border-color:#3B82F6;"
                             "}");
}


void IfElseWidget::setBoxValues(int opIndex, QStringList valList, int valIndex)
{
    operatorBox->setCurrentIndex(opIndex);
    //valueBox->addItems(valList);
    //valueBox->setCurrentIndex(valIndex);
}

void IfElseWidget::setInputBoxList(const QStringList &newInputBoxList)
{
    inputBox->clear();
    inputBoxList = newInputBoxList;
    inputBox->addItems(newInputBoxList);
}
