#define BLYNK_TEMPLATE_ID "TMPLleG4TX6w"
#define BLYNK_DEVICE_NAME "REALTIME WEATHER MONITORING USING IoT"
#define BLYNK_AUTH_TOKEN "atM7q-6dH-i1v3YfyRlVCtoBFRyeM2Wj"
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "Billa";  // type your wifi name
char pass[] = "9066732316";  // type your wifi password

BlynkTimer timer;
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

Adafruit_BMP280 bmp; // I2C
//Adafruit_BMP280 bmp(BMP_CS); // hardware SPI
//Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);

void setup() {
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(100L, sendSensor);
  Serial.begin(9600);
  while ( !Serial ) delay(100);   // wait for native usb
  Serial.println(F("BMP280 test"));
  unsigned status;
  //status = bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);
  status = bmp.begin(0x76);
  if (!status) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    Serial.print("SensorID was: 0x"); Serial.println(bmp.sensorID(),16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (1) delay(10);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void sendSensor()
{
    int temp =bmp.readTemperature();
    float pressure= bmp.readPressure();
    float altitude=bmp.readAltitude(1017);
    Serial.print(F("Temperature = "));
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");

    Serial.print(F("Pressure = "));
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");

    Serial.print(F("Approx altitude = "));
    Serial.print(bmp.readAltitude(1017)); /* Adjusted to local forecast! */
    Serial.println(" m");                    //If you don't know it, modify it until you get your current altitude
    Serial.println();
    Blynk.virtualWrite(V0, temp);
    Blynk.virtualWrite(V1, pressure);
    Blynk.virtualWrite(V2, altitude);                                  //The "1019.66" is the pressure(hPa) at sea level in day in your region
    delay(2000);
}
void loop()
{
Blynk.run();
  timer.run();
  }