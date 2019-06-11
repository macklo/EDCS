#include "waitforconnnection.h"
#include "ui_waitforconnnection.h"

WaitForConnnection::WaitForConnnection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WaitForConnnection)
{
    setWindowFlags( Qt::CustomizeWindowHint );
    ui->setupUi(this);

}

WaitForConnnection::~WaitForConnnection()
{
    delete ui;
}

void WaitForConnnection::showConnectionFailMsg(){
    ui->label->setText("[ERR]Cannot connet to this user!!!");
}
