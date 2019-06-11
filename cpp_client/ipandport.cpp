#include "ipandport.h"
#include "ui_ipandport.h"

#include <iostream>
#include <QJsonValue>
#include <QVariantMap>

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
   std::for_each(contacts.begin(), contacts.end(), [&](const auto &v){
       labels.append(v.toObject().value("name").toString());
   });
   ui->setupUi(this);
   ui->listWidget->addItems(labels);
   connect(ui->pushButton, SIGNAL (released()), this, SLOT (close()));
   connect(ui->listWidget, SIGNAL (itemDoubleClicked(QListWidgetItem*)), this,
           SLOT (itemChosen(QListWidgetItem *)));
}

void IpAndPort::itemChosen(QListWidgetItem *item){
    int idx;
    for(int i = 0; i<labels.size();++i){
        if(labels[i]==item->text())
            idx = i;
    }
    emit userChosen(idx);
    close();
}

