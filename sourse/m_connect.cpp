#include "m_connect.h"
#include "ui_m_connect.h"
#include <qbluetoothaddress.h>
#include <qbluetoothdevicediscoveryagent.h>
#include <qbluetoothlocaldevice.h>
#include <qbluetoothdeviceinfo.h>
#include <qbluetoothservicediscoveryagent.h>
#include <QDebug>
#include <QList>
#include <QtEndian>
#include <QMetaEnum>
#include <QTimer>
#include <QThread>
#include <QMessageBox>

#include <QProcess>
M_connect::M_connect(QWidget *parent) :
     QDialog(parent),
    ui(new Ui::M_connect)
{
     ui->setupUi(this);
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent();
    discoveryAgent->setLowEnergyDiscoveryTimeout(1500);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &M_connect::addDevice);
    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(doSomething(QListWidgetItem*)));
     connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &M_connect::deviceScanFinished);


}
/*********************************************************************
 * @fn      doSomething
 *
 * @brief  list widget item double clicked , get UUID and starting discovered.
 *
 * @param  item - sensor UUID
 *
 * @return   none
 */

void M_connect::doSomething(QListWidgetItem *item)
{
    if(device_san_fin==true){
    QString temp =  item->text();
    QStringList temp_l = temp.split("\n");
    temp = temp_l[1];
    persent  = 0 ;
    emit send_temp(temp,devices);
    //qDebug()<<temp;
    //qDebug()<<temp;

    }
    else {
        QMessageBox TEMP_Box;
        //QSpacerItem* horizontalSpacer = new QSpacerItem(200,200,QSizePolicy::Minimum,QSizePolicy::Expanding);
        //QGridLayout* layout =(QGridLayout*)TEMP_Box.layout();
        //layout->addItem(horizontalSpacer,layout->rowCount(),0,1,layout->columnCount());

        TEMP_Box.setText("Wait for the search to complete.");
        TEMP_Box.setWindowTitle("Warring");
        TEMP_Box.exec();
    }
}
/*********************************************************************
 * @fn      deviceScanFinished
 *
 * @brief  if device Scan Finished , change status.
 *
 * @param   none
 *
 * @return   none
 */

void M_connect::deviceScanFinished()
{
    emit devicesUpdated();
    emit stateChanged();
    device_san_fin=true;

  //  if (devices.isEmpty()) ui->pushButton->setText("fail");
    //setUpdate("No Low Energy devices found...");
   //else ui->pushButton->setText("success");
       // setUpdate("Done! Scan Again!");
}


M_connect::~M_connect()
{
    persent = 0;
    if(success_flag==true) emit Dialog_Close_SIG();
    delete ui;
    delete m_timer;
}
/*********************************************************************
 * @fn      ProgressBar_update_SLOT
 *
 * @brief  ProgressBar persent update and text change.
 *
 * @param  Text - Label Text
 *
 * @return   none
 */

void M_connect::ProgressBar_update_SLOT(QString Text){
    if(Text.contains("Fail",Qt::CaseInsensitive)){
          ui->label->setText(Text);
          persent=0;
          ui->Done_Button->setDisabled(0);
    }
    else{
        persent+=20;
        ui->progressBar->setValue(persent);
        ui->label->setText(Text);
        if(persent>=120){
            success_flag = true;
            m_timer = new QTimer(this);

            connect(m_timer,SIGNAL(timeout()),this,SLOT(myfunction()));
            m_timer->start(2000);
            ui->label->setText("Wait a second...");

            ui->pushButton->setDisabled(1);

            //ui->Done_Button->setDisabled(0);
        }
    }
}
/*********************************************************************
 * @fn      myfunction
 *
 * @brief  search button disable and change label text
 *
 * @param    none
 *
 * @return   none
 */

void M_connect::myfunction(){
     ui->Done_Button->setDisabled(0);
     ui->label->setText("done");
}
/*********************************************************************
 * @fn      startDeviceDiscovery
 *
 * @brief  starting discovryAgent.
 *
 * @param  none
 *
 * @return   none
 */

void M_connect::startDeviceDiscovery()
{

    qDeleteAll(devices);
    devices.clear();
    emit devicesUpdated();

   // setUpdate("Scanning for devices ...");
    //! [les-devicediscovery-2]
    discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
    //! [les-devicediscovery-2]

    if (discoveryAgent->isActive()) {
        //m_deviceScanState = true;

        Q_EMIT stateChanged();
    }

}

//! [les-devicediscovery-3]

/*********************************************************************
* @fn      addDevice
*
* @brief  find MSensor and add to listWidget
*
* @param  info - ble Device Infomation
*
* @return   none
*/

void M_connect::addDevice(const QBluetoothDeviceInfo &info)
{

    QString Itme;

    if (info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration) {
        auto d = new DeviceInfo(info);
        devices.append(d);
        if(d->getName()=="MSensor") {
            Itme = d->getName()+"\n"+d->getAddress();
            ui->listWidget->addItem(Itme);
            }

    }

}
/*********************************************************************
 * @fn      on_pushButton_clicked
 *
 * @brief   find ble start.
 *
 * @param  none
 *
 * @return   none
 */

void M_connect::on_pushButton_clicked()
{
   ui->listWidget->clear();
   M_connect::startDeviceDiscovery();
}
/*********************************************************************
 * @fn      on_Done_Button_clicked
 *
 * @brief  close dialog
 *
 * @param   none
 *
 *
 * @return   none
 */


void M_connect::on_Done_Button_clicked()
{

    this->close();
}
