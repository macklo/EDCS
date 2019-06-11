#ifndef IPANDPORT_H
#define IPANDPORT_H

#include <QWidget>
#include <QListWidgetItem>
#include <QJsonArray>
#include <QJsonObject>

namespace Ui {
class IpAndPort;
}

class IpAndPort : public QWidget
{
    Q_OBJECT

public:
    QStringList labels;
    explicit IpAndPort(QWidget *parent = 0);
    IpAndPort(const QJsonArray contacts,  QWidget *parent = 0);
    ~IpAndPort();

private:
    Ui::IpAndPort *ui;

public slots:
    void itemChosen(QListWidgetItem *item);

signals:
    int userChosen(int idx);

private slots:

};

#endif // IPANDPORT_H
