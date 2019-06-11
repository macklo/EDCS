#ifndef WAITFORCONNNECTION_H
#define WAITFORCONNNECTION_H

#include <QWidget>

namespace Ui {
class WaitForConnnection;
}

class WaitForConnnection : public QWidget
{
    Q_OBJECT

public:
    explicit WaitForConnnection(QWidget *parent = 0);
    ~WaitForConnnection();
    void showConnectionFailMsg();

private:
    Ui::WaitForConnnection *ui;
};

#endif // WAITFORCONNNECTION_H
