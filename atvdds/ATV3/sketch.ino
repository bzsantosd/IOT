// Definindo os pinos onde os LEDs estão conectados
const int pinosLED[] = {2, 3, 4, 5};
const int numeroDePinos = 4;

// Variável para armazenar o número atual
int contador = 0;

void setup() {
  // Configura cada pino como saída
  for (int i = 0; i < numeroDePinos; i++) {
    pinMode(pinosLED[i], OUTPUT);
  }
}

void loop() {
  for (int i = 0; i < numeroDePinos; i++) {
    int bitStatus = bitRead(contador, i);
    digitalWrite(pinosLED[i], bitStatus);
  }

  contador++;

  if (contador >= 16) {
    contador = 0;
  }

  delay(500);
}
