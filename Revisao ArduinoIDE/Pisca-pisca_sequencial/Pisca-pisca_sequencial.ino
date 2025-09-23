void setup() {
  for(int i = 0; i <= 12; i++) {
    pinMode((i+3), OUTPUT);
  }
}
void loop() {

  int i;

  if (digitalRead(i) != HIGH) {
    for(int i = 0; i <= 12; i++) {
      digitalWrite(i+3, HIGH);
      delay(10);
      digitalWrite(i+3, LOW);
      delay(10);
    }
  } else {
    for(int i = 15; i >= 3; i--) {
      digitalWrite(i-3, HIGH);
      delay(10);
      digitalWrite(i-3, LOW);
      delay(10);
    }
  }
}
