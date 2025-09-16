
// Pinos dos LEDs
const int ledVerde = 2;
const int ledAmarelo = 3;
const int ledVermelho = 4;

// Pino do sensor LM35
const int sensorTemp = A0;

void setup() {
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  Serial.begin(9600); // Monitor serial (opcional)
}

void loop() {
  int leitura = analogRead(sensorTemp);
  float tensao = leitura * 5.0 / 1023.0;
  float temperatura = tensao * 100; // LM35: 10mV por grau

  Serial.print("Temperatura: ");
  Serial.println(temperatura);

  // Lógica dos LEDs
  if (temperatura <= 25) {
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVermelho, LOW);
  } else if (temperatura > 25 && temperatura <= 35) {
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, HIGH);
    digitalWrite(ledVermelho, LOW);
  } else {
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVermelho, HIGH);
  }

  delay(1000); // Atualiza a cada 1 segundo
}

