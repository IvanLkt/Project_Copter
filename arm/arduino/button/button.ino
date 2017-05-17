int buttonPin = 2;
boolean button = LOW;  

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);

}

void loop() {
  button = dedbounce( button, buttonPin);
  Serial.print(button );
  Serial.print("\n" );
}

boolean dedbounce( boolean last, int Pin){ // проверяет состояние кнопки, избегая дребезга
  boolean current = digitalRead(Pin);
  if (last != current)
     {
      delay(10);
      current = digitalRead(Pin);
     }
return current;
}

