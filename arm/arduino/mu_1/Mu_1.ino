#include <Wire.h>
#include "Kalman.h"
#include <SPI.h>
#include "RF24.h"


Kalman kalmanX;
Kalman kalmanY;

const uint64_t pipe = 0xE8E8F0F0E1LL; // адрес канала передачи
RF24 radio(9,10);
int msg[3];

uint8_t IMUAddress = 0x68;

/* IMU Data Чистые данные с гироскопа */ 
int16_t accX;
int16_t accY;
int16_t accZ;
int16_t tempRaw;
int16_t gyroX;
int16_t gyroY;
int16_t gyroZ;

double accXangle; // Angle calculate using the accelerometer
double accYangle;
double temp;
double gyroXangle = 180; // Angle calculate using the gyro
double gyroYangle = 180;
double compAngleX = 180; // Calculate the angle using a Kalman filter
double compAngleY = 180;
double kalAngleX; // Calculate the angle using a Kalman filter
double kalAngleY;

uint32_t timer;

int buttonPin = 2;      //пин кнопки
boolean button = LOW;   // начальное значение(не нажата)

void setup() {  
  Serial.begin(115200);
  Wire.begin();  
  i2cWrite(0x6B,0x00); // Disable sleep mode  
  if(i2cRead(0x75,1)[0] != 0x68) { // Read "WHO_AM_I" register
    Serial.print(F("MPU-6050 with address 0x"));
    Serial.print(IMUAddress,HEX);
    Serial.println(F(" is not connected"));
    while(1);
  }      
  kalmanX.setAngle(180); // Set starting angle
  kalmanY.setAngle(180);
  timer = micros();

  pinMode(buttonPin, INPUT);  //будем читать с пина кнопки

    //============================================================Модуль NRF24
  radio.begin();                      // Включение модуля
  radio.setAutoAck(1);                // Установка режима подтверждения приема;
  radio.setRetries(1,1);              // Установка интервала и количества попыток
  radio.setDataRate(RF24_250KBPS);    // Устанавливаем скорость
  radio.setPALevel(RF24_PA_LOW);      // Установка максимальной мощности;
  radio.setChannel(10);               // Устанавливаем канал
  radio.openWritingPipe(pipe);        // Открываем канал передачи
}

void loop() {
  
  button = dedbounce( button, buttonPin);  //чекаем кнопку
  
  /* Update all the values (читаем с регистров в гироскопеи раскладывем по переменным)*/
  uint8_t* data = i2cRead(0x3B,14);    
  accX = ((data[0] << 8) | data[1]);
  accY = ((data[2] << 8) | data[3]);
  accZ = ((data[4] << 8) | data[5]);  
  tempRaw = ((data[6] << 8) | data[7]);  
  gyroX = ((data[8] << 8) | data[9]);
  gyroY = ((data[10] << 8) | data[11]);
  gyroZ = ((data[12] << 8) | data[13]);
  
  /* Calculate the angls based on the different sensors and algorithm */
  accYangle = (atan2(accX,accZ)+PI)*RAD_TO_DEG;
  accXangle = (atan2(accY,accZ)+PI)*RAD_TO_DEG;    
  
  double gyroXrate = (double)gyroX/131.0;
  double gyroYrate = -((double)gyroY/131.0);
  gyroXangle += gyroXrate*((double)(micros()-timer)/1000000); // Calculate gyro angle without any filter  
  gyroYangle += gyroYrate*((double)(micros()-timer)/1000000);
  //gyroXangle += kalmanX.getRate()*((double)(micros()-timer)/1000000); // Calculate gyro angle using the unbiased rate
  //gyroYangle += kalmanY.getRate()*((double)(micros()-timer)/1000000);
  
  compAngleX = (0.93*(compAngleX+(gyroXrate*(double)(micros()-timer)/1000000)))+(0.07*accXangle); // Calculate the angle using a Complimentary filter
  compAngleY = (0.93*(compAngleY+(gyroYrate*(double)(micros()-timer)/1000000)))+(0.07*accYangle);  
  
  kalAngleX = kalmanX.getAngle(accXangle, gyroXrate, (double)(micros()-timer)/1000000); // Calculate the angle using a Kalman filter
  kalAngleY = kalmanY.getAngle(accYangle, gyroYrate, (double)(micros()-timer)/1000000);
  timer = micros();
  
  temp = ((double)tempRaw + 12412.0) / 340.0;
  
  /* Print Data */   
  /*
  Serial.print(accX);Serial.print("\t"); // выводит чистые данные с датчиков
  Serial.print(accY);Serial.print("\t");  
  Serial.print(accZ);Serial.print("\t");    
  
  Serial.print(gyroX);Serial.print("\t");  
  Serial.print(gyroY); Serial.print("\t");   
  Serial.print(gyroZ);Serial.print("\t");  
  */
  Serial.print(accXangle);Serial.print("\t"); //выводит фильтрованые значения
  Serial.print(accYangle);Serial.print("\t"); 
    
  Serial.print(gyroXangle);Serial.print("\t");
  Serial.print(gyroYangle);Serial.print("\t");
  
  Serial.print(compAngleX);Serial.print("\t");
  Serial.print(compAngleY); Serial.print("\t");
  
  Serial.print(kalAngleX);Serial.print("\t");
  Serial.print(kalAngleY);Serial.print("\t");
  
  //Serial.print(temp);Serial.print("\t");
   
  Serial.print("\n");
  
  delay(1); // The accelerometer's maximum samples rate is 1kHz

  msg[0] = (int)button;
  msg[1] = (int)kalAngleX;
  msg[2] = (int)kalAngleY;
  
  radio.write(&msg, sizeof(msg));

  Serial.print(msg[1]);Serial.print("\t");
  Serial.print(msg[2]);Serial.print("\t");
  delay(50); // The accelerometer's maximum samples rate is 1kHz
}
void i2cWrite(uint8_t registerAddress, uint8_t data){
  Wire.beginTransmission(IMUAddress);
  Wire.write(registerAddress);
  Wire.write(data);
  Wire.endTransmission(); // Send stop
}
uint8_t* i2cRead(uint8_t registerAddress, uint8_t nbytes) {
  uint8_t data[nbytes];  
  Wire.beginTransmission(IMUAddress);
  Wire.write(registerAddress);
  Wire.endTransmission(false); // Don't release the bus
  Wire.requestFrom(IMUAddress, nbytes);   // Send a repeated start and then release the bus after reading
  for(uint8_t i = 0; i < nbytes; i++)
    data[i] = Wire.read();
  return data;
} 

boolean dedbounce( boolean last, int Pin){    // проверяет состояние кнопки избегая дребезга
  boolean current = digitalRead(Pin);
  if (last != current)
     {
      delay(10);
      current = digitalRead(Pin);
     }
return current;
}

