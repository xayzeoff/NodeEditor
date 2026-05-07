#include "rightframe.h"
#include "ui_rightframe.h"
#include <QDebug>
#include "graphicsview.h"

#include <QFileDialog>
#include <QStandardPaths>


RightFrame::RightFrame(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::RightFrame)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentWidget(ui->NonePage);
    connect(ui->nodeNameTextPage,&QLineEdit::textChanged, this, &RightFrame::lineEditTextChangeSlot);
    connect(ui->nodeNameOneOfManyPage,&QLineEdit::textChanged, this, &RightFrame::lineEditTextChangeSlot);

    connect(ui->q1,&QLineEdit::textChanged, this, &RightFrame::lineEditTextChangeSlot);
    connect(ui->q2,&QLineEdit::textChanged, this, &RightFrame::lineEditTextChangeSlot);
    connect(ui->q3,&QLineEdit::textChanged, this, &RightFrame::lineEditTextChangeSlot);
    connect(ui->q4,&QLineEdit::textChanged, this, &RightFrame::lineEditTextChangeSlot);

    connect(ui->nextButtonName_TextPage,&QLineEdit::textChanged, this, &RightFrame::lineEditTextChangeSlot);
    connect(ui->nextButtonNameOneOfManyPage,&QLineEdit::textChanged, this, &RightFrame::lineEditTextChangeSlot);

    connect(ui->returnButtonNameOneOfManyPage,&QLineEdit::textChanged, this, &RightFrame::lineEditTextChangeSlot);
    connect(ui->returnButtonName_TextPage,&QLineEdit::textChanged, this, &RightFrame::lineEditTextChangeSlot);

}

RightFrame::~RightFrame()
{
    delete ui;
}

void RightFrame::changeCurrentPage(QString p, Node* n)
{
    qDebug() << "changeCurrentPage" << p;

    if(n){
        Node::Settings st = n->settings();
        if (p == "QuestionPage"){
            ui->stackedWidget->setCurrentWidget(ui->QuestionPage);
        }else if(p == "TextPage"){
            ui->stackedWidget->setCurrentWidget(ui->TextPage);

            ui->nodeNameTextPage->setText(st.name);
            ui->plainTextEdit_TextPage->setPlainText(st.text);

            ui->nextButtonName_TextPage->setText(st.nextButtonText);
            ui->nextButton_TextPage->setChecked(st.nextButton);

            ui->returnButtonName_TextPage->setText(st.returnButtonText);
            ui->returnButton_TextPage->setChecked(st.returnButton);
        }else if(p == "oneOfMany"){
            ui->stackedWidget->setCurrentWidget(ui->oneOfManyPage);

            ui->nodeNameOneOfManyPage->setText(st.name);
            ui->plainTextEditOneOfManyPage->setPlainText(st.text);

            ui->nextButtonNameOneOfManyPage->setText(st.nextButtonText);
            ui->nextButtonOneOfManyPage->setChecked(st.nextButton);

            ui->returnButtonNameOneOfManyPage->setText(st.returnButtonText);
            ui->returnButtonOneOfManyPage->setChecked(st.returnButton);

            ui->test->setChecked(st.test);
            ui->q1->setText(st.q1);
            ui->q2->setText(st.q2);
            ui->q3->setText(st.q3);
            ui->q4->setText(st.q4);

            if(!st.correctAnswer.isEmpty()){
                switch(st.idAnswer) {
                case 1:     ui->radioButton_1->setEnabled(true);
                case 2:     ui->radioButton_1->setEnabled(true);
                case 3:     ui->radioButton_1->setEnabled(true);
                case 4:     ui->radioButton_1->setEnabled(true);
                default:    ;
                }
            }
        }else if(p == "Server"){

            ui->stackedWidget->setCurrentWidget(ui->ServerPage);

            ui->checkBoxServer->setChecked(st.serverSave);
            ui->checkBoxLocal->setChecked(st.localSave);

            ui->lineEditLocal->setText(st.path);
            ui->lineEditServer->setText(st.url);

            ui->comboBox->setCurrentText(st.format);




        }else{
           ui->stackedWidget->setCurrentWidget(ui->NonePage);
            qDebug() << "----";
        }

        currentNode = n;
    }else{
        ui->stackedWidget->setCurrentWidget(ui->NonePage);
    }
}

void RightFrame::setView(GraphicsView *newView)
{
    view = newView;
}

void RightFrame::on_pushButton_clicked()
{
    Node::Settings st;
    if(ui->stackedWidget->currentWidget() == ui->TextPage){
        st.name = ui->nodeNameTextPage->text();
        st.text = ui->plainTextEdit_TextPage->toPlainText();
        st.nextButtonText = ui->nextButtonName_TextPage->text();
        st.returnButtonText = ui->returnButtonName_TextPage->text();
        st.nextButton = ui->nextButton_TextPage->isChecked();
        st.returnButton = ui->returnButton_TextPage->isChecked();
        currentNode->setSettings(st);

    }else if(ui->stackedWidget->currentWidget() == ui->QuestionPage){

    }else if(ui->stackedWidget->currentWidget() == ui->oneOfManyPage){
        st.name = ui->nodeNameOneOfManyPage->text();
        st.text = ui->plainTextEditOneOfManyPage->toPlainText();
        st.nextButtonText = ui->nextButtonNameOneOfManyPage->text();
        st.returnButtonText = ui->returnButtonNameOneOfManyPage->text();
        st.nextButton = ui->nextButtonOneOfManyPage->isChecked();
        st.returnButton = ui->returnButtonOneOfManyPage->isChecked();

        st.test = ui->test->isChecked();
        st.q1 = ui->q1->text();
        st.q2 = ui->q2->text();
        st.q3 = ui->q3->text();
        st.q4 = ui->q4->text();



        if(ui->radioButton_1->isChecked()){
            st.correctAnswer = ui->q1->text();
            st.idAnswer = 1;
        }else if(ui->radioButton_2->isChecked()){
            st.correctAnswer = ui->q2->text();
            st.idAnswer = 2;
        }else if(ui->radioButton_3->isChecked()){
            st.correctAnswer = ui->q3->text();
            st.idAnswer = 3;
        }else if(ui->radioButton_4->isChecked()){
            st.correctAnswer = ui->q4->text();
            st.idAnswer = 4;
        }

    }else if(ui->stackedWidget->currentWidget() == ui->ServerPage){
        st.localSave = ui->checkBoxLocal->isChecked();
        st.serverSave = ui->checkBoxServer->isChecked();

        st.url = ui->lineEditServer->text();
        st.path = ui->lineEditLocal->text();

        st.format = ui->comboBox->currentText();

    }

    currentNode->setSettings(st);





}

void RightFrame::lineEditTextChangeSlot(const QString &arg1)
{


}



void RightFrame::on_test_checkStateChanged(const Qt::CheckState &arg1)
{
    QList<QRadioButton*> list = ui->oneOfManyPage->findChildren<QRadioButton*>();
    if(arg1 == Qt::Checked){
        //ui->radioButton_1->setCheckable(true);
        for (auto b : list) {
            b->setEnabled(true);
        }
    }else{
        for (auto b : list) {
            b->setEnabled(false);
        }
    }
}





// --------------  удалить по кнопке -----------

void RightFrame::on_pushButton_2_clicked()
{
    view->removeNodeFromFrame(currentNode);
}



void RightFrame::on_pushButton_3_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, "Сохранить данные",QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),"Tables (*.csv)");
    if(path.isEmpty()){
        return;
    }
    ui->lineEditLocal->setText(path);
}

