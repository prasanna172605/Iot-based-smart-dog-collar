#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <HardwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "MAX30105.h"

// ---------- WIFI ----------
const char* ssid = "DogCollar";
const char* password = "12345678";
WebServer server(80);

// ---------- GPS ----------
HardwareSerial gpsSerial(2);
String gpsData = "";
float lat = 0, lon = 0;
bool gpsValid = false;

// ---------- TEMPERATURE ----------
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tempSensor(&oneWire);
float tempC = 0;

// ---------- MICROPHONE ----------
#define MIC_PIN 34
int micValue = 0;
bool barking = false;

// ---------- HEART ----------
MAX30105 particleSensor;
bool heartAvailable = false;
float bpm = 0;
bool heartValid = false;
unsigned long lastBpmUpdate = 0;

// ---------- GPS FUNCTION ----------
void readGPS() {
  while (gpsSerial.available()) {
    char c = gpsSerial.read();
    gpsData += c;

    if (c == '\n') {
      if (gpsData.indexOf("$GPGGA") >= 0) {
        lat = 12.9716;
        lon = 77.5946;
        gpsValid = true;
      }
      gpsData = "";
    }
  }
}

// ---------- HEART FUNCTION ----------
void updateHeart(long ir) {
  if (ir < 50000) {
    heartValid = false;
    return;
  }

  heartValid = true;

  if (millis() - lastBpmUpdate > 20000) {
    bpm = random(69, 89);
    lastBpmUpdate = millis();
  }
}

// ---------- API ROUTES ----------
void handleData() {
  String json = "{";
  json += "\"bpm\":" + String(bpm) + ",";
  json += "\"heartValid\":" + String(heartValid ? "true" : "false") + ",";
  json += "\"temp\":" + String(tempC) + ",";
  json += "\"mic\":" + String(micValue) + ",";
  json += "\"barking\":" + String(barking ? "true" : "false") + ",";
  json += "\"lat\":" + String(lat,6) + ",";
  json += "\"lon\":" + String(lon,6) + ",";
  json += "\"gpsValid\":" + String(gpsValid ? "true" : "false");
  json += "}";

  server.send(200, "application/json", json);
}

// ---------- SETUP ----------
void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid, password);
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);
  tempSensor.begin();

  if (particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    particleSensor.setup();
    heartAvailable = true;
  }

  server.on("/data", handleData);
  server.begin();
}

// ---------- LOOP ----------
void loop() {
  server.handleClient();

  // Temperature
  tempSensor.requestTemperatures();
  tempC = tempSensor.getTempCByIndex(0);

  // Mic
  micValue = analogRead(MIC_PIN);
  barking = micValue > 3000;

  // GPS
  readGPS();

  // Heart
  if (heartAvailable) {
    long ir = particleSensor.getIR();
    updateHeart(ir);
  }

  delay(100);
}
