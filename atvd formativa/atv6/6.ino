// Define o pino analógico conectado ao sensor LDR
const int pinoLDR = A0;

void setup() {
  // Inicializa a comunicação serial a 9600 bits por segundo
  Serial.begin(9600);
  Serial.println("Medidor de Brilho Ambiente com LDR");
  Serial.println("----------------------------------");
}

void loop() {
  // Lê o valor analógico do sensor LDR (de 0 a 1023)
  int valorLDR = analogRead(pinoLDR);

  // Imprime o valor lido no Monitor Serial
  Serial.print("Luminosidade: ");
  Serial.println(valorLDR);

  // Aguarda 500 milissegundos antes da próxima leitura
  delay(500);
}