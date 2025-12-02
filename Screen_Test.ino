// (SSD1306) OLED Screen Test (I2C)
// (OLED SDA = A4) 
// (OLED SCL = A5)

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() 
{
  Serial.begin(9600);
  while (!Serial);

  Serial.println("Start OLED Screen Test");

  // Initialize Display (I2C Address 0x3C)
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
  {
    Serial.println("OLED NOT FOUND!");
    while (true);  // Stop if Incorrect
  }

  Serial.println("OLED Found!");
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Print Each Line (8-pixel spacing)
  display.setCursor(0, 0);
  display.println("OLED TEST START");

  display.setCursor(0, 10);
  display.println("Line 1: Hello");

  display.setCursor(0, 20);
  display.println("Line 2: Screen GOOD");

  display.setCursor(0, 30);
  display.println("Line 3: Wow");

  display.setCursor(0, 40);
  display.println("Line 4: 128x64");

  display.setCursor(0, 50);
  display.println("Line 5: Sweet");

  display.display();
}

void loop() 
{
}
