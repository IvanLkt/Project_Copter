#include <stdio.h>

int main () {
  typedef struct _Ground {
      double x,y;
  } Ground; //Coordinates of the control point
  int quantity_coordinates = 0;
  FILE *file;
  file = fopen("input.txt", "r");
  fscanf (file, "%d", &quantity_coordinates);
  Ground Input_Coordinates[quantity_coordinates]; //array of coordinates from file

  for (int i = 0; i < quantity_coordinates; i++) {
      fscanf (file, "%lf%lf", &(Input_Coordinates[i].x), &(Input_Coordinates[i].y));
      //if (fscanf (file, "%lf%lf", &(Input_Coordinates[i].x), &(Input_Coordinates[i].y)) == EOF) {
      //return 1; 
      //}
  printf("OK, %lf\n",Input_Coordinates[i].x);
  }
return 0;
}
