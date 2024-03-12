#ifndef SHELFLABLE_H
#define SHELFLABLE_H

#include <QLabel>
#include<QTime>

class ShelfLable: public QLabel
{
    Q_OBJECT
public:
    explicit ShelfLable(QString picname,int maxCap,int speed,int reTime);

    QString nomalPic;
    QTime* waitTime;
    int shelfId;
    int maxCap;//最大容量
    int nowCap;//当前容量
    int consumeSpeed;//消耗速率
    int replenishTime;//补货时间
    int shelfState;//1代表非空 0代表空
    int isChecked;
    int replenishCount;//补货总数
    int picgroup;
    int totalWaitTime;

    void shelfUpdate(int consumerFlow);

signals:
    void shelfIsEmpty();
};

#endif // SHELFLABLE_H
