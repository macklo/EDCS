#include "customtextedit.h"


#include <QCompleter>
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QtDebug>
#include <QApplication>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QScrollBar>

CustomTextEdit::CustomTextEdit(QWidget *parent)
: QTextEdit(parent)
{

}

CustomTextEdit::~CustomTextEdit()
{
}



void CustomTextEdit::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Return){
        emit enterPressed();
    }else{
        QTextEdit::keyPressEvent(e);
    }

}

