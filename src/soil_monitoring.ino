#include <WiFi.h>
#include <ThingSpeak.h>

#include "DHT.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// =================================================
// WIFI
// =================================================

const char* ssid = "iPhone";
const char* password = "12345678";


// =================================================
// THINGSPEAK
// =================================================

unsigned long channelID = 3474573;
const char* writeAPIKey = "8NREBTZPZ8YF50HO";

WiFiClient client;


// =================================================
// DHT22
// =================================================

#define DHT_PIN 4
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);


// =================================================
// SOIL MOISTURE
// =================================================

#define SOIL_PIN 34


// =================================================
// FC-37 RAIN SENSOR
// =================================================

#define RAIN_PIN 35


// =================================================
// OLED
// =================================================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);


// =================================================
// SETUP
// =================================================

void setup() {

  Serial.begin(115200);

  delay(1000);

  Serial.println("ESP32 SENSOR SYSTEM STARTING...");


  // ---------- DHT ----------

  dht.begin();


  // ---------- OLED ----------

  Wire.begin(OLED_SDA, OLED_SCL);

  if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        OLED_ADDR
      )) {

    Serial.println("OLED NOT FOUND!");

    while (1);
  }


  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  display.setCursor(25, 25);
  display.println("SENSOR SYSTEM");

  display.display();

  delay(2000);


  // ---------- WIFI ----------

  Serial.println();
  Serial.println("Connecting to WiFi...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int attempts = 0;

  while (WiFi.status() != WL_CONNECTED && attempts < 30) {

    delay(500);

    Serial.print(".");

    attempts++;
  }

  Serial.println();


  if (WiFi.status() == WL_CONNECTED) {

    Serial.println("WIFI CONNECTED!");

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

  } 
  else {

    Serial.println("WIFI CONNECTION FAILED!");

  }


  // ---------- THINGSPEAK ----------

  ThingSpeak.begin(client);

  Serial.println("ThingSpeak Ready!");

  delay(1000);
}


// =================================================
// LOOP
// =================================================

void loop() {

  // ---------- READ SENSORS ----------

  float temperature = dht.readTemperature();

  float humidity = dht.readHumidity();

  int soilValue = analogRead(SOIL_PIN);

  int rainValue = analogRead(RAIN_PIN);


  // =================================================
  // SERIAL MONITOR
  // =================================================

  Serial.println();
  Serial.println("----------------------------");

  if (isnan(temperature)) {

    Serial.println("Temperature: ERROR");

  } 
  else {

    Serial.print("Temperature: ");
    Serial.print(temperature, 1);
    Serial.println(" C");
  }


  if (isnan(humidity)) {

    Serial.println("Humidity: ERROR");

  } 
  else {

    Serial.print("Humidity: ");
    Serial.print(humidity, 1);
    Serial.println(" %");
  }


  Serial.print("Soil Raw: ");
  Serial.println(soilValue);


  Serial.print("Rain Raw: ");
  Serial.println(rainValue);


  // =================================================
  // OLED
  // =================================================

  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);


  // Temperature

  display.setCursor(0, 0);

  display.print("TEMP: ");

  if (isnan(temperature)) {

    display.print("ERROR");

  } 
  else {

    display.print(temperature, 1);
    display.print(" C");
  }


  // Humidity

  display.setCursor(0, 12);

  display.print("HUM : ");

  if (isnan(humidity)) {

    display.print("ERROR");

  } 
  else {

    display.print(humidity, 1);
    display.print(" %");
  }


  // Soil

  display.setCursor(0, 24);

  display.print("SOIL: ");
  display.print(soilValue);


  // Rain

  display.setCursor(0, 36);

  display.print("RAIN: ");
  display.print(rainValue);


  // Separator

  display.drawLine(
    0, 50,
    127, 50,
    SSD1306_WHITE
  );


  // Rain status

  display.setCursor(0, 54);

  if (rainValue < 1500) {

    display.print("RAIN DETECTED");

  } 
  else {

    display.print("NO RAIN");
  }


  display.display();


  // =================================================
  // SEND TO THINGSPEAK
  // =================================================

  if (WiFi.status() == WL_CONNECTED) {

    Serial.println();
    Serial.println("Sending data to ThingSpeak...");


    // Field 1
    ThingSpeak.setField(1, temperature);


    // Field 2
    ThingSpeak.setField(2, humidity);


    // Field 3
    ThingSpeak.setField(3, soilValue);


    // Field 4
    ThingSpeak.setField(4, rainValue);


    int response = ThingSpeak.writeFields(
      channelID,
      writeAPIKey
    );


    if (response == 200) {

      Serial.println("ThingSpeak update SUCCESS!");

    } 
    else {

      Serial.print("ThingSpeak ERROR: ");
      Serial.println(response);
    }

  } 
  else {

    Serial.println("WiFi is disconnected!");

  }


  // =================================================
  // WAIT 20 SECONDS
  // =================================================

  Serial.println("Waiting 20 seconds...");

  delay(20000);
}
