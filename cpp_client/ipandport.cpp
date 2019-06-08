#include "ipandport.h"
#include "ui_ipandport.h"

#include <iostream>
#include <QJsonValue>

IpAndPort::IpAndPort(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IpAndPort)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL (released()), this, SLOT (close()));
}

IpAndPort::~IpAndPort()
{
    delete ui;
}

IpAndPort::IpAndPort(const QJsonArray contacts,  QWidget *parent):
    QWidget(parent),
    ui(new Ui::IpAndPort)
{
    for(int i=0; i<contacts.size();++i){
        QJsonObject v = contacts[i].toObject();
        bool b = v.contains("name");
        const QJsonValue v1 = v.value(QString("name"));
        //std::cout<<v.toStdString()<<std::endl;
                //[0].toObject()["name"];
        //auto k = v.isObject();

    }
   std::for_each(contacts.begin(), contacts.end(), [&](const auto &v){
       auto a = v.toObject().value("name").toString();
       ui->listWidget->addItem(a);
   });

}
