#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTextStream>
#include <QJsonDocument>
#include <QJsonArray>

#include <algorithm>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton_4, SIGNAL (released()), this, SLOT (connectionPanelPopUp()));

    readContactFile("contacts.txt");

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::connectionPanelPopUp(){
    IpAndPort *ipWindow = new IpAndPort(contacts);
    ipWindow -> show();
}

bool MainWindow::readContactFile(QString filename){
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    QString json_string;
    json_string = in.readAll();
    file.close();
    QByteArray json_bytes = json_string.toLocal8Bit();

    QJsonDocument json_doc = QJsonDocument::fromJson(json_bytes);

    contacts = json_doc.array();

    return true;
}
