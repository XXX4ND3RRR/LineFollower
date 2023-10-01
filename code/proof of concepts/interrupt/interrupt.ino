#define button 6
#define led 7
int buttonState = 0;
int runStop = 0;
int lastButtonState = LOW;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 25;

void setup() {
  pinMode(button, INPUT);
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int reading = digitalRead(button);
  Serial.println("reading");
  if (reading != lastButtonState){
    lastDebounceTime = millis();
    Serial.println("button pressed");
  }
  if((millis()- lastDebounceTime) > debounceDelay){
    if(reading != buttonState){
      buttonState = reading;
      Serial.println("buttonstate changed");
        if(buttonState == HIGH){
        runStop = !runStop;
        Serial.println(runStop);
      }
    }
  }
  if(runStop == HIGH){
    digitalWrite(led, HIGH);
  } else if(runStop == LOW){
    digitalWrite(led, LOW);
  }
  lastButtonState = reading;
}
