#ifndef DRAGWIDGET_H
#define DRAGWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QLabel>
#include <QPixmap>

class DragWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DragWidget(QWidget *parent = nullptr);
    DragWidget(QPixmap p, QString name, QString mimeText, QString pixPath, QWidget *parent = nullptr);

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void setData(QPixmap p, QString name, QString mimeText, QString pixPath);

private:
    void setup();
    QPoint dragStartPosition;
    QLabel* pixLabel;
    QLabel* nameLabel;
    QString mimeText;
    QString pixPath;
};

#endif // DRAGWIDGET_H
