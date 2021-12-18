#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include "DHT.h"
#include "SPI.h"
#include "SD.h" 

#define SD_CS  10
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define DHTPIN 0
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define ORANGE      0xFD20
#define GREENYELLOW 0xAFE5
#define NAVY        0x000F
#define DARKGREEN   0x03E0
#define DARKCYAN    0x03EF
#define MAROON      0x7800
#define PURPLE      0x780F
#define OLIVE       0x7BE0
#define LIGHTGREY   0xC618
#define DARKGREY    0x7BEF

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
int mq2 = 0;
void setup(void) {
  Serial.begin(9600);
  /*Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("failed!");
    return; }*/
    
  Serial.begin(9600);
  Serial.println(F("TFT LCD test"));
  dht.begin();

  Serial.print("TFT size is "); Serial.print(tft.width()); Serial.print("x"); Serial.println(tft.height());
  tft.reset();
  uint16_t identifier = tft.readID();
  tft.begin(identifier);
  tft.fillScreen(BLUE);
}

void loop(void) {
    tft.setRotation(1);
    tft.fillScreen(NAVY);
    testText();
    delay(30000);
}

unsigned long testText() {  
  mq2 = analogRead(A5)*2;
  tft.setCursor(50, 7);
  float h = dht.readHumidity();
  float t = dht.readTemperature()-0.5;  //   --- capteur dans le boitier (décalage)
 
  tft.setTextColor(RED); tft.setTextSize(3);
  tft.println("TEMPERATURE"); tft.println(); tft.setTextSize(5); tft.setTextColor(GREEN);tft.setCursor(80, 40);
  tft.println(t); tft.println();
  
  tft.setTextColor(RED); tft.setTextSize(3);tft.setCursor(80, 100);
  tft.print("HUMIDITE  "); tft.println(); tft.setTextSize(5); tft.setTextColor(GREENYELLOW);tft.setCursor(80, 100+33);
  tft.println(h);
   tft.setTextColor(RED); tft.setTextSize(3);tft.setCursor(15, 200);tft.print("Qualite AIR  ");tft.setTextSize(5);tft.setTextColor(BLUE); tft.print(mq2);
}
