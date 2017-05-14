#include <time.h>
#include <math.h>
#include <wiringPi.h>

int main () {
clock_t tm;
for (int i = 0; i<100000000; i++)
{
  tm = clock();
  long k = tm*1000/CLOCKS_PER_SEC;
  printf("%ld \n", k);
}
//delay(3000);

}
