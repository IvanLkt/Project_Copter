
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
 
 
 int main (void)
 {
   double x0,y0,z;        // координаты начала(дом)
   double x1,x2,y1,y2;   // координаты 2 смежных вершин 4 угольника
   
   printf("Введите координаты старта(с высотой), и 2х вершин(без)");
   scanf("%lf%lf%lf%lf%lf%lf%lf",  &x0, &y0, &z, &x1, &y1, &x2, &y2);
   
   int m,n;                // частоты измерений
   printf("Ввидите частоты измерений по обоим векторам(2 натуральных числа)");
   scanf("%d", &n);
   
   double dx,dy;  // период шага
   
   double x,y;
  
   FILE *shot , *notshot;
   
   shot = fopen("shot.txt","w");
   notshot = fopen("notshot.txt","w");
  
   dx = (x1 - x0) / n;
   dy = (y1 - y0) / n;
   x = x0;
   y = y0;
   int i;
   fprintf(shot, "%d", (2*n+2));
   fprintf(notshot, "QGC WPL 110\n");
   int num;
   for (i = 0; i <= n; i++) {
        x = x0 + i * dx;
        y = y0 + i * dy;
        num = 2 * i;
        fprintf(shot, " %f  %f\n", x, y);
        fprintf(notshot,"%n 0	0	16	0.000000	0.000000	0.000000	0.000000 %.6lf %.6lf %.6lf 1", &num, x ,y ,z);
        
        num = 2 * i + 1;
        x = x1 + i * dx;
        y = y1 + i * dy;
        fprintf(shot, " %f  %f\n", x, y);
        fprintf(notshot," %n 0	0	16	0.000000	0.000000	0.000000	0.000000 %.6lf %.6lf %.6lf 1", &num, x ,y ,z);
        }
    return(0);
 }   
