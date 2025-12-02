//  Soil Moisture Monitor (Continuous Polling)
//  Sensor: SunFounder ST0160 Capacitive Soil Moisture Sensor
//  Reads Analog Voltage from Soil Moisture Sensor
//  Converts Raw to Percentage Based (Dry/Wet) Reference Values
//  Print Raw, Percentage, and Moisture Status
//  Smaller Values = Wetter Soil.

// Ran on PinA0
const int SENSOR_PIN = A0;

// Calibration Points
const int DRY_VALUE = 800;  // Bone Dry Soil
const int WET_VALUE = 300;  // Saturated soil

// Status Thresholds
const int NEED_WATER_PCT = 35;  // Needs Water
const int IDEAL_MIN_PCT  = 40;  // Ideal Lower Bound
const int IDEAL_MAX_PCT  = 70;  // Ideal Upper Bound
const int TOO_WET_PCT    = 85;  // Too Wet

// # Samples to Average (Gives Smoother Output)
const int AVG_SAMPLES = 10;

void setup() 
{
  Serial.begin(9600);

    // Wait for Serial
    while (!Serial) ;
    Serial.println("Soil Moisture Monitor Started");
}


void loop() 
{

  // Read Sensor / Take AVG
  int raw = readAverage(SENSOR_PIN);

  // Convert Sensor (Analog (0-1023)) into (Percentage (0-100%))
  int percent = mapToPercent(raw);
  
  // Determine Soil Condition (Need Water, Ideal, Too Wet)
  String state = classifyState(percent);

  // Print Raw Analog Reading (0-1023)
  Serial.print("Raw: ");
  Serial.print(raw);

  // Print Moisture Percentage (0-100%)
  Serial.print("   Moisture: ");
  Serial.print(percent);

  // Print Soil Status
  Serial.print("%   Status: ");
  Serial.println(state);

  // 500ms Delay 
  delay(500);
}

// Read Soil Sensor Multiple Times, Averages Results, Return Average
int readAverage(int pin) 
{
  long sum = 0;
 
  // Take 10 Readings
  for (int i = 0; i < AVG_SAMPLES; i++) 
  {
    // Read Sensor, Add Value to Total
    sum += analogRead(pin);
    delay(5);
  }

  // Calculate/Return Average
  return sum / AVG_SAMPLES;
}

// Convert Raw Sensor Reading -> (0-100%)
int mapToPercent(int raw) 
{
  // Calculate Range Between Wet & Dry 
  int span = DRY_VALUE - WET_VALUE;

  // Ignore Division by 0
  if (span <= 0) 
  {
    span = 1;
  }
     
  // Convert Raw into (Percent%)
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
      // Return Percentage  
      return percent;
}


// Check State of Moisture Level
String classifyState(int percent) 
{
  // Check if Below (Need Water) Level
  if (percent < NEED_WATER_PCT)
    {
      // Too Dry, Water Needed
      return "Too Dry (NEED WATER)";
    }
   
  else 
  {
    // Check Within Ideal Moisture Level
    if (percent <= IDEAL_MAX_PCT && percent >= IDEAL_MIN_PCT)
    {
      // Ideal Moisture, No Water Needed
      return "Ideal (NO WATER)";
    }

  }
  
  else 
  {
    // Too Wet No Water Needed
    if (percent > TOO_WET_PCT)     
    {
      // Too Wet No Water Needed
      return "Too Wet (NO WATER)";
    }

  else
  {
    // Treat As No Water Needed
    return "Too Wet (NO WATER)";
  } 
}
