#include <time.h>
int main () {
clock_t tm;
tm = clock();
long k = tm*1000/CLOCKS_PER_SEC;
printf("%ld", k);
}
