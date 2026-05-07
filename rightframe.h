#ifndef RIGHTFRAME_H
#define RIGHTFRAME_H

#include <QFrame>
#include "node.h"

class GraphicsView;

namespace Ui {
class RightFrame;
}

class RightFrame : public QFrame
{
    Q_OBJECT

public:
    explicit RightFrame(QWidget *parent = nullptr);
    ~RightFrame();

    void changeCurrentPage(QString p, Node * n);
    void setView(GraphicsView *newView);

signals:
    void applySettings(Node::Settings st);
private slots:
    void on_pushButton_clicked();

    void lineEditTextChangeSlot(const QString &arg1);

    void on_test_checkStateChanged(const Qt::CheckState &arg1);
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::RightFrame *ui;
    Node* currentNode;
    GraphicsView* view;
};

#endif // RIGHTFRAME_H
