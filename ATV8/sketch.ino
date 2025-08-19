const int pinoPotenciometro = A0;
const int pinosLEDs[] = {2, 3, 4, 5, 6, 7, 8, 9};
const int numeroDeLEDs = 8;

void setup() {
  for (int i = 0; i < numeroDeLEDs; i++) {
    pinMode(pinosLEDs[i], OUTPUT);
  }
}

void loop() {
  int valorPotenciometro = analogRead(pinoPotenciometro);
  int ledsParaAcender = map(valorPotenciometro, 0, 1023, 0, numeroDeLEDs);

  for (int i = 0; i < ledsParaAcender; i++) {
    digitalWrite(pinosLEDs[i], HIGH);
  }

  for (int i = ledsParaAcender; i < numeroDeLEDs; i++) {
    digitalWrite(pinosLEDs[i], LOW);
  }

  delay(10);
}