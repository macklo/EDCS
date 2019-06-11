#include "mainwindow.h"
#include <QApplication>
#include <iostream>>

void test () {
    std::cout<<"dupa123"<<std::endl;
}

int main(int argc, char *argv[])
{
   /* QTcpServer* server_ = new QTcpServer();

    connect(server_, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
    bool res  = server_->listen(QHostAddress::Any, 11000);*/

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}


