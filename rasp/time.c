#include <sys/time.h>
struct timeval tv1, tv2, dtv;
struct timezone tz;
//функция для начала замера времени, сохраняющая в переменной tv1 время начала измерений
void time_start() 
{ gettimeofday(&tv1, &tz); }
//функция для окончания замера времени, возвращающая количество миллисекунд, прошедших с начала замера времени
long time_stop()
{ gettimeofday(&tv2, &tz);
  dtv.tv_sec= tv2.tv_sec  - tv1.tv_sec;	//разница секунд
  dtv.tv_usec=tv2.tv_usec - tv1.tv_usec;	//разница микросекунд
  if (dtv.tv_usec<0) { dtv.tv_sec--; dtv.tv_usec+=1000000; }
  //возвращение количества прошедших миллисекунд
  return dtv.tv_sec*1000 + dtv.tv_usec/1000;  
}
int main {
   time_start();
   delay(500);
   long o = time_stop();
   printf("%lf", o);
