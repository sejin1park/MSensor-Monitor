#ifndef M_connect_H
#define M_connect_H

#include <QMainWindow>
#include <qbluetoothlocaldevice.h>
#include <QObject>
#include <QVariant>
#include <QList>
#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>
#include <QBluetoothServiceInfo>
#include "deviceinfo.h"
#include "serviceinfo.h"
#include "characteristicinfo.h"
#include <QListWidgetItem>
#include <QTimer>
#include <QDialog>

namespace Ui {
class M_connect;
}

class M_connect : public QDialog
{
    Q_OBJECT


public slots:
     void ProgressBar_update_SLOT(QString Text);

public:
    explicit M_connect(QWidget *parent = nullptr);
    ~M_connect();
     void startDeviceDiscovery();
private slots:
    void on_pushButton_clicked();
    void deviceScanFinished();
    void addDevice(const QBluetoothDeviceInfo&);
    void doSomething(QListWidgetItem *item);
    void on_Done_Button_clicked();
    void myfunction();
Q_SIGNALS:
    void devicesUpdated();
    void stateChanged();
    void send_temp(QString temp,QList<QObject *> List);
    void Dialog_Close_SIG();
private:
    Ui::M_connect *ui;
    int persent = 0;

    QTimer *m_timer;
    bool success_flag = false;
    DeviceInfo currentDevice;
     bool device_san_fin = false;
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QList<QObject *> devices;
};

#endif // M_connect_H

