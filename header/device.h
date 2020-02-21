/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef DEVICE_H
#define DEVICE_H

#include <qbluetoothlocaldevice.h>
#include <QObject>
#include <QVariant>
#include <QList>
#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>
#include <QBluetoothServiceInfo>
#include<QThread>
#include <QDebug>
#include <QtEndian>
#include <QMetaEnum>
#include <QDateTime>
#include <QTimer>
#include <QMessageBox>
#include <QQuaternion>




#include "header/my_messagebox.h"
#include "header/deviceinfo.h"
#include "header/serviceinfo.h"
#include "header/characteristicinfo.h"
#include "header/Data_Convert.h"

#include "header/Protocol.h"

QT_FORWARD_DECLARE_CLASS (QBluetoothDeviceInfo)
QT_FORWARD_DECLARE_CLASS (QBluetoothServiceInfo)

/* ERROR CODE */
#define SUCCESS 0
#define BLE_OFF 1 // OR NOT Paring
#define DEVICE_POWER_OFF 2
#define WRONG_DEVICE 3
#define MULTIPLE_DEVICE_CONNECT 4
/* ERROR CODE FIN */

#define DISCOVEY_TIMEOUT 3000

class Device: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant devicesList READ getDevices NOTIFY devicesUpdated)
    Q_PROPERTY(QVariant servicesList READ getServices NOTIFY servicesUpdated)
    Q_PROPERTY(QVariant characteristicList READ getCharacteristics NOTIFY characteristicsUpdated)
    Q_PROPERTY(bool state READ state NOTIFY stateChanged)
    Q_PROPERTY(bool controllerError READ hasControllerError)
public:
    Device();
    ~Device();
    QVariant getDevices();
    QVariant getServices();
    QVariant getCharacteristics();
    QString getUpdate();
    Protocol *Protocol_Helper = new Protocol();
    Data_Convert *Data_Converter_Helper;
    int get_measure_method();
    bool state();
    bool hasControllerError() const;

    bool isRandomAddress() const;
    void setRandomAddress(bool newValue);
signals:
    void Data_Receive0_SIG(const QByteArray &value,double gyro_x,double gyro_y,double gyro_z,double acc_x,double acc_y,double acc_z,double magnet_x,double magnet_y,double magnet_z);//type 1
    void Data_Receive1_SIG(const QByteArray &value,float q0,float q1,float q2,float q3,float roll,float pitch,float yaw ); // type 2
    void Data_Receive2_SIG(int16_t gyro_x,int16_t gyro_y,int16_t gyro_z,int16_t magnet_x,int16_t magnet_y,int16_t magnet_z); // get Calibration Parameter
    void Message_Receive_SIG(QString recive_data, int cmd);
    void Message_Send_SIG(QString send_data,int cmd);

public slots:
    void startDeviceDiscovery();
    void scanServices(const QString &address);

    void connectToService(const QString &uuid);
    void disconnectFromDevice();
    void Button_Clicked_SLOT(QString button_name);
    void Period_Change_Button_clicked_SLOT(int period);
    void Config_Clicked_SLOT(QString *module_name,QString *module_address,QString *module_version,QString *module_manufacture);


private slots:
    // QBluetoothDeviceDiscoveryAgent related
    void addDevice(const QBluetoothDeviceInfo&);
    void deviceScanFinished();
    void deviceScanError(QBluetoothDeviceDiscoveryAgent::Error);

    // QLowEnergyController realted
    void addLowEnergyService(const QBluetoothUuid &uuid);
    void deviceConnected();
    void errorReceived(QLowEnergyController::Error);
    void serviceScanDone();
    void deviceDisconnected();

    // QLowEnergyService related
    void serviceDetailsDiscovered(QLowEnergyService::ServiceState newState);
    void ReceiveValue(const QLowEnergyCharacteristic &c,
                                  const QByteArray &value);
    void Written_Check(const QLowEnergyCharacteristic &c,
                                  const QByteArray &value);
    void DataReceiveValue(const QLowEnergyCharacteristic &c,
                         const QByteArray &value);


Q_SIGNALS:
    void devicesUpdated();
    void servicesUpdated();
    void characteristicsUpdated();
    void updateChanged();
    void stateChanged();
    void disconnected();
    void randomAddressChanged();

signals:
   void ProgressBar_update_SIG(QString Text);

private:
    void setDebug(const QString &message);
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QLowEnergyCharacteristic Terminal_character;
    QLowEnergyCharacteristic Recive_character;
    QLowEnergyCharacteristic DataPeriod_character;
    QLowEnergyService *Terminal_Service;
    QLowEnergyService *Data_Service;
    QLowEnergyService *Device_info_Service;
    QLowEnergyCharacteristic DataRecive_character;
    QLowEnergyDescriptor m_notificationDesc;
    DeviceInfo currentDevice;
    QList<QObject *> devices;
    QList<QObject *> m_services;
    QList<QObject *> m_characteristics;
    QString m_previousAddress;
    QString m_message;
    QString device_name;
    QString module_number;
    QString firmware_revision;
    QString manufacturer_name;

    QLowEnergyController *controller = nullptr;
    bool connected = false;
    bool calibration_parameter_get = false;
    int error_code= SUCCESS;
    int device_count = 0;
    bool m_deviceScanState = false;
    bool randomAddress = false;
    int measure_method = 0;
};

#endif // DEVICE_H
