#include <stdio.h>
#include <time.h>

int main(void)
{
  time_t timer = time(NULL);
  printf("%s\n", timer);
  return 0;
}

