#include <DHT.h>
#include <DHT_U.h>

// =======================================================
// SOLUÇÃO IOT DE MONITORAMENTO TÉRMICO
// Versão 3.0 - Controle Manual do Ventilador por Botão
// =======================================================

// -----------------------------------------------------
// 1. DEFINIÇÕES DE PINOS E CONSTANTES
// -----------------------------------------------------

// Pinos dos LEDs (Saídas Digitais)
const int LED_VERMELHO = 13; 
const int LED_AZUL = 12;
const int LED_VERDE = 11;
const int LED_AMARELO = 10;

// Pinos dos Atuadores (NOVAS SAÍDAS DIGITAIS)
const int PINO_VENTILADOR = 9; // Aciona o Ventilador (Cooling)
// Não adicionamos o aquecedor, pois a lógica se concentra apenas no resfriamento manual.

// Pino do Botão (NOVA ENTRADA DIGITAL)
const int PINO_BOTAO = 7; // Sugestão: Pino 7

// O sensor LM35 não será mais usado, apenas o DHT (no pino 6).
// const int PINO_LM35 = A0; 

// Limites de Temperatura (em graus Celsius)
const float TEMP_MIN_IDEAL = 21.0; 
const float TEMP_MAX_IDEAL = 25.0;

// ------------------------------------------------------------------
// VARIÁVEIS DE ESTADO
// ------------------------------------------------------------------

// NOVO: Variável para armazenar o estado do modo manual
bool modoVentiladorManualAtivo = false;
int estadoAnteriorBotao = LOW; // Para detecção de borda (pressionamento)

// Variáveis para estabilizar a leitura e output serial (Não-Bloqueante)
const unsigned long INTERVALO_LEITURA = 1000; // Leitura/Serial a cada 1 segundo
unsigned long previousMillisLeitura = 0;
DHT dht(6,DHT11); // Sensor DHT no Pino 6

// As variáveis de 'Fim de Aula' foram removidas.

// -----------------------------------------------------
// 2. SETUP
// -----------------------------------------------------

void setup() {
  // Configura os pinos dos LEDs e Atuadores como SAÍDAS
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(PINO_VENTILADOR, OUTPUT); // Novo atuador

  // Configura o pino do botão como ENTRADA
  // Usamos INPUT_PULLUP para simplificar a fiação (botão para GND)
  pinMode(PINO_BOTAO, INPUT_PULLUP); 

  // Inicializa o estado dos LEDs e Atuadores
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_AZUL, LOW);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AMARELO, LOW);
  digitalWrite(PINO_VENTILADOR, LOW); // Garante que o ventilador esteja desligado

  // Inicia a comunicação serial
  Serial.begin(9600);
  Serial.println("=============================================");
  Serial.println("Monitoramento Termico - Iniciado (V3.0 - Controle Manual)");
  Serial.print("Zona Ideal: "); Serial.print(TEMP_MIN_IDEAL);
  Serial.print("C a "); Serial.print(TEMP_MAX_IDEAL); Serial.println("C");
  Serial.println("=============================================");
  dht.begin();
}


// -----------------------------------------------------
// 3. FUNÇÃO PARA LER A TEMPERATURA
// -----------------------------------------------------

float lerTemperatura() {
  float temperaturaC = dht.readTemperature();
  if (isnan(temperaturaC)) {
    Serial.println("Falha na leitura do sensor DHT!");
    return -999.0; // Valor de erro
  }
  return temperaturaC;
}


// -----------------------------------------------------
// 4. FUNÇÃO DE CONTROLE DO BOTÃO (NOVA)
// -----------------------------------------------------

void gerenciarBotao() {
  // Leitura do pino do botão (PULLUP: LOW = Pressionado, HIGH = Solto)
  int estadoAtualBotao = digitalRead(PINO_BOTAO);

  // Detecção de Borda de Descida (Botão Pressionado)
  if (estadoAtualBotao != estadoAnteriorBotao) {
    if (estadoAtualBotao == LOW) { // O botão foi acionado (pressionado)
      
      // Alterna o estado do modo manual
      modoVentiladorManualAtivo = !modoVentiladorManualAtivo;
      
      // Feedback Serial
      Serial.println("\n--- BOTAO ACIONADO ---");
      if (modoVentiladorManualAtivo) {
        Serial.println("MODO VENTILADOR MANUAL ATIVADO. (LED AMARELO)");
      } else {
        Serial.println("MODO VENTILADOR MANUAL DESATIVADO.");
      }
      Serial.println("-----------------------\n");
    }
  }

  // Salva o estado atual para a próxima iteração
  estadoAnteriorBotao = estadoAtualBotao;
}


// -----------------------------------------------------
// 5. FUNÇÃO PRINCIPAL (LOOP)
// -----------------------------------------------------

void loop() {
  unsigned long currentMillis = millis();
  
  // A) GERENCIAMENTO DO BOTÃO (Sempre rodando)
  gerenciarBotao();

  // B) LEITURA DO SENSOR E CONTROLE DOS ATUADORES/LEDS (A cada INTERVALO_LEITURA)
  if (currentMillis - previousMillisLeitura >= INTERVALO_LEITURA) {
    previousMillisLeitura = currentMillis; 

    float tempAtual = lerTemperatura();
    
    // Garantir que todos os LEDs de status estejam desligados antes de acender o correto
    digitalWrite(LED_VERMELHO, LOW);
    digitalWrite(LED_AZUL, LOW);
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_AMARELO, LOW); // Desliga o Amarelo (será ligado se o modo manual estiver ativo)

    // --- LÓGICA DE CONTROLE ---
    
    if (tempAtual == -999.0) {
      // Erro de Leitura: Desliga tudo e não faz nada
      digitalWrite(PINO_VENTILADOR, LOW);
    }
    
    // 1. MODO MANUAL ATIVO
    else if (modoVentiladorManualAtivo) {
      // Força o LED Amarelo e o Ventilador a LIGAR
      digitalWrite(LED_AMARELO, HIGH); 
      digitalWrite(PINO_VENTILADOR, HIGH); 
    }
    
    // 2. MODO AUTOMÁTICO (Manual Desativado)
    else {
      // Desliga o ventilador, para que a lógica de status o ligue se necessário
      digitalWrite(PINO_VENTILADOR, LOW); 

      if (tempAtual < TEMP_MIN_IDEAL) {
        digitalWrite(LED_AZUL, HIGH);      // Muito Frio
        digitalWrite(PINO_VENTILADOR, LOW);  // Ventilador desligado no frio
      } 
      
      else if (tempAtual > TEMP_MAX_IDEAL) {
        digitalWrite(LED_VERMELHO, HIGH);    // Muito Quente
        
        // NOVO: Lógica de Resfriamento Automático
        // O sistema pode ligar o ventilador automaticamente quando quente
        // Se desejar que o ventilador SÓ ligue no manual, comente a linha abaixo.
        // digitalWrite(PINO_VENTILADOR, HIGH); 
        
      } else {
        digitalWrite(LED_VERDE, HIGH);       // Ideal
        digitalWrite(PINO_VENTILADOR, LOW);  // Ventilador desligado no ideal
      }
    }


    // --- OUTPUT SERIAL ---
    Serial.print("[");
    Serial.print(currentMillis / 1000); 
    Serial.print("s] Temp: ");
    Serial.print(tempAtual, 2); 
    Serial.print(" C. Status: ");

    if (modoVentiladorManualAtivo) {
      Serial.println("MODO MANUAL (LED AMARELO LIGADO). VENTILADOR: LIGADO.");
    } else if (tempAtual == -999.0) {
      Serial.println("ERRO DE LEITURA");
    } else if (tempAtual < TEMP_MIN_IDEAL) {
      Serial.println("FRIO (LED AZUL). VENTILADOR: DESLIGADO.");
    } else if (tempAtual > TEMP_MAX_IDEAL) {
      Serial.print("QUENTE (LED VERMELHO). VENTILADOR: "); Serial.println(digitalRead(PINO_VENTILADOR) == HIGH ? "LIGADO (AUTO)" : "DESLIGADO");
    } else {
      Serial.println("IDEAL (LED VERDE). VENTILADOR: DESLIGADO.");
    }
  }
}