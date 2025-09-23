#define led_blink 10
#define led_black 9 
void setup() {
  pinMode(led_blink, OUTPUT);
  pinMode(led_black, OUTPUT);
}
void loop() {
  digitalWrite(led_blink, HIGH);
  delay(1000);
  digitalWrite(led_blink, LOW);
  delay(1000);
  digitalWrite(led_black, HIGH);
  delay(2000);
  digitalWrite(led_black, LOW);
  delay(2000);

}