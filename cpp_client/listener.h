#ifndef LISTENER_H
#define LISTENER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QHostAddress>
#include <QStringList>
#include <QMutex>
#include <QJsonArray>
#include <QJsonObject>
#include <QThread>


class Listener: public QObject
{
    Q_OBJECT

public:
    explicit Listener(QMap<QString, QStringList> *msgMap, QMutex *msgMapMutex,
             QHostAddress myIp, QJsonArray contacts);
    ~Listener();
    void run();// override;

signals:
    void msgMapChangeListener(QString ip);

private:
    static const uint socket_no = 11000;

    QWidget *parent_;
    QTcpServer *server_;
    QList<QTcpSocket*>  sockets_;
    QMap<QString, QStringList> *msgMap_;
    QMutex *msgMapMutex_;
    QHostAddress myIp_;
    QJsonArray contacts_;
    static const QString globalIp;


    void pushMsg(const QJsonObject &jobj);

public slots:
    void onNewConnection();
    void onSocketStateChanged(QAbstractSocket::SocketState socketState);
    void onReadyRead();
};

#endif // LISTENER_H
