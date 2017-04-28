#include <stdio.h>
#include <time.h>

int main ()
{
  time_t seconds;

  seconds = time (NULL); // получить текущую дату, выраженную в секундах

  printf('%d', seconds); //Количество часов, прошедших с 1 января 1970 года

  return 0;
}

