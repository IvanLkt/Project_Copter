#include <stdio.h>
#include <time.h>

int main(void)
{
  const time_t timer = time(NULL);
  printf("%s\n", ctime(&timer));
  return 0;
}
