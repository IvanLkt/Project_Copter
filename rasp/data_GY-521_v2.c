#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdint.h>
#include <math.h>

/* IMU Data */
int16_t tempRaw;
uint16_t gyroX;
uint16_t gyroY;
uint16_t gyroZ;
double accelX;
double accelY;
double accelZ;
uint16_t gyroX_out;
uint16_t gyroY_out;
uint16_t gyroZ_out;
uint16_t accelX_out;
uint16_t accelY_out;
uint16_t accelZ_out;


int read_value_i2c(int fd, int addres_register)
{
    int16_t value = read_value(fd, addres_register);
    if (value >= 0x8000)
    {
        return -((65535 - value) + 1);
    }
    else
    {
        return value;
    }
}

int read_value(fd, addres_register)
{
    int high = wiringPiI2CReadReg16(fd, addres_register);
    int low = wiringPiI2CReadReg16(fd, addres_register + 1);
    int value = (high << 8) + low;
    return value;
}

double dist(double a, double b)
{
    return sqrt((a*a)+(b*b));
}

double get_y_rotation(int x, int y, int z)
{
    double radians = atan2(x, dist(y,z));
    return -1*(180.0/3.14*radians);
}

double get_x_rotation(double x, double y, double z)
{
    double radians = atan2(y, dist(x,z));
    return 180.0/3.14*radians;
}

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
                wiringPiI2CWriteReg16(fd, 0x6b, 0x00); /*register 107 by datasheet -power management*/
                for (;;)
                {
                        data=wiringPiI2CRead(fd);
                        gyroX_out = read_value_i2c(fd, 0x43);
                        gyroX = gyroX_out/131;
                        gyroY_out = read_value_i2c(fd, 0x45);
                        gyroY = gyroY_out/131;
                        gyroZ_out = read_value_i2c(fd, 0x47);
                        gyroZ = gyroZ_out/131;
                        
                        accelX_out = read_value_i2c(fd, 0x3b);
                        accelX = (double)accelX_out / 16384.0;
                        accelY_out = read_value_i2c(fd ,0x3d);
                        accelY = (double)accelY_out / 16384.0;
                        accelZ_out = read_value_i2c(fd, 0x3f);
                        accelZ = (double)accelZ_out / 16384.0;

                        double xAngle =  get_x_rotation(accelX, accelY, accelZ);
				
                        if(data==-1)
                        {
                                printf("No data\n");
                                //return -1;
                        }
                        else
                        {
                                //print data

                                printf("%f", xAngle);
				printf("accelX_out%f\n",accelX, accelY, accelZ);
				printf('_________________________________________________________\n');
				
                        }
                }
        }
        return 0;
}
