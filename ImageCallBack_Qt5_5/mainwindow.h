#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "MvCameraControl.h"
#include <QPainter>
#include <QMutex>
#include "mythread1.h"
#include <QThread>
#include "taserialthread.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    MyThread1 *HKVS;
    QThread *mythread;
    QThread *mythread_CeJu;

    static unsigned char * pData_static;
    static int key_startGrabing;


private slots:
//图像显示模块
    void on_pushButton_2_clicked();
    void drawImage();
    void dealClose();
    void threadClose();
    void on_pushButton_takePhoto_clicked();
    void pushButton_tran();
    void Grabingstate_show(int state2);
//数据采集模块
    void MWOnoffPort(void);
    void MWReadData(void);
    void MWClearRecv(void);

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    QTimer*m_timer;
    TA_SerialThread *sPortThread;



//protected:
//    void paintEvent(QPaintEvent *);

};

#endif // MAINWINDOW_H
