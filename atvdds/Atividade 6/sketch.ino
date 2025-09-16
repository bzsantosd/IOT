bool ledState = false;       
bool lastButtonState = false; 

void setup() {
  pinMode(13, OUTPUT);     
  pinMode(2, INPUT);   
}

void loop() {
  bool currentButtonState = digitalRead(2);

  if (currentButtonState == HIGH && lastButtonState == LOW) {
    ledState = !ledState;
    
    if (ledState) {
      digitalWrite(13, HIGH); 
    } else {
      digitalWrite(13, LOW);  
    }
    
    delay(50);
  }
  lastButtonState = currentButtonState;
}