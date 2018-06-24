#include "serialsingleton.h"

bool SerialSingleton::openPort(int com)
{
    // open serial port
    if (m_ucom != INVALID_HANDLE_VALUE)
    {
        return true;
    }
    char tt[100]= "\\\\.\\COM";
    char tmp[10];
    itoa(com,tmp,10);
    strcat(tt,tmp);
    string com_port = tt;


    m_ucom = CreateFileA(com_port.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (m_ucom == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    return true;
}

bool SerialSingleton::initPort(int com)
{
    // initilize opend serial port
    if (!openPort(com))
    {
        printf("open serialfailed!/n");
        return false;
    }
    DCB dcb;
    memset(&dcb, 0, sizeof(dcb));
    GetCommState(m_ucom, &dcb);
    dcb.BaudRate = 115200;
    dcb.ByteSize = 8;
    dcb.StopBits = ONESTOPBIT;
    dcb.fParity = FALSE;
    dcb.fNull = FALSE;
    dcb.Parity = NOPARITY;
    // set serial port device parameter
    if (!SetCommState(m_ucom, &dcb))
    {
        printf("Set serialport error:%d!!/n", GetLastError());
        return false;
    }

    // set serial port receive buffer andsend buffer size
    /*if (!SetupComm(m_ucom, 1048576,1048576))
    {
    printf("Set serialreceive buffer and send buffer failed!, error %d/n", GetLastError());
    return 0;
    }*/

    // clear serial port receive bufferand send buffer
    if (!PurgeComm(m_ucom, PURGE_TXCLEAR | PURGE_RXCLEAR))
    {
        printf("clear receivebuffer and send buffer failed!, error %d/n", GetLastError());
        return 0;
    }

    return true;
}

int SerialSingleton::readFromPort(void* buff,DWORD size,unsigned int timeout)
{
   // read data from serial
   COMMTIMEOUTS readCommTimeOuts;
   memset(&readCommTimeOuts, 0,sizeof(readCommTimeOuts));
   readCommTimeOuts.ReadTotalTimeoutConstant=timeout;
   SetCommTimeouts(m_ucom,&readCommTimeOuts);
   DWORD dwReadBytes = 0;
   if (ReadFile(m_ucom,buff,size, &dwReadBytes,NULL))
   {
       return dwReadBytes;
   }
   else
   {
       printf("read data fromserial port failed, error %d/n", GetLastError());
       return -1;
   }
}

bool SerialSingleton::writeToPort(const void* buff,DWORD size,unsigned int timeout)
{
   // write data to port serial
   COMMTIMEOUTS writeCommTimeOuts;
   memset(&writeCommTimeOuts, 0,sizeof(writeCommTimeOuts));
   writeCommTimeOuts.WriteTotalTimeoutConstant=timeout;
   SetCommTimeouts(m_ucom,&writeCommTimeOuts);
   DWORD dwWriteBytes = 0;
   if (WriteFile(m_ucom,buff,size, &dwWriteBytes,NULL))
   {
       return true;
   }
   else
   {
       printf("write data toserial port failed, eror %d/n", GetLastError());
       return false;
   }
}

bool SerialSingleton::closePort()
{
  if (m_ucom != INVALID_HANDLE_VALUE)
     {
         return CloseHandle(m_ucom);
     }
  return true;
}

SerialSingleton::SerialSingleton():m_ucom(INVALID_HANDLE_VALUE)
{

}

SerialSingleton::~SerialSingleton()
{

}

SerialSingleton& SerialSingleton::getInstance()
{
   static SerialSingleton serial;
   return serial;

}

void JudgeHeartRate(int a)
{
    if(a<60)
    cout<<"您的心率过慢"<<endl;
    else if(a>100)
    cout<<"您的心率过快"<<endl;
    else
    cout<<"您的心率正常";
}


//0-failed  1-successful
int kk(SerialSingleton& s, int* hp,int* lp,int *h,int timeout){
    char bufftmp[2] = {'\0','\0'};
    char buffr[100];
    memset(buffr,'\0',sizeof(char)*100);
    *hp=*lp=*h=255;
    int flag = 0;
    while(1){
        Sleep(20);
        memset(bufftmp,'\0',sizeof(char)*2);
        if(s.readFromPort(bufftmp,1,10)!=-1)
        if(strlen(bufftmp)!=0)
         {
            if('0'<=bufftmp[0]&&bufftmp[0]<='9')
                strcat(buffr,bufftmp);
            else if(bufftmp[0] == 'a'){
                *hp = atoi(buffr);
                memset(buffr,'\0',sizeof(char)*100);
             }
             else if(bufftmp[0] == 'b'){
                 *lp = atoi(buffr);
                memset(buffr,'\0',sizeof(char)*100);
             }
             else if(bufftmp[0] == 'c'){
                *h = atoi(buffr);
                memset(buffr,'\0',sizeof(char)*100);
                flag = 1;
             }
             else{
                memset(buffr,'\0',sizeof(char)*100);
             }
         }
         if(flag == 1 &&((*hp!=0&&*hp!=255)||(*lp!=0&&*lp!=255)||(*h!=0&&*h!=255))){
            return 1;
         }
         if(timeout<0){
            return 0;
         }
         timeout -= 20;
    }
}
