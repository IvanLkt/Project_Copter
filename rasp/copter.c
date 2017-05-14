
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdint.h>

typedef struct Data {
    int COPT;
    int START;
    int TRIG;
    int ECHO;

    int line; //number of line in input file
    int quantity_coordinates; //quantity of coordinates in input file
    clock_t real_time_clocks, start_line_time_clocks, start_time_clocks;
    long real_time;
    long start_line_time;
    long start_time;
    bool status_of_flight;
    bool status_of_turn; // 0 not turn; 1 - turn
    double U;
    int fd;

    int alt;
} Data;

typedef struct IMU_Data {
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
    int data_MPU;
} IMU_Data;

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


double speed (Ground *Input_Coordinates, double U){
    double x_1 = Input_Coordinates[0].x; //широта
    double y_1 = Input_Coordinates[0].y; //долгота
    double x_2 = Input_Coordinates[1].x; //широта
    double y_2 = Input_Coordinates[1].y; //долгота
    double k =0; //coefficient
    double dist_metr = pow(pow((40074000/360)*fabs(y_2 - y_1)*cos(fabs((x_1 + x_2)/2)), 2) + pow(fabs(x_2 - x_1)*(40074000/360), 2), 0.5);
    double dist_deg = pow(pow(y_2 - y_1, 2) + pow(x_2 - x_1, 2), 0.5);
    k = dist_metr/dist_deg;// (metr/derges)
    return U/k; //U - copter's speed
}

void get_coordinate (Ground *Input_Coordinates, double real_time, double start_line_time, double *X, double *Y, int line,
                     double U){
    double x, y; // local variables
    x = Input_Coordinates[2*line-2].x + (Input_Coordinates[2*line-1].x - Input_Coordinates[2*line-2].x)*(((double)real_time - (double)start_line_time)/1000)*speed(Input_Coordinates)*pow(sqrt(pow(Input_Coordinates[2*line-1].x - Input_Coordinates[2*line-2].x, 2) + pow(Input_Coordinates[2*line-1].y - Input_Coordinates[2*line-2].y, 2)), (-1));
    y = Input_Coordinates[2*line-2].y + (Input_Coordinates[2*line-1].y - Input_Coordinates[2*line-2].y)*(((double)real_time - (double)start_line_time)/1000)*speed(Input_Coordinates)*pow(sqrt(pow(Input_Coordinates[2*line-1].x - Input_Coordinates[2*line-2].x, 2) + pow(Input_Coordinates[2*line-1].y - Input_Coordinates[2*line-2].y, 2)), (-1));
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

Data* init_variables () {
    Data *variables = (Data *) malloc(sizeof(Data));
    variables->COPT = 1;
    variables->START = 7;
    variables->TRIG = 5;
    variables->ECHO = 6;
    variables->U =1.0;
    variables->line = 0;
    variables->quantity_coordinates = 0;

}

IMU_Data* init_IMU () {
    IMU_Data *IMU = (IMU_Data *) malloc(sizeof(IMU_Data));
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
void setup_HCSR04(int TRIG, int ECHO) {
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    //TRIG pin must start LOW
    digitalWrite(TRIG, LOW);
    delay(30);
}

int getCM(int TRIG, int ECHO) {
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
int read_value(int fd, int addres_register)
{
    int high = wiringPiI2CReadReg16(fd, addres_register);
    int low = wiringPiI2CReadReg16(fd, addres_register + 1);
    int value = (high << 8) + low;
    return value;
}

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


void get_data_from_MPU (IMU_Data *IMU, int fd) {
    wiringPiI2CWriteReg16(fd, 0x6b, 0x00); /*register 107 by datasheet -power management*/
        IMU->data_MPU = wiringPiI2CRead(fd);
        IMU->gyroX_out = read_value_i2c(fd, 0x43);
        IMU->gyroX = IMU->gyroX_out / 131;
        IMU->gyroY_out = read_value_i2c(fd, 0x45);
        IMU->gyroY =IMU-> gyroY_out / 131;
        IMU->gyroZ_out = read_value_i2c(fd, 0x47);
        IMU->gyroZ = IMU->gyroZ_out / 131;

        IMU->accelX_out = read_value_i2c(fd, 0x3b);
        IMU->accelX = (double) IMU->accelX_out / 16384.0;
        IMU->accelY_out = read_value_i2c(fd, 0x3d);
        IMU->accelY = (double) IMU->accelY_out / 16384.0;
        IMU->accelZ_out = read_value_i2c(fd, 0x3f);
        IMU->accelZ = (double) IMU->accelZ_out / 16384.0;

}


int check_turn(Array_of_Angles *database_angles, int *line, bool *status_of_turn, long *start_line_time) {
    int tmp_turn = 0;
    for (int i=0; i < database_angles->size; i++) {
        Angle *tmp = database_angles->head;
        int j = 0;
        while (j < i) {
            tmp = tmp->next;
            j++;
        }
        int turn = abs(tmp->angle + 450); // defolt -452
        if (abs(tmp->angle) > 2000) {
            tmp_turn ++;
        }
    }
    if (*status_of_turn == false && line > 0) {
        if (tmp_turn == 30) {
            *status_of_turn = true; //now at turn
            *line = (-1)*(*line); // now at short line
        }
	    return 0;
    }
    if (*status_of_turn == true) {
        if (tmp_turn == 0) {
            *status_of_turn = false; //turn is over
        }
	    return 0;
    }
    if (*status_of_turn == false && line < 0) {
        if (tmp_turn == 30) {
            *status_of_turn = true;
            *line = (-1)*(*line) + 1; // long line
            clock_t start_line_time_clocks = clock();
            *start_line_time = 1000.0 * (start_line_time_clocks) / CLOCKS_PER_SEC;
        }
        return 0;
    }
}


void setup_port(int COPT, int START) {
    wiringPiSetup();
    pinMode(COPT, INPUT);
    pinMode(START, INPUT);
}

int main (int argc, char *argv[]) {
    Data *variables = init_variables();
    IMU_Data *IMU = init_IMU();
    setup_port(variables->COPT, variables->START);
    while (digitalRead(variables->START) == LOW);
    printf ("START_SETUP\n");
    FILE *file;
    file = fopen("input.txt", "r");
    fscanf (file, "%d", &(variables->quantity_coordinates));
    Ground Input_Coordinates[variables->quantity_coordinates]; //array of coordinates from file

    for (int i = 0; i < variables->quantity_coordinates; i++) {
        fscanf(file, "%lf%lf", &(Input_Coordinates[i].x), &(Input_Coordinates[i].y));
        //printf("OK, %lf\n", Input_Coordinates[i].x);
    }

    Dynamic_array *database = init_database_point();
    Array_of_Angles *database_angles =  init_database_angles();

    variables->fd = wiringPiI2CSetup (0x68);  /*Use i2c detect command to find your respective device address*/
    if(variables->fd==-1) {
        printf("Can't setup the I2C device\n");
        return -1;
    }
    printf ("FINISH_SETUP\n");

    while (digitalRead(variables->COPT) == LOW);

    if (digitalRead(variables->COPT) == HIGH){
        setup_HCSR04(variables->TRIG, variables->ECHO);
        variables->start_time_clocks = clock();
        variables->start_time = 1000.0 * (variables->start_time_clocks) / CLOCKS_PER_SEC;
        variables->status_of_flight = true;
        variables->line = 1;
        variables->start_line_time = variables->real_time;
        while (variables->status_of_flight == true) {
            get_data_from_MPU(IMU, variables->fd);
            add_angle(database_angles, IMU->gyroX_out);
            if (database_angles->size > 30) {
                delete_Angle(database_angles);
            }
            int code = check_turn(database_angles, &(variables->line), &(variables->status_of_turn), &(variables->start_line_time));
            if (variables->line > 0) {
                variables->real_time_clocks = clock();
                variables->real_time = 1000.0 *(variables->real_time_clocks) / CLOCKS_PER_SEC;
                variables->alt = getCM(variables->TRIG, variables->ECHO);
                double X, Y;
                get_coordinate(Input_Coordinates, variables->real_time, variables->start_line_time, &X, &Y, variables->line, variables->U);
                add_point(database, X, Y, variables->alt);
            }
            if (digitalRead(variables->COPT) == LOW) {
                variables->status_of_flight = false;
            }
        }
    }
    FILE *output_data;
    output_data = fopen ("output_data.txt", "w");
    Point *tmp = database->head;
    for (int i = 0; i < database->size; i++) {
        fprintf(output_data, "%lf      %lf      %d\n", tmp->data.x, tmp->data.y, tmp->data.z);
        tmp = tmp->next;
    }
    printf("STOP\n");
}
