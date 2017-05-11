#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#define COPT 1


void setup_port() {
        wiringPiSetup();
        pinMode(COPT, INPUT);
}

int main(void) {
  setup_port();
  if (digitalRead(COPT) == LOW) {
  printf ("LOW");
  }
  if (digitalRead(COPT) == HIGH) {
  printf ("HIGH");
  }
}
