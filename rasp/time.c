#include <stdio.h>
#include <time.h>

int main(void)
{
  time_t timer = time(NULL);
  printf("%d\n", timer);
  clock_t timer2;
  gettimeofday(&timer2, NULL);
  printf("%d\n", timer2);
  return 0;
}

