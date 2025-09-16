
// Pinos dos LEDs
const int ledVerde = 2;
const int ledAmarelo = 3;
const int ledVermelho = 4;
const int ledPedestre = 5; // LED branco

// Pino do botão
const int botao = 6;

void setup() {
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledPedestre, OUTPUT);
  pinMode(botao, INPUT); // Com resistor pull-down de 10kΩ
}

void loop() {
  if (digitalRead(botao) == HIGH) {
    // Acionado o botão — interrompe para pedestre
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, HIGH);
    delay(1000);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVermelho, HIGH);
    digitalWrite(ledPedestre, HIGH); // Pedestre pode passar
    delay(5000);
    digitalWrite(ledPedestre, LOW);
  }

  // Ciclo normal do semáforo
  digitalWrite(ledVermelho, LOW);
  digitalWrite(ledVerde, HIGH);
  delay(5000);
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledAmarelo, HIGH);
  delay(2000);
  digitalWrite(ledAmarelo, LOW);
  digitalWrite(ledVermelho, HIGH);
  delay(5000);
}
