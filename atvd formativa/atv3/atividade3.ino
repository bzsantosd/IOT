// Programa: Controlando um servo motor com potenciômetro
 
#include <Servo.h> // Inclui a biblioteca Servo para controlar servos
 
Servo meuServo; // Cria um objeto Servo para controlar o servo motor
int pos; // Variável para armazenar a posição do servo motor
int potPin = A0; // Pino analógico onde o potenciômetro está conectado
int val; // Variável para armazenar o valor lido do potenciômetro
 
void setup() {
    meuServo.attach(6); // Associa o servo motor ao pino digital 6 do Arduino
	meuServo.write(0); // Define a posição inicial do servo motor para 0 graus 
}
 
void loop() {
    val = analogRead(potPin); // Lê o valor do potenciômetro (0 a 1023)
    pos = map(val, 0, 1023, 0, 180); // Mapeia o valor do potenciômetro para a faixa de 0 a 180 graus
    meuServo.write(pos); // Define a posição do servo motor
    delay(15); // Aguarda um curto intervalo para estabilizar o movimento do servo
}

