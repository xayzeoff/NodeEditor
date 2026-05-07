#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "nodeeditor.h"
#include <QIcon>
#include <QFileDialog>
#include <QStandardPaths>
#include <QFile>
#include <QMessageBox>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    nodeEditor = new NodeEditor;
    setCentralWidget(nodeEditor);
    setWindowIcon(QIcon(":/res/icons/iconka.ico"));
    updateTitle();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::updateTitle()
{
    GraphicsView* gv = nodeEditor->getGraphicsView();
    QString path = gv ? gv->currentFilePath() : "";
    if (path.isEmpty())
        setWindowTitle("NodeEditor — Новый проект");
    else
        setWindowTitle("NodeEditor — " + QFileInfo(path).fileName());
}

bool MainWindow::saveAs()
{
    QString path = QFileDialog::getSaveFileName(
        this,
        "Сохранить проект как",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        "NodeEditor Files (*.xml);;All Files (*)"
    );
    if (path.isEmpty()) return false;

    GraphicsView* gv = nodeEditor->getGraphicsView();
    if (gv->saveProject(path)) {
        updateTitle();
        return true;
    }
    return false;
}

bool MainWindow::save()
{
    GraphicsView* gv = nodeEditor->getGraphicsView();
    QString path = gv->currentFilePath();
    if (path.isEmpty()) return saveAs();

    if (gv->saveProject(path)) {
        updateTitle();
        return true;
    }
    return false;
}


void MainWindow::on_action_triggered()
{
    int ret = QMessageBox::question(
        this, "Очистить сцену",
        "Создать новую сцену?\nВсе несохранённые данные будут потеряны.",
        QMessageBox::Yes | QMessageBox::No
    );
    if (ret == QMessageBox::Yes) {
        nodeEditor->clearScene();
        nodeEditor->getGraphicsView()->setCurrentFilePath("");
        updateTitle();
    }
}

void MainWindow::on_action_2_triggered()
{
    nodeEditor->generateForm();
}


void MainWindow::on_action_3_triggered()
{
    int ret = QMessageBox::question(
        this, "Новый проект",
        "Создать новый проект?\nВсе несохранённые данные будут потеряны.",
        QMessageBox::Yes | QMessageBox::Cancel
    );
    if (ret == QMessageBox::Yes) {
        nodeEditor->clearScene();
        nodeEditor->getGraphicsView()->setCurrentFilePath("");
        updateTitle();
    }
}

void MainWindow::on_action_4_triggered()
{
    QString path = QFileDialog::getOpenFileName(
        this,
        "Открыть проект",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        "NodeEditor Files (*.xml);;All Files (*)"
    );
    if (path.isEmpty()) return;

    GraphicsView* gv = nodeEditor->getGraphicsView();
    if (gv->loadProject(path)) {
        updateTitle();
    }
}

void MainWindow::on_action_5_triggered()
{
    save();
}

void MainWindow::on_action_6_triggered()
{
    saveAs();
}



void MainWindow::on_action_7_triggered()
{
    QTransform transform = nodeEditor->getGraphicsView()->transform();
    qDebug() << transform.m11();
    qreal scale = transform.m11()*1.1;
    if(scale > 3){
        return;
    }
    QTransform matrix;
    matrix.scale(scale, scale);
    matrix.rotate(0);

    nodeEditor->getGraphicsView()->setTransform(matrix);
}


void MainWindow::on_action_8_triggered()
{
    QTransform transform = nodeEditor->getGraphicsView()->transform();
    qreal scale = transform.m11()*0.9;
    if(scale < 0.5){
        return;
    }
    QTransform matrix;
    matrix.scale(scale, scale);
    matrix.rotate(0);

    nodeEditor->getGraphicsView()->setTransform(matrix);
}

