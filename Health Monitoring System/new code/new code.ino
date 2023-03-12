#define BLYNK_TEMPLATE_ID "TMPLxcSXxwzc"
#define BLYNK_TEMPLATE_NAME "Health Monitoring System"
#define BLYNK_AUTH_TOKEN "w27qvxYvrluGTvY07kuZ0C5GKABWZrbC"

#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#define BLYNK_PRINT Serial
#include <Blynk.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "DHT.h"

#define DHTPIN 27 //Connect Out pin to D2 in NODE MCU
#define DHTTYPE DHT11  
#define REPORTING_PERIOD_MS 1000
#define DS18B20 5

char auth[] = BLYNK_AUTH_TOKEN;          // You should get Auth Token in the Blynk App.
char ssid[] = "ForOctaPi";                                     // Your WiFi credentials.
char pass[] = "^^0nlyR4allow3d()";
PulseOximeter pox;

DHT dht(DHTPIN, DHTTYPE);
float Temperature;
float Humidity;
float bodytemperature;
float BPM, SpO2;
uint32_t tsLastReport = 0;

OneWire oneWire(DS18B20);
DallasTemperature sensors(&oneWire);

void onBeatDetected()
{
  Serial.println("Beat Detected!");
}

void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  //timer.setInterval(100L, sendSensor);
  pinMode(19, OUTPUT);
  Serial.print("Initializing Pulse Oximeter..");

  Serial.print("Initializing pulse oximeter..");
  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;);
  } else {
    Serial.println("SUCCESS");
    pox.setOnBeatDetectedCallback(onBeatDetected);
  }
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
  // Register a callback for the beat detection
}
void loop()
{
  pox.update();
  //timer.run();
  Blynk.run();
  BPM = pox.getHeartRate();
  SpO2 = pox.getSpO2();
  bodytemperature = sensors.getTempCByIndex(0);

  if (millis() - tsLastReport > REPORTING_PERIOD_MS)
  {
    Serial.print("Heart rate:");
    Serial.print(BPM);
    Serial.print(" bpm / SpO2:");
    Serial.print(SpO2);
    Serial.println(" %");
    Serial.println("%");
    //Serial.print("Body Temperature: ");
    //Serial.print(bodytemperature);
    Serial.println("°C");
    Blynk.virtualWrite(V1, BPM);
    Blynk.virtualWrite(V2, SpO2);
    Blynk.virtualWrite(V0, bodytemperature);
    tsLastReport = millis();
  }
}



void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
    Blynk.virtualWrite(V6, t);
    Blynk.virtualWrite(V7, h);
    Serial.print("Temperature : ");
    Serial.print(t);
    Serial.print("    Humidity : ");
    Serial.println(h);
}