#include "clerklable.h"
#include<queue>

struct Node {
    int id;       // 节点的标识符
    int distance; // 起点到当前节点的最短距离

    // 构造函数
    Node(int _id, int _distance) : id(_id), distance(_distance) {}

    // 重载比较运算符
    bool operator<(const Node& other) const {
        return distance > other.distance;
    }
};

ClerkLable::ClerkLable(QLabel *parent) : QLabel(parent)
{
    QPixmap tep;
    tep.load(":/image/clerk.png");
    this->setPixmap(tep);
    this->resize(tep.width()*0.3,tep.height()*0.3);
    this->setScaledContents(true);
    this->nowState=0;
    this->workLength=0;
    this->workTime=0;
    this->strategy=1;

    this->replenishTime=new QTimer(this);
}

void ClerkLable::moveAction(int x, int y){//运动到指定位置

    QPropertyAnimation* moveAni=new QPropertyAnimation(this,"geometry");
    int moveLength=qAbs(this->x()-x)+qAbs(this->y()-y);
    this->workLength+=moveLength;
    moveAni->setDuration(100);
    moveAni->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
    moveAni->setEndValue(QRect(x,y,this->width(),this->height()));
    moveAni->setEasingCurve(QEasingCurve::Linear);
    moveAni->start();
}

void ClerkLable::replenishAction(ShelfLable& shelf)
{
    this->nowState=1;
    shelf.nowCap=shelf.maxCap;
    shelf.shelfState=1;
    shelf.replenishCount+=1;
    shelf.isChecked=0;
    QPixmap shelfpic;
    shelfpic.load(QString(":/image/shelfmid0%1.png").arg(shelf.picgroup));
    shelf.setPixmap(shelfpic);
    shelf.totalWaitTime+=shelf.waitTime->elapsed();

    this->workTime+=shelf.replenishTime;
    this->replenishId=shelf.shelfId;

    int calculateTime=(shelf.maxCap-shelf.nowCap)*50*shelf.replenishTime/shelf.maxCap;
    this->replenishTime->start(calculateTime);
    connect(this->replenishTime,&QTimer::timeout,this,[=](){
        this->nowState=0;  
        this->replenishTime->stop();
        emit this->replenishIsOver(this->replenishId);
    });
}

void ClerkLable::findNextPath(QMap<int,QVector<int>>& pathNet,QMap<int,QPair<int,int>>& pathConfig)
{
    //若需求非空
    if(!this->demandShelf.empty())
    {
        //记录最短距离，初始化为极大值 记录最短路径
        QVector<int> shortDistance(pathNet.size(),1000);
        QVector<QVector<int>> shortPath(pathNet.size());
        shortDistance[this->nowPos]=0;
        shortPath[this->nowPos]={this->nowPos};
        //定义搜索队列
        std::priority_queue<Node> pq;
        pq.push(Node(this->nowPos,0));

        while (!pq.empty()) {
            Node currNode=pq.top();
            pq.pop();
            int currId=currNode.id;
            int currDis=currNode.distance;

            for (auto itr : qAsConst(pathNet[currId])) {
                int nextId=itr;
                int nextDis=currDis+qAbs(pathConfig[nextId].first-pathConfig[currId].first)
                        +qAbs(pathConfig[nextId].second-pathConfig[currId].second);
                if(nextDis<shortDistance[nextId])
                {
                    shortDistance[nextId]=nextDis;
                    pq.push(Node(nextId,nextDis));

                    QVector<int> newPath=shortPath[currId];
                    newPath.push_back(nextId);
                    shortPath[nextId]=newPath;
                }
            }
        }
//        this->nowPos=demandShelf.first();
        this->targetPos=demandShelf.first();
        this->Path=shortPath[this->targetPos];
        if(this->Path.size()>1)//防止补货点正好为当前位置造成不补货
            this->Path.pop_front();
//        this->workLength+=shortDistance[this->nowPos];

        this->demandShelf.pop_front();
    }
}
