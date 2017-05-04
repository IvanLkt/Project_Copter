#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>

clock_t start, current_time;
double speed;
bool status_of_flight;

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


Dynamic_array* init()
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

Array_of_Angles* init()
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
    tmp->prev = database_angles->tail
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
        exut(1);
    }
    database_angles->head = tmp->next;
    free(tmp);
    tmp =NULL;
    database_angles->size--;
}

int main()
{
    start = clock() / CLOCKS_PER_SEC;

    

}
