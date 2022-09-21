#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mybutton.h"
#include <QButtonGroup>
#include <QDebug>
#include <qrandom.h>
#include <QDateTime>
#include <QImage>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    QButtonGroup *modelGroup;          // 用于存放模板元素，
    QButtonGroup *levelGroup;          // 用于存放关卡中涉及的元素
    QList<MyButton*> *deleteBtnList;   // 用于存放消除区的元素
    QRandomGenerator generator;        // 随机数产生器

    //添加到下面消除窗口
    void addToDeleteWidget(MyButton *btn);

    // 销毁三个相同物品
    void removeSame();

    // 加载模版图案
    void load_element();

    //生成关卡中涉及的图案集
    void create_element_list(int level = 0); //level代表关卡数量

    // 将关卡元素随机分配到空间中
    void distribution_element(int level = 0); // level代表关卡数量

    //产生随机数
    void init_randomGenerator();

    //判断是否可以点击
    bool isClickable(MyButton *btn);

    QImage toGray( QImage image );
    //判断按钮状态并设置图片

    void setPictureByStatus(MyButton *btn);
    //设置邻近的按钮的状态

    void setSideBtn(QPoint current_btn_point);

    ~MainWindow();
public slots:
    //添加到消除栏槽函数
    void addToDeleteSlot(QAbstractButton *);
    // 初始化界面的所有按钮能否被点击及颜色
    void initClickedSlot();

    void on_beginGameBtn_clicked();


signals:
    void initClickedSig();

private:
    Ui::MainWindow *ui;


};
#endif // MAINWINDOW_H
