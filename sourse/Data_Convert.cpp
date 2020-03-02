#include "header/Data_Convert.h"
/*********************************************************************
 * @fn      Gyro_Convert
 *
 * @brief  change rawData to gyroData(deg/s)
 *
 * @param raw gyro data
 *
 * @return  gyro data
 */

double Data_Convert::Gyro_Convert(double data){
    return data = (data * 1.0) / (65536 / 4000);
}
/*********************************************************************
 * @fn      Accel_Convert
 *
 * @brief  change rawData to Accel Data(g)
 *
 * @param raw gyro data
 *
 * @return  Accel data
 */

double Data_Convert::Accel_Convert(double data){
    return data = (data * 1.0) / (32768/16
                                  );
}
/*********************************************************************
 * @fn      Magnet_Convert
 *
 * @brief  change rawData to gyroData(uT)
 *
 * @param raw magnet data
 *
 * @return  magnet data
 */

double Data_Convert::Magnet_Convert(double data){
    return data = (data* 48./32760.0);
}
/*********************************************************************
 * @fn      int_to_char
 *
 * @brief   chage 10 Decimal to String.
 *             it used period change.
 *
 * @param period - user input
 *
 * @return  String period data
 */
char *Data_Convert:: int_to_char(int period){
    static QString period_s;
    period_s.sprintf("%x",period);

    static QByteArray temp_b;
    temp_b = period_s.toLocal8Bit();
    return temp_b.data();
}


