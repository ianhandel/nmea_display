

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>
#include <TinyGPS++.h>



#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define  BLUE    0x001F
#define  RED     0xF800
#define  GREEN   0x07E0
#define  CYAN    0x07FF
#define  MAGENTA 0xF81F
#define  YELLOW  0xFFE0
#define  WHITE   0xFFFF

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);



TinyGPSPlus gps;

TinyGPSCustom depthBT(gps, "SDDPT", 1);
TinyGPSCustom depthOFFSET(gps, "SDDPT", 2);
TinyGPSCustom windAWS(gps, "SWWMV", 3);


void setup(void) {
  // tft stuff
  tft.reset();
  uint16_t identifier = 0x9341;

  tft.begin(identifier);
  tft.fillScreen(BLACK);
  tft.setRotation(1);

  Serial.begin(4800);
}

void write_number(int x, int y, String name, double num, int colour) {
  tft.fillRect(x * 160, y * 120, x * 160 + 160, y * 120 + 120, BLACK);
  tft.drawRect(x * 160 + 1, y * 120 + 1, x * 160 + 160 - 1, y * 120 + 120 - 1, colour);
  tft.setCursor(x * 160 + 5, y * 120 + 30);
  tft.setTextColor(colour);
  tft.setFont(&FreeMonoBold12pt7b);
  tft.setTextSize(1);
  tft.println(name);
  tft.setCursor(x * 160 + 5, y * 120 + 80);
  tft.setTextSize(1);
  tft.setFont(&FreeMonoBold24pt7b);
  tft.println(num);
}

void loop() {
  while (Serial.available() > 0)
    gps.encode(Serial.read());

  if (depthBT.isUpdated() & depthOFFSET.isUpdated()) {
    write_number(0, 0,
                 "Depth",
                 atof(depthBT.value()) + atof(depthOFFSET.value()),
                 YELLOW);
  }

  
  if (windAWS.isUpdated()) {
    write_number(0, 1,
                 "AWS",
                 atof(windAWS.value()),
                 RED);
  }

 
  write_number(1, 0, "LAT",   random(0,  600) / 10.0, BLUE);
  write_number(1, 1, "LON",   random(0,  600) / 10.0, GREEN);

  delay(300);
}




/*

  /*
  Code to read output from chartplotter (NMEA DPT sentence)
  Then parse out depth
  Convert to 7 segments etc
  Then write out to NASA Depth Repeater
  I2C bit banging and 7 segment code from modified from Peter Holtermann
  See... http://wiki.openseamap.org/wiki/De:NASA_Clipper_Range
*/



/*
  #include <TinyGPS++.h>
  TinyGPSPlus gps;

  TinyGPSCustom depthBT(gps, "SDDPT", 1);
  TinyGPSCustom depthOFFSET(gps, "SDDPT", 2);

  #define RE_PIN 4
  #define DE_PIN 5

  int I2C_SCL = 3; // RED
  int I2C_SDA = 2; // WHITE

  void setup()
  {
  pinMode(I2C_SCL, OUTPUT);      // sets the digital pin as output
  pinMode(I2C_SDA, OUTPUT);      // sets the digital pin as output
  pinMode(RE_PIN, OUTPUT);
  pinMode(DE_PIN, OUTPUT);
  digitalWrite(I2C_SCL, HIGH);   // sets the pin on
  digitalWrite(I2C_SDA, HIGH);   // sets the pin on
  digitalWrite(RE_PIN, LOW);
  digitalWrite(DE_PIN, LOW);
  Serial.begin(4800);

  }



  testing data
  $SDDPT,10.1,-1.5,*62
  $SDDPT,100.7,-1.7,*56
  $SDDPT,0.7,-1.5,*55
  $SDDPT,11.7,-1.5,*65
  $SDDPT,78,-1.5,*73
  $SDDPT,198.7,-1.5,*55
  $SDDPT,1.5,-1.5,*56
  $SDDPT,999,-1.7,*47
  $SDDPT,94,-1.7,*73

  $SWWMV,180, R, 7.3, N, A*2A
*/
