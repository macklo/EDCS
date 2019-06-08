#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ipandport.h"

#include <QMainWindow>
#include <QString>
#include <QJsonArray>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void connectionPanelPopUp();

private:
    Ui::MainWindow *ui;
    IpAndPort *ipWindow;
    QJsonArray contacts;

    enum  state{PRE_CONNCTED};

    bool readContactFile(QString filename);


};

#endif // MAINWINDOW_H
