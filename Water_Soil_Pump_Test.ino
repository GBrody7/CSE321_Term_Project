//  Soil Moisture / Water Basin Sensor / Water Pump
//  Soil Moisture Sensor (A0)
//  Basin Water Level Sensor (D2)
//  Motor Driver L298N (DIR = D4) (PWM = D3)
//  GREEN LED: Basin has Water
//  RED LED: Basin low Water


// Analog Moisture Sensor
const int SENSOR_PIN = A0;
const int DRY_VALUE      = 500;
const int WET_VALUE      = 200;
const int NEED_WATER_PCT = 35;   // (Below = TOO DRY)
const int IDEAL_MIN_PCT  = 40;
const int IDEAL_MAX_PCT  = 70;
const int TOO_WET_PCT    = 85;   // (Above = TOO WET)
const int AVG_SAMPLES    = 10;

// Basin Water Sensor with LEDs
const int PIN_S = 2;    // (WET = 1)
const int LED_G = 8;    // (Green = Full)
const int LED_R = 10;   // (Red = Empty)

// Pump Driver (L298N)
const int DIR_PIN = 4;   // (D4 = IN1)
const int PWM_PIN = 3;   // (D3 = ENA(PWM))

void setup() 
{
  Serial.begin(9600);
  while (!Serial);

  Serial.println("Soil Moisture & Basin & Pump");

  pinMode(PIN_S, INPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);

  pinMode(DIR_PIN, OUTPUT);
  pinMode(PWM_PIN, OUTPUT);

  digitalWrite(DIR_PIN, HIGH);   // Pump Direction
  analogWrite(PWM_PIN, 0);       // Pump OFF at Start
}

void loop() 
{
  // Soil
  int raw = readAverage(SENSOR_PIN);
  int percent = mapToPercent(raw);

  // Calcualte State
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
  
  //  Basin Water Sensor
  bool basinWet = digitalRead(PIN_S);   // (Water = 1)

  //  Pump Logic
  bool pumpOn = false;

  // Pump ON Only (Soil = DRY) & (Basin has Water)
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
    digitalWrite(LED_G, HIGH);   // Water: Green ON
    digitalWrite(LED_R, LOW);    // Water: Red OFF
  } 
  
  else 
  {
    digitalWrite(LED_G, LOW);    // No Water: Green OFF
    digitalWrite(LED_R, HIGH);   // NO Water: Red ON
  }

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


// AVG Generator Function 
int readAverage(int pin) 
{
  long sum = 0;

  // Take 10 Readings
  for (int i = 0; i < AVG_SAMPLES; i++) 
  {
    sum += analogRead(pin);
    delay(5);
  }
  // Return Average
  return sum / AVG_SAMPLES;
}

// Convert Raw (0-1023) to Percentage (0-100%)
int mapToPercent(int raw) 
{
  int span = DRY_VALUE - WET_VALUE;
  
  if (span <= 0) 
  {
    span = 1;
  }
  
  int percent = (DRY_VALUE - raw) * 100L / span;
  
  // Clamp (0-100%)
  if (percent < 0)
  {
    percent = 0;
  }
    
  if (percent > 100) 
  {
    percent = 100;
  }
  // Return (%)
  return percent;
}
