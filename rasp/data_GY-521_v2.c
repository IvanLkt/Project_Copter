#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

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
                        data=wiringPiI2CRead(fd);
                        accX = ((data[0] << 8) | data[1]);
                        accY = ((data[2] << 8) | data[3]);
                        accZ = ((data[4] << 8) | data[5]);
                        tempRaw = ((data[6] << 8) | data[7]);
                        gyroX = ((data[8] << 8) | data[9]);
                        gyroY = ((data[10] << 8) | data[11]);
                        gyroZ = ((data[12] << 8) | data[13]);
                        if(data==-1)
                        {
                                printf("No data\n");
                                //return -1;
                        }
                        else
                        {
                                //print data
                                printf("accX=%d\n", accX);
                                printf("accY=%d\n", accY);
                                printf("accZ=%d\n", accZ);
                        }
                }
        }
        return 0;
}
