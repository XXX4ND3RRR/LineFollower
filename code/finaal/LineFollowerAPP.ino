#define REMOTEXY_MODE__ESP32CORE_BLE

#include <BLEDevice.h>

// RemoteXY connection settings
#define REMOTEXY_BLUETOOTH_NAME "RemoteXY"


#include <RemoteXY.h>

// RemoteXY GUI configuration
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 101 bytes
{ 255, 13, 0, 0, 0, 94, 0, 18, 0, 0, 0, 31, 1, 106, 200, 1, 1, 7, 0, 129,
  7, 3, 15, 12, 64, 17, 75, 112, 0, 129, 8, 31, 11, 12, 64, 17, 75, 105, 0, 129,
  8, 59, 15, 12, 64, 17, 75, 100, 0, 7, 7, 16, 40, 10, 108, 64, 2, 26, 2, 2,
  7, 8, 43, 40, 10, 108, 64, 2, 26, 2, 2, 7, 8, 71, 40, 10, 108, 64, 2, 26,
  2, 2, 1, 16, 121, 75, 75, 0, 2, 31, 115, 116, 97, 114, 116, 45, 115, 116, 111, 112,
  0
};

struct {
  // input variables
  float ValueProp;
  float ValueInt;
  float ValueDif;
  uint8_t StartStop; // =1 if button pressed, else =0

  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;
#pragma pack(pop)


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
const uint8_t basespeeda = 150;
const uint8_t basespeedb = 155;

int motor1In1 = 22;
int motor1In2 = 23;
int motor2In1 = 18;
int motor2In2 = 19;

int buttonCalibrate = 4;
int buttonStateCalibrate;
int buttonStart = 5;
int buttonStateStartButton;
int buttonStateStartApp;
int lastStateCalibrate = LOW;
int lastStateStartButton = LOW;
int lastStateStartApp = LOW;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

boolean Ok = false;

void setup() {
  Serial.begin(9600);
  RemoteXY_Init ();
  RemoteXY.ValueProp = Kp;
  RemoteXY.ValueInt = Ki;
  RemoteXY.ValueDif = Kd;
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

  pinMode(LED_BUILTIN, OUTPUT);
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
  RemoteXY_Handler ();
  if (Ok == false) {
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
  } else {
    Kp = RemoteXY.ValueProp;
    Ki = RemoteXY.ValueInt;
    Kd = RemoteXY.ValueDif;
    int readingStart = digitalRead(buttonStart);
    debounce(RemoteXY.StartStop, lastStateStartApp, buttonStateStartApp);
    debounce(readingStart, lastStateStartButton, buttonStateStartButton);
    if (onoff == 1) {
      PID_control();
    }
    else if (onoff == 0) {
      forward_brake(0, 0);
    }
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

void debounce (int reading, int lastStateStart, int buttonStateStart) {
  if (reading != lastStateStart) {
    lastDebounceTime = millis();
    Serial.println("debounce started");
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonStateStart) {
      buttonStateStart = reading;
      if (buttonStateStart == HIGH) {
        Serial.print("Buttonstart triggered");
        onoff = ! onoff;
        if (onoff == 1) {
          RemoteXY_delay(1000);
        }
        else if (onoff == 0) {
          RemoteXY_delay(100);
        }
      }
    }
  } lastStateStart = reading;
}
