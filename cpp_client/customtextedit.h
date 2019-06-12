#ifndef CUSTOMTEXTEDIT_H
#define CUSTOMTEXTEDIT_H

#include <QTextEdit>
#include <QKeyEvent>
#include <QObject>

class CustomTextEdit : public QTextEdit
{
    Q_OBJECT


public:
    CustomTextEdit(QWidget *parent = 0);
    ~CustomTextEdit();

signals:
    void enterPressed();

protected:
    void keyPressEvent(QKeyEvent *e) override;

private slots:

private:

private:
};

#endif // CUSTOMTEXTEDIT_H
