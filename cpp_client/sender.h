#ifndef SENDER_H
#define SENDER_H

#include <QJsonObject>
#include <QJsonDocument>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMutex>
#include <QThread>


class Sender: public QObject
{Q_OBJECT
public:
    Sender(QHostAddress myIp, QMap<QString, QStringList> *msgMap, QMutex *msgMapMutex);
    ~Sender();
    bool sendMsgTo(QString msg, QHostAddress ipAdd, quint16 port, bool isAlert = false);
    bool checkIfAlive(QHostAddress ipAdd, quint16 port);

signals:
    void msgMapChangeSignal(QString ip);

private:
    QWidget *parent_;
    QTcpSocket socket_;
    QHostAddress myIp_;
    QMutex socketMutex_;
    QMap<QString, QStringList> *msgMap_;
    QMutex *msgMapMutex_;
    bool connectTo(QHostAddress ipAdd, quint16 port);
    bool disconectFrom();
};

#endif // SENDER_H
