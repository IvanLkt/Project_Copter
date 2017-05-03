#include <time.h>
int main () {
clock_t tm;
tm = clock()
printf("%ld", tm*1000/CLOCKS_PER_SEC);
}
