#include "sender.h"

const QString Sender::globalIp =  "192.168.2.189";

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
    socket_.disconnectFromHost();
    return socket_.waitForDisconnected();
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
    toSend.insert("senderAddress", globalIp);

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
    toSend.insert("senderAddress", globalIp);

    QJsonDocument jdoc;
    jdoc.setObject(toSend);
    QByteArray byteToSend = jdoc.toJson();
    byteToSend.append(QByteArray::fromStdString(QString("<EOF>").toStdString()));

    socket_.write(byteToSend);
    socket_.waitForBytesWritten(10000);
    bool res = socket_.waitForReadyRead(10000);
    if(!res){
        disconectFrom();
        socketMutex_.unlock();
        return false;
    }
    QByteArray data = socket_.readAll();
    //std::string data_string = data.toStdString();
    //data_string = data_string.substr(0, data_string.length()-4);
    //data = QString::fromStdString(data_string).toLocal8Bit();
    QJsonDocument json_doc = QJsonDocument::fromJson(data);
    QJsonObject recieved = json_doc.object();

    disconectFrom();
    socketMutex_.unlock();
    return recieved.value("areYouAlive").toBool();
}


