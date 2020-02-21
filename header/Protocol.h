#ifndef PROTOCOL_H
#define PROTOCOL_H

#endif // PROTOCOL_H
#include <QList>
#include <QObject>
#include <QtBluetooth/QLowEnergyService>

#define RECIVE 0
#define SEND 1


class Protocol : public QObject{

Q_SIGNALS:
    void characteristicsUpdated();

public:
    void Setting(QLowEnergyService * Termianl_Service,QLowEnergyCharacteristic Terminal_character);
    void Send_Protocol(QByteArray s);
    void Change_Period(QByteArray s,QLowEnergyService* Data_Service,QLowEnergyCharacteristic Data_Period);
    QString Interpret_Protocol(const QByteArray &value,bool type);
    QString Interpret_Data(const QByteArray &value,bool measure_method);
private:
     QLowEnergyService* m_service;
     QLowEnergyCharacteristic m_link;

};

