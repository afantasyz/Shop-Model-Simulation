#include "widget.h"
#include "ui_widget.h"
#include"modelwidget.h"
#include"outputwidget.h"
//#include<QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setFixedSize(1000,600);
    this->setWindowIcon(QIcon(":/image/shop.png"));

    //界面布局
    ui->groupBox->resize(100,this->height());
    ui->groupBox->move(0,0);
    ui->stackedWidget->resize(900,this->height());
    ui->stackedWidget->move(100,0);

    //创建输出页
    OutputWidget* outputPage=new OutputWidget(this);
    ui->stackedWidget->addWidget(outputPage);
    //创建仿真页
    this->modelPage=new ModelWidget(this);
    ui->stackedWidget->addWidget(this->modelPage);
    ui->stackedWidget->setCurrentIndex(2);
    //切换至对应页面
    connect(ui->modelBtn,&QPushButton::clicked,this,[=](){
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->inputBtn,&QPushButton::clicked,this,[=](){
        ui->stackedWidget->setCurrentIndex(1);
    });
    connect(ui->outputBtn,&QPushButton::clicked,this,[=](){
        ui->stackedWidget->setCurrentIndex(2);
    });

    //处理完成信号
    connect(this->modelPage,&ModelWidget::modelIsOver,this,[=](){

//        qDebug()<<"model is over";
        QVector<int> tepCount;
        QVector<int> tepWaitTime;
        for (auto& itr : this->modelPage->shelf) {
            tepCount.append(itr->replenishCount);
            tepWaitTime.append(itr->totalWaitTime/50);
        }
        outputPage->drawChart(modelPage->consumerFlow,modelPage->replenishFlow,tepCount);
        outputPage->setInfoFinal(tepWaitTime);
//        delete modelPage;
//        modelPage=new ModelWidget(this);
//        ui->stackedWidget->addWidget(this->modelPage);
        modelPage->modelReset();
    });

}

Widget::~Widget()
{
    delete ui;
}


