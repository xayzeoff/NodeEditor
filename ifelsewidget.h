#ifndef IFELSEWIDGET_H
#define IFELSEWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>

class IfElseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit IfElseWidget(QWidget *parent = nullptr);
    void setBoxValues(int opIndx, QStringList valList, int valIndex);
    void setInputBoxList(const QStringList &newInputBoxList);

    // ---- Геттеры для генератора HTML ----
    QString getInputBoxText()  const { return inputBox->currentText();  }
    QString getOperatorText()  const { return operatorBox->currentText(); }
    QString getValueText()     const { return valueEdit->text();         }

signals:

private:
    QComboBox*  inputBox;
    QComboBox*  operatorBox;
    QComboBox*  valueBox;
    QLabel*     falseLb;
    QLineEdit*  valueEdit;

    QStringList inputBoxList;
};

#endif // IFELSEWIDGET_H
