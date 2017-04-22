#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdint.h>
#include <math.h>

/* IMU Data */
int16_t tempRaw;
int16_t gyroX;
int16_t gyroY;
int16_t gyroZ;
int16_t gyroX_out;
int16_t gyroY_out;
int16_t gyroZ_out;

def read_value_i2c(int addres_register):
    int value = read_value(addres_register)
    if (value >= 0x8000):
        return -((65535 - value) + 1)
    else:
        return value

def read_value(addres_register):
    int high = wiringPiI2CReadReg16(address, addres_register)
    int low = wiringPiI2CReadReg16(address, addres_register + 1)
    int value = (high << 8) + low
    return value

int main (int argc, char *argv[])
{
        int fd;
        int data;
        wiringPiSetup () ;
        fd = wiringPiI2CSetup (0x68);  /*Use i2cdetect command to find your respective device address*/
        if(fd==-1)
        {
                printf("Can't setup the I2C device\n");
                return -1;
        }
        else
        {       
                wiringPiI2CWriteReg16(fd, 0x6b, 0x00); /*register 107 datasheet -power management*/
                for (;;)
                {
                        data=wiringPiI2CRead(fd);
                        gyroX_out = wiringPiI2CReadReg16(fd, 0x43);
                        gyroX = gyroX_out/16384
                        gyroY_out = wiringPiI2CReadReg16(fd, 0x45);
                        gyroY = gyroY_out/16384
                        gyroZ_out = wiringPiI2CReadReg16(fd, 0x47);
                        gyroZ = gyroZ_out/16384
                                
                        if(data==-1)
                        {
                                printf("No data\n");
                                //return -1;
                        }
                        else
                        {
                                //print data
                                printf("gyro_x:%d", gyroX);
                                printf("gyro_y:%d", gyroY);
                                printf("gyro_z:%d\n", gyroZ);
                        }
                }
        }
        return 0;
}
