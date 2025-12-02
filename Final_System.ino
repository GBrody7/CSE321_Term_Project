//  Soil Moisture / Basin Level / Pump / OLED Screen
//  Soil Moisture Sensor (A0)
//  Basin Water Level Sensor (D2)
//  Motor Driver L298N (DIR = D4) (PWM = D3)
//  GREEN LED: Basin has Water
//  RED LED: Basin Empty
//  OLED Screen (SDA = A4) (SCL = A5)

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Soil Moisture Sensor
const int SENSOR_PIN = A0;
const int DRY_VALUE      = 500;
const int WET_VALUE      = 200;
const int NEED_WATER_PCT = 35;   // (Below = TOO DRY)
const int IDEAL_MIN_PCT  = 40;
const int IDEAL_MAX_PCT  = 70;
const int TOO_WET_PCT    = 85;   // (Above = TOO WET)
const int AVG_SAMPLES    = 10;

// Basin Water Sensor and LEDs
const int PIN_S = 2;    // Basin Water Sensor (D2)
const int LED_G = 8;    // (Green = Water)
const int LED_R = 10;   // (Red = Water Empty)

// Motor Driver (L298N)
const int DIR_PIN = 4;   // (D4 = IN1)
const int PWM_PIN = 3;   // (D3 = ENA(PWM))

void setup() 
{
  Serial.begin(9600);
  while (!Serial);

  Serial.println("Plant Watering System Starting");

  pinMode(PIN_S, INPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);

  pinMode(DIR_PIN, OUTPUT);
  pinMode(PWM_PIN, OUTPUT);

  digitalWrite(DIR_PIN, HIGH);   // Pump Direction
  analogWrite(PWM_PIN, 0);       // Pump OFF at Start

  // Intializing OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
  { 
    Serial.println("SSD1306 Allocation Failed");
    while (true) { } // Display NOT Done
  }

  display.clearDisplay();
  display.setTextSize(1);

  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
 
  display.println("Plant Monitor");
  display.println("Starting...");
  
  display.display();
  
  delay(1000);
}

void loop() 
{
  // Soil Moisture
  int raw = readAverage(SENSOR_PIN);
  int percent = mapToPercent(raw);

  // Decide State for Screen Display
  String plantState;
  if (percent < NEED_WATER_PCT) 
  {
    plantState = "Too Dry"; 
  } 
  else if (percent > TOO_WET_PCT) 
  {
    plantState = "Too Wet";
  } 
  else 
  {
    plantState = "Idle";
  }

  // Basin Water 
  bool basinWet = digitalRead(PIN_S);   // (Water = 1)

  // Pump Logic
  bool pumpOn = false;

  // Pump ON Only if (Soil = DRY) & (Basin has Water)
  if (percent < NEED_WATER_PCT && basinWet) 
  {
    pumpOn = true;
  }  
  if (pumpOn) 
  {
    analogWrite(PWM_PIN, 255);   // Pump ON
  } 
  else 
  {
    analogWrite(PWM_PIN, 0);     // Pump OFF
  }

  // LED Logic
  if (basinWet) 
  {
    digitalWrite(LED_G, HIGH);   // (Water = Green ON)
    digitalWrite(LED_R, LOW);    // (Water = Red OFF)
  } 
  else 
  {
    digitalWrite(LED_G, LOW);    // (NO Water = Green OFF)
    digitalWrite(LED_R, HIGH);   // (NO Water = Red ON)
  }
  
  // OLED Display Output
  display.clearDisplay();

  // Top line
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Plant Monitor");

  // Next Line Moisture Percentage
  display.setCursor(0, 12);
  display.print("Moisture: ");
  display.print(percent);
  display.println("%");

  // Plant State in Middle
  display.setTextSize(2);
  display.setCursor(0, 28);
  display.print("State:");
  display.setCursor(0, 44);
  display.print(plantState);

  display.display();


  // Serial Output
  Serial.print("Moisture Raw: "); 
  Serial.print(raw);
  
  Serial.print(" | Moisture %: "); 
  Serial.print(percent);
 
  Serial.print("% | Plant State: "); 
  Serial.print(plantState);

  Serial.print(" | Basin Water: "); 
  if (basinWet) 
  {
    Serial.print("YES");
  } 
  else 
  {
    Serial.print("NO");
  }

  Serial.print(" | Pump: "); 
  if (pumpOn) 
  {
    Serial.println("ON");
  } 
  else 
  {
    Serial.println("OFF");
  }

  delay(1000);
}


// Average Soil Sample Function
int readAverage(int pin) 
{
  long sum = 0;
  
  for (int i = 0; i < AVG_SAMPLES; i++) 
  {
    sum += analogRead(pin);
    delay(5);
  }
  // Return AVG
  return sum / AVG_SAMPLES;
}

// Take Raw (0-1023) to Percentage (0-100%)
int mapToPercent(int raw) 
{
  int span = DRY_VALUE - WET_VALUE;
  if (span <= 0) 
  {
    span = 1;
  }
  // Calculate
  int percent = (DRY_VALUE - raw) * 100L / span;

  // Clamp (0-100)
  if (percent < 0) 
  {
    percent = 0;
  } 

  if (percent > 100) 
  {
    percent = 100;
  }
 
  return percent;
}
