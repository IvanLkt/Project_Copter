#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#define COPT 7


void setup() {
        wiringPiSetup();
        pinMode(COPT, INPUT);
}

int main(void) {
  if (digitalRead(COPT) == LOW) {
  printf ("LOW");
  }
  if (digitalRead(COPT) == HIGH) {
  printf ("HIGH");
  }
}
