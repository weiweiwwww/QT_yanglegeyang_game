#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QObject>
#include <QPushButton>

class MyButton:public QPushButton
{
    Q_OBJECT
public:
    MyButton(QWidget *parent = nullptr);
    MyButton * copyMyButton();

public:
    QList<MyButton> *buttomBtnList;
    QList<MyButton> *topBtnList;
    QString myId;
};

#endif // MYBUTTON_H
