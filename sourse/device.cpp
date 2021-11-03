#include "header/device.h"

#include <qbluetoothaddress.h>
#include <qbluetoothdevicediscoveryagent.h>
#include <qbluetoothlocaldevice.h>
#include <qbluetoothdeviceinfo.h>
#include <qbluetoothservicediscoveryagent.h>
#include <QDebug>
#include <QList>

#include <QMetaEnum>
#include <QTimer>

/*********************************************************************
 * @fn      Device
 *
 * @brief  Object initialize
 *
 * @param none
 *
 * @return  none
 */

Device::Device()
{

    discoveryAgent = new QBluetoothDeviceDiscoveryAgent();
    discoveryAgent->setLowEnergyDiscoveryTimeout(DISCOVEY_TIMEOUT);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &Device::addDevice);
    connect(discoveryAgent, QOverload<QBluetoothDeviceDiscoveryAgent::Error>::of(&QBluetoothDeviceDiscoveryAgent::error),
            this, &Device::deviceScanError);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &Device::deviceScanFinished);
}

/*********************************************************************
 * @fn      Device
 *
 * @brief  Object destroy
 *
 * @param none
 *
 * @return  none
 */
Device::~Device()
{
    controller->disconnectFromDevice();
    delete discoveryAgent;
    delete controller;

    qDeleteAll(devices);
    qDeleteAll(m_services);
    qDeleteAll(m_characteristics);
    devices.clear();
    m_services.clear();
    m_characteristics.clear();
}

/*********************************************************************
 * @fn      startDeviceDiscovery
 *
 * @brief  Scanning Devices
 *
 * @param none
 *
 * @return  none
 */

void Device::startDeviceDiscovery()
{
    qDeleteAll(devices);
    devices.clear();
    emit devicesUpdated();

    //setDebug("Scanning for devices ...");
    //emit ProgressBar_update_SIG("Scanning for devices ...");
    //discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
    //error_code = BLE_OFF;

    if (discoveryAgent->isActive()) {

        m_deviceScanState = true;
        Q_EMIT stateChanged();
    }

}
void Device::get_devices( QList<QObject *> list)
{
    devices = list;
}
/*********************************************************************
 * @fn      addDevice
 *
 * @brief   register Device if name is MSensor.
 *
 * @param info - Information of the device found
 *
 * @return  none
 */

void Device::addDevice(const QBluetoothDeviceInfo &info)
{
    QMessageBox msgBox;
    if (info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration)
    {
        auto d = new DeviceInfo(info);
        devices.append(d);
        device_count++;
        //Device name check
        if(d->getName().contains("MSensor",Qt::CaseInsensitive))
        {
           // Device::scanServices(d->getAddress());
        }
        else
        {
            //error_code = WRONG_DEVICE;
        }
       // if(device_count>=2) error_code = MULTIPLE_DEVICE_CONNECT;
    }
}

/*********************************************************************
 * @fn      deviceScanFinished
 *
 * @brief   if finished signal emit termianl Service,character registration,
 *            and check error during connecting.
 *
 * @param none
 *
 * @return  none
 */

void Device::deviceScanFinished()
{
    Protocol_Helper->Setting(Terminal_Service,Terminal_character);
    emit devicesUpdated();
    m_deviceScanState = false;
    emit stateChanged();
    //error Check
    switch(error_code){
    case SUCCESS:
        emit ProgressBar_update_SIG("Finish");
        break;
    case BLE_OFF:
       // emit ProgressBar_update_SIG(QString("Connecting Fail(Code : %1)").arg(BLE_OFF));
        break;
    case DEVICE_POWER_OFF:
        emit ProgressBar_update_SIG(QString("Connecting Fail(Code : %1)").arg(DEVICE_POWER_OFF));
        break;
    case  WRONG_DEVICE:
        emit ProgressBar_update_SIG(QString("Connecting Fail(Code : %1)").arg(WRONG_DEVICE));
        break;
    case MULTIPLE_DEVICE_CONNECT:
        emit ProgressBar_update_SIG(QString("Connecting Fail(Code : %1)").arg(MULTIPLE_DEVICE_CONNECT));
        break;
    }
    qDebug()<<"aaaaaa";
    error_code =SUCCESS;
    device_count = 0;
}
/*********************************************************************
 * @fn      getDevices
 *
 * @brief   getting Devices List.
 *
 * @param none
 *
 * @return  devices List
 */

QVariant Device::getDevices()
{
    return QVariant::fromValue(devices);
}

/*********************************************************************
 * @fn      getServices
 *
 * @brief  getting Services List.
 *
 * @param none
 *
 * @return  services List
 */

QVariant Device::getServices()
{

    return QVariant::fromValue(m_services);

}
/*********************************************************************
 * @fn      getCharacteristics
 *
 * @brief  getting Characteristic List.
 *
 * @param none
 *
 * @return  Characteristic List
 */
QVariant Device::getCharacteristics()
{
    return QVariant::fromValue(m_characteristics);
}
/*********************************************************************
 * @fn      scanServices
 *
 * @brief   Search for services of found devices and try to connect.
 *
 * @param address - Device Uuid
 *
 * @return  Characteristic List
 */

void Device::scanServices(const QString &address)
{
    // We need the current device for service discovery.
      discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);

    for (auto d: qAsConst(devices)) {
        auto device = qobject_cast<DeviceInfo *>(d);
        if (!device)
            continue;

        if (device->getAddress() == address )
            currentDevice.setDevice(device->getDevice());
    }

    if (!currentDevice.getDevice().isValid()) {
        qWarning() << "Not a valid device";
        return;
    }

    qDeleteAll(m_characteristics);
    m_characteristics.clear();
    emit characteristicsUpdated();
    qDeleteAll(m_services);
    m_services.clear();
    emit servicesUpdated();

    setDebug("Connecting to device...");
    emit ProgressBar_update_SIG("Connecting to device");
    error_code = DEVICE_POWER_OFF;

    if (controller && m_previousAddress != currentDevice.getAddress()) {
        controller->disconnectFromDevice();
        delete controller;
        controller = nullptr;

    }

    //! [les-controller-1]
    if (!controller) {
        // Connecting signals and slots for connecting to LE services.
        controller = QLowEnergyController::createCentral(currentDevice.getDevice());
        connect(controller, &QLowEnergyController::connected,
                this, &Device::deviceConnected);
        connect(controller, QOverload<QLowEnergyController::Error>::of(&QLowEnergyController::error),
                this, &Device::errorReceived);
        connect(controller, &QLowEnergyController::disconnected,
                this, &Device::deviceDisconnected);
        connect(controller, &QLowEnergyController::serviceDiscovered,
                this, &Device::addLowEnergyService);
        connect(controller, &QLowEnergyController::discoveryFinished,
                this, &Device::serviceScanDone);
    }
    controller->connectToDevice();
    //! [les-controller-1]

    m_previousAddress = currentDevice.getAddress();
}

/*********************************************************************
 * @fn      addLowEnergyService
 *
 * @brief  add service to service List
 *
 * @param serviceUuid - service Uuid
 *
 * @return  none
 */

void Device::addLowEnergyService(const QBluetoothUuid &serviceUuid)
{
    //! [les-service-1]
    //!
    qDebug()<<"6";
    QLowEnergyService *service = controller->createServiceObject(serviceUuid);

    if (!service) {
        qWarning() << "Cannot create service for uuid";
        return;
    }
    //! [les-service-1]
    auto serv = new ServiceInfo(service);
    m_services.append(serv);

      qDebug()<<serv->getName();

    emit servicesUpdated();
}
//! [les-service-1]
/*********************************************************************
 * @fn      serviceScanDone
 *
 * @brief   if serviceScand finished, update services.
 *
 * @param none
 *
 * @return  none
 */

void Device::serviceScanDone()
{
    if (m_services.isEmpty())emit servicesUpdated();

    Device::connectToService("f000aa70-0451-4000-b000-000000000000"); // terminal service
    Device::connectToService("f000aa80-0451-4000-b000-000000000000"); // data service
    Device::connectToService("0x180a"); //Device info service
}

/*********************************************************************
 * @fn      connectToService
 *
 * @brief   find Terminal Service and Data Service and find Characteristic
 *
 * @param Service uuid
 *
 * @return  none
 */

void Device::connectToService(const QString &uuid)
{
    qDebug()<<"Scdda";
    QLowEnergyService *service = nullptr;
    for (auto s: qAsConst(m_services)) {
        auto serviceInfo = qobject_cast<ServiceInfo *>(s);
        if (!serviceInfo)
            continue;
        qDebug()<<serviceInfo->getUuid();
        qDebug()<<uuid;
        if (serviceInfo->getUuid() == uuid) {
            service = serviceInfo->service();
            if(uuid.contains("f000aa70",Qt::CaseInsensitive)){
                Terminal_Service = serviceInfo->service();
            }
            else if(uuid.contains("f000aa80",Qt::CaseInsensitive)){
                Data_Service = serviceInfo->service();
                error_code =SUCCESS;
            }
            else if(uuid.contains("0x180a",Qt::CaseInsensitive)){
                QLowEnergyCharacteristic temp;
                Device_info_Service = serviceInfo->service();
            }
            break;
        }
    }

    if (!service)
        return;

    qDeleteAll(m_characteristics);
    m_characteristics.clear();
    emit characteristicsUpdated();
    qDebug()<<service->state();
    if (service->state() == QLowEnergyService::DiscoveryRequired) {
        //! [les-service-3]
        connect(service, &QLowEnergyService::stateChanged,
                this, &Device::serviceDetailsDiscovered);
        service->discoverDetails();
        setDebug("Discovering details...");
        emit ProgressBar_update_SIG("Discovering details...");
        //! [les-service-3]
        return;
    }


}
/*********************************************************************
 * @fn      deviceConnected
 *
 * @brief   if device Connected find Service and Characteristic.
 *
 * @param none
 *
 * @return  none
 */
void Device::deviceConnected()
{
    setDebug("Discovering services...");
    emit ProgressBar_update_SIG("Discovering services...");
    connected = true;
    //! [les-service-2]
    controller->discoverServices();
    //! [les-service-2]
    qDebug()<<"Bbbb";

}

/*********************************************************************
 * @fn      errorReceived
 *
 * @brief   If an error occurs while connecting, update the progress bar.
 *
 * @param Error_name
 *
 * @return  none
 */
void Device::errorReceived(QLowEnergyController::Error /*error*/)
{
    error_code = DEVICE_POWER_OFF;
    emit ProgressBar_update_SIG(QString("Connecting Fail(Code : %1)").arg(DEVICE_POWER_OFF));
}

/*********************************************************************
 * @fn      disconnectFromDevice
 *
 * @brief   when disconnect() signal calling, disconnect device.
 *
 * @param none
 *
 * @return  none
 */


void Device::disconnectFromDevice()
{


    if (controller->state() != QLowEnergyController::UnconnectedState)
        controller->disconnectFromDevice();
    else
        deviceDisconnected();
}

/*********************************************************************
 * @fn      deviceDisconnected
 *
 * @brief  disconnected() signal make.
 *
 * @param none
 *
 * @return  none
 */
void Device::deviceDisconnected()
{
    qWarning() << "Disconnect from device";
    emit disconnected();
}

/*********************************************************************
 * @fn      serviceDetailsDiscovered
 *
 * @brief   Scanning Characteristics, find terminal character, data character and setting noti.
 *
 * @param newState -Service Status.
 *
 * @return  none
 */
void Device::serviceDetailsDiscovered(QLowEnergyService::ServiceState newState)
{

    qDebug()<<"10";
    qDebug()<<newState;

    if (newState != QLowEnergyService::ServiceDiscovered) {

        // do not hang in "Scanning for characteristics" mode forever
        // in case the service discovery failed
        // We have to queue the signal up to give UI time to even enter
        // the above mode

        if (newState != QLowEnergyService::DiscoveringServices) {
            QMetaObject::invokeMethod(this, "characteristicsUpdated",
                                      Qt::QueuedConnection);
        }
        return;
    }

    auto service = qobject_cast<QLowEnergyService *>(sender());
    if (!service)
        return;


    //! [les-chars]
    const QList<QLowEnergyCharacteristic> chars = service->characteristics();

    for (const QLowEnergyCharacteristic &ch : chars) {
        auto cInfo = new CharacteristicInfo(ch);
        m_characteristics.append(cInfo);
    }
    //! [les-chars]

    for (const QLowEnergyCharacteristic &ch : chars) {

        auto cInfo = new CharacteristicInfo(ch);

        if(cInfo->getUuid().contains("f000aa71",Qt::CaseInsensitive)){

            Recive_character = cInfo->getCharacteristic();
            m_notificationDesc = Recive_character.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);
            if(m_notificationDesc.isValid()){
                service->writeDescriptor(m_notificationDesc,QByteArray::fromHex("0100")); // notification Activation
            }
            connect(Terminal_Service, SIGNAL(characteristicChanged(QLowEnergyCharacteristic,QByteArray)),
                    this, SLOT(ReceiveValue(QLowEnergyCharacteristic,QByteArray)));
            connect(Terminal_Service, SIGNAL(characteristicWritten(QLowEnergyCharacteristic,QByteArray)),
                    this, SLOT(Written_Check(QLowEnergyCharacteristic,QByteArray)));

        }
        else if(cInfo->getUuid().contains("f000aa72",Qt::CaseInsensitive)){

            Terminal_character = cInfo->getCharacteristic();
        }
        else if(cInfo->getUuid().contains("f000aa81",Qt::CaseInsensitive)){

            DataRecive_character = cInfo->getCharacteristic();
            m_notificationDesc = DataRecive_character.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);
            if(m_notificationDesc.isValid()){
                service->writeDescriptor(m_notificationDesc,QByteArray::fromHex("0100")); // notification Activation

            }
            connect(Data_Service, SIGNAL(characteristicChanged(QLowEnergyCharacteristic,QByteArray)),
                    this, SLOT(DataReceiveValue(QLowEnergyCharacteristic,QByteArray)));
            connect(Data_Service, SIGNAL(characteristicWritten(QLowEnergyCharacteristic,QByteArray)),
                    this, SLOT(Written_Check(QLowEnergyCharacteristic,QByteArray)));

        }
        else if(cInfo->getUuid().contains("f000aa83",Qt::CaseInsensitive)){
            DataPeriod_character = cInfo->getCharacteristic();

        }
        else if(cInfo->getUuid().contains("0x2a26",Qt::CaseInsensitive)){
            module_number = currentDevice.getAddress();
            device_name = currentDevice.getName();
            firmware_revision = cInfo->getValue();
            //module_number
        }
        else if(cInfo->getUuid().contains("0x2a29",Qt::CaseInsensitive)){
            manufacturer_name = cInfo->getValue();
            //module_number
        }



        //test

    }
    emit characteristicsUpdated();
}

/*********************************************************************
 * @fn      deviceScanError
 *
 * @brief   if finding error, show reason.
 *
 * @param error - finding error.
 *
 * @return  none
 */

void Device::deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    if (error == QBluetoothDeviceDiscoveryAgent::PoweredOffError)
        setDebug("The Bluetooth adaptor is powered off, power it on before doing discovery.");
    else if (error == QBluetoothDeviceDiscoveryAgent::InputOutputError)
        setDebug("Writing or reading from the device resulted in an error.");
    else {
        static QMetaEnum qme = discoveryAgent->metaObject()->enumerator(
                    discoveryAgent->metaObject()->indexOfEnumerator("Error"));
        setDebug("Error: " + QLatin1String(qme.valueToKey(error)));
    }

    m_deviceScanState = false;
    emit devicesUpdated();
    emit stateChanged();
}
/*********************************************************************
 * @fn      state
 *
 * @brief   device status check.
 *
 * @param none
 *
 * @return  true or false
 */

bool Device::state()
{
    return m_deviceScanState;
}

/*********************************************************************
 * @fn      hasControllerError
 *
 * @brief   BLE Controller error check.
 *
 * @param none
 *
 * @return  true or false
 */
bool Device::hasControllerError() const
{
    return (controller && controller->error() != QLowEnergyController::NoError);
}
/*********************************************************************
 * @fn      setDebug
 *
 * @brief   When debugging, it shows a message.
 *
 * @param message - Message to be displayed on console.
 *
 * @return  none
 */
void Device::setDebug(const QString &message)
{
    qDebug()<<message;
}

/*********************************************************************
 * @fn      ReceiveValue
 *
 * @brief   When the module passes a protocol, it checks the value, and
 *             Pass the value to the mainWindow.
 *
 *
 * @param c - Characteristic from which the message originated
 *             value - data from characteristic
 *
 * @return  none
 */

void Device::ReceiveValue(const QLowEnergyCharacteristic &c,
                          const QByteArray &value){

    QLowEnergyService* service;
    const quint8  *data = reinterpret_cast<const quint8 *>(value.constData());
    int8_t cmd = data[2];

    const QLowEnergyCharacteristic link = Terminal_character;
    const QLowEnergyCharacteristic link2 = Recive_character;
    service = Terminal_Service;

    service->readCharacteristic(link2);
    QString recive_data = QDateTime::currentDateTime().toString("[h:m:s]");
    recive_data.append(Protocol_Helper->Interpret_Protocol(value,RECIVE));
    emit Message_Receive_SIG(recive_data,cmd);

}
/*********************************************************************
 * @fn      DataReceiveValue
 *
 * @brief   When the module passes a data, it checks the value, and
 *             Pass the value to the mainWindow.
 *
 *
 * @param c - Characteristic from which the message originated
 *             value - data from characteristic
 *
 * @return  none
 */

void Device::DataReceiveValue(const QLowEnergyCharacteristic &c,
                              const QByteArray &value)
{
    QLowEnergyService* service;
    const QLowEnergyCharacteristic link3= DataRecive_character;
    service = Terminal_Service;
    service->readCharacteristic(link3);

    const quint8  *data = reinterpret_cast<const quint8 *>(value.constData());

    if(calibration_parameter_get==true){
        int16_t gyro_x =  static_cast<int16_t>((data[0]<<8)|(data[1]));
        int16_t gyro_y =  static_cast<int16_t>((data[2]<<8)|(data[3]));
        int16_t gyro_z =  static_cast<int16_t>((data[4]<<8)|(data[5]));
        int16_t magnet_x =  static_cast<int16_t>((data[6]<<8)|(data[7]));
        int16_t magnet_y =  static_cast<int16_t>((data[8]<<8)|(data[9]));
        int16_t magnet_z =  static_cast<int16_t>((data[10]<<8)|(data[11]));

        emit Data_Receive2_SIG(gyro_x,gyro_y,gyro_z,magnet_x,magnet_y,magnet_z);
    }

    else if(measure_method==0)
    {

        int16_t gyro_x =  static_cast<int16_t>((data[0]<<8)|(data[1]));
        int16_t gyro_y =  static_cast<int16_t>((data[2]<<8)|(data[3]));
        int16_t gyro_z =  static_cast<int16_t>((data[4]<<8)|(data[5]));
        int16_t acc_x =  static_cast<int16_t>((data[6]<<8)|(data[7]));
        int16_t acc_y =  static_cast<int16_t>((data[8]<<8)|(data[9]));
        int16_t acc_z =  static_cast<int16_t>((data[10]<<8)|(data[11]));
        int16_t magnet_x =  static_cast<int16_t>((data[12]<<8)|(data[13]));
        int16_t magnet_y =  static_cast<int16_t>((data[14]<<8)|(data[15]));
        int16_t magnet_z = static_cast<int16_t>((data[16]<<8)|(data[17]));

        double F_gyro_x = Data_Converter_Helper->Gyro_Convert(static_cast<double>(gyro_x));
        double F_gyro_y = Data_Converter_Helper->Gyro_Convert(static_cast<double>(gyro_y));
        double F_gyro_z = Data_Converter_Helper->Gyro_Convert(static_cast<double>(gyro_z));

        double F_acc_x = Data_Converter_Helper->Accel_Convert(static_cast<double>(acc_x));
        double F_acc_y = Data_Converter_Helper->Accel_Convert(static_cast<double>(acc_y));
        double F_acc_z = Data_Converter_Helper->Accel_Convert(static_cast<double>(acc_z));

        double F_magnet_x = Data_Converter_Helper->Magnet_Convert(static_cast<double>(magnet_x));
        double F_magnet_y = Data_Converter_Helper->Magnet_Convert(static_cast<double>(magnet_y));
        double F_magnet_z = Data_Converter_Helper->Magnet_Convert(static_cast<double>(magnet_z));
        emit Data_Receive0_SIG(value,F_gyro_x,F_gyro_y,F_gyro_z,F_acc_x,F_acc_y,F_acc_z,F_magnet_x,F_magnet_y,F_magnet_z);
    }

    else if(measure_method==1)
    {
        int16_t q1_T =  static_cast<int16_t>((data[0]<<8)|(data[1]));
        int16_t q2_T =  static_cast<int16_t>((data[2]<<8)|(data[3]));
        int16_t q3_T =  static_cast<int16_t>((data[4]<<8)|(data[5]));
        int16_t q0_T =  static_cast<int16_t>((data[6]<<8)|(data[7]));

        float q0 = (static_cast<float>(q0_T)/10000);
        float q1 = (static_cast<float>(q1_T)/10000);
        float q2 =(static_cast<float>(q2_T)/10000);
        float q3 = (static_cast<float>(q3_T)/10000);

        //  Quaternion to EulerAngles
        QQuaternion a(q0,q1,q2,q3);
        QVector3D b = a.toEulerAngles();
        emit Data_Receive1_SIG(value,q0,q1,q2,q3,b.x(),b.y(),b.z());
    }
}

/*********************************************************************
 * @fn      Written_Check
 *
 * @brief   When a program sends a value to a module, it checks the value, and
 *             Pass the value to the mainWindow.
 *
 * @param c - Characteristic from which the message originated
 *             value - data from characteristic
 *
 * @return  none
 */
void Device::Written_Check(const QLowEnergyCharacteristic &c,
                           const QByteArray &value){

    QString send_data = QDateTime::currentDateTime().toString("[h:m:s]");
    const quint8  *data = reinterpret_cast<const quint8 *>(value.constData());
    int8_t cmd = data[2];
    send_data.append(Protocol_Helper->Interpret_Protocol(value,SEND));
    emit Message_Send_SIG(send_data,cmd);
}
/*********************************************************************
 * @fn      get_measure_method
 *
 * @brief   return Current measurement method
 *
 * @param none
 *
 * @return  measure method
 */

int Device::get_measure_method()
{
    return measure_method;
}
/*********************************************************************
 * @fn      Period_Change_Button_clicked_SLOT
 *
 * @brief   user clicked period change button, change period.
 *
 * @param period - user input
 *
 * @return  none
 */
void Device::Period_Change_Button_clicked_SLOT(int period){
    const char* temp ;

    temp = Data_Converter_Helper->int_to_char(period);
    Protocol_Helper->Change_Period(QByteArray::fromHex(temp),Data_Service,DataPeriod_character);
    emit characteristicsUpdated();
}
void Device::Config_Clicked_SLOT(QString *module_name,QString *module_address,QString *module_version,QString *module_manufacture){
    *module_name = device_name;
    *module_address = module_number;
    *module_version = firmware_revision;
    *module_manufacture = manufacturer_name;

    calibration_parameter_get = true;
    Protocol_Helper->Send_Protocol(QByteArray::fromHex("000006"));

}
/*********************************************************************
 * @fn      Button_Clicked_SLOT
 *
 * @brief When the user presses the button, the command is sent to the device.
 *
 * @param button_name - user cliked button name
 *
 * @return  none
 */

void Device::Button_Clicked_SLOT(const QString button_name){
    if(button_name.contains("Gyro_Calibration",Qt::CaseInsensitive)) Protocol_Helper->Send_Protocol(QByteArray::fromHex("000004"));
    else if(button_name.contains("Magnet_Calibration",Qt::CaseInsensitive)) Protocol_Helper->Send_Protocol(QByteArray::fromHex("000005"));
    else if(button_name.contains("Disconnect",Qt::CaseInsensitive)){
        if(m_notificationDesc.isValid()&&Data_Service){
            Data_Service->writeDescriptor(m_notificationDesc,QByteArray::fromHex("0000")); // notification disActivation

        }
        if(m_notificationDesc.isValid()&&Terminal_Service){
            Terminal_Service->writeDescriptor(m_notificationDesc,QByteArray::fromHex("0000")); // notification disActivation

        }
        controller->disconnectFromDevice();

        delete controller;
        controller = nullptr;
    }
    else if(button_name.contains("Stop",Qt::CaseInsensitive)) Protocol_Helper->Send_Protocol(QByteArray::fromHex("000003"));
    else if(button_name.contains("AHRS",Qt::CaseInsensitive)){
        Protocol_Helper->Send_Protocol(QByteArray::fromHex("000201"));
        QThread::msleep(50);
        measure_method = 1;
        Protocol_Helper->Send_Protocol(QByteArray::fromHex("000002"));
    }
    else if(button_name.contains("Raw_Data",Qt::CaseInsensitive)){
        Protocol_Helper->Send_Protocol(QByteArray::fromHex("000101"));
        QThread::msleep(50);
        measure_method = 0;
        Protocol_Helper->Send_Protocol(QByteArray::fromHex("000002"));
    }
    else if(button_name.contains("Connecting",Qt::CaseInsensitive)){

    }
    else setDebug("Error");
    emit characteristicsUpdated();
}
