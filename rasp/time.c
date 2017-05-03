#include <stdio.h>
#include <time.h>
#include <wiringPi.h>

int main(void)
{
  time_t timer = time(NULL);
  printf("%d\n", timer);
  unsigned long timer2;
  timer2 = millis();
  printf("%d\n", delta_us);
  return 0;
}

