int analogPin = 4;
int val;

void setup() {
  Serial.begin(9600);

}

void loop() {
  val = analogRead(analogPin);
  if (val != 0){
  Serial.println(val);
  }
}
