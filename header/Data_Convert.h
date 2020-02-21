#ifndef DATA_CONVERT_H
#define DATA_CONVERT_H
#include<stdint.h>
#include <QObject>

class Data_Convert : public QObject{

public:
    double Gyro_Convert(double data);
    double Accel_Convert(double data);
    double Magnet_Convert(double data);
    char * int_to_char(int period);
    const char *QString_to_char(QString msg);
};

#endif // DATA_CONVERT_H
