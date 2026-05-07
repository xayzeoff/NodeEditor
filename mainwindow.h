#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "nodeeditor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_triggered();    // Очистить сцену
    void on_action_2_triggered();  // Генерировать форму
    void on_action_3_triggered();  // Создать новый
    void on_action_4_triggered();  // Открыть
    void on_action_5_triggered();  // Сохранить
    void on_action_6_triggered();  // Сохранить как

    void on_action_7_triggered();

    void on_action_8_triggered();

private:
    void updateTitle();
    bool saveAs();
    bool save();

    Ui::MainWindow *ui;
    NodeEditor *nodeEditor;
};

#endif // MAINWINDOW_H
