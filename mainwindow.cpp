#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cstdlib>
#include <thread>


int index=50;
Measure predata[100],latest[5];
double ave=0;

void Translate(int heart, int high,int low)
{   
    for(int i=0;i<4;i++)
    {
        latest[i].heartrate=latest[i+1].heartrate;
        latest[i].highpreasure=latest[i+1].highpreasure;
        latest[i].lowpreasure=latest[i+1].lowpreasure;
    }
    latest[4].heartrate=heart;
    latest[4].highpreasure=high;
    latest[4].lowpreasure=low;
    predata[index].heartrate=heart;
    predata[index].highpreasure=high;
    predata[index].lowpreasure=low;
    index++;
}

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow)
{
    heartValue = vector<int>(30,80);
    ui->setupUi(this);
    setMinimumSize (800, 920);  //设置最小尺寸
    connect (ui->upBtn, SIGNAL(clicked()), this, SLOT(buttonClick()));
    connect (ui->anBtn, SIGNAL(clicked()), this, SLOT(analyse()));
    Paint();
}

MainWindow::~MainWindow()
{
    render.stop = 1;
    render.quit();
    render.wait();
    delete ui;    
}

void MainWindow::buttonClick()
{
    connect(&render,SIGNAL(notify(int,int,int)),this,SLOT(update(int,int,int)));
    //this->moveToThread(&render);
    render.test();
}

void MainWindow::update(int hp, int highp, int lowp){
    Translate(hp,highp,lowp);
    heartValue.erase(heartValue.begin());
    heartValue.push_back(hp);
    strPrint.sprintf("%d %d %d", hp, highp, lowp);
    ui->textBrowser0->setText(strPrint);
    strPrint.sprintf("%d %d %d", latest[3].heartrate, latest[3].highpreasure, latest[3].lowpreasure);
    ui->textBrowserh1->setText(strPrint);
    strPrint.sprintf("%d %d %d", latest[2].heartrate, latest[2].highpreasure, latest[2].lowpreasure);
    ui->textBrowserh2->setText(strPrint);
    strPrint.sprintf("%d %d %d", latest[1].heartrate, latest[1].highpreasure, latest[1].lowpreasure);
    ui->textBrowserh3->setText(strPrint);
    strPrint.sprintf("%d %d %d", latest[0].heartrate, latest[0].highpreasure, latest[0].lowpreasure);
    ui->textBrowserh4->setText(strPrint);
    Paint();
}

void MainWindow::analyse()
{
    if(ave>60&&ave<100)
        strPrint.sprintf("您当前的健康状况:良好\n\n请继续保持！\n荤素搭配均衡点，少油少盐清淡点，纤维果蔬多吃点，食勿过量控制点，劳逸结合多走点，生活作息规律点，愿你健康多一点！");
    else
        strPrint.sprintf("您当前的健康状况:不容乐观\n\n请您注意！\n每天饮食别贪多，多餐少吃比较好。蔬菜肉蛋不可缺，新鲜瓜果务必要，营养均衡是口号，适度运动最重要，祝身体好！");
    ui->textBrowserhc->setText(strPrint);

}

void MainWindow::Paint()
{
    image = QImage(800,1000,QImage::Format_RGB32);  //画布的初始化大小设为600*500，使用32位颜色
    QColor backColor = qRgb(255,255,255);    //画布初始化背景色使用白色
    image.fill(backColor);//对画布进行填充
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);//设置反锯齿模式，好看一点

    int pointx=100,pointy=900;//确定坐标轴起点坐标，这里定义(35,280)
    int width=580-pointx,height=260;//确定坐标轴宽度跟高度 上文定义画布为600X300，宽高依此而定。

    //绘制坐标轴 坐标轴原点(35，280)
    painter.drawRect(10,610,740,700);//外围的矩形，从(5,5)起，到(590,290)结束，周围留了5的间隙。

    painter.drawLine(pointx,pointy,width+pointx,pointy);//坐标轴x宽度为width
    painter.drawLine(pointx,pointy-height,pointx,pointy);//坐标轴y高度为height

    //获得数据中最大值和最小值、平均数
    int n=30;//n为数据个数
    double sum=0;

    int _ma=0;//数组里的最大值
    int _mi=inf;
    int maxpos=0,minpos=0;
    for(int i=0;i<n;i++)
    {
        sum+=heartValue[i];
        if(heartValue[i]>_ma){
            _ma=heartValue[i];
            maxpos=i;
        }
        if(heartValue[i]<_mi){
            _mi=heartValue[i];
            minpos=i;
        }
    }
    ave=sum/n;//平均数

    double kx=(double)width/(n-1); //x轴的系数
    double ky=(double)height/_ma;//y方向的比例系数
    QPen pen,penPoint;
    pen.setColor(Qt::black);
    pen.setWidth(2);

    penPoint.setColor(Qt::blue);
    penPoint.setWidth(5);
    for(int i=0;i<n-1;i++)
    {
        //由于y轴是倒着的，所以y轴坐标要pointy-a[i]*ky 其中ky为比例系数
        painter.setPen(pen);//黑色笔用于连线
        painter.drawLine(pointx+kx*i,pointy-heartValue[i]*ky,pointx+kx*(i+1),pointy-heartValue[i+1]*ky);
        painter.setPen(penPoint);
        painter.drawPoint(pointx+kx*i,pointy-heartValue[i]*ky);
    }
    painter.drawPoint(pointx+kx*(n-1),pointy-heartValue[n-1]*ky);

    //绘制平均线
    QPen penAve;
    penAve.setColor(Qt::red);//选择红色
    penAve.setWidth(2);
    penAve.setStyle(Qt::DotLine);//线条类型为虚线
    painter.setPen(penAve);
    painter.drawLine(pointx,pointy-ave*ky,pointx+width,pointy-ave*ky);

    //绘制最大值和最小值
    QPen penMaxMin;
    penMaxMin.setColor(Qt::darkGreen);//暗绿色
    painter.setPen(penMaxMin);
    painter.drawText(pointx+kx*maxpos-kx,pointy-heartValue[maxpos]*ky-5,
                     "Max"+QString::number(_ma));
    painter.drawText(pointx+kx*minpos-kx,pointy-heartValue[minpos]*ky+15,
                     "Min"+QString::number(_mi));

    penMaxMin.setColor(Qt::red);
    penMaxMin.setWidth(7);
    painter.setPen(penMaxMin);
    painter.drawPoint(pointx+kx*maxpos,pointy-heartValue[maxpos]*ky);//
    painter.drawPoint(pointx+kx*minpos,pointy-heartValue[minpos]*ky);//


    //绘制刻度线
    QPen penDegree;
    penDegree.setColor(Qt::black);
    penDegree.setWidth(2);
    painter.setPen(penDegree);
    //画上x轴刻度线
    for(int i=0;i<10;i++)//分成10份
    {
        //选取合适的坐标，绘制一段长度为4的直线，用于表示刻度
        painter.drawLine(pointx+(i+1)*width/10,pointy,pointx+(i+1)*width/10,pointy+4);
        painter.drawText(pointx+(i+0.65)*width/10,
                         pointy+10,QString::number((int)((i+1)*((double)n/10))));
    }
    //y轴刻度线
    double _maStep=(double)_ma/10;//y轴刻度间隔需根据最大值来表示
    for(int i=0;i<10;i++)
    {
        //代码较长，但是掌握基本原理即可。
        //主要就是确定一个位置，然后画一条短短的直线表示刻度。
        painter.drawLine(pointx,pointy-(i+1)*height/10,
                         pointx-4,pointy-(i+1)*height/10);
        painter.drawText(pointx-20,pointy-(i+0.85)*height/10,
                         QString::number((int)(_maStep*(i+1))));
    }

    this->repaint();
}
