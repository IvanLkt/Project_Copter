#include < stdio.h >   //Для printf
#include < time.h >   //Для time, gmtime, asctime

int main (void)
{    
   // Переменная для сохранения текущего системного времени
   long int s_time;
   // Указатель, в который будет помещен адрес структуры с 
   // преобразованным временем
   struct tm *m_time;

   // Считываем текущее системное время
   s_time = time (NULL);

   // Преобразуем системное время в локальное
   m_time  = gmtime (&s_time);

   // С помощью функции asctime преобразуем локальное время в строку
   // и выводим результат на консоль
   printf (“Время: %s\n”,asctime (m_time) );

   return 0;
}
