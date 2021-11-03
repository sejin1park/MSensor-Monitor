#include "header/dialog2.h"
#include "ui_dialog2.h"

Dialog2::Dialog2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog2)
{

    ui->setupUi(this);
}

Dialog2::~Dialog2()
{
    delete ui;
}
void Dialog2::getConfigData(QString module_name,QString module_address,QString module_version,QString module_manufacture){
    ui->textEdit->setText("module name: "+module_name+"\nmodule UUID: "+module_address+"\nmodule version: "+module_version+"\nmade by "+module_manufacture+"\nAccelermeter: 16.0g\nGyroscope: 2000.0dps");
}
void Dialog2::Get_Calibration_Paraemter(int16_t gyro_x,int16_t gyro_y,int16_t gyro_z,int16_t magnet_x,int16_t magnet_y,int16_t magnet_z){
    ui->lineEdit->setText(QString::number(gyro_x));
    ui->lineEdit_2->setText(QString::number(gyro_y));
    ui->lineEdit_3->setText(QString::number(gyro_z));

    ui->lineEdit_4->setText(QString::number(magnet_x));
    ui->lineEdit_5->setText(QString::number(magnet_y));
    ui->lineEdit_6->setText(QString::number(magnet_z));
}

void Dialog2::on_gyro_x_fix_button_clicked()
{
    emit Change_Calibration_Parameter(ui->lineEdit->text().toInt(),0x07);
}

void Dialog2::on_gyro_y_fix_button_clicked()
{
    emit Change_Calibration_Parameter(ui->lineEdit_2->text().toInt(),0x08);
}

void Dialog2::on_gyro_z_fix_button_clicked()
{
    emit Change_Calibration_Parameter(ui->lineEdit_3->text().toInt(),0x09);
}

void Dialog2::on_magnet_x_fix_button_clicked()
{
    emit Change_Calibration_Parameter(ui->lineEdit_4->text().toInt(),0x0A);
}

void Dialog2::on_magnet_y_fix_button_clicked()
{
    emit Change_Calibration_Parameter(ui->lineEdit_5->text().toInt(),0x0B);
}

void Dialog2::on_magnet_z_fix_button_clicked()
{
    emit Change_Calibration_Parameter(ui->lineEdit_6->text().toInt(),0x0C);
}
