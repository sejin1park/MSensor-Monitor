#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog {
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

signals :
    void Dialog_Close_SIG();
public slots:
     void ProgressBar_update_SLOT(QString Text);
private slots:
    void on_Done_Button_clicked();

private:
    int persent = 0;
    bool success_flag = false;
    Ui::Dialog *ui;
};

#endif // DIALOG_H
