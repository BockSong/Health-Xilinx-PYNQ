#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QtGui>
#include <vector>
#include <ctime>
#include "serialsingleton.h"

#define inf 0x3f3f3f3f

using namespace std;

namespace Ui {

class RenderThread : public QThread
{
    Q_OBJECT
signals:
    void notify(int,int,int);
public:
    int stop;
    RenderThread(QObject *parent = 0): QThread(parent){

    }
    void test(){
        start(HighestPriority);
    }
protected:
    void run(){      
        int hp = 255;
        int highp = 255;
        int lowp = 255;
        stop = 0;
        SerialSingleton& serial = SerialSingleton::getInstance();
        serial.initPort(4);
        while(1){
            if(kk(serial,&highp,&lowp,&hp, 20000) == 1){
                emit notify(hp,highp,lowp);
            }
            if(stop == 1){
                serial.closePort();
                break;
            }
        }

//        srand(time(NULL));
//        while(1){
//            hp = 50 + rand()%60;
//            highp++;
//            lowp++;
//            emit notify(hp,highp,lowp);
//            cout<<1<<endl;
//            msleep(500);
//            if(stop == 1)
//                break;
//        }
    }
};

class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QImage image;
    vector<int> heartValue;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void Paint();
public slots:
    void update(int , int , int );
protected:
    void paintEvent(QPaintEvent *){
        QPainter painter(this);
        painter.drawImage(0,0,image);
    }


private slots:
    void buttonClick();
    void analyse();

private:
    Ui::MainWindow *ui;
    QString strPrint;
    Ui::RenderThread render;
};

#endif // MAINWINDOW_H
