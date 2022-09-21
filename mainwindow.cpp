#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListIterator>
#include <QMessageBox>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    deleteBtnList=new QList<MyButton *>();

    //初始化随机数生成器，设置种子
    init_randomGenerator();

    //加载模版图案
    //创建10个模版按钮
    modelGroup = new QButtonGroup();
    this->load_element();

    // 随机生成关卡中需要的元素
    levelGroup = new QButtonGroup();
    this->create_element_list(1);
    qDebug()<<levelGroup->buttons().size();

    connect(this,&MainWindow::initClickedSig,this,&MainWindow::initClickedSlot);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addToDeleteWidget(MyButton *btn)
{
    //按照按钮的文字排序
    if(deleteBtnList->size()==0)
    {
        deleteBtnList->append(btn);
    }
    else{
        for (int i = 0; i < deleteBtnList->size(); i++)
        {
            MyButton *oldBtn = (MyButton *)deleteBtnList->at(i);
            if (oldBtn->myId == btn->myId)
            {
                deleteBtnList->insert(i,btn);
                break;
            }
            else if(i==deleteBtnList->size()-1)
            {
                deleteBtnList->append(btn);
                break;
            }
        }
    }

    //消除连续相同的三个按钮
    removeSame();

    int size=deleteBtnList->size();
    for(int i=0;i<size;i++){
        deleteBtnList->at(i)->setParent(ui->deleteWidget);
        deleteBtnList->at(i)->setGeometry(i*33+8,8,30,30);
        deleteBtnList->at(i)->show();
    }
    //gameover
    if(deleteBtnList->size()==7)
    {
        QMessageBox *messageBox=new QMessageBox(this);
        messageBox->setText("GAME OVER!");
        messageBox->exec();
        this->close();
    }
}

void MainWindow::removeSame()
{
    int sum=1;
    for (int i = 0; i < deleteBtnList->size(); i++)
    {
        MyButton *oldBtn = (MyButton *)deleteBtnList->at(i);
        for (int j = i+1; j < deleteBtnList->size(); j++)
        {
            MyButton *nextBtn = (MyButton *)deleteBtnList->at(j);
            if(oldBtn->myId==nextBtn->myId){
                sum++;
                continue;
            }
        }
        if(sum==3)
        {
            //remove
            qDebug()<<"找到三个，起始位置是："<<i;
            deleteBtnList->at(i)->setParent(NULL);
            deleteBtnList->removeAt(i);
            deleteBtnList->at(i)->setParent(NULL);
            deleteBtnList->removeAt(i);
            deleteBtnList->at(i)->setParent(NULL);
            deleteBtnList->removeAt(i);
            break;
        }
        else
        {
            sum=1;
        }
    }
}


void MainWindow::addToDeleteSlot(QAbstractButton *aBtn)
{
    MyButton *btn=(MyButton *)(aBtn);
    // 首先判断button是否为最外层的元素, 通过判断元素四个顶角是否为最表层元素
    bool ret=isClickable(btn);
    if(!ret)
        return;

    // 从levelGroup中移除btn，并加入到deleteWidget。
    QPoint current_btn_point = btn->pos();
    levelGroup->removeButton(btn);
    btn->setParent(ui->deleteWidget);
    ui->deleteWidget->show();
    addToDeleteWidget((MyButton*)btn);

    //重新计算是否可以被点击及样式
//    initClickedSlot();
    setSideBtn(current_btn_point);
}


bool MainWindow::isClickable(MyButton *btn)
{
    QPoint left_below(btn->pos().rx(),btn->pos().ry() + 28);
    MyButton * tmp_btn = (MyButton *) ui->goodsWidget->childAt(left_below);
    if (tmp_btn != btn)
    {
        qDebug() <<tmp_btn << btn;
        return false;
    }

    QPoint left_top(btn->pos().rx(),btn->pos().ry());
    tmp_btn = (MyButton *) ui->goodsWidget->childAt(left_top);
    if (tmp_btn != btn)
    {
        qDebug() <<tmp_btn << btn;
        return false;
    }

    QPoint rigth_top(btn->pos().rx() + 28,btn->pos().ry());
    tmp_btn = (MyButton *) ui->goodsWidget->childAt(rigth_top);
    if (tmp_btn != btn)
    {
        return false;
    }
    QPoint rigth_below(btn->pos().rx() + 28,btn->pos().ry() + 28);
    tmp_btn = (MyButton *) ui->goodsWidget->childAt(rigth_below);
    if (tmp_btn != btn)
    {
        return false;
    }
    return true;
}

void MainWindow::initClickedSlot()
{
    for (int i = 0 ; i < levelGroup->buttons().size(); i ++)
    {
        qDebug()<<"根据是否可点击设置颜色"<<i;
        //根据是否可点击设置颜色
        MyButton *btn=(MyButton *)levelGroup->buttons().at(i);
        setPictureByStatus(btn);
    }
}


void MainWindow::setPictureByStatus(MyButton *btn)
{
    bool btnStatus=isClickable(btn);
    if(btnStatus)
    {
        QString style=btn->styleSheet();
        btn->setStyleSheet(style+QString("image : url(:/icons/%1.png);").arg(btn->myId));
    }
    else
    {

        QString style=btn->styleSheet();
        btn->setStyleSheet(style+QString("image : url(:/icons/%1gray.png);").arg(btn->myId));
    }
}

void MainWindow::setSideBtn(QPoint current_btn_point)
{

     MyButton * tmp_btn = (MyButton *) ui->goodsWidget->childAt(current_btn_point);
     if(tmp_btn)
     {
         setPictureByStatus(tmp_btn);
     }

     QPoint left_below(current_btn_point.rx(),current_btn_point.ry() + 28);
     tmp_btn = (MyButton *) ui->goodsWidget->childAt(left_below);
     if(tmp_btn)
     {
         setPictureByStatus(tmp_btn);
     }

     QPoint rigth_top(current_btn_point.rx() + 28,current_btn_point.ry());
     tmp_btn = (MyButton *) ui->goodsWidget->childAt(rigth_top);
     if(tmp_btn)
     {
         setPictureByStatus(tmp_btn);
     }
     QPoint rigth_below(current_btn_point.rx() + 28,current_btn_point.ry() + 28);
     tmp_btn = (MyButton *) ui->goodsWidget->childAt(rigth_below);
     if(tmp_btn)
     {
         setPictureByStatus(tmp_btn);
     }
}


QImage MainWindow::toGray( QImage image )
{
    int height = image.height();
    int width = image.width();
    QImage ret(width, height, QImage::Format_Indexed8);
    ret.setColorCount(256);
    for(int i = 0; i < 256; i++)
    {
        ret.setColor(i, qRgb(i, i, i));
    }
    switch(image.format())
    {
    case QImage::Format_Indexed8:
        for(int i = 0; i < height; i ++)
        {
            const uchar *pSrc = (uchar *)image.constScanLine(i);
            uchar *pDest = (uchar *)ret.scanLine(i);
            memcpy(pDest, pSrc, width);
        }
        break;
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
        for(int i = 0; i < height; i ++)
        {
            const QRgb *pSrc = (QRgb *)image.constScanLine(i);
            uchar *pDest = (uchar *)ret.scanLine(i);

            for( int j = 0; j < width; j ++)
            {
                 pDest[j] = qGray(pSrc[j]);
            }
        }
        break;
    }
    return ret;
}


void MainWindow::on_beginGameBtn_clicked()
{
    // 将关卡元素随机分配到空间中
    this->distribution_element(0);
    emit initClickedSig();
}

//----------------------qinyoucai-----------------
void MainWindow::load_element()
{
    //加载模版图案
    //创建15个模版按钮
    for(int i=0; i<15; i++){
        MyButton *btn = new MyButton(NULL);
        btn->myId=QString::number(i+1);
        btn->setStyleSheet(QString("image : url(:/icons/%1.png);"
                                   "border-top: 1px solid green;"
                                   "border-left: 1px solid green;"
                                   "border-right: 1px solid green;"
                                   "border-bottom: 5px solid green;"
                                   "border-radius:5px;").arg(QString::number(i+1)));


        this->modelGroup->addButton(btn);
    }
}

void MainWindow::create_element_list(int level)
{
    //生成关卡中涉及的图案集
    //level代表关卡数量
    int max_element;
    if( 0 == level ){
         max_element = 195;
    }
    else
    {
        max_element = 156;
    }

    for(int i=0; i < max_element / 3; i ++)
    {
        int luck_index = generator.bounded(0,this->modelGroup->buttons().size());
        MyButton *btn=((MyButton *) (this->modelGroup->buttons().at(luck_index)))->copyMyButton();
        MyButton *btn2=((MyButton *) (this->modelGroup->buttons().at(luck_index)))->copyMyButton();
        MyButton *btn3=((MyButton *) (this->modelGroup->buttons().at(luck_index)))->copyMyButton();

        this->levelGroup->addButton(btn);
        this->levelGroup->addButton(btn2);
        this->levelGroup->addButton(btn3);

    }
    connect(levelGroup,SIGNAL(buttonClicked(QAbstractButton *)),this,SLOT(addToDeleteSlot(QAbstractButton *)));
    qDebug() << this->levelGroup->buttons().size();
}

void MainWindow::distribution_element(int level)
{
    //将关卡元素随机分配到空间中
    //并显示到parent上。
    if( 0 == level ){


        // 读取每个元素的位置
        QString path=QDir::currentPath();
        qDebug()<<path;
        QFile json_file(path+"/json/level_2.json");
        json_file.open(QIODevice::ReadOnly | QIODevice::Text);
        QString value = json_file.readAll();
        QJsonParseError error;
        QJsonDocument document = QJsonDocument::fromJson(value.toUtf8(),&error);
        json_file.close();

        QJsonObject jsonObject=document.object();
        QJsonArray array=jsonObject["node"].toArray();


        // 打乱levelGroup的顺序
        int max_element = array.size();
        QVector<int> *placement_num = new QVector<int>();     //顺序编号
        QVector<int> *placement_state = new QVector<int>();   //levelGroup中每个button放置的位置
        for(int i = 0 ; i < max_element ; i ++){
            placement_num->append(i);
        }
        for(int i = 0; i < max_element ;i ++){
            int luck_index = generator.bounded(0,placement_num->size());
            placement_state->append(placement_num->at(luck_index));
            placement_num->removeAt(luck_index);
        }

        //  循环读取每个位置的图案
        for (int i = 0 ; i < max_element; i ++)
        {
            QList<QString> tmp_list = array.at(i)[QString::number(i)].toString().split(",");
            int tmp_x = tmp_list.at(0).toInt();
            int tmp_y = tmp_list.at(1).toInt();

            this->levelGroup->buttons().at(placement_state->at(i))->setParent(ui->goodsWidget);
            this->levelGroup->buttons().at(placement_state->at(i))->setGeometry(tmp_x,tmp_y,30,30);
            this->levelGroup->buttons().at(placement_state->at(i))->show();
        }
    }
}

void MainWindow::init_randomGenerator()
{
    quint32 seed = quint32(QDateTime::currentDateTime().toMSecsSinceEpoch());
    this->generator.seed(seed);
}
