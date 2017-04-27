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
    int numrecords = 0;
} Dynamic_array;

void init()
{
    Dynamic_array *database = (Dynamic_array *);
    database.start = NULL;
    database.finish = database.start;
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
