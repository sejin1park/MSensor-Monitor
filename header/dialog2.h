#ifndef DIALOG2_H
#define DIALOG2_H

#include <QDialog>
#include "header/device.h"

namespace Ui {
class Dialog2;
}

class Dialog2 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog2(QWidget *parent = nullptr);
    void getConfigData(QString module_name,QString module_address,QString module_version,QString module_manufacture);
    void Get_Calibration_Paraemter(int16_t gyro_x,int16_t gyro_y,int16_t gyro_z,int16_t magnet_x,int16_t magnet_y,int16_t magnet_z);
    ~Dialog2();
signals:
    void Change_Calibration_Parameter(int16_t Parameter,int8_t cmd);
private slots:
    void on_gyro_x_fix_button_clicked();
    
    void on_gyro_y_fix_button_clicked();
    
    void on_gyro_z_fix_button_clicked();
    
    void on_magnet_x_fix_button_clicked();
    
    void on_magnet_y_fix_button_clicked();
    
    void on_magnet_z_fix_button_clicked();
    
private:
    Ui::Dialog2 *ui;

};

#endif // DIALOG2_H
