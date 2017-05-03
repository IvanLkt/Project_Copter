#include <stdio.h>
#include <time.h>

int main(void)
{
  time_t timer = time(NULL);
  printf("%d\n", timer);
  clock_t timer2;
  gettimeofday(CLOCK_MONOTONIC_RAW, &timer2);
  uint64_t delta_us = timer2.tv_sec * 1000000 + timer2.tv_nsec / 1000;
  printf("%d\n", delta_us);
  return 0;
}

