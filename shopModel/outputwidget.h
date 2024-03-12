#ifndef OUTPUTWIDGET_H
#define OUTPUTWIDGET_H

#include <QWidget>
#include<QChart>
#include<QChartView>
#include<QLineSeries>
#include<QValueAxis>
#include<QBarSet>
#include<QBarSeries>
QT_CHARTS_USE_NAMESPACE

class OutputWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OutputWidget(QWidget *parent = nullptr);
    QChartView* chartView01;//人流表
    QChartView* chartView02;//需求补货表
    QChartView* chartView03;//货架表
    QChartView* chartView04;//等待时长表

    void drawChart(QVector<int>& consumerFlow,QVector<int>& demandFlow,QVector<int>& shelfCount);
    void setInfoFinal(QVector<int>& shelfWaitTime);

signals:

};

#endif // OUTPUTWIDGET_H
