#ifndef IPANDPORT_H
#define IPANDPORT_H

#include <QWidget>
#include <QVariantList>
#include <QJsonArray>
#include <QJsonObject>

namespace Ui {
class IpAndPort;
}

class IpAndPort : public QWidget
{
    Q_OBJECT

public:
    explicit IpAndPort(QWidget *parent = 0);
    IpAndPort(const QJsonArray contacts,  QWidget *parent = 0);
    ~IpAndPort();

private:
    Ui::IpAndPort *ui;

private slots:

};

#endif // IPANDPORT_H
