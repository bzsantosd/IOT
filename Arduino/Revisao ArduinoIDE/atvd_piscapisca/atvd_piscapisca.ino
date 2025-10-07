int led_blink = 10;
int led2 = 9;
int led3 = 8;

void setup() {
  pinMode(led_blink, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
}

void loop() {
  digitalWrite(led_blink, HIGH);
  delay(1000);
  digitalWrite(led_blink, LOW);
  delay(1000);
  digitalWrite(led2, HIGH);
  delay(2000);
  digitalWrite(led2, LOW);
  delay(2000);
  digitalWrite(led3, HIGH);
  delay(3000);
  digitalWrite(led3, LOW);
  delay(3000);

}