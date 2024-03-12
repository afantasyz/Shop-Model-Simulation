#include "modelwidget.h"
#include<QLabel>
#include<QPushButton>
//#include<QPainter>
//#include<QDebug>
#include<QTimer>
#include<QVBoxLayout>
#include<QScrollArea>
#include<QTextBrowser>
#include <QRandomGenerator>
#include <cmath>
#include <QComboBox>
#include <algorithm>

ModelWidget::ModelWidget(QWidget *parent) : QWidget(parent)
{
    //初始化
    this->modelInit();
    //创建员工
    this->clerk=new ClerkLable();
    this->clerk->setParent(this);
    this->clerk->move(this->pathConfig[0].first,this->pathConfig[0].second);
    this->clerk->nowPos=0;

    //测试部分
//    this->clerk->demandShelf.push_back(13);
//    this->clerk->findNextPath(this->pathNet,this->pathConfig);
//    qDebug()<<this->clerk->Path<<this->clerk->demandShelf;
//    for (auto itr : qAsConst(this->clerk->Path) ) {
//        this->clerk->moveAction(this->pathConfig[itr].first,this->pathConfig[itr].second);
//    }

    //右侧数值显示栏
//    QToolBox* newToolBox=new QToolBox(this);
//    newToolBox->resize(200,600);
//    newToolBox->move(700,0);
//    newToolBox->show();
//    QWidget *shopWidget = new QWidget();
//    QWidget *clerkWidget = new QWidget();
//    QWidget *shelfWidget = new QWidget();
//    shopWidget->resize(200,200);
//    clerkWidget->resize(200,200);
//    shelfWidget->resize(200,200);
//    //QVBoxLayout *layout = new QVBoxLayout(widget);
//    // 添加子部件到布局中
//    layout->addWidget(childWidget1);
//    layout->addWidget(childWidget2);

//    newToolBox->addItem(shopWidget, "运行时间");
//    newToolBox->addItem(clerkWidget, "补货员数据");
//    newToolBox->addItem(shelfWidget, "货架数据");
    QScrollArea* newScroll=new QScrollArea(this);
    newScroll->resize(200,600);
    newScroll->move(700,0);
    newScroll->show();
    QWidget* infoWidget=new QWidget;
    QVBoxLayout* infoLayout=new QVBoxLayout();

    QLabel* timeLabel=new QLabel("系统运行时间");
    infoLayout->addWidget(timeLabel);
    QLabel* timeInfo=new QLabel;
    infoLayout->addWidget(timeInfo);
    QLabel* clerkLabel=new QLabel("员工数据");
    infoLayout->addWidget(clerkLabel);
    QTextBrowser* clerkInfo=new QTextBrowser;
    infoLayout->addWidget(clerkInfo);
    QLabel* shelfLabel=new QLabel("货架数据");
    infoLayout->addWidget(shelfLabel);
    QTextBrowser* shelfInfo=new QTextBrowser;
    infoLayout->addWidget(shelfInfo);

    infoWidget->setLayout(infoLayout);
    newScroll->setWidget(infoWidget);

    //处理售罄的信号
    for (const auto& itr : qAsConst(this->shelf)) {
        connect(itr,&ShelfLable::shelfIsEmpty,this,[=](){
            if(this->clerk->strategy==2)
            {
                if(this->clerkSight[this->clerk->nowPos].contains(itr->shelfId))
                {
                    this->clerk->demandShelf.append(this->shelfToPath[itr->shelfId]);
                    itr->isChecked=1;
                }
            }
            else if(this->clerk->strategy==1)
                this->clerk->demandShelf.append(this->shelfToPath[itr->shelfId]);
            else if(this->clerk->strategy==3)
            {

            }
            this->replenishTotal+=1;
        });
    }

    //处理补货完信号
    connect(this->clerk,&ClerkLable::replenishIsOver,this,[=](int id){
        QPixmap shelfpic;
        shelfpic.load(QString(":/image/shelffull0%1.png").arg(this->shelf[id-1]->picgroup));//
        this->shelf[id-1]->setPixmap(shelfpic);

    });

    //创建员工时钟
    QTimer* clerkTime=new QTimer(this);
//    clerkTime->start(100);
    connect(clerkTime,&QTimer::timeout,this,[=](){
        if(this->clerk->nowState!=1)
        {
            if(!this->clerk->Path.empty())
            {
                int nextPos=this->clerk->Path.first();                
                this->clerk->moveAction(this->pathConfig[nextPos].first,this->pathConfig[nextPos].second);
                this->clerk->nowPos=nextPos;

                if(this->clerk->strategy==2)//员工补货策略为2时四处观察
                {
                    for (auto& itr : this->clerkSight[nextPos]) {
                        if((this->shelf[itr-1]->shelfState==0)&&(this->shelf[itr-1]->isChecked==0 ) )//
                        {
                            this->clerk->demandShelf.append(this->shelfToPath[itr]);
                            this->shelf[itr-1]->isChecked=1;
                        }
                    }
                }

                this->clerk->Path.pop_front();

                if(this->clerk->strategy!=3)
                {
                    if(nextPos==this->clerk->targetPos)
                    {
    //                    ShelfLable* tepShelf=this->shelf[this->pathToShelf[nextPos]];
                        int s_id=this->pathToShelf[nextPos]-1;
                        this->replenishTotalScale+=(this->shelf[s_id]->maxCap-this->shelf[s_id]->nowCap);
                        this->clerk->replenishAction(*this->shelf[this->pathToShelf[nextPos]-1]);//
                    }
                }

                else if(this->clerk->strategy==3 && this->pathToShelf.count(nextPos)!=0)
                {
                    int s_id=this->pathToShelf[nextPos]-1;
                    if(this->shelf[s_id]->nowCap<0.5*this->shelf[s_id]->maxCap)
                    {
                        this->replenishTotalScale+=(this->shelf[s_id]->maxCap-this->shelf[s_id]->nowCap);
                        this->clerk->replenishAction(*this->shelf[s_id]);
                    }
                }
            }

            else{
                if(!this->clerk->demandShelf.empty())
                {
                    this->clerk->findNextPath(this->pathNet,this->pathConfig);
//                    qDebug()<<this->clerk->Path;
                }
                else{
                    if(this->clerk->strategy==2)//补货策略2时四处移动
                    {
                        int randomPos=(QRandomGenerator::global()->generate())%this->pathNet[this->clerk->nowPos].size();
                        randomPos=this->pathNet[this->clerk->nowPos][randomPos];
                        this->clerk->moveAction(this->pathConfig[randomPos].first,this->pathConfig[randomPos].second);
                        this->clerk->nowPos=randomPos;

                        for (auto& itr : this->clerkSight[randomPos]) {
                            if((this->shelf[itr-1]->shelfState==0)&&(this->shelf[itr-1]->isChecked==0 ) )//
                            {
                                this->clerk->demandShelf.append(this->shelfToPath[itr]);
                                this->shelf[itr-1]->isChecked=1;
                            }
                        }
                    }

                }
            }
        }
        QString pathStr="员工运行路线: ";
        for (const auto& itr : qAsConst(this->clerk->Path)) {
            pathStr=pathStr+QString::number(itr)+' ';
        }
        QString demandStr="待补货货架编号： ";
        for (const auto& itr : qAsConst(this->clerk->demandShelf)) {
            demandStr=demandStr+QString::number(itr)+' ';
        }
        QString totalLength="员工补货总距离: ";
        totalLength+=(QString::number(this->clerk->workLength/50)+" m");
        QString totalTime="员工工作总时间: ";
        totalTime+=(QString::number(this->clerk->workTime)+" min");
        QString tepstate="员工当前状态: ";
        QVector<QString> tepVector={"空闲","补货中"};
        tepstate+=tepVector[this->clerk->nowState];
        clerkInfo->setText(pathStr+'\n'+demandStr+'\n'+totalLength+'\n'+totalTime+"\n"+tepstate);
    });

    //创建系统时钟
    QTimer* sysTime=new QTimer(this);
    this->runtime=0;
//    sysTime->start(1000);
    this->repeatTime=0;
    int* repeatSpeed=new int;
    *repeatSpeed=0;

    connect(sysTime,&QTimer::timeout,this,[=](){
        this->runtime+=20;

        int tepSpeed=this->generateRandomNormalDistribution(19.0,12.3)/3;
        while(tepSpeed<=0)
        {
            tepSpeed=this->generateRandomNormalDistribution(19.0,12.3)/3;
        }
//        qDebug()<<tepSpeed;

        QString shelfStr;
        QVector<QString> tepVector={"空","非空"};
        for (const auto& itr : qAsConst(this->shelf)) {
            itr->shelfUpdate(tepSpeed);
            shelfStr+=QString("货架编号：%1货量：%2状态:%3\n").arg(itr->shelfId).arg(itr->nowCap).arg(tepVector[itr->shelfState]);
        }
        shelfStr+=QString("总补货需求数: %1 次\n").arg(this->replenishTotal);
        shelfStr+=QString("总补货货物量: %1 单位").arg(this->replenishTotalScale);
        timeInfo->setText(QString::number((int)this->runtime/60)+" h "+QString::number(this->runtime%60)+" min");
        shelfInfo->setText(shelfStr);

        this->repeatTime++;
        *repeatSpeed+=tepSpeed;
        if(this->repeatTime>=3)
        {
           this->repeatTime=0;
           this->consumerFlow.append(*repeatSpeed);
            this->replenishFlow.append(this->replenishTotal);
//            qDebug()<<*repeatSpeed<<this->replenishTotal;
            *repeatSpeed=0;
            if(this->clerk->Path.empty()&&this->clerk->strategy==3)
            {
                this->clerk->Path=this->fixedRoute;
                std::reverse(this->fixedRoute.begin(),this->fixedRoute.end());
            }
        }



        if(this->runtime>=960)
        {
            sysTime->stop();
            clerkTime->stop();            
//            delete repeatSpeed;
            emit this->modelIsOver();
        }
    });

    //开始按钮 选择按钮
    QComboBox* stragetySelect=new QComboBox(this);
    stragetySelect->addItem("策略1：随时待命");
    stragetySelect->addItem("策略2：随机移动");
    stragetySelect->addItem("策略3：定时检查");
    stragetySelect->setCurrentIndex(0);
    stragetySelect->resize(200,30);
    stragetySelect->move(300,250);

    QPushButton* startBut=new QPushButton("开始仿真");
    startBut->setParent(this);
    startBut->move(300,300);
    connect(startBut,&QPushButton::clicked,this,[=](){
        this->clerk->strategy=stragetySelect->currentIndex()+1;
        clerkTime->start(100);
        sysTime->start(1000);
        startBut->hide();
        stragetySelect->hide();
    });

    connect(this,&ModelWidget::modelIsOver,this,[=](){
        startBut->show();
        stragetySelect->show();
    });
}

void ModelWidget::modelInit(){
    this->horizonUnit=15;
    this->verticalUnit=15;
    //货架坐标赋值
    int shelfPosX[9]={0,12,24,0,13,0,13,24,37};
    int shelfPosY[9]={0,0,0,21,21,34,34,21,21};
    for (int i=1;i<=9 ;i++ ) {
        this->shelfConfig.insert(i,qMakePair(shelfPosX[i-1]*this->horizonUnit,shelfPosY[i-1]*this->verticalUnit));
    }
    //货架形状赋值
    int shelfLenX[9]={6,6,23,7,6,7,6,7,7};
    int shelfLenY[9]={13,13,12,8,8,5,5,18,18};
    for (int i=1; i<=9;i++ ) {
        this->shelfShape.insert(i,qMakePair(shelfLenX[i-1]*this->horizonUnit,shelfLenY[i-1]*this->verticalUnit));
    }
    //货架容量赋值
    int shelfCap[9]={160,100,100,100,150,200,150,300,120};
    //货架消耗比率
    int shelfConsume[9]={7,14,0,21,12,5,20,13,8};
    //货架补货时间
    int shelfReplenish[9]={10,10,0,8,6,6,4,15,15};
    //货架图片编号
    int shelfPicgroup[9]={1,1,4,2,2,2,2,3,3};
    //显示出货架
    for (int i=1;i<=9 ;i++ ) {
        ShelfLable* tep=new ShelfLable(":/image/fullshelf.png",shelfCap[i-1],shelfConsume[i-1],shelfReplenish[i-1]);
        tep->shelfId=i;
        tep->picgroup=shelfPicgroup[i-1];
        this->shelf.append(tep);
        tep->setParent(this);
        tep->resize(this->shelfShape[i].first,this->shelfShape[i].second);
        tep->move(this->shelfConfig[i].first,this->shelfConfig[i].second);
        QPixmap shelfpic;
        shelfpic.load(QString(":/image/shelffull0%1.png").arg(tep->picgroup));
//        shelfpic.scaled(tep->width(),tep->height(),Qt::KeepAspectRatio);

        tep->setPixmap(shelfpic);
        tep->setScaledContents(true);
        tep->show();
    }

    //路径横坐标
    int pathPosX[14]={7,19,7,19,32,45,7,19,7,19,32,45,7,19};
    int pathPosY[14]={2,2,15,15,15,15,22,22,30,30,23,23,35,35};
    for (int i=0;i<14 ;i++ ) {
        this->pathConfig.insert(i,qMakePair(pathPosX[i]*this->horizonUnit,pathPosY[i]*this->verticalUnit));
    }
    //连接图
    QVector<QVector<int>> pathNets={
        {2},
        {3},
        {0,3,6},
        {1,2,4,7},
        {3,5,10},
        {4,11},
        {2,8},
        {3,9},
        {6,9,12},
        {7,8,13},
        {4},
        {5},
        {8},
        {9}
    };
    for (int i=0;i<14 ;i++ ) {
        this->pathNet.insert(i,pathNets[i]);
    }
    //路径对货架
    int shelfTo[9]={1,2,3,4,5,6,7,8,9};
    int pathTo[9]={0,1,4,6,7,12,13,10,11};
    for (int i=0;i<9 ; i++) {
        this->shelfToPath.insert(shelfTo[i],pathTo[i]);
        this->pathToShelf.insert(pathTo[i],shelfTo[i]);
    }
    //员工视线范围
    QVector<QVector<int>> sightView={
        {1,2,4,5},
        {2,5,8},
        {1,2,4,5,6,7},
        {2,5,7,8},
        {2,5,8,9},
        {8,9},
        {1,2,4,5,6,7},
        {2,5,7,8},
        {4,5,6,7},
        {5,7,8},
        {8,9},
        {9},
        {4,5,6,7},
        {5,7,8}
    };
    this->clerkSight=sightView;
    //设置固定检查路线
    QVector<int> fixedPath={0,2,6,8,12,8,9,13,9,7,3,1,3,4,10,4,5,11};
    this->fixedRoute=fixedPath;
}

//void ModelWidget::paintEvent(QPaintEvent* ev)
//{
//    QPainter* shelfPainter=new QPainter(this);
//    for (const auto& itr : qAsConst(this->pathConfig)) {
//        shelfPainter->drawEllipse(itr.first,itr.second,10,10);
//    }
//    delete shelfPainter;
//}

double ModelWidget::generateRandomNormalDistribution(double mean, double standardDeviation)
{
    // 使用QRandomGenerator生成均匀分布的随机数
        double u1 = QRandomGenerator::global()->generateDouble();
        double u2 = QRandomGenerator::global()->generateDouble();

        // 使用Box-Muller转换生成正态分布的随机数
        double z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);

        // 调整均值和标准差
        return mean + standardDeviation * z0;
}

void ModelWidget::modelReset()
{
    //系统重置
    this->runtime=0;
    this->replenishTotal=0;
    this->consumerFlow.clear();
    this->replenishFlow.clear();
    this->replenishTotalScale=0;

    //货架重置
    for (const auto& itr : qAsConst(this->shelf)) {
        itr->replenishCount=0;
        itr->isChecked=0;
        itr->totalWaitTime=0;
        itr->nowCap=itr->maxCap;
        QPixmap shelfpic;
        shelfpic.load(QString(":/image/shelffull0%1.png").arg(itr->picgroup));//
        itr->setPixmap(shelfpic);
    }

    //补货员重置
    this->clerk->workLength=0;
    this->clerk->workTime=0;
    this->clerk->Path.clear();
    this->clerk->demandShelf.clear();
    this->clerk->nowState=0;
}
