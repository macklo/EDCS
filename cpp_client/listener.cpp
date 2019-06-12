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
}
void Listener::run(){
    server_ = new QTcpServer();
    server_->listen(QHostAddress(myIp_), socket_no);
    connect(server_, &QTcpServer::newConnection, this, &Listener::onNewConnection);
}

void Listener::onNewConnection()
{
    QTcpSocket *clientSocket = server_->nextPendingConnection();
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(clientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onSocketStateChanged(QAbstractSocket::SocketState)));
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

    QJsonDocument jdoc = QJsonDocument::fromJson(data);
    QJsonObject jobj = jdoc.object();

    if(jobj.value("areYouAlive").toBool()){
        QString senderAdd(jobj.value("senderAddress").toString());
        if(!msgMap_->contains(senderAdd)){
            msgMapMutex_->lock();
            msgMap_->insert(senderAdd, QStringList());
            msgMapMutex_->unlock();
            emit msgMapChangeListener(senderAdd);
        }
        QJsonObject toSend;
        toSend.insert("areYouAlive", true);
        toSend.insert("isAlert", false);
        toSend.insert("message", "");
        toSend.insert("receiverAddress", jobj.value("senderAddress").toString());
        toSend.insert("senderAddress", myIp_.toString());

        QJsonDocument jdoc;
        jdoc.setObject(toSend);
        QByteArray byteToSend = jdoc.toJson();
        for (QTcpSocket* socket : sockets_) {
            if (socket == sender)
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
    QString  toAppend = nick+"\t"+jobj.value("message").toString();
    (*msgMap_)[senderIp].append(toAppend);
    msgMapMutex_->unlock();

    emit msgMapChangeListener(senderIp);
}

