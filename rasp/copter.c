#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdint.h>

int line = 0; //number of line in input file
int quantity_coordinates = 0; //quantity of coordinates in input file
clock_t real_time_clocks, start_line_time_clocks;
long real_time;
bool  status_of_flight;
double U = 1.0;

/* IMU Data */
int16_t tempRaw;
int16_t gyroX;
int16_t gyroY;
int16_t gyroZ;
double accelX;
double accelY;
double accelZ;
int16_t gyroX_out;
int16_t gyroY_out;
int16_t gyroZ_out;
int16_t accelX_out;
int16_t accelY_out;
int16_t accelZ_out;
int fd;
int data_MPU;

typedef struct Ground {
    double x,y;
} Ground; //Coordinates of the control point

typedef struct Coordinates {
    double x, y;
    int z;
} Coordinates;

typedef struct Point{
    struct Coordinates data;
    struct Point *next;
    struct Point *prev;
} Point;

typedef struct Dynamic_array{
    struct Point *head;
    struct Point *tail;
    int size;
} Dynamic_array;

typedef struct Array_of_Angles{
    struct Angle *head;
    struct Angle *tail;
    int size;
} Array_of_Angles;

typedef struct Angle{
    struct Angle *next;
    struct Angle *prev;
    int angle;
}Angle;


double speed (Groung *Input_Coordinates, U){
    double x_1 = Input_Coordinates[0]->x; //широта
    double y_1 = Input_Coordinates[0]->y; //долгота
    double x_2 = Input_Coordinates[1]->x; //широта
    double y_2 = Input_Coordinates[1]->y; //долгота
    double k =0; //coefficient
    k = (40074000/360)*sin(arctg(abs(y_2-y_1)/abs(x_2-x_1)))+(40074000/360)*cos(x_1)*cos(arctg(abs(y_2-y_1)/abs(x_2-x_1))); // (metr/derges)
    return U/k; //U - copter's speed
}

void get_coordinate (Groung *Input_Coordinates, long *real_time, long *start_line_time, double U, double *X, double *Y){
    double x, y; // local variables
    x = Input_Coordinates[2*line-2]->x + (Input_Coordinates[2*line-1]->x - Input_Coordinates[2*line-2]->x)*(*real_time - *start_line_time)*speed(Input_Coordinates, U)*(sqrt((Input_Coordinates[2*line-1]->x - Input_Coordinates[2*line-2]->x)^2 + (Input_Coordinates[2*line-1]->y - Input_Coordinates[2*line-2]->y)^2))^(-1);
    y = Input_Coordinates[2*line-2]->y + (Input_Coordinates[2*line-1]->y - Input_Coordinates[2*line-2]->y)*(*real_time - *start_line_time)*speed(Input_Coordinates, U)*(sqrt((Input_Coordinates[2*line-1]->x - Input_Coordinates[2*line-2]->x)^2 + (Input_Coordinates[2*line-1]->y - Input_Coordinates[2*line-2]->y)^2))^(-1);
    *X = x; // Link to an external variable
    *Y = y;
}

Dynamic_array* init_database_point()
{
    Dynamic_array *database = (Dynamic_array *) malloc(sizeof(Dynamic_array));
    database->size = 0;
    database->tail = database->head = NULL;
    return database;
}

void add_point (Dynamic_array* database, double x, double y, int z)
{
    Point *tmp = (Point*) malloc(sizeof(Point));
    if (tmp == NULL) {
        exit(1);
    }
    tmp->data.x = x;
    tmp->data.y = y;
    tmp->data.z = z;
    tmp->next = NULL;
    tmp->prev = database->tail;
    if (database->head == NULL) {
        database->head = tmp;
    }
    if (database->tail == NULL) {
        database->tail = tmp;
    }
    if (database->tail) {
        database->tail->next = tmp;
    }
    database->tail = tmp;
    database->size++;
}

Array_of_Angles* init_database_angles()
{
    Array_of_Angles *database_angles = (Array_of_Angles *) malloc(sizeof(Array_of_Angles));
    database_angles->tail = database_angles->head = NULL;
    database_angles->size = 0;
    return database_angles;
}

void add_angle(Array_of_Angles *database_angles, int angle)
{
    Angle *tmp = (Angle*) malloc(sizeof(Angle));
    if (tmp == NULL) {
        exit (1);
    }
    tmp->angle = angle;
    tmp->next = NULL;
    tmp->prev = database_angles->tail;
    if (database_angles->head == NULL) {
        database_angles->head = tmp;
    }
    if (database_angles->tail == NULL) {
        database_angles->tail = tmp;
    }
    if (database_angles->tail) {
        database_angles->tail->next = tmp;
    }
    database_angles->tail = tmp;
    database_angles->size++;
}

void delete_Angle(Array_of_Angles *database_angles)
{
    Angle *tmp = database_angles->head;
    if (tmp == NULL){
        exit(1);
    }
    database_angles->head = tmp->next;
    free(tmp);
    tmp =NULL;
    database_angles->size--;
}

/*Data from HC-SR04*/
void setup_HCSR04() {
    wiringPiSetup();
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    //TRIG pin must start LOW
    digitalWrite(TRIG, LOW);
    delay(30);
}

int getCM() {
    //Send trig pulse
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(20);
    digitalWrite(TRIG, LOW);

    //Wait for echo start
    while(digitalRead(ECHO) == LOW);

    //Wait for echo end
    long startTime = micros();
    while(digitalRead(ECHO) == HIGH);
    long travelTime = micros() - startTime;

    //Get distance in cm
    int distance = travelTime / 58;

    return distance;
}

/*Data from GY-521*/
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

double get_y_rotation(double x, double y, double z)
{
    double radians = atan2(x, dist(y,z));
    return -1*(180.0/3.14*radians);
}

double get_x_rotation(double x, double y, double z)
{
    double radians = atan2(y, dist(x,z));
    return 180.0/3.14*radians;
}

void get_data_from_MPU () {
    wiringPiI2CWriteReg16(fd, 0x6b, 0x00); /*register 107 by datasheet -power management*/
    for (;;) {
        data_MPU = wiringPiI2CRead(fd);
        gyroX_out = read_value_i2c(fd, 0x43);
        gyroX = gyroX_out / 131;
        gyroY_out = read_value_i2c(fd, 0x45);
        gyroY = gyroY_out / 131;
        gyroZ_out = read_value_i2c(fd, 0x47);
        gyroZ = gyroZ_out / 131;

        accelX_out = read_value_i2c(fd, 0x3b);
        accelX = (double) accelX_out / 16384.0;
        accelY_out = read_value_i2c(fd, 0x3d);
        accelY = (double) accelY_out / 16384.0;
        accelZ_out = read_value_i2c(fd, 0x3f);
        accelZ = (double) accelZ_out / 16384.0;

        double X_rotation = get_x_rotation(accelX, accelY, accelZ);
        double Y_rotation = get_y_rotation(accelX, accelY, accelZ);

        if (data == -1) {
            printf("No data\n");
            return -1;
        }
    }
}

int main (int argc, char *argv[]) {
    FILE *file;
    file = fopen("input.txt", "r");
    fscanf (file, "%d", &quantity_coordinates);
    Ground Input_Coordinates[quantity_coordinates]; //array of coordinates from file

    for (int i = 0; i < quantity_coordinates; i++) {
        fscanf(file, "%lf%lf", &(Input_Coordinates[i].x), &(Input_Coordinates[i].y));
        //printf("OK, %lf\n", Input_Coordinates[i].x);
    }

    Dynamic_array *database = init_database_point();
    Array_of_Angles *database_angles =  init_database_angles();


    wiringPiSetup () ;
    fd = wiringPiI2CSetup (0x68);  /*Use i2c detect command to find your respective device address*/
    if(fd==-1) {
        printf("Can't setup the I2C device\n");
        return -1;
    }

    if (...........){
        setup_HCSR04();
        status_of_flight = 1;
        line = 1;
        while (status_of_flight == 1) {
            if (line > 0) {
                real_time_clocks = clock();
                real_time = real_time * 1000 / CLOCKS_PER_SEC;
            }
            if (line == -1) {

            }
        }
    }
}