#include <WiFi.h>

// WiFi credentials
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// Pin configuration
#define RELAY_PIN 26
#define VOLTAGE_SENSOR_PIN 34
#define CURRENT_SENSOR_PIN 35
#define BATTERY_SENSOR_PIN 32
#define SOLAR_SENSOR_PIN 33

bool chargingStatus = false;

float voltage = 0.0;
float current = 0.0;
float batteryVoltage = 0.0;
float solarVoltage = 0.0;
float energyConsumed = 0.0;

unsigned long previousMillis = 0;

void setup() {
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  WiFi.begin(ssid, password);

  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  readSensors();
  safetyCheck();
  calculateEnergy();
  displayData();

  delay(1000);
}

void readSensors() {
  int voltageRaw = analogRead(VOLTAGE_SENSOR_PIN);
  int currentRaw = analogRead(CURRENT_SENSOR_PIN);
  int batteryRaw = analogRead(BATTERY_SENSOR_PIN);
  int solarRaw = analogRead(SOLAR_SENSOR_PIN);

  voltage = (voltageRaw * 3.3 / 4095.0) * 5.0;
  current = ((currentRaw * 3.3 / 4095.0) - 2.5) / 0.185;
  batteryVoltage = (batteryRaw * 3.3 / 4095.0) * 5.0;
  solarVoltage = (solarRaw * 3.3 / 4095.0) * 5.0;
}

void startCharging() {
  chargingStatus = true;
  digitalWrite(RELAY_PIN, HIGH);
  Serial.println("Charging Started");
}

void stopCharging() {
  chargingStatus = false;
  digitalWrite(RELAY_PIN, LOW);
  Serial.println("Charging Stopped");
}

void safetyCheck() {
  if (batteryVoltage < 10.5) {
    stopCharging();
    Serial.println("Alert: Low Battery Voltage");
  }

  if (current > 10.0) {
    stopCharging();
    Serial.println("Alert: Over Current Detected");
  }

  if (solarVoltage < 8.0) {
    Serial.println("Alert: Low Solar Power");
  }
}

void calculateEnergy() {
  unsigned long currentMillis = millis();

  if (chargingStatus && currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;

    float power = voltage * current;
    energyConsumed += power / 3600.0;
  }
}

void displayData() {
  Serial.println("----------- EV Charging Data -----------");
  Serial.print("Charging Status: ");
  Serial.println(chargingStatus ? "ON" : "OFF");

  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.println(" V");

  Serial.print("Current: ");
  Serial.print(current);
  Serial.println(" A");

  Serial.print("Battery Voltage: ");
  Serial.print(batteryVoltage);
  Serial.println(" V");

  Serial.print("Solar Voltage: ");
  Serial.print(solarVoltage);
  Serial.println(" V");

  Serial.print("Energy Consumed: ");
  Serial.print(energyConsumed);
  Serial.println(" Wh");

  Serial.println("----------------------------------------");
}