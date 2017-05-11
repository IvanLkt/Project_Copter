#include <SPI.h>
#include "RF24.h"

const uint64_t pipe = 0xE8E8F0F0E1LL; // адрес канала передачи
RF24 radio(9,10);
int msg[3];

void setup(){
  Serial.begin(9600);
  //============================================================Модуль NRF24
  radio.begin();                      // Включение модуля
  radio.setAutoAck(1);                // Установка режима подтверждения приема;
  radio.setDataRate(RF24_250KBPS);    // Устанавливаем скорость
  radio.setChannel(10);               // Устанавливаем канал
  radio.openReadingPipe(1,pipe);      // Открываем 1 канал приема
  radio.startListening();             // Начинаем слушать эфир
  
}

void loop(){
  if (radio.available()){
    while (radio.available()){
      radio.read(&msg, sizeof(msg));
      Serial.print(msg[1]);Serial.print("\t");
      Serial.print(msg[2]);Serial.print("\t");
      Serial.print("\n");
    }
  }
}

