#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTextStream>
#include <QJsonDocument>
#include <QJsonArray>

#include <algorithm>
#include <iostream>
#include <unistd.h>
#include <QNetworkInterface>
#include <QtConcurrent/QtConcurrent>
#include <QTimer>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton_4, SIGNAL (released()), this, SLOT (connectionPanelPopUp()));
    connect(ui->pushButton, SIGNAL(released()), this, SLOT (sendMessage()));

    readContactFile("contacts.txt");
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost){
            myIp =  address;
            break;
        }
    }
    port =1100;

    sender = new Sender(QHostAddress(myIp),&msgMap_, &msgMapMutex_);
    connect(sender, SIGNAL(msgMapChangeSignal(QString)), this, SLOT (updateMsgWindow(QString)));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(checkConnectionsSlot()));
    timer->start(10000);

    listener = new Listener(&msgMap_, &msgMapMutex_, myIp, contacts);
    connect(listener, SIGNAL(msgMapChangeListener(QString)), this, SLOT (updateMsgWindow(QString)));
    listener->start();
}


MainWindow::~MainWindow()
{
    delete ui;
    delete sender;
    delete listener;
}

void MainWindow::sendMessage(){
    if(!currentIp.isEmpty()){
        return;
    }
    QString msg = ui->plainTextEdit_2->toPlainText();
    bool isAlert = ui->checkBox->isChecked();
    sender->sendMsgTo(msg, QHostAddress(currentIp), port, isAlert);
    //QtConcurrent::run((Sender::sendMsgTo), sender, msg,QHostAddress(currentIp), port, isAlert);
}

void MainWindow::updateMsgWindow(QString ip){
    if(ip != currentIp){
        return;
    } else{
        ui->plainTextEdit->insertPlainText("\n"+msgMap_[ip].last());
    }
}

void MainWindow::checkConnectionsSlot(){
    //checkConnections();
}

void MainWindow::checkConnections(){
    bool changed = false;
    for(QString key: conversingWith.keys()){
        QString ip = conversingWith[key];
        if(!sender->checkIfAlive(QHostAddress(ip),port)){
            QStringList::iterator it = std::find(connectedUsers.begin(),
                                                    connectedUsers.end(), key);
            if(it != connectedUsers.end()){
                connectedUsers.erase(it);
                conversingWith[key].prepend("<DISCONECTED>");
                changed = true;
            }
        } else {
            QStringList::iterator it = std::find(connectedUsers.begin(),
                                                    connectedUsers.end(), key);
            if(it == connectedUsers.end()){
                connectedUsers.append(ip);
                conversingWith[key].remove("<DISCONECTED>", Qt::CaseInsensitive);
                changed = true;
            }
        }
    }
    if(changed){
        ui->listWidget->clear();
        for(QString key: conversingWith.keys()){
            ui->listWidget->addItem(conversingWith[key]);
        }
    }
}

void MainWindow::addUserToListWidget(QString ip){
    QString name = "";
    for(QJsonValue jv : contacts){
        if(!jv.toObject().value("ipAddress").toString().compare(ip)){
            name = jv.toObject().value("name").toString();
        }
    }
    conversingWith.insert(ip, name);
    ui->listWidget->addItem(name);
    currentIp = ip;
}

void MainWindow::connectionPanelPopUp(){
    IpAndPort *ipWindow = new IpAndPort(contacts);
    ipWindow -> show();
    connect(ipWindow, SIGNAL(userChosen(int)), this, SLOT(connectToUser(int)));
}

bool MainWindow::connectToUser(int idx){
    QHostAddress ip(contacts.at(idx).toObject().value("ipAddress").toString());
    //WaitForConnnection *wcw = new WaitForConnnection();
    //wcw->show();
    if(!sender->checkIfAlive(QHostAddress(ip), port)){
        //wcw->showConnectionFailMsg();
        usleep(350);
        //wcw->close();
        return false;
    } else {
        QString ip = contacts.at(idx).toObject().value("ipAddress").toString();
        if(connectedUsers.contains(ip)){
           // wcw->close();
            return true;
        }
        connectedUsers.append(ip);
        if(msgMap_.contains(ip)){
           // wcw->close();
            return true;
        }
        msgMapMutex_.lock();
        msgMap_.insert(ip, QStringList());
        msgMapMutex_.unlock();
        addUserToListWidget(ip);
    }
    //wcw->close();
    return true;
}

bool MainWindow::readContactFile(QString filename){
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    QString json_string;
    json_string = in.readAll();
    file.close();
    QByteArray json_bytes = json_string.toLocal8Bit();

    QJsonDocument json_doc = QJsonDocument::fromJson(json_bytes);

    contacts = json_doc.array();

    return true;
}



