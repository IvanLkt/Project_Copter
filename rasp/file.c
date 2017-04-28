#include <stdio.h>

main () {
  typedef struct _2D_Coordinates {
      double x,y;
  }2D_Coordinates;
  int quantity_coordinates = 0;
  FILE *file;
  file = fopen("input.txt", "r");
  fscanf (file, "%d", &quantity_coordinates);
  2D_Coordinates Input_Coordinates[quantity_coordinates];

  for (int i = 0, i < quantity_coordinates, i++) {
      fscanf (file, "%lf%lf", &(Input_Coordinates[i].x), &(Input_Coordinates[i].y));
      if (fscanf (file, "%lf%lf", &(Input_Coordinates[i].x), &(Input_Coordinates[i].y)) == EOF) {
        exit(1);
      }
  }
}