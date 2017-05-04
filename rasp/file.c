#include <stdio.h>
#include <math.h>

int line = 0;

int main () {
  typedef struct Ground {
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

double speed (Groung *Input_Coordinates, U){
  double x_1 = Input_Coordinates[0]->x; //широта
  double y_1 = Input_Coordinates[0]->y; //долгота
  double x_2 = Input_Coordinates[1]->x; //широта
  double y_2 = Input_Coordinates[1]->y; //долгота
  double k =0; //coefficient
  k = (40074000/360)*sin(arctg(abs(y_2-y_1)/abs(x_2-x_1)))+(40074000/360)*cos(x_1)*cos(arctg(abs(y_2-y_1)/abs(x_2-x_1))); // (metr/derges)
  return U/k;
}

void get_coordinate (Groung *Input_Coordinates, long *real_time, long *start_line_time, U){
  double x = 0;
  double y = 0;
  x = Input_Coordinates[line+1]->x + (Input_Coordinates[line + 2]->x - Input_Coordinates[line]->x)*(*real_time - *start_line_time)*speed(Input_Coordinates, U)*(sqrt((Input_Coordinates[line+2]->x - Input_Coordinates[line+1]->x)^2 + (Input_Coordinates[line+2]->y - Input_Coordinates[line+1]->y)^2))^(-1);
  y = Input_Coordinates[line+1]->y + (Input_Coordinates[line + 2]->y - Input_Coordinates[line]->y)*(*real_time - *start_line_time)*speed(Input_Coordinates, U)*(sqrt((Input_Coordinates[line+2]->x - Input_Coordinates[line+1]->x)^2 + (Input_Coordinates[line+2]->y - Input_Coordinates[line+1]->y)^2))^(-1);
}
