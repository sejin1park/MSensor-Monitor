#ifndef MY_MESSAGEBOX_H
#define MY_MESSAGEBOX_H
#include  <QMessageBox>

class My_messagebox:public QObject{

public:
    void Make_Message_Box_Fail(QString Message);
    void Make_Message_Box(QString Message,QString Title);
};

#endif // MY_MESSAGEBOX_H
