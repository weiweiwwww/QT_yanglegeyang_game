#include "mybutton.h"

MyButton::MyButton(QWidget *parent)
{
    this->setParent(parent);
    this->setMaximumSize(30,30);
    this->setMinimumSize(30,30);
    // 初始化上面和下面按钮的链表
    this->topBtnList = new QList<MyButton>();
    this->buttomBtnList = new QList<MyButton>();
}

MyButton *MyButton::copyMyButton()
{
   MyButton * res = new MyButton();
   //在初始化中，仅仅增加了Text，
   res->setText(this->text());
   res->myId=this->myId;
   res->setStyleSheet(this->styleSheet());
   return res;
}
