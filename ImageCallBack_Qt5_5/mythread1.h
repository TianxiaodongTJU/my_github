#ifndef MYTHREAD1_H
#define MYTHREAD1_H

#include <QObject>
#include <QThread>
#include "MvCameraControl.h"



class MyThread1 : public QObject
{
    Q_OBJECT
public:
    explicit MyThread1(QObject *parent = nullptr);
    void PressEnterToExit(void);
    bool PrintDeviceInfo(MV_CC_DEVICE_INFO* pstMVDevInfo);
    static void __stdcall ImageCallBackEx(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser);

signals:
    void stopGrabing(int state);


public slots:
    void connectDevice();
};

#endif // MYTHREAD1_H
