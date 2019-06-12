#include "waitforconnnection.h"
#include "ui_waitforconnnection.h"

#include <QTimer>

WaitForConnnection::WaitForConnnection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WaitForConnnection)
{
    ui->setupUi(this);
    QTimer *timer = new QTimer();
    connect(timer, SIGNAL(timeout()),this, SLOT(close()));

       // msec
       timer->start(1000);
        delete timer;
}

WaitForConnnection::~WaitForConnnection()
{

    delete ui;
}

void WaitForConnnection::showConnectionFailMsg(){
    ui->label->setText("This user is not connected");
}

void WaitForConnnection::showConnectionOkMsg(){
    ui->label->setText("This user is online");
}
