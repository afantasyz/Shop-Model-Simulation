#ifndef CLERKLABLE_H
#define CLERKLABLE_H

#include <QObject>
#include<QLabel>
#include<QPropertyAnimation>
#include"shelflable.h"
#include<QTimer>

class ClerkLable : public QLabel
{
    Q_OBJECT
public:
    explicit ClerkLable(QLabel *parent = nullptr);
    void moveAction(int x,int y);//运动动画
    void replenishAction(ShelfLable& shelf);
    void findNextPath(QMap<int,QVector<int>>& pathNet,QMap<int,QPair<int,int>>& pathConfig);//寻路

    QVector<int> demandShelf;//货架需求列表
    QVector<int> Path;//路径列表
    int nowPos;//当前位置
    int targetPos;//目标货架
    int nowState;//当前状态 工作 休息
    int workTime;
    int workLength;
    QTimer* replenishTime;
    int replenishId;
    int strategy;//补货策略 1时间先后 2随机移动 3定时检查
signals:
    void replenishIsOver(int);
};

#endif // CLERKLABLE_H
