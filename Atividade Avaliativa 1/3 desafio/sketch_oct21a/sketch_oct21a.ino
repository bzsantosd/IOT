#include <DHT.h>
#include <DHT_U.h>

// =======================================================
// SOLUÇÃO IOT DE CONTROLE DE CLIMATIZAÇÃO EM ESTUFAS/CÂMARAS TÉRMICAS
// Versão 3.0 - Adaptação do Código Não-Bloqueante
// =======================================================

// -----------------------------------------------------
// 1. DEFINIÇÕES DE PINOS E CONSTANTES
// -----------------------------------------------------

// Pinos dos LEDs de Status (Saídas Digitais)
const int LED_VERMELHO = 13; // Liga Aquecedor / Temperatura Abaixo do Ideal
const int LED_AZUL = 12;     // Liga Ventilador / Temperatura Acima do Ideal
const int LED_VERDE = 11;    // Condições Ideais / Conforto
// NOTA: O LED Amarelo (PINO 10) será desativado, pois a lógica de "Fim de Aula" não se aplica.
// Se quiser um alerta visual de falha, poderia reusá-lo.

// Pinos de Controle (Saídas Digitais para Relés/Transistores)
// Estes pinos controlariam o Aquecedor e o Ventilador
const int PINO_AQUECEDOR = 5; 
const int PINO_VENTILADOR = 4;

// Pino do Sensor DHT11 (Entrada Digital)
const int PINO_DHT = 6; 
DHT dht(PINO_DHT, DHT11);

// Limites de Temperatura (em graus Celsius) - IDEAL PARA PRODUÇÃO
const float TEMP_MIN_IDEAL = 22.0; // Ponto de partida para ligar o Aquecedor
const float TEMP_MAX_IDEAL = 28.0; // Ponto de partida para ligar o Ventilador

// Variações de Histerese (opcional, para evitar ligar/desligar rápido)
// Exemplo: desliga o Aquecedor em TEMP_MIN_IDEAL + HISTERESE_AQUEC
const float HISTERESE_AQUECEDOR = 1.0; 
const float HISTERESE_VENTILADOR = 1.0; 

// Variáveis para estabilizar a leitura e output serial (Não-Bloqueante)
const unsigned long INTERVALO_LEITURA_CONTROLE = 2000; // Leitura/Controle a cada 2 segundos
unsigned long previousMillisLeitura = 0;


// -----------------------------------------------------
// 2. SETUP
// -----------------------------------------------------

void setup() {
  // Configura os pinos dos LEDs como SAÍDAS
pinMode(LED_VERMELHO, OUTPUT);
pinMode(LED_AZUL, OUTPUT);
pinMode(LED_VERDE, OUTPUT);
  // pinMode(LED_AMARELO, OUTPUT); // Desativado no novo contexto

  // Configura os pinos de controle (Aquecedor/Ventilador) como SAÍDAS
 pinMode(PINO_AQUECEDOR, OUTPUT);
pinMode(PINO_VENTILADOR, OUTPUT);

  // Inicializa o estado dos LEDs e dos Dispositivos (TUDO DESLIGADO)
digitalWrite(LED_VERMELHO, LOW);
digitalWrite(LED_AZUL, LOW);
digitalWrite(LED_VERDE, LOW);
  // digitalWrite(LED_AMARELO, LOW); // Desativado
digitalWrite(PINO_AQUECEDOR, LOW);
digitalWrite(PINO_VENTILADOR, LOW);

  // Inicia a comunicação serial e o Sensor DHT
 Serial.begin(9600);
Serial.println("=============================================");
 Serial.println("CONTROLE DE CLIMATIZACAO - ESTUFA (V3.0)");
 Serial.print("Zona Ideal: "); Serial.print(TEMP_MIN_IDEAL);
Serial.print("C a "); Serial.print(TEMP_MAX_IDEAL); Serial.println("C");
Serial.println("Histerese: +/- 1.0C para desligar.");
Serial.println("=============================================");
 dht.begin();
}


// -----------------------------------------------------
// 3. FUNÇÃO PARA LER A TEMPERATURA
// -----------------------------------------------------

// Retorna a temperatura lida pelo DHT11/22
float lerTemperatura() {
float temperaturaC = dht.readTemperature();

  // Verifica se a leitura falhou (DHT retorna NaN ou valor muito baixo)
if (isnan(temperaturaC) || temperaturaC < -50.0) {
Serial.println("ERRO: Falha ao ler o sensor DHT!");
 return -999.0; // Retorna um valor que indica falha
}
return temperaturaC;
}


// -----------------------------------------------------
// 4. FUNÇÃO DE CONTROLE DE CLIMATIZAÇÃO
// -----------------------------------------------------

void controlarClimatizacao(float tempAtual) {

  // Se a leitura falhou, desliga tudo por segurança.
 if (tempAtual == -999.0) {
 digitalWrite(PINO_AQUECEDOR, LOW);
 digitalWrite(PINO_VENTILADOR, LOW);
digitalWrite(LED_VERMELHO, HIGH); // Sinaliza erro com LED Vermelho ligado
 digitalWrite(LED_AZUL, LOW);
 digitalWrite(LED_VERDE, LOW);
 return;
}

  // 1. LÓGICA DE CONTROLE DO AQUECEDOR (Temperatura Baixa)
  // Condição para LIGAR o Aquecedor: Temperatura abaixo do limite mínimo ideal.
 if (tempAtual < TEMP_MIN_IDEAL) {
 digitalWrite(PINO_AQUECEDOR, HIGH);  // LIGA Aquecedor
digitalWrite(PINO_VENTILADOR, LOW);   // GARANTE que o Ventilador esteja DESLIGADO

 digitalWrite(LED_VERMELHO, HIGH);    // Acende LED Vermelho (Indicador de Aquecimento)
 digitalWrite(LED_AZUL, LOW);
 digitalWrite(LED_VERDE, LOW);

 Serial.print(" -> ** AQUECENDO ** ");
} 
  // Condição para DESLIGAR o Aquecedor (Com Histerese): Temperatura atingiu o limite mínimo + histerese.
else if (digitalRead(PINO_AQUECEDOR) == HIGH && tempAtual >= (TEMP_MIN_IDEAL + HISTERESE_AQUECEDOR)) {
digitalWrite(PINO_AQUECEDOR, LOW);  // DESLIGA Aquecedor
 Serial.print(" -> Aquecedor Desligado (Histerese). ");
 }


  // 2. LÓGICA DE CONTROLE DO VENTILADOR (Temperatura Alta)
  // Condição para LIGAR o Ventilador: Temperatura acima do limite máximo ideal.
 else if (tempAtual > TEMP_MAX_IDEAL) {
 digitalWrite(PINO_VENTILADOR, HIGH); // LIGA Ventilador
 digitalWrite(PINO_AQUECEDOR, LOW);   // GARANTE que o Aquecedor esteja DESLIGADO

 digitalWrite(LED_VERMELHO, LOW);
 digitalWrite(LED_AZUL, HIGH);       // Acende LED Azul (Indicador de Resfriamento)
 digitalWrite(LED_VERDE, LOW);

 Serial.print(" -> ** RESFRIANDO ** ");
 }
  // Condição para DESLIGAR o Ventilador (Com Histerese): Temperatura atingiu o limite máximo - histerese.
 else if (digitalRead(PINO_VENTILADOR) == HIGH && tempAtual <= (TEMP_MAX_IDEAL - HISTERESE_VENTILADOR)) {
 digitalWrite(PINO_VENTILADOR, LOW);  // DESLIGA Ventilador
 Serial.print(" -> Ventilador Desligado (Histerese). ");
 }


  // 3. LÓGICA DE STATUS IDEAL (Quando nada está ligando/desligando)
  // Se Aquecedor e Ventilador estão DESLIGADOS, e a temperatura está na faixa ideal.
else if (digitalRead(PINO_AQUECEDOR) == LOW && digitalRead(PINO_VENTILADOR) == LOW &&
tempAtual >= TEMP_MIN_IDEAL && tempAtual <= TEMP_MAX_IDEAL) {
 
 digitalWrite(LED_VERMELHO, LOW);
 digitalWrite(LED_AZUL, LOW);
 digitalWrite(LED_VERDE, HIGH);      // Acende LED Verde (Condição Ideal)

 Serial.print(" -> IDEAL "); 
 }

  // Se a temperatura ainda não atingiu o ponto de histerese
else {
    // Mantém o estado atual dos LEDs de Status (pode ser o Verde se estiver estável, ou o
    // Vermelho/Azul se estiver em processo de retorno à zona ideal).
    // O mais importante é que os dispositivos de controle já têm seu estado definido.

    // No caso de histerese, o LED de status pode ficar 'no meio', o que é aceitável,
    // ou você pode mantê-lo indicando a zona anterior até que atinja o IDEAL.
 
    // Simplificando, se não está ativamente ligando/desligando E não é ideal:
    // O estado anterior dos LEDs de status é mantido para indicar o que está *próximo* de acontecer.
Serial.print(" -> Em Transição/Estável ");
 }

}


// -----------------------------------------------------
// 5. FUNÇÃO PRINCIPAL (LOOP)
// -----------------------------------------------------

void loop() { unsigned long currentMillis = millis();

  // Executa a leitpreviousMillisLeitura >= INTERVALO_LEITURA_CONTROLE) {
previousMillisLeitura = currentMillis; // Salva o tempo da última execução

float tempAtual = lerTemperatura();

    // Realiza a lógica de controle
 controlarClimatizacao(tempAtual);

    // Output Serial
 Serial.print("[");
 Serial.print(currentMillis / 1000); // Tempo em segundos
Serial.print("s] Temp: ");
Serial.print(tempAtual, 1); // Exibe com 1 casa decimal
 Serial.print(" C. ");
 Serial.print("Aquecedor: "); Serial.print(digitalRead(PINO_AQUECEDOR) ? "LIGADO" : "DESLIGADO");
 Serial.print(", Ventilador: "); Serial.print(digitalRead(PINO_VENTILADOR) ? "LIGADO" : "DESLIGADO");
 Serial.println();
}

  // O loop() continua a executar outras tarefas rapidamente, mas neste caso,
  // apenas a seção de tempo (if) é executada a cada 2 segundos.
}