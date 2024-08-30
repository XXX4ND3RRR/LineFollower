//FINAL CODE

#include <QTRSensors.h>
QTRSensors qtr;
const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];
float Kp = .2;
float Ki = 0;
float Kd = .1;
int P;
int I;
int D;
int lastError = 0;
boolean onoff = false;

const uint8_t maxspeeda = 250;
const uint8_t maxspeedb = 250;
const uint8_t basespeeda = 175;
const uint8_t basespeedb = 175;

/*motor PHASE/ENABLE
  int mode = 8;
  int aphase = 9;
  int aenbl = 6;
  int bphase = 5;
  int benbl = 3;
*/
int motor1In1 = 22;
int motor1In2 = 23;
int motor2In1 = 18;
int motor2In2 = 19;
//int EEP = 21 //SleepMode


int buttonCalibrate = 4;
int buttonStateCalibrate;
int buttonStart = 5;
int buttonStateStart;
int lastStateCalibrate = LOW;
int lastStateStart = LOW;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup() {
  Serial.begin(9600);
  qtr.setTypeAnalog();

  qtr.setSensorPins((const uint8_t[]) {
    32, 33, 25, 26, 27, 14, 15, 13
  }, SensorCount);
  qtr.setEmitterPin(17);//LEDON PIN
  Serial.println("QTR DECLARATION DONE");

  pinMode(motor1In1, OUTPUT);
  pinMode(motor1In2, OUTPUT);
  pinMode(motor2In1, OUTPUT);
  pinMode(motor2In2, OUTPUT);
  Serial.println("MOTOR DECLARATION DONE");

  pinMode(buttonCalibrate, INPUT);
  pinMode(buttonStart, INPUT);

  delay(500);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.print("STEP 1 CALIBRATE");
  boolean Ok = false;
  while (Ok == false) {
    int readingCalibrate = digitalRead(buttonCalibrate);
    if (readingCalibrate != lastStateCalibrate) {
      lastDebounceTime = millis();
    }
    if ((millis() - lastDebounceTime) > debounceDelay) {
      if (readingCalibrate != buttonStateCalibrate) {
        buttonStateCalibrate = readingCalibrate;
        if (buttonStateCalibrate == HIGH) {
          Serial.print("buttoncalibrate triggered");
          digitalWrite(LED_BUILTIN, HIGH);
          calibration();
          Ok = true;
          digitalWrite(LED_BUILTIN, LOW);
        }
      }
    } lastStateCalibrate = readingCalibrate;
  }
  forward_brake(0, 0);
}

void calibration() {
  digitalWrite(4, HIGH);
  for (uint16_t i = 0; i < 400; i++)
  {
    qtr.calibrate();
  }
  digitalWrite(4, LOW);
}

void loop() {
  int readingStart = digitalRead(buttonStart);
  if (readingStart != lastStateStart) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (readingStart != buttonStateStart) {
      buttonStateStart = readingStart;
      if (buttonStateStart == HIGH) {
        Serial.print("Buttonstart triggered");
        onoff = ! onoff;
        if (onoff = true) {
          delay(1000);
        }
        else {
          delay(50);
        }
      }
    }
  } lastStateStart = readingStart;
  if (onoff == true) {
    PID_control();
  }
  else {
    forward_brake(0, 0);
  }
}
void forward_brake(int posa, int posb) {
  analogWrite(motor1In1, 0);
  analogWrite(motor1In2, posa);
  analogWrite(motor2In1, 0);
  analogWrite(motor2In2, posb);

}
void PID_control() {
  uint16_t position = qtr.readLineBlack(sensorValues);
  int error = 3500 - position;

  P = error;
  I = I + error;
  D = error - lastError;
  lastError = error;
  int motorspeed = P * Kp + I * Ki + D * Kd;

  int motorspeeda = basespeeda + motorspeed;
  int motorspeedb = basespeedb - motorspeed;

  if (motorspeeda > maxspeeda) {
    motorspeeda = maxspeeda;
  }
  if (motorspeedb > maxspeedb) {
    motorspeedb = maxspeedb;
  }
  if (motorspeeda < 0) {
    motorspeeda = 0;
  }
  if (motorspeedb < 0) {
    motorspeedb = 0;
  }
  forward_brake(motorspeeda, motorspeedb);
}
