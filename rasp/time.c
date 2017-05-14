#include <time.h>
#include <math.h>
#include <wiringPi.h>

int main () {
clock_t tm;
for (int i = 0; i<100000000; i++)
{
  tm = clock();
  long k = 1000.0*(tm)/CLOCKS_PER_SEC;
  printf("%ld \n", k);
}
//delay(3000);

}
