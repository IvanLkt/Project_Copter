#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdint.h>

/* IMU Data */
int16_t accX;
int16_t accY;
int16_t accZ;
int16_t tempRaw;
int16_t gyroX;
int16_t gyroY;
int16_t gyroZ;


int main (int argc, char *argv[])
{
        int fd;
        int data;
        wiringPiSetup () ;
        fd=wiringPiI2CSetup (0x68) ;  /*Use i2cdetect command to find your respective device address*/
        if(fd==-1)
        {
                printf("Can't setup the I2C device\n");
                return -1;
        }
        else
        {
                for (;;)
                {
                        uint8_t* data=wiringPiI2CRead(fd);
                        gyroX = wiringPiI2CReadReg16(fd, 0x43);
                        gyroY = wiringPiI2CReadReg16(fd, 0x45);
                        gyroZ = wiringPiI2CReadReg16(fd, 0x47);
                                
                        if(data==-1)
                        {
                                printf("No data\n");
                                //return -1;
                        }
                        else
                        {
                                //print data
                                printf("gyro_xout: ", gyroX, " scaled: ", (gyroX / 131));
                                printf("gyro_yout: ", gyroY, " scaled: ", (gyroY / 131));
                                printf("gyro_zout: ", gyroZ, " scaled: ", (gyroZ / 131));
                        }
                }
        }
        return 0;
}
