#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <malloc.h>

clock_t start, current_time;
double speed;
bool status_of_flight;

typedef struct _Coordinates {
    double x, y;
    int z;
} Coordinates;

typedef struct _Point{
    Coordinates data;
    _Point *next;
    _Point *prev;
} Point;

typedef struct _Dynamic_array{
    Point *head = NULL;
    Point *tail = head;
    int size = 0;
} Dynamic_array;

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
    tmp->data->x = x;
    tmp->data->y = y;
    tmp->data->z = z;
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



int main()
{
    start = clock() / CLOCKS_PER_SEC;

    

}
