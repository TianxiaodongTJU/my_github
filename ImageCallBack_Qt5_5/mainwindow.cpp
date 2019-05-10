#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "MvCameraControl.h"
#include <QDebug>
#include <QPainter>
#include <opencv2/opencv.hpp>
#include <QTimer>
#include <QMutex>
#include <QThread>
#include "mythread1.h"
#include <QMessageBox>
#include <QDateTime>

unsigned char*MainWindow::pData_static = NULL;
int MainWindow::key_startGrabing = 1;


QMutex m_mutex;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
//图像采集
    ui->setupUi(this);
    HKVS = new MyThread1;
    mythread = new QThread(this);
    HKVS->moveToThread(mythread);
    mythread->start();
    m_timer = new QTimer();
    connect(ui->pushButton,&QPushButton::clicked,HKVS,&MyThread1::connectDevice);
    connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::pushButton_tran);
    connect(m_timer,&QTimer::timeout,this,&MainWindow::drawImage) ;
    connect(HKVS,&MyThread1::stopGrabing,this,&MainWindow::Grabingstate_show);
    connect(this, &MainWindow::destroyed,this, &MainWindow::dealClose);
    connect(this, &MainWindow::destroyed,this, &MainWindow::threadClose);
 //测距
    sPortThread = new TA_SerialThread;
    mythread_CeJu = new QThread(this);
    sPortThread->moveToThread(mythread_CeJu);
   connect(ui->pushButton_OpenCom, &QPushButton::clicked, this, &MainWindow::MWOnoffPort);
   connect(ui->pushButton_ClearRecv, &QPushButton::clicked, this, &MainWindow::MWClearRecv);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawImage()
{
    if (pData_static)
    {
    cv::Mat image1(cv::Size(5472, 3648), CV_8UC1);
    size_t size=5472*3648;

    QMutexLocker lock(&m_mutex);
    unsigned char* temp_char = pData_static;

    memcpy(image1.data, temp_char, size);
    QImage image2(image1.cols, image1.rows, QImage::Format_Indexed8);
    // Set the color table (used to translate colour indexes to qRgb values)
    image2.setColorCount(256);
    for(int i = 0; i < 256; i++)
    {
        image2.setColor(i, qRgb(i, i, i));
    }
    // Copy input Mat
    uchar *pSrc = image1.data;
    for(int row = 0; row < image1.rows; row ++)
    {
        uchar *pDest = image2.scanLine(row);
        memcpy(pDest, pSrc, image1.cols);
        pSrc += image1.step;
    }

    QPixmap pixmap(QPixmap::fromImage(image2));
    pixmap.scaled(ui->label->size(),Qt::KeepAspectRatio);
    ui->label->setScaledContents(1);
    ui->label->setPixmap(pixmap);
    }
}

void MainWindow::Grabingstate_show(int state2)
{
    if (state2==1)
    {
        ui->graphicsView_Grabingstate->setStyleSheet("background-color: rgb(0, 255, 0);");
    }
    else
    {
        ui->graphicsView_Grabingstate->setStyleSheet("background-color: rgb(255, 0, 0);");
    }
}

void MainWindow::threadClose()
{
    mythread->quit();
    mythread->wait();
    delete HKVS;
    delete pData_static;
}

void MainWindow::dealClose()
{
    m_timer->stop();

}

void MainWindow::on_pushButton_2_clicked()
{
    m_timer->start(200);
}

void MainWindow::on_pushButton_takePhoto_clicked()
{
    QMutexLocker lock(&m_mutex);
    unsigned char* temp_char2 = pData_static;
    if (temp_char2)
    {
    m_timer->stop();
    cv::Mat image1(cv::Size(5472, 3648), CV_8UC1);
    size_t size=5472*3648;
    memcpy(image1.data, temp_char2, size);
    QImage image2(image1.cols, image1.rows, QImage::Format_Indexed8);
    // Set the color table (used to translate colour indexes to qRgb values)
    image2.setColorCount(256);
    for(int i = 0; i < 256; i++)
    {
        image2.setColor(i, qRgb(i, i, i));
    }
    // Copy input Mat
    uchar *pSrc = image1.data;
    for(int row = 0; row < image1.rows; row ++)
    {
        uchar *pDest = image2.scanLine(row);
        memcpy(pDest, pSrc, image1.cols);
        pSrc += image1.step;
    }

    QPixmap pixmap(QPixmap::fromImage(image2));
    pixmap.scaled(ui->label->size(),Qt::KeepAspectRatio);
    ui->label->setScaledContents(1);
    ui->label->setPixmap(pixmap);

    QMessageBox::StandardButton rb = QMessageBox::information(NULL,"拍摄图片","确认是否保存该图片",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
    if (rb == QMessageBox::Yes)
    {
        pixmap.scaled(ui->label_2->size(),Qt::KeepAspectRatio);
        ui->label_2->setScaledContents(1);
        ui->label_2->setPixmap(pixmap);
        QString str_time;
        QDateTime time = QDateTime::currentDateTime();
        str_time = time.toString("yyyy-MM-dd_hh-mm-ss");
        QString str_name = QString("%1%2").arg(str_time).arg(".bmp");
        std::string str_name1 = str_name.toStdString();
        cv::imwrite(str_name1,image1);
     //   pixmap.save(str_time,);
        m_timer->start(200);
    }
    else
    {
        m_timer->start(200);
    }

    }

}

void MainWindow::pushButton_tran()
{
    if(ui->pushButton->text()==tr("连接摄像头"))
    {
        MainWindow::key_startGrabing = 1;
        ui->pushButton->setText(tr("断开连接"));
    }
    else {
        MainWindow::key_startGrabing = 0;
        ui->pushButton->setText(tr("连接摄像头"));
    }
}

void MainWindow::MWOnoffPort(void)
{
    uint8_t comIndex = ui->comboBox_com->currentIndex();
    qDebug()<<ui->comboBox_com->currentIndex();
    ui->graphicsView_ComStatus->setStyleSheet("background-color: rgb(255, 0, 0);");
    if(ui->pushButton_OpenCom->text()==tr("打开串口"))
    {
        sPortThread->InitPortName(comIndex,ui->comboBox_com->currentText());
        qint32 baudRate_setnumber = 9600;
        sPortThread->InitPortBaudRate(comIndex,baudRate_setnumber);
        sPortThread->InitPortDataBits(comIndex,QSerialPort::Data8);
        //qDebug()<<ui->comboBox_SPortDataBit->currentIndex();
        sPortThread->InitPortStopBits(comIndex,QSerialPort::OneStop);
        sPortThread->InitPortParity(comIndex,QSerialPort::NoParity);
        sPortThread->OpenPort(comIndex);
        connect(&(sPortThread->m_arrSerial[comIndex]), SIGNAL(readyRead()), this, SLOT(MWReadData()));

        //参数设置禁止
        ui->comboBox_com->setEnabled(false);
        ui->pushButton_OpenCom->setText(tr("关闭串口"));
        //发送设置使能
        ui->label_ComStatus->setText(tr("连接"));
        ui->graphicsView_ComStatus->setStyleSheet("background-color: rgb(0, 255, 0);");
    }
    else
    {
        sPortThread->ClosePort(comIndex);
        //参数设置使能
        ui->comboBox_com->setEnabled(true);
        ui->pushButton_OpenCom->setText(tr("打开串口"));
        //发送设置禁止
        ui->label_ComStatus->setText(tr("断开"));
        ui->graphicsView_ComStatus->setStyleSheet("background-color: rgb(255, 0, 0);");
    }
}

void MainWindow::MWReadData(void)
{
    QByteArray buf;

    buf = sPortThread->GetDataBuf(ui->comboBox_com->currentIndex());

    if(!buf.isEmpty())
    {
        QString str = ui->textBrowser->toPlainText();
        str += tr(buf);
        ui->textBrowser->clear();
        ui->textBrowser->append(str);
    }
    buf.clear();
}

void MainWindow::MWClearRecv(void)
{
    ui->textBrowser->clear();
}

void MainWindow::on_pushButton_3_clicked()
{
    sPortThread->SearchPort();
    for(uint8_t i = 0;i <= sPortThread->portCnt;i++)
        ui->comboBox_com->addItem(sPortThread->m_arrSerial[i].portName());
}
