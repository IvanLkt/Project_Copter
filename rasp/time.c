#include <stdio.h>
#include <time.h>

int main(void)
{
  time_t timer = time(NULL);
  printf("%d\n", timer);
  return 0;
}

