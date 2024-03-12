#ifndef MODELWIDGET_H
#define MODELWIDGET_H

#include <QWidget>
#include"clerklable.h"
#include<shelflable.h>

class ModelWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ModelWidget(QWidget *parent = nullptr);
    ClerkLable* clerk;//员工对象
    QVector<ShelfLable*> shelf;//货架指针   这里有个bug找了我半天，货架的Id是从1开始，而在容器中的下标是从0开始，导致走到9号货架数组越界
    void modelInit();
//    void paintEvent(QPaintEvent* ev);
    double generateRandomNormalDistribution(double mean, double standardDeviation);
    void modelReset();

    QMap<int,QPair<int,int>> shelfConfig;//存储货架坐标的配置
    QMap<int,QPair<int,int>> shelfShape;//存储货架长宽
    QMap<int,QVector<int>> pathNet;//存储网路链接的邻接表
    QMap<int,QPair<int,int>> pathConfig;//存储路径坐标的配置
    QMap<int,int> shelfToPath;//货架对应的路径节点
    QMap<int,int> pathToShelf;//路径节点对应的货架
    QVector<QVector<int>> clerkSight;
    QVector<int> fixedRoute;//固定检查路线
    int horizonUnit;
    int verticalUnit;
    int repeatTime;
    int runtime;//运行时间
    int replenishTotal=0;//补货总次数
    int replenishTotalScale=0;
    QVector<int> consumerFlow;//客流量分布
    QVector<int> replenishFlow;//补货量分布
signals:
    void modelIsOver();

};

#endif // MODELWIDGET_H
