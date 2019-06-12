#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ipandport.h"
#include "waitforconnnection.h"
#include "listener.h"
#include "sender.h"
#include "customtextedit.h"

#include <QMainWindow>
#include <QString>
#include <QJsonArray>
#include <QHostAddress>
#include <QMutex>
#include <QFuture>
#include <QStringList>
#include <QListWidgetItem>
#include <QListWidget>


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
    void changeConversation(QListWidgetItem* item);
    void checkConnection(int idx);

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
    CustomTextEdit *msgBox;


    QString currentIp;

    Listener *listener;
    Sender *sender;

    enum  state{PRE_CONNCTED};

    bool readContactFile(QString filename);
    void checkConnection(QString ip);
    void addUserToListWidget(QString ip);
    QString getNickOfIp(QString ip);
    QString getIpOfNick(QString nick);
};

#endif // MAINWINDOW_H
