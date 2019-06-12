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
    return socket_.waitForConnected(1000);
}

bool Sender::disconectFrom(){
    if(socket_.state()!=QTcpSocket::ConnectedState){
        return false;
    }
    bool result = true;
    socket_.disconnectFromHost();
    if(socket_.state()==QTcpSocket::ConnectedState){
        result = socket_.waitForDisconnected();
    }
    return result;
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

    QJsonDocument jdoc;
    jdoc.setObject(toSend);
    QByteArray byteToSend = jdoc.toJson();

    socket_.write(byteToSend);
    socket_.waitForBytesWritten();

    QString nick("<me>");
    if(isAlert){
        nick.prepend("<A!>");
    }

    msgMapMutex_->lock();
    (*msgMap_)[ipAdd.toString()].append(nick+"\t"+msg);
    msgMapMutex_->unlock();

    emit msgMapChangeSignal(ipAdd.toString());

    disconectFrom();
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

    QJsonDocument jdoc;
    jdoc.setObject(toSend);
    QByteArray byteToSend = jdoc.toJson();

    socket_.write(byteToSend);
    socket_.waitForBytesWritten(10000);
    bool res = socket_.waitForReadyRead(10000);
    if(!res){
        disconectFrom();
        socketMutex_.unlock();
        return false;
    }
    QByteArray data = socket_.readAll();
    QJsonDocument json_doc = QJsonDocument::fromJson(data);
    QJsonObject recieved = json_doc.object();

    disconectFrom();
    socketMutex_.unlock();
    return recieved.value("areYouAlive").toBool();
}


