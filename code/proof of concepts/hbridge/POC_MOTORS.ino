int motor1In1 = 22;
int motor1In2 = 23;
int motor2In1 = 18;
int motor2In2 = 19;

void setup() {
  pinMode(motor1In1, OUTPUT);
  pinMode(motor1In2, OUTPUT);
  pinMode(motor2In1, OUTPUT);
  pinMode(motor2In2, OUTPUT);
  Serial.println("MOTOR DECLARATION DONE");
}

void loop() {
  analogWrite(motor1In1, 0);
  analogWrite(motor1In2, 100);
  analogWrite(motor2In1, 0);
  analogWrite(motor2In2, 100);
  delay(2000);
  analogWrite(motor1In1, 0);
  analogWrite(motor1In2, 0);
  analogWrite(motor2In1, 0);
  analogWrite(motor2In2, 0);
  delay(1000);
  analogWrite(motor1In1, 100);
  analogWrite(motor1In2, 0);
  analogWrite(motor2In1, 100);
  analogWrite(motor2In2, 0);
  delay(2000);
  analogWrite(motor1In1, 0);
  analogWrite(motor1In2, 0);
  analogWrite(motor2In1, 0);
  analogWrite(motor2In2, 0);
  delay(1000);
}
