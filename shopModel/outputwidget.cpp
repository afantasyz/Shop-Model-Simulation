#include "outputwidget.h"
#include<QString>

OutputWidget::OutputWidget(QWidget *parent) : QWidget(parent)
{
    this->chartView01=new QChartView(this);//人流表
    this->chartView02=new QChartView(this);//需求补货表
    this->chartView03=new QChartView(this);//货架表
    this->chartView04=new QChartView(this);

    this->chartView01->resize(400,300);
    this->chartView01->move(0,0);
    this->chartView02->resize(400,300);
    this->chartView02->move(450,0);
    this->chartView03->resize(400,300);
    this->chartView03->move(0,300);
    this->chartView04->resize(400,300);
    this->chartView04->move(450,300);


}

void OutputWidget::drawChart(QVector<int>& consumerFlow,QVector<int>& demandFlow,QVector<int>& shelfCount)
{
    QChart* chart01=new QChart;
    QChart* chart02=new QChart;
    QChart* chart03=new QChart;

    this->chartView01->setChart(chart01);
    this->chartView02->setChart(chart02);
    this->chartView03->setChart(chart03);
        // 创建01表的折线图
        QLineSeries *series01 = new QLineSeries;
        series01->setName("交易量数据");
        for (int i = 0; i < consumerFlow.length(); ++i) {
            series01->append(i, consumerFlow[i]);
        }


        // 创建02表的折线图
        QLineSeries *series02 = new QLineSeries;
        series02->setName("累计需求量");
        for (int i = 0; i < demandFlow.length(); ++i) {
            series02->append(i, demandFlow[i]);
        }

        //创建03表的柱状图
        QBarSeries* series03= new QBarSeries;
        QBarSet* shelfSet=new QBarSet("货架补货次数");
        for (auto& itr : shelfCount) {
            shelfSet->append(itr);
        }
        series03->append(shelfSet);

        // 添加系列到图表
        chart01->addSeries(series01);
        chart02->addSeries(series02);
        chart03->addSeries(series03);
        chart01->createDefaultAxes();
        chart02->createDefaultAxes();
        chart03->createDefaultAxes();

}

void OutputWidget::setInfoFinal(QVector<int>& shelfWaitTime)
{
    //QString infoStr=QString("系统运行时间：\n员工补货总时间：\n员工移动距离：\n货架总补货数：\n");
    QChart* chart04=new QChart;
    this->chartView04->setChart(chart04);
    QBarSeries* series04= new QBarSeries;
    QBarSet* shelfSet=new QBarSet("货架总等待时间/min");
    for (auto& itr : shelfWaitTime) {
        shelfSet->append(itr);
    }
    series04->append(shelfSet);
    chart04->addSeries(series04);
    chart04->createDefaultAxes();
}
