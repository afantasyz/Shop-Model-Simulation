#include "shelflable.h"
#include<QTimer>

ShelfLable::ShelfLable(QString picname,int maxCap,int speed,int reTime)
{
    this->nomalPic=picname;  
    this->shelfState=1;
    this->maxCap=maxCap;
    this->nowCap=this->maxCap;
    this->consumeSpeed=speed;
    this->replenishTime=reTime;
    this->replenishCount=0;
    this->isChecked=0;
    this->totalWaitTime=0;

    this->waitTime=new QTime();
//    QPixmap shelfpic;
//    shelfpic.load(this->nomalPic);
//    this->setPixmap(shelfpic);
}

void ShelfLable::shelfUpdate(int consumerFlow)
{
    if(this->shelfState!=0)
    {
        this->nowCap-=this->consumeSpeed*consumerFlow*0.2;
        if(this->nowCap<=0)
        {
            this->nowCap=0;
            this->shelfState=0;
            this->waitTime->restart();

            QPixmap shelfpic;
            shelfpic.load(QString(":/image/shelfempty0%1.png").arg(this->picgroup));
            this->setPixmap(shelfpic);
            this->setScaledContents(true);

            emit this->shelfIsEmpty();
        }
    }
}

