#include "sender.h"

Sender::Sender(QHostAddress myIp,QMap<QString, QStringList> *msgMap, QMutex *msgMapMutex):
   myIp_(myIp),msgMap_(msgMap), msgMapMutex_(msgMapMutex)
{

}
Sender::~Sender(){
    parent_=nullptr;
}

bool Sender::connectTo(QHostAddress ipAdd, quint16 port){ 
    socket_.connectToHost(ipAdd, port);
    return socket_.waitForConnected();
}

bool Sender::disconectFrom(){
    if(socket_.state()!=QTcpSocket::ConnectedState){
        return false;
    }
    socket_.disconnectFromHost();
    bool res = socket_.waitForDisconnected();
    return res;
}

bool Sender::sendMsgTo(QString msg, QHostAddress ipAdd, quint16 port, bool isAlert){
    if(socket_.state()!=QTcpSocket::UnconnectedState){
        return false;
    }
    socketMutex_.lock();
    if(!connectTo(ipAdd,  port)){
        socketMutex_.unlock();
        return false;
    }
    QJsonObject toSend;
    toSend.insert("areYouAlive", false);
    toSend.insert("isAlert", isAlert);
    toSend.insert("message", msg);
    toSend.insert("receiverAddress", ipAdd.toString());
    toSend.insert("senderAddress", myIp_.toString());

    QByteArray byteToSend = QJsonDocument(toSend).toBinaryData();
    byteToSend.append(QByteArray::fromStdString(QString("<EOF>").toStdString()));

    socket_.write(byteToSend);
    socket_.waitForBytesWritten();

    QString nick("<me>");
    if(isAlert){
        nick.prepend("<A!>");
    }

    msgMapMutex_->lock();
    (*msgMap_)[ipAdd.toString()].append(nick+msg);
    msgMapMutex_->unlock();

    emit msgMapChangeSignal(ipAdd.toString());

    while(!disconectFrom()){}
    socketMutex_.unlock();
    return true;
}


bool Sender::checkIfAlive(QHostAddress ipAdd, quint16 port){
    socketMutex_.lock();
    if(!connectTo(ipAdd,  port)){
        disconectFrom();
        socketMutex_.unlock();
        return false;
    }
    QJsonObject toSend;
    toSend.insert("areYouAlive", true);
    toSend.insert("isAlert", false);
    toSend.insert("message", "");
    toSend.insert("receiverAddress", ipAdd.toString());
    toSend.insert("senderAddress", myIp_.toString());

    QByteArray byteToSend = QJsonDocument(toSend).toBinaryData();
    byteToSend.append(QByteArray::fromStdString(QString("<EOF>").toStdString()));

    socket_.write(byteToSend);
    socket_.waitForBytesWritten();

    if(!socket_.waitForReadyRead(1000)){
        disconectFrom();
        socketMutex_.unlock();
        return false;
    }
    QByteArray data = socket_.readAll();
    std::string data_string = data.toStdString();
    data = QByteArray::fromStdString(data_string.substr(0, data_string.length()-5));
    QJsonDocument json_doc = QJsonDocument::fromJson(data);
    QJsonObject recieved = json_doc.object();

    while(!disconectFrom()){}
    socketMutex_.unlock();
    return recieved.value("areYouAlive").toBool();
}


