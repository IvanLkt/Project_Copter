#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <malloc.h>

clock_t start, current_time;
double speed;
bool status_of_flight;


typedef struct {
    double x, y;
    int z;
    Point *next;
    Point *prev;    
} Point;

typedef struct {
    Point *start = NULL;
    Point *finish = start;
    int size = 0;
} Dynamic_array;

Dynamic_array* init()
{
    Dynamic_array *database = (Dynamic_array *) malloc(sizeof(Dynamic_array));
    database->size = 0;
    database->finish = database->start = NULL;
    return database;
}
 
void add_point (Point *new_point)
{
    if !database.
}

void Enter_point(point* Array_of_point)
{
    
}

int main()
{
    start = clock() / CLOCKS_PER_SEC;
    (Point*) Array_of_point malloc(2*sizeof(Point));
    

}
