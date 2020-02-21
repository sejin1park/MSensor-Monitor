#include "header/mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    connect(this,SIGNAL(Button_Clicked_SIG(QString)),device_helper,SLOT(Button_Clicked_SLOT(QString)));
    connect(this,SIGNAL(Period_Change_Button_SIG(int)),device_helper,SLOT(Period_Change_Button_clicked_SLOT(int)));
    connect(device_helper,SIGNAL(Data_Receive0_SIG(QByteArray,double,double,double,double,double,double,double,double,double))
            ,this,SLOT(Data_Receive0_SLOT(QByteArray,double,double,double,double,double,double,double,double,double)));
    connect(device_helper,SIGNAL(Data_Receive1_SIG(QByteArray,float,float,float,float,float,float,float)),
            this,SLOT(Data_Receive1_SLOT(QByteArray,float,float,float,float,float,float,float)));
    connect(device_helper,SIGNAL(Data_Receive2_SIG(int16_t,int16_t,int16_t,int16_t ,int16_t ,int16_t )),
            this,SLOT(Data_Receive2_SLOT(int16_t,int16_t,int16_t,int16_t ,int16_t ,int16_t)));

    connect(device_helper,SIGNAL(Message_Send_SIG(QString, int)),this,SLOT(Message_Send_SLOT(QString, int)));
    connect(device_helper,SIGNAL(Message_Receive_SIG(QString, int)),this,SLOT(Message_Receive_SLOT(QString, int )));
    connect(this,SIGNAL(Config_SIG(QString *,QString *,QString *,QString *))
            ,device_helper,SLOT(Config_Clicked_SLOT(QString *,QString *,QString*,QString *)));


    ui->setupUi(this);
    Graph_Print();


}

MainWindow::~MainWindow()
{
    delete ui;
    delete device_helper;
}


void MainWindow::on_Connecting_Button_clicked()
{
    device_helper->startDeviceDiscovery();
    QString button_name = "Connecting_Button";
    emit Button_Clicked_SIG(button_name);

    my_Dialog = new Dialog(); // progress bar Dialog
    connect(my_Dialog,SIGNAL(Dialog_Close_SIG()),this,SLOT(Dialog_Close_SLOT()));
    connect(device_helper,SIGNAL(ProgressBar_update_SIG(QString)),my_Dialog,SLOT(ProgressBar_update_SLOT(QString)));

    my_Dialog->exec();
    delete my_Dialog;

}
void MainWindow::Dialog_Close_SLOT(){

    ui->Connecting_Button->setDisabled(1);
    ui->Period_Chagne_Button->setDisabled(1);
    ui->Data_Save_Start_Button->setDisabled(1);
    ui->Stop_Button->setDisabled(1);


    ui->actionConfig->setDisabled(0);
    ui->Raw_Data_Button->setDisabled(0);
    ui->AHRS_Button->setDisabled(0);
    ui->Gyro_Calibration_Button->setDisabled(0);
    ui->Magnet_Calibration_Button->setDisabled(0);
    ui->Disconnect_Button->setDisabled(0);

}
void MainWindow::on_Raw_Data_Button_clicked()
{

    ui->lineEdit_17->setText("10");

    QString button_name = "Raw_Data_Button";
    emit Button_Clicked_SIG(button_name);

    ui->Raw_Data_Button->setDisabled(1);
    ui->AHRS_Button->setDisabled(1);
    ui->Gyro_Calibration_Button->setDisabled(1);
    ui->Magnet_Calibration_Button->setDisabled(1);
    ui->Disconnect_Button->setDisabled(1);

    ui->Period_Chagne_Button->setDisabled(0);
    ui->Data_Save_Start_Button->setDisabled(0);
    ui->Stop_Button->setDisabled(0);
}
void MainWindow::on_AHRS_Button_clicked()
{
    ui->lineEdit_17->setText("10");


    QString button_name = "AHRS_Button";
    emit Button_Clicked_SIG(button_name);

    ui->Raw_Data_Button->setDisabled(1);
    ui->AHRS_Button->setDisabled(1);
    ui->Gyro_Calibration_Button->setDisabled(1);
    ui->Magnet_Calibration_Button->setDisabled(1);
    ui->Disconnect_Button->setDisabled(1);

    ui->Period_Chagne_Button->setDisabled(0);
    ui->Data_Save_Start_Button->setDisabled(0);
    ui->Stop_Button->setDisabled(0);

}


void MainWindow::on_Stop_Button_clicked() // button stop
{
    Count_Start = 0 ;

    QString button_name = "Stop_Button";
    emit Button_Clicked_SIG(button_name);

    ui->Stop_Button->setDisabled(1);
    ui->Data_Save_Start_Button->setDisabled(1);
    ui->Period_Chagne_Button->setDisabled(1);
    ui->Disconnect_Button->setDisabled(0);
    ui->Magnet_Calibration_Button->setDisabled(0);
    ui->Gyro_Calibration_Button->setDisabled(0);
    ui->Raw_Data_Button->setDisabled(0);
    ui->AHRS_Button->setDisabled(0);

}



void MainWindow::on_Magnet_Calibration_Button_clicked() // start magnetCalibration
{

    QString button_name = "Magnet_Calibration_Button";
    emit Button_Clicked_SIG(button_name);

    ui->Stop_Button->setDisabled(1);
    ui->Data_Save_Start_Button->setDisabled(1);
    ui->Period_Chagne_Button->setDisabled(1);
    ui->Disconnect_Button->setDisabled(1);
    ui->Magnet_Calibration_Button->setDisabled(1);
    ui->Gyro_Calibration_Button->setDisabled(1);
    ui->Raw_Data_Button->setDisabled(1);
    ui->AHRS_Button->setDisabled(1);
    ui->Connecting_Button->setDisabled(1);

}

void MainWindow::on_Data_Save_Start_Button_clicked() //Data Save
{
    if(save_flag==false)
    {
        file->setFileName(QApplication::applicationDirPath()+"/"+filename+".txt");

        ui->Stop_Button->setDisabled(1);
        ui->Data_Save_Start_Button->setDisabled(0);
        ui->Period_Chagne_Button->setDisabled(0);
        ui->Disconnect_Button->setDisabled(1);
        ui->Magnet_Calibration_Button->setDisabled(1);
        ui->Gyro_Calibration_Button->setDisabled(1);
        ui->Raw_Data_Button->setDisabled(1);
        ui->AHRS_Button->setDisabled(1);
        ui->Connecting_Button->setDisabled(1);

        while(1){
            if(file->exists()){
                data_log_counter ++;
                file->setFileName(QApplication::applicationDirPath()+"/"+filename+"_"+QString::number(data_log_counter)+".txt");
            }
            else{
                if(!file->open(QIODevice::WriteOnly)) qDebug()<<"File Write ERROR";
                break;
            }
        }
        save_flag = true;
        ui->Data_Save_Start_Button->setText(QString::fromLocal8Bit("Data Save Finish"));
        my_messagebox->Make_Message_Box("Save Start","Save Start");
    }

    else if(save_flag==true){
        QString Filename_Temp = filename;
        if(data_log_counter!=0) Filename_Temp.append(QString("_%1").arg(data_log_counter));

        ui->Data_Save_Start_Button->setText("Data Save Start");
        my_messagebox->Make_Message_Box(QString(" Save Finish\n File Name : %1").arg(Filename_Temp),"Save Finish");
        save_flag = false;
        file->close();

        ui->Stop_Button->setDisabled(0);
        ui->Data_Save_Start_Button->setDisabled(0);
        ui->Period_Chagne_Button->setDisabled(0);
        ui->Disconnect_Button->setDisabled(1);
        ui->Magnet_Calibration_Button->setDisabled(1);
        ui->Gyro_Calibration_Button->setDisabled(1);
        ui->Raw_Data_Button->setDisabled(1);
        ui->AHRS_Button->setDisabled(1);
        ui->Connecting_Button->setDisabled(1);
    }
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox qBox;
    qBox.setWindowTitle("Information");
    qBox.setText("<font size = 12>MSensor Monitor </font>");
    qBox.setInformativeText(QString::fromLocal8Bit(" ver : 1.0 (2020, 01,06)\n MSensor Product Test\n Copyright ⓒ 2020 ,Mtome \n"));

    qBox.exec();
}

void MainWindow::on_Gyro_Calibration_Button_clicked() // gyro_calibraiton start
{
    QString button_name = "Gyro_Calibration_Button";
    emit Button_Clicked_SIG(button_name);

    ui->Stop_Button->setDisabled(1);
    ui->Data_Save_Start_Button->setDisabled(1);
    ui->Period_Chagne_Button->setDisabled(1);
    ui->Disconnect_Button->setDisabled(1);
    ui->Magnet_Calibration_Button->setDisabled(1);
    ui->Gyro_Calibration_Button->setDisabled(1);
    ui->Raw_Data_Button->setDisabled(1);
    ui->AHRS_Button->setDisabled(1);
    ui->Connecting_Button->setDisabled(1);
}


void MainWindow::on_Period_Chagne_Button_clicked() //Period Change
{

    switch(device_helper->get_measure_method()){
    case 0:
        if(ui->lineEdit_17->text().toInt()<10 ||ui->lineEdit_17->text().toInt()>2500){
            my_messagebox->Make_Message_Box_Fail(QString::fromLocal8Bit("Wrong range\n가능범위 : 10ms~2500ms"));
            return;
        }
        break;
    case 1:
        if(ui->lineEdit_17->text().toInt()<10 ||ui->lineEdit_17->text().toInt()>1000){
            my_messagebox->Make_Message_Box_Fail(QString::fromLocal8Bit("Wrong range\n가능범위 : 10ms~1000ms"));
            return;
        }
    }
    int period = (ui->lineEdit_17->text().toInt())/10;
    emit Period_Change_Button_SIG(period);
}

void MainWindow::on_Disconnect_Button_clicked()
{

    QString button_name = "Disconnect_Button";

    emit Button_Clicked_SIG(button_name);
    my_messagebox->Make_Message_Box("BLE Disconnected","Done");

    ui->Stop_Button->setDisabled(1);
    ui->Data_Save_Start_Button->setDisabled(1);
    ui->Period_Chagne_Button->setDisabled(1);
    ui->Disconnect_Button->setDisabled(1);
    ui->Magnet_Calibration_Button->setDisabled(1);
    ui->Gyro_Calibration_Button->setDisabled(1);
    ui->Raw_Data_Button->setDisabled(1);
    ui->AHRS_Button->setDisabled(1);
    ui->Connecting_Button->setDisabled(0);
}

void MainWindow::Data_Receive0_SLOT(const QByteArray &value,double gyro_x,double gyro_y,double gyro_z,double acc_x,double acc_y,double acc_z,double magnet_x,double magnet_y,double magnet_z)
{
    QString Data;

    ui->lineEdit->setText(QString::number(acc_x,'g',3)+"g");
    ui->lineEdit_2->setText(QString::number(acc_y,'g',3)+"g");
    ui->lineEdit_3->setText(QString::number(acc_z,'g',3)+"g");

    ui->lineEdit_4->setText(QString::number(gyro_x,'g',3)+"deg/s");
    ui->lineEdit_5->setText(QString::number(gyro_y,'g',3)+"deg/s");
    ui->lineEdit_6->setText(QString::number(gyro_z,'g',3)+"deg/s");
    ui->lineEdit_7->setText(QString::number(magnet_x,'g',3)+"uT");
    ui->lineEdit_8->setText(QString::number(magnet_y,'g',3)+"uT");
    ui->lineEdit_9->setText(QString::number(magnet_z,'g',3)+"uT");

    Graph_Replot(count,acc_x,acc_y,acc_z,gyro_x,gyro_y,gyro_z,magnet_x,magnet_y,magnet_z);
    count ++;

    Data = QDateTime::currentDateTime().toString("[h:m:s]");
    Data.append(device_helper->Protocol_Helper->Interpret_Data(value,0));

    if(save_flag == true)file->write(Data.toUtf8()+"\n");

    if((ui->listWidget_2->count())>LIST_COUNT_LIMIT)
    {
        delete ui->listWidget_2->takeItem(ui->listWidget_2->row(ui->listWidget_2->item(0)));
        ui->listWidget_2->addItem(Data);

    }
    else ui->listWidget_2->addItem(Data);
    ui->listWidget_2->scrollToBottom();

}
void MainWindow::Data_Receive1_SLOT(const QByteArray &value,float q0,float q1, float q2,float q3,float roll,float pitch ,float yaw){

    QString Data;

    ui->lineEdit_10->setText(QString::number(q0,'g',2));
    ui->lineEdit_11->setText(QString::number(q1,'g',2));
    ui->lineEdit_12->setText(QString::number(q2,'g',2));
    ui->lineEdit_13->setText(QString::number(q3,'g',2));

    ui->lineEdit_14->setText(QString::number(roll));
    ui->lineEdit_15->setText(QString::number(pitch));
    ui->lineEdit_16->setText(QString::number(yaw));
    Data = QDateTime::currentDateTime().toString("[h:m:s]");
    Data.append(device_helper->Protocol_Helper->Interpret_Data(value,1));

    if(save_flag == true)file->write(Data.toUtf8()+"\n");

    // list widget limit setting
    if((ui->listWidget_2->count())>LIST_COUNT_LIMIT)
    {
        delete ui->listWidget_2->takeItem(ui->listWidget_2->row(ui->listWidget_2->item(0)));
        ui->listWidget_2->addItem(Data);
    }
    else    ui->listWidget_2->addItem(Data);


    ui->listWidget_2->scrollToBottom();

    // change 3D Cube
    QQuaternion a(q0,q1,q2,q3);
    ui->widget->rotation = a;
    ui->widget->update();

}
void MainWindow::Data_Receive2_SLOT(int16_t gyro_x,int16_t gyro_y,int16_t gyro_z,int16_t magnet_x,int16_t magnet_y,int16_t magnet_z)
{
    my_Dialog2->Get_Calibration_Paraemter(gyro_x,gyro_y,gyro_z,magnet_x,magnet_y,magnet_z);
}
void MainWindow::Message_Receive_SLOT(QString recive_data,int cmd){
    // if do Calibration
    if(cmd == 0x04 || cmd == 0x05)
    {
        ui->Disconnect_Button->setDisabled(0);
        ui->Magnet_Calibration_Button->setDisabled(0);
        ui->Gyro_Calibration_Button->setDisabled(0);
        ui->Raw_Data_Button->setDisabled(0);
        ui->AHRS_Button->setDisabled(0);
    }

    ui->listWidget->addItem(recive_data);
    ui->listWidget->scrollToBottom();

}
void MainWindow::Message_Send_SLOT(QString send_data,int cmd){

    ui->listWidget->addItem(send_data);
    ui->listWidget->scrollToBottom();
}
void MainWindow::Graph_Print(){

    ui->widget_2->plotLayout()->insertRow(0);
    ui->widget_2->plotLayout()->addElement(0, 0, new QCPTextElement(ui->widget_2, "Acceleration", QFont("sans", 12, QFont::Bold)));
    ui->widget_2->addGraph();
    ui->widget_2->graph(0)->setPen(QPen(Qt::red));
    ui->widget_2->addGraph();
    ui->widget_2->graph(1)->setPen(QPen(Qt::green));
    ui->widget_2->addGraph();
    ui->widget_2->graph(2)->setPen(QPen(Qt::blue));
    ui->widget_2->yAxis->setRange(-17,17);

    ui->widget_3->plotLayout()->insertRow(0);
    ui->widget_3->plotLayout()->addElement(0, 0, new QCPTextElement(ui->widget_3, "Gyro", QFont("sans", 12, QFont::Bold)));
    ui->widget_3->addGraph();
    ui->widget_3->graph(0)->setPen(QPen(Qt::red));
    ui->widget_3->addGraph();
    ui->widget_3->graph(1)->setPen(QPen(Qt::green));
    ui->widget_3->addGraph();
    ui->widget_3->graph(2)->setPen(QPen(Qt::blue));
    ui->widget_3->yAxis->setRange(-300,300);

    ui->widget_4->plotLayout()->insertRow(0);
    ui->widget_4->plotLayout()->addElement(0, 0, new QCPTextElement(ui->widget_4, "Magnet", QFont("sans", 12, QFont::Bold)));

    ui->widget_4->addGraph();
    ui->widget_4->graph(0)->setPen(QPen(Qt::red));
    ui->widget_4->addGraph();
    ui->widget_4->graph(1)->setPen(QPen(Qt::green));
    ui->widget_4->addGraph();
    ui->widget_4->graph(2)->setPen(QPen(Qt::blue));
    ui->widget_4->yAxis->setRange(-2,2);

    QPalette textPalette;
    textPalette.setBrush( QPalette::WindowText, QBrush( QColor(255, 0, 0 )));
    ui->label_21->setPalette( textPalette );
    textPalette.setBrush( QPalette::WindowText, QBrush( QColor(0, 150, 0 )));
    ui->label_22->setPalette( textPalette );
    textPalette.setBrush( QPalette::WindowText, QBrush( QColor(0, 0, 255 )));
    ui->label_23->setPalette( textPalette );


}
void MainWindow::Graph_Replot(int count,double acc_x,double acc_y,double acc_z,double gyro_x,double gyro_y,double gyro_z,double magnet_x,double magnet_y,double magnet_z){

    ui->widget_2->graph(0)->addData(count,acc_x);
    ui->widget_2->graph(1)->addData(count,acc_y);
    ui->widget_2->graph(2)->addData(count,acc_z);

    ui->widget_2->xAxis->setRange(count,300,Qt::AlignRight);
    ui->widget_2->replot();

    ui->widget_3->graph(0)->addData(count,gyro_x);
    ui->widget_3->graph(1)->addData(count,gyro_y);
    ui->widget_3->graph(2)->addData(count,gyro_z);

    ui->widget_3->xAxis->setRange(count,300,Qt::AlignRight);
    ui->widget_3->replot();

    ui->widget_4->graph(0)->addData(count,magnet_x);
    ui->widget_4->graph(1)->addData(count,magnet_y);
    ui->widget_4->graph(2)->addData(count,magnet_z);

    ui->widget_4->xAxis->setRange(count,300,Qt::AlignRight);
    ui->widget_4->replot();

}

void MainWindow::on_actionConfig_triggered()
{
    QString button_name = "Config_Button";
    QString module_name;
    QString module_address;
    QString module_version;
    QString module_manufacture;
    emit Config_SIG(&module_name,&module_address,&module_version,&module_manufacture);
    qDebug()<<module_name;

    my_Dialog2 = new Dialog2(); // progress bar Dialog
    connect(my_Dialog2,SIGNAL(Change_Calibration_Parameter(int16_t ,int8_t )),this,SLOT(set_Calibration_Paraeter(int16_t,int8_t)));
    my_Dialog2->getConfigData(module_name,module_address,module_version,module_manufacture);
    my_Dialog2->exec();
    delete my_Dialog2;
}
void MainWindow::set_Calibration_Paraeter(int16_t data ,int8_t cmd)
{
   QByteArray temp;
    temp.append(((int8_t)(data>>8)));
    temp.append(((int8_t)data));
    temp.append(cmd);

    qDebug()<<temp;

    device_helper->Protocol_Helper->Send_Protocol(temp);

}
