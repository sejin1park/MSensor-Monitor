#include "header/dialog.h"
#include "ui_dialog.h"
#include <qthread.h>

/*********************************************************************
 * @fn      Dialog
 *
 * @brief   seutupUi
 *
 * @param parent - Widget to inherit
 *
 * @return   none
 */

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{

    ui->setupUi(this);

}

/*********************************************************************
 * @fn      ~Dialog
 *
 * @brief    Destory Object.
 *
 * @param  none
 *
 * @return   none
 */
Dialog::~Dialog()
{

    persent = 0;
    if(success_flag==true) emit Dialog_Close_SIG();
    delete ui;
}
/*********************************************************************
 * @fn      ProgressBar_update_SLOT
 *
 * @brief    ProgressBar_update signal occurs, change progressbar and change text.
 *
 * @param  text -String to change
 *
 * @return   none
 */
void Dialog::ProgressBar_update_SLOT(QString Text){
    if(Text.contains("Fail",Qt::CaseInsensitive)){
          ui->label->setText(Text);
          ui->Done_Button->setDisabled(0);
    }
    else{
        persent+=20;
        ui->progressBar->setValue(persent);
        ui->label->setText(Text);
        if(persent==120){
            success_flag = true;
            QThread::msleep(1000);
            ui->Done_Button->setDisabled(0);
        }
    }
}
/*********************************************************************
 * @fn      on_Done_Button_clicked
 *
 * @brief    closed dialog
 *
 * @param  none
 *
 * @return   none
 */
void Dialog::on_Done_Button_clicked()
{
    this->close();
}
