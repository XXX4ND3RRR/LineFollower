//POC SENSORS

#include <QTRSensors.h>
QTRSensors qtr;
const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

int buttonCalibrate = 4;
int buttonStateCalibrate;
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

  pinMode(LED_BUILTIN, OUTPUT);
  Serial.print("STEP 1 CALIBRATE");
  digitalWrite(LED_BUILTIN, HIGH);
  calibration();
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
uint16_t position = qtr.readLineBlack(sensorValues);
  for(int i = 0; i < SensorCount; i++){
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }Serial.println();
  delay(500);
}

void calibration() {
  digitalWrite(4, HIGH);
  for (uint16_t i = 0; i < 400; i++)
  {
    qtr.calibrate();
  }
  digitalWrite(4, LOW);
}
