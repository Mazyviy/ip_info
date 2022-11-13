#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <windows.h>
#include <wininet.h>
#include <string>
#include <iostream>
#include <QTimer>
#include <QDir>
#include <QDateTime>
#include <QTextStream>
#include <QDesktopServices>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("IP info");
    this->setWindowFlags(Qt::WindowMinimizeButtonHint|Qt::WindowCloseButtonHint);
    on_pushButton_clicked();
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_pushButton_clicked()));
    connect(timer, SIGNAL(timeout()), this, SLOT(saveIp()));
    timer->start(30000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

std::string real_ip() {

    HINTERNET net = InternetOpenA("IP retriever",
        INTERNET_OPEN_TYPE_PRECONFIG,
        NULL,
        NULL,
        0);

    HINTERNET conn = InternetOpenUrlA(net,
                                     "http://myexternalip.com/raw",
                                      NULL,
                                      0,
                                      INTERNET_FLAG_RELOAD,
                                      0);

    char buffer[4096];
    DWORD read;

    InternetReadFile(conn, buffer, sizeof(buffer)/sizeof(buffer[0]), &read);
    InternetCloseHandle(net);

    return std::string(buffer, read);
}

void MainWindow::saveIp()
{
    QDateTime DateTime = QDateTime::currentDateTime();
    QString datetime = DateTime.toString("dd")+"."+DateTime.toString("MM")+"."+DateTime.toString("yyyy")+"-"+
    DateTime.toString("hh")+":"+DateTime.toString("mm")+":"+DateTime.toString("ss");

    QDir dir(QDir::homePath()+"/Desktop/IpLogs");
    if (!dir.exists()) {
        QDir().mkpath(QDir::homePath()+"/Desktop/IpLogs");
    }

    const QString qString = real_ip().c_str();
    const QString qPath(QDir::homePath()+"/Desktop/IpLogs/"+ DateTime.toString("dd")+"-"+DateTime.toString("MM")+"-"+DateTime.toString("yyyy")+".log");
    QFile qFile(qPath);
    if (qFile.open(QIODevice::Append)) {
        QTextStream out(&qFile);
        out << datetime+"\tIP: "+qString<<"\n";
        qFile.close();
    }
}

void MainWindow::on_pushButton_clicked()
{
    ui->label1->setText(real_ip().c_str());
}

void MainWindow::on_pushButton_2_clicked()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(QDir::homePath()+"/Desktop/IpLogs"));
}
