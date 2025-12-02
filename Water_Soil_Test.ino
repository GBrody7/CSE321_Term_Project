//  Soil Moisture & Water Sensor Working
//  Reads Analog Voltage from Soil Moisture Sensor
//  Converts Raw Value to Percentage Based on (DRY/WET) Ref. Values
//  States (Too Dry, Ideal, Too Wet)
//  Reads Digital Water Sensor Drives (GREEN/RED) LED
//  Print Status Info

// Analog Moisture Sensor (Pin A0)
const int SENSOR_PIN = A0;

// Cal Points (Plant Dependent)
const int DRY_VALUE = 800;  // Air Dry Soil
const int WET_VALUE = 300;  // Wet Soil

// Status Thresholds (Percentage)
const int NEED_WATER_PCT = 35;  // Too Dry (Needs Water)
const int IDEAL_MIN_PCT  = 40;  // Ideal Lower Bound
const int IDEAL_MAX_PCT  = 70;  // Ideal Upper Bound
const int TOO_WET_PCT    = 85;  // Too Wet (No Water)

// # Samples to Average
const int AVG_SAMPLES = 10;


// Digital Water Sensor & LEDs

// Water sensor: (WET = 1) & (LOW = 0)
const int PIN_S    = 2;   // Digital Water Sensor (Pin D2)
const int LED_G    = 8;   // Green LED (Pin D8)
const int LED_R    = 10;  // Red LED (Pin D10)


void setup() 
{
  Serial.begin(9600);

  while (!Serial) ;

  Serial.println("Soil Moisture & Water Sensor Started");

  // Digital water sensor + LEDs
  pinMode(PIN_S, INPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
}


void loop() 
{
  // Analog Soil Moisture Sensor
  // Read & Take Average
  int raw = readAverage(SENSOR_PIN);

  // Convert Raw (0–1023) to Percentage (0–100%)
  int percent = mapToPercent(raw);
  
  // Determine Soil State (Too Dry, Ideal, Too Wet)
  String state = classifyState(percent);

  // Print Raw (0–1023)
  Serial.print("Raw: ");
  Serial.print(raw);

  // Print Percentage (0–100%)
  Serial.print("   Moisture: ");
  Serial.print(percent);

  // Print Soil State
  Serial.print("%   Status: ");
  Serial.println(state);


  // Digital Water Level Sensor & LEDs
  bool wet = digitalRead(PIN_S);  // (WET = 1) & (DRY = 0)
 
  Serial.print("Digital Water Sensor: ");
  Serial.println(wet);

  if (wet)
  {
    // Basin has Water: (GREEN = ON) / (RED = OFF)
    digitalWrite(LED_G, HIGH);
    digitalWrite(LED_R, LOW);
  }
    
  else
  {
    // Basin Low: (GREEN = OFF) / (RED = ON)
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_R, HIGH);
  }
  delay(1000);
}


// Take 10 Soil Moisture Readings, Average Results, Return Average
int readAverage(int pin) 
{
  long sum = 0;
 
  // Take Readings
  for (int i = 0; i < AVG_SAMPLES; i++) 
  {
    // Read, Add Val to Total
    sum += analogRead(pin);
    delay(5);
  }

  // Calculate / Return AVG
  return sum / AVG_SAMPLES;
}


// Convert Raw Sensor Voltage -> (0–100%)
int mapToPercent(int raw) 
{
  // Calculate (WET/DRY) Range
  int span = DRY_VALUE - WET_VALUE;

  // Avoid Div by 0
  if (span <= 0) 
  {
    span = 1;
  }
  
  // Convert Raw to (%) To Range (LOW = WET)
  int percent = (DRY_VALUE - raw) * 100L / span;
  
  // Clamp (0–100%)
  if (percent < 0)   
  {
    percent = 0;
  }
  if (percent > 100) 
  {
    percent = 100;
  }
  
  // Return percentage  
  return percent;
}


// Check Mooisture State
String classifyState(int percent) 
{
  // Below (NEED WATER) Level
  if (percent < NEED_WATER_PCT)
  {
    // TOO DRY (Need Water)
    return "Too Dry (NEED WATER)";
  }
  // Within Ideal State
  else if (percent >= IDEAL_MIN_PCT && percent <= IDEAL_MAX_PCT)
  {
    // NO Water Needed
    return "Ideal (NO WATER)";
  }
  // Too Wet (NO Water Needed)
  else 
  {
    // NO Water Needed
    return "Too Wet (NO WATER)";
  }
}
