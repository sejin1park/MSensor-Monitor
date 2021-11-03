#include"header/Protocol.h"
#include"header/Data_Convert.h"
#include<QDebug>
#include <QQuaternion>

QString Error_Interpret(int8_t error_code);

/*********************************************************************
 * @fn      Setting
 *
 * @brief  Set up terminal services and properties for sending data to the module.
 *
 * @param  Termianl_Service - Service for sending data to the module
 *              Terminal_characteristic - characteristic for sending data to the module
 * @return   none
 */

void Protocol::Setting(QLowEnergyService * Termianl_Service,QLowEnergyCharacteristic Terminal_character){
    m_service = Termianl_Service;
    m_link = Terminal_character;
}
/*********************************************************************
 * @fn      Send_Protocol
 *
 * @brief  sending protocol to module
 *
 * @param  s - protocol data
 *
 * @return   none
 */

void Protocol::Send_Protocol(QByteArray s){

    m_service->writeCharacteristic(m_link,s,QLowEnergyService::WriteWithResponse);

}
/*********************************************************************
 * @fn      Change_Period
 *
 * @brief  changing data period
 *
 * @param  s - period
 *              Data_Service -Service that outputs data from module
 *              Data_Period  -period characteristics of the data
 * @return   none
 */

void Protocol::Change_Period(QByteArray s,QLowEnergyService* Data_Service,QLowEnergyCharacteristic Data_Period){
    static QLowEnergyService* service;

    const QLowEnergyCharacteristic link = Data_Period;
    service = Data_Service;
    service->writeCharacteristic(link,s,QLowEnergyService::WriteWithResponse);
}
/* type 0 : recive
 * type 1 : send
 */

/*********************************************************************
 * @fn     Interpret_Protocol
 *
 * @brief  Analyze the protocol content.
 *
 * @param  value - Protocol value.
 *              type - I/O checking. Input(module to pc) = 0 , output(pc to module) = 1
 * @return   none
 */

QString Protocol::Interpret_Protocol(const QByteArray &value,bool type){
    const quint8  *data = reinterpret_cast<const quint8 *>(value.constData());

    int8_t data1 = data[0];
    int8_t data2 =  data[1];
    int8_t cmd =  data[2];
    QString result;

    switch(type){
    case RECIVE:
        result = "Recive> ";
         if(data2 == 0x06) {
            if(cmd==0x05) result.append( "Magnet Calibration Finish");
            else result.append(QString(" CMD : %1, success").arg(cmd));
        }
        else if(data2 == 0x15) {
             QString reason = Error_Interpret(data1);
             result.append("Fail , reason: "+reason);
         }

        break;
    case SEND:
        result = "Send> ";
        switch(cmd){
        case 0x01:
            result.append(QString("CMD : %1, Change Measure type to %2").arg(cmd).arg(data2));
            break;
        case 0x02:
            result.append(QString("CMD : %1, Measure Start").arg(cmd));
            break;
        case 0x03:
            result.append(QString("CMD : %1, Measure Stop").arg(cmd));
            break;
        case 0x04:
            result.append(QString("CMD : %1, Gyro Calibration Star").arg(cmd));
            break;
        case 0x05:
            result.append(QString("CMD : %1, Magnet Calibration Start").arg(cmd));
            break;
       case 0x06:
             result.append(QString("CMD : %1, Send gyro,magnet bias Data").arg(cmd));
            break;
        case 0x07:
            result.append(QString("CMD : %1, Set gyro_x bias").arg(cmd));
            break;
        case 0x08:
            result.append(QString("CMD : %1, Set gyro_y bias").arg(cmd));
            break;
        case 0x09:
            result.append(QString("CMD : %1, Set gyro_z bias").arg(cmd));
            break;
        case 0x0A:
            result.append(QString("CMD : %1, Set Magnet_x bias").arg(cmd));
            break;
        case 0x0B:
            result.append(QString("CMD : %1, Set Magnet_y bias").arg(cmd));
            break;
        case 0x0C:
            result.append(QString("CMD : %1, Set Magnet_z bias").arg(cmd));
            break;


            break;
        }
        break;
    }
    return result ;
}

/*********************************************************************
 * @fn     Interpret_Data
 *
 * @brief  Analyze the data content.
 *
 * @param  value - Data value.
 *              measure_method - module's measure method( 0 - raw data, 1 - quaternion)
 * @return   none
 */

QString Protocol::Interpret_Data(const QByteArray &value,bool measure_method){
    const quint8  *data = reinterpret_cast<const quint8 *>(value.constData());
    Data_Convert *Data_Converter_Helper = new Data_Convert();
    QString result ;
    if(measure_method==0){

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

        result = QString("Gyro x,y,z : %1, %2, %3 Accel x,y,z  %4 %5 %6 magnet x,y,z %7 %8 %9").arg(F_gyro_x).arg(F_gyro_y).arg(F_gyro_z).arg(F_acc_x).arg(F_acc_y).arg(F_acc_z).arg(F_magnet_x).arg(F_magnet_y).arg(F_magnet_z);


    }
    else if(measure_method==1)
    {
        int16_t q1 =  static_cast<int16_t>((data[0]<<8)|(data[1]));
        int16_t q2 =  static_cast<int16_t>((data[2]<<8)|(data[3]));
        int16_t q3 =  static_cast<int16_t>((data[4]<<8)|(data[5]));
        int16_t q0 =  static_cast<int16_t>((data[6]<<8)|(data[7]));


        double q0_D = (static_cast<double>(q0)/10000);
        double q1_D = (static_cast<double>(q1)/10000);
        double q2_D =(static_cast<double>(q2)/10000);
        double q3_D = (static_cast<double>(q3)/10000);

        QQuaternion a(q0_D,q1_D,q2_D,q3_D);
        QVector3D b = a.toEulerAngles();

        result = QString("quterion w,x,y,z : %1, %2, %3 %4 roll,pitch,yaw : %5 %6 %7").arg(q0_D).arg(q1_D).arg(q2_D).arg(q3_D).arg(b.x()).arg(b.y()).arg(b.z());
    }

    return result;

}
QString Error_Interpret(int8_t error_code){
    switch(error_code){
        case 1:
        return "현재 상태에서 해당 CMD 수행 불가";

    case 2:
        return "등록되지 않은 CMD";

    case 3:
        return "모듈에 등록되지 않은 타입";

    case 4:
        return "측정 타입이 지정되지 않았습니다.";
   default:
         return "잘못된 에러 코드입니다.";
    }
}
