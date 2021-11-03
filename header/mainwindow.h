#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "header/device.h"
#include "header/dialog.h"
#include "dialog2.h"
#include "m_connect.h"

#define LIST_COUNT_LIMIT 15 // List_Widget2 count limit
#define FILENAME "Data_Log" // Save_file name

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void Button_Clicked_SIG(QString button_name);
    void Period_Change_Button_SIG(int period);
    void Config_SIG(QString *module_name,QString *module_address,QString *module_version,QString *module_manufacture);
private slots:
    void Data_Receive0_SLOT(const QByteArray &value,double gyro_x,double gyro_y,double gyro_z,double acc_x,double acc_y,double acc_z,double magnet_x,double magnet_y,double magnet_z);
    void Data_Receive1_SLOT(const QByteArray &value,float q0,float q1, float q2,float q3,float roll,float pitch ,float yaw);
    void Data_Receive2_SLOT(int16_t gyro_x,int16_t gyro_y,int16_t gyro_z,int16_t magnet_x,int16_t magnet_y,int16_t magnet_z);
    void Message_Receive_SLOT(QString recive_data, int cmd);
    void Message_Send_SLOT(QString send_data,int cmd);
    void Dialog_Close_SLOT();
    void receive_temp(QString temp,QList<QObject *> List);
    void on_Connecting_Button_clicked();

    void on_Raw_Data_Button_clicked();

    void on_Stop_Button_clicked();

    void on_AHRS_Button_clicked();

    void on_Magnet_Calibration_Button_clicked();

    void on_Data_Save_Start_Button_clicked();

    void on_actionAbout_triggered();

    void on_Gyro_Calibration_Button_clicked();

    void on_Period_Chagne_Button_clicked();

    void on_Disconnect_Button_clicked();

    void on_actionConfig_triggered();

    void set_Calibration_Paraeter(int16_t data,int8_t cmd);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void Graph_Print();
    void Graph_Replot(int count,double acc_x,double acc_y,double acc_z,double gyro_x,double gyro_y,double gyro_z,double magnet_x,double magnet_y,double magnet_z);
     //Push Button Click
    Device *device_helper = new Device();
    My_messagebox *my_messagebox;
    M_connect *my_Dialog ;
    Dialog2 *my_Dialog2 ;
    QFile *file = new QFile;
    int data_log_counter = 0;
    int count=0;
    QString filename = FILENAME;

    bool Graph_Draw = true;
    bool save_flag = false;
    bool Count_Start  = false;
    Ui::MainWindow *ui;


};
#endif // MAINWINDOW_H
