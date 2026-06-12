#define BLYNK_TEMPLATE_ID "TMPL3sAMxb1cd"
#define BLYNK_TEMPLATE_NAME "SCB with IoT"
#define BLYNK_AUTH_TOKEN "4xzLYshT19A5MOcEtEzhEnoN_E2z-Hw8"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// ===== WIFI =====
char ssid[] = "ESP32";
char pass[] = "1234567890";

// ===== PINS =====
#define VOLTAGE_PIN 34
#define CURRENT_PIN 35
#define RELAY_PIN 32
#define BUTTON_PIN 14 

// ===== VARIABLES =====
float voltage = 0;
float current = 0;

bool faultActive = false;
bool relayState = true;

// ===== BUTTON =====
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 200;

// ===== THRESHOLDS =====
float overVoltage = 243.0;
float underVoltage = 150.0;
float overCurrent = 6.0;

// ===== OFFSET =====
float offset = 1.65;

// ===== TIMER =====
BlynkTimer timer;

// ===== EVENT CONTROL =====
String lastEvent = "";
bool sendEventFlag = false;
String eventName = "";

// ===== MOBILE CONTROL =====
BLYNK_WRITE(V4) {
  int value = param.asInt();

  if (!faultActive) {
    relayState = value;
    digitalWrite(RELAY_PIN, relayState ? LOW : HIGH);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // ===== OFFSET CALIBRATION =====
  float sum = 0;
  for (int i = 0; i < 200; i++) {
    sum += analogRead(CURRENT_PIN);
    delay(5);
  }
  offset = (sum / 200 / 4095.0) * 3.3;

  Serial.print("Offset: ");
  Serial.println(offset);

  timer.setInterval(1000L, sendData);
}

void loop() {
  Blynk.run();
  timer.run();
}

// ===== MAIN FUNCTION =====
void sendData() {

  // ===== BUTTON =====
  bool reading = digitalRead(BUTTON_PIN);

  if (reading == LOW && lastButtonState == HIGH &&
      (millis() - lastDebounceTime) > debounceDelay) {

    if (!faultActive) {
      relayState = !relayState;
    }

    lastDebounceTime = millis();
  }

  lastButtonState = reading;

  // ===== SENSOR SAMPLING =====
  int samples = 100;
  float vSum = 0, cSum = 0;

  for (int i = 0; i < samples; i++) {
    vSum += analogRead(VOLTAGE_PIN);
    cSum += analogRead(CURRENT_PIN);
    delayMicroseconds(200);
  }

  float vADC = (vSum / samples / 4095.0) * 3.3;
  float cADC = (cSum / samples / 4095.0) * 3.3;

  voltage = vADC * 150;

  // ===== CURRENT FIX (IMPORTANT) =====
  float diff = cADC - offset;

  if (voltage < 50) {
    current = 0;   // ⭐ FIX: No fake current when OFF
  }
  else if (abs(diff) < 0.03) {
    current = 0;
  }
  else {
    current = abs(diff * 2);
  }

  String status = "NORMAL";
  faultActive = false;
  sendEventFlag = false;

  // ===== FAULT LOGIC =====
  if (voltage > overVoltage) {
    status = "OVER VOLTAGE";
    faultActive = true;

    if (lastEvent != "OV") {
      sendEventFlag = true;
      eventName = "over_voltage";
      lastEvent = "OV";
    }
  }
  else if (voltage < underVoltage) {
    status = "UNDER VOLTAGE";
    faultActive = true;

    if (lastEvent != "UV") {
      sendEventFlag = true;
      eventName = "under_voltage";
      lastEvent = "UV";
    }
  }
  else if (current > overCurrent) {
    status = "OVER CURRENT";
    faultActive = true;

    if (lastEvent != "OC") {
      sendEventFlag = true;
      eventName = "over_current";
      lastEvent = "OC";
    }
  }
  else {
    lastEvent = "";
  }

  // ===== RELAY CONTROL =====
  if (faultActive) {
    digitalWrite(RELAY_PIN, HIGH);
    relayState = false;
  } else {
    digitalWrite(RELAY_PIN, relayState ? LOW : HIGH);
  }

  // ===== SAFE EVENT TRIGGER =====
  if (sendEventFlag) {
    delay(50);  // ⭐ STABILITY DELAY
    Blynk.logEvent(eventName.c_str());
    sendEventFlag = false;
  }

  // ===== BLYNK UPDATE =====
  Blynk.virtualWrite(V1, voltage);
  Blynk.virtualWrite(V2, current);
  Blynk.virtualWrite(V3, status);
  Blynk.virtualWrite(V4, relayState);

  // ===== SERIAL =====
  Serial.println("-----------");
  Serial.print("Voltage: "); Serial.println(voltage);
  Serial.print("Current: "); Serial.println(current);
  Serial.print("Relay: "); Serial.println(relayState ? "ON" : "OFF");
  Serial.print("Status: "); Serial.println(status);
}
