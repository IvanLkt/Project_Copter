#include <stdio.h> //Для printf
#include <time.h>  //Для clock_gettime

int main (void)
{  
   //Структуры для сохранения определенного времени
   struct timespec mt1, mt2; 
   //Переменная для расчета дельты времени
   long int tt;      
   
   //Определяем текущее время
   clock_gettime (CLOCK_REALTIME, &mt1);
   
   //Выводим определенное время на экран консоли
   printf (“Секунды: %ld\n”,mt1.tv_sec);
   printf (“Наносекунды: %ld\n”,mt1.tv_nsec);

   //Определяем текущее время
   clock_gettime (CLOCK_REALTIME, &mt2);

   //Рассчитываем разницу времени между двумя измерениями
   tt=1000000000*(mt2.tv_sec - mt1.tv_sec)+(mt2.tv_nsec - mt1.tv_nsec);

   //Выводим результат расчета на экран
   printf (“Затрачено время: %ld нс\n”,tt);

   return 0;
