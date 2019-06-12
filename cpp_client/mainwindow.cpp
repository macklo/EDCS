#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTextStream>
#include <QJsonArray>

#include <algorithm>
#include <iostream>
#include <unistd.h>
#include <QNetworkInterface>
#include <QTimer>
#include <QColor>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton_4, SIGNAL (released()), this, SLOT (connectionPanelPopUp()));
    connect(ui->pushButton_3, SIGNAL (released()), this, SLOT (checkConnectionsSlot()));
    connect(ui->pushButton, SIGNAL(released()), this, SLOT (sendMessage()));
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this,
            SLOT(changeConversation(QListWidgetItem*)));
    ui->plainTextEdit->setReadOnly(true);
    msgBox = new CustomTextEdit();
    ui->verticalLayout->addWidget(msgBox);
    connect(msgBox, SIGNAL(enterPressed()), this, SLOT(sendMessage()));

    readContactFile("contacts.txt");
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost){
            myIp =  address;
            break;
        }
    }
    port = 11000;

    sender = new Sender(QHostAddress(myIp),&msgMap_, &msgMapMutex_);
    connect(sender, SIGNAL(msgMapChangeSignal(QString)), this, SLOT (updateMsgWindow(QString)));

    listener = new Listener(&msgMap_, &msgMapMutex_, myIp, contacts);
    connect(listener, SIGNAL(msgMapChangeListener(QString)), this, SLOT (updateMsgWindow(QString)));
    listener->run();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete sender;
    delete listener;
}

void MainWindow::changeConversation(QListWidgetItem* item){
    QString ip = getIpOfNick(item->text());
    if(!ip.compare("NA")){
        return;
    } else if(!ip.compare(currentIp)){
        return;
    }
    currentIp = ip;
    ui->plainTextEdit->clear();
    ui->plainTextEdit->update();
    for(QString msg: msgMap_[ip]){
        ui->plainTextEdit->insertPlainText(msg+"\n");
    }
    return;
}

void MainWindow::sendMessage(){
    if(currentIp.isEmpty()){
        msgBox->clear();
        return;
    }
    QString msg = msgBox->toPlainText();
    bool isAlert = ui->checkBox->isChecked();
    ui->checkBox->setChecked(false);
    msgBox->clear();
    bool res = sender->sendMsgTo(msg, QHostAddress(currentIp), port, isAlert);
    if(!res)
        checkConnection(currentIp);
}

void MainWindow::updateMsgWindow(QString ip){
    if(ip != currentIp){
        if(!msgMap_[ip].size()){
            currentIp=ip;
            connectedUsers.append(ip);
            conversingWith.insert(ip,getNickOfIp(ip));
            ui->listWidget->clear();
            for(QString key: conversingWith.keys()){
                ui->listWidget->addItem(conversingWith[key]);
            }
        } else{
            return;
        }
    } else{
        if(msgMap_[ip].last().contains("<A!>")){
            ui->plainTextEdit->setTextBackgroundColor(QColor(255, 176,0));
        } else{
            ui->plainTextEdit->setTextBackgroundColor(QColor(255, 255,255));
        }
        ui->plainTextEdit->insertPlainText(msgMap_[ip].last()+"\n");
    }
}

void MainWindow::checkConnectionsSlot(){
    IpAndPort *ipWindow = new IpAndPort(contacts);
    ipWindow -> show();
    connect(ipWindow, SIGNAL(userChosen(int)), this, SLOT(checkConnection(int)));
}

void MainWindow::checkConnection(int idx){
    QString ip(contacts.at(idx).toObject().value("ipAddress").toString());
    checkConnection(ip);
}

void MainWindow::checkConnection(QString ip){
    bool changed = false;
        if(!sender->checkIfAlive(QHostAddress(ip),port)){
            QStringList::iterator it = std::find(connectedUsers.begin(),
                                                    connectedUsers.end(), ip);
            if(it != connectedUsers.end()){
                connectedUsers.erase(it);
                conversingWith[ip].prepend("<DISCONECTED>");
                changed = true;
            } else {
                WaitForConnnection *wcw = new WaitForConnnection();
                wcw->showConnectionFailMsg();
                wcw->show();
            }
        } else {
            QStringList::iterator it = conversingWith.keys().end();
            if(!conversingWith.empty()){
                it = std::find(conversingWith.keys().begin(), conversingWith.keys().end(), ip);
            }
            if(it != conversingWith.keys().end()){
                connectedUsers.append(ip);
                conversingWith[ip].remove("<DISCONECTED>", Qt::CaseInsensitive);
                changed = true;
            } else {
                WaitForConnnection *wcw = new WaitForConnnection();
                wcw->showConnectionOkMsg();
                wcw->show();
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
    WaitForConnnection *wcw = new WaitForConnnection();
    wcw->show();
    if(!sender->checkIfAlive(QHostAddress(ip), port)){
        wcw->showConnectionFailMsg();
        return false;
    } else {
        QString ip = contacts.at(idx).toObject().value("ipAddress").toString();
        if(connectedUsers.contains(ip)){
            wcw->close();
            return true;
        }
        connectedUsers.append(ip);
        if(msgMap_.contains(ip)){
            wcw->close();
            return true;
        }
        msgMapMutex_.lock();
        msgMap_.insert(ip, QStringList());
        msgMapMutex_.unlock();
        addUserToListWidget(ip);
    }
    wcw->close();
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


QString MainWindow::getNickOfIp(QString ip){
    QString nick = "NA";
    for(QJsonValue jo : contacts){
        if(!jo.toObject().value("ipAddress").toString().compare(ip)){
            nick = jo.toObject().value("name").toString();
            break;
        }
    }
    return nick;
}

QString MainWindow::getIpOfNick(QString nick){
    QString ip = "NA";
    for(QJsonValue jo : contacts){
        if(!jo.toObject().value("name").toString().compare(nick)){
            ip = jo.toObject().value("ipAddress").toString();
            break;
        }
    }
    return ip;
}
