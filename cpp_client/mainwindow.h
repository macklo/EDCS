#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ipandport.h"
#include "waitforconnnection.h"
#include "listener.h"
#include "sender.h"

#include <QMainWindow>
#include <QString>
#include <QJsonArray>
#include <QHostAddress>
#include <QMutex>
#include <QFuture>
#include <QStringList>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


public slots:
    void connectionPanelPopUp();
    bool connectToUser(int idx);
    void checkConnectionsSlot();
    void sendMessage();
    void updateMsgWindow(QString ip);
    void onNewConnection();

private:
    //QTcpServer *server_;
    Ui::MainWindow *ui;
    IpAndPort *ipWindow;
    QJsonArray contacts;
    QHostAddress myIp;
    QMap<QString, QStringList> msgMap_;
    QMutex msgMapMutex_;
    quint16 port;
    //QFuture<bool> future;
    QStringList connectedUsers;
    QMap<QString, QString> conversingWith;

    QString currentIp;

    Listener *listener;
    Sender *sender;

    enum  state{PRE_CONNCTED};

    bool readContactFile(QString filename);
    void checkConnections();
    void addUserToListWidget(QString ip);

};

#endif // MAINWINDOW_H
