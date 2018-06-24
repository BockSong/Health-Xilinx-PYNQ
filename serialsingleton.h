#pragma once
#ifndef SERIALSINGLETON_H_

#define SERIALSINGLETON_H_

#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

typedef struct measure{
    int highpreasure;
    int lowpreasure;
    int heartrate;
}Measure;

class SerialSingleton

{

public:

    ~SerialSingleton();

    static SerialSingleton& getInstance(); //获取串口对象，单例模式下，每个串口只有一个类对象

    bool openPort(int com);   //打开串口

    bool initPort(int com);   //串口初始化

    int readFromPort(void* buff, DWORD size,unsigned int timeout); //从串口读数据

    bool writeToPort(const void* buff,DWORD size,unsigned int timeout);//写数据到串口

    bool closePort();   //关闭串口



private:

    SerialSingleton(); //将构造函数声明为私有，单例模式时常用方式

    SerialSingleton(const SerialSingleton& ref); //只声明不定义，防止调用隐式复制构造函数

    SerialSingleton& operator = (const SerialSingleton& ref);//只声明不定义，防止调用隐式赋值函数

    HANDLE m_ucom;  //串口句柄

};

int kk(SerialSingleton& s, int* hp,int* lp,int *h,int timeout);



#endif

