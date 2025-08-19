const int pinoLED = 9; 

void setup() {
}

void loop() {
  for (int i = 0; i <= 255; i++) {
    analogWrite(pinoLED, i);
    delay(10);
  }

  for (int i = 255; i >= 0; i--) {
    analogWrite(pinoLED, i);
    delay(10); 
  }
}
