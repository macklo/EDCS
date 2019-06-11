#include "listener.h"

#include <QJsonDocument>
#include <QJsonValue>
#include <string>
#include <iostream>

Listener::Listener(QMap<QString, QStringList> *msgMap, QMutex *msgMapMutex,
                   QHostAddress myIp, QJsonArray contacts):
   msgMap_(msgMap), msgMapMutex_(msgMapMutex), myIp_(myIp), contacts_(contacts)
{

}

Listener::~Listener(){
    parent_=nullptr;
}
void Listener::run(){
    server_ = new QTcpServer();
    server_->listen(myIp_, socket_no);
    connect(server_, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
    exec();
}

void Listener::onNewConnection()
{
    QTcpSocket *clientSocket = server_->nextPendingConnection();
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(clientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onSocketStateChanged(QAbstractSocket::SocketState)));
    std::cout<<"lol"<<std::endl;
    sockets_.push_back(clientSocket);
}

void Listener::onSocketStateChanged(QAbstractSocket::SocketState socketState)
{
    if (socketState == QAbstractSocket::UnconnectedState)
    {
        QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
        sockets_.removeOne(sender);
    }
}

void Listener::onReadyRead()
{
    QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
    QByteArray data = sender->readAll();
    std::string data_string = data.toStdString();
    data = QByteArray::fromStdString(data_string.substr(0, data_string.length()-5));

    QJsonDocument jdoc = QJsonDocument::fromBinaryData(data);
    QJsonObject jobj = jdoc.object();

    if(jobj.value("areYouAlive").toBool()){
        QString senderAdd(jobj.value("senderAddress").toString());
        if(!msgMap_->contains(senderAdd)){
            msgMapMutex_->lock();
            msgMap_->insert(senderAdd, QStringList());
            msgMapMutex_->unlock();
        }
        QJsonObject toSend;
        toSend.insert("areYouAlive", true);
        toSend.insert("isAlert", false);
        toSend.insert("message", "");
        toSend.insert("receiverAddress", jobj.value("senderAddress").toString());
        toSend.insert("senderAddress", myIp_.toString());

        QByteArray byteToSend = QJsonDocument(toSend).toBinaryData();
        byteToSend.append(QByteArray::fromStdString(QString("<EOF>").toStdString()));

        for (QTcpSocket* socket : sockets_) {
            if (socket != sender)
                socket->write(byteToSend);
        }
    } else {
        pushMsg(jobj);
    }
}

void Listener::pushMsg(const QJsonObject &jobj){
    QString senderIp = jobj.value("senderAddress").toString();
    QString nick = "NA";
    for(QJsonValue jo : contacts_){
        if(!jo.toObject().value("ipAddress").toString().compare(senderIp)){
            nick = "<"+jo.toObject().value("name").toString()+">";
            break;
        }
    }
    if(jobj.value("isAlert").toBool())
        nick.prepend("<A!>");
    msgMapMutex_->lock();
    (*msgMap_)[senderIp].append(nick+jobj.value("message").toString());
    msgMapMutex_->unlock();

    emit msgMapChangeListener(senderIp);
}

