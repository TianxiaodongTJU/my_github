#ifndef CAMERASHOW_H
#define CAMERASHOW_H

#include <QObject>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "MvCameraControl.h"
#include <QDebug>
#include <QPainter>
#include <QImage>
#include <QThread>


extern QImage image;

class Camerashow : public QThread
{
    Q_OBJECT
public:
    static Camerashow*Camerashow_change;
    explicit Camerashow(QObject *parent = nullptr);
    static void __stdcall ImageCallBackEx(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser);
    void static_emitSignal(QImage temp);
    void run();
private:
    void PressEnterToExit(void);
    bool PrintDeviceInfo(MV_CC_DEVICE_INFO* pstMVDevInfo);

signals:
    void updateImage(QImage temp);

public slots:
};

#endif // CAMERASHOW_H
