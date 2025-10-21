// Inclui as bibliotecas necessárias
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Endereço I2C do seu módulo LCD (o mais comum é 0x27 ou 0x3F)
// Se não funcionar, você pode precisar executar um código "I2C Scanner" para descobrir o endereço correto.
LiquidCrystal_I2C lcd(0x27, 16, 2); // (Endereço, Colunas, Linhas)

// Definição dos pinos
const int PINO_SENSOR = A0;      // Pino para o sensor de porta/janela (Reed Switch)
const int PINO_LED_VERDE = 6;    // Pino para o LED Verde (Status: Fechado)
const int PINO_LED_VERMELHO = 5; // Pino para o LED Vermelho (Status: Aberto)

void setup() {
  // Inicialização do LCD
  lcd.init();
  lcd.backlight();
  
  // Mensagem de inicialização no LCD
  lcd.setCursor(0, 0);
  lcd.print("Monitor de Status");
  lcd.setCursor(0, 1);
  lcd.print("Inicializando...");
  
  // Configura os pinos dos LEDs como SAÍDA
  pinMode(PINO_LED_VERDE, OUTPUT);
  pinMode(PINO_LED_VERMELHO, OUTPUT);
  
  // Configura o pino do sensor como ENTRADA
  // Usaremos INPUT_PULLUP para simplificar a fiação:
  // - O pino estará HIGH por padrão.
  // - Quando o sensor (Reed Switch) fechar o circuito para o GND (porta fechada), o pino vai para LOW.
  pinMode(PINO_SENSOR, INPUT_PULLUP); 

  // Se você usou uma configuração com resistor de PULL-DOWN, use:
  // pinMode(PINO_SENSOR, INPUT); 
  // E inverta a lógica (HIGH = FECHADO, LOW = ABERTO)
  
  delay(2000); // Espera para a mensagem de inicialização
  lcd.clear();
}

void loop() {
  // Lê o estado do sensor
  int statusSensor = digitalRead(PINO_SENSOR);

  // Variável para a lógica PULL-UP: LOW = FECHADO, HIGH = ABERTO
  bool portaAberta = (statusSensor == HIGH); 
  
  // Se o pino está em HIGH (circuito aberto = porta aberta)
  if (portaAberta) { 
    // Porta/Janela ABERTA
    
    // LEDs
    digitalWrite(PINO_LED_VERMELHO, HIGH); // Liga LED Vermelho
    digitalWrite(PINO_LED_VERDE, LOW);      // Apaga LED Verde
    
    // LCD
    lcd.setCursor(0, 0);
    lcd.print("Porta/Janela:");
    lcd.setCursor(0, 1);
    lcd.print("    ABERTA      "); // Espaços para limpar a linha

  } else { 
    // Porta/Janela FECHADA (o pino está em LOW devido ao PULL-UP + Reed Switch fechado)

    // LEDs
    digitalWrite(PINO_LED_VERMELHO, LOW);  // Apaga LED Vermelho
    digitalWrite(PINO_LED_VERDE, HIGH);     // Liga LED Verde
    
    // LCD
    lcd.setCursor(0, 0);
    lcd.print("Porta/Janela:");
    lcd.setCursor(0, 1);
    lcd.print("    FECHADA     "); // Espaços para limpar a linha
  }
  
  delay(500); // Pequeno atraso para não sobrecarregar
}
