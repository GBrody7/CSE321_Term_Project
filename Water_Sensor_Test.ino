// Water Level Sensor Test
// Sensor: (Wet = HIGH) & Dry = LOW)
// LEDs: (HIGH = ON, LOW = OFF)

const int PIN_S = 2;    // Sensor Pin
const int LED_G = 8;    // Green LED
const int LED_R = 10;   // Red LED

void setup() 
{
  Serial.begin(9600);
  pinMode(PIN_S, INPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
}

void loop() 
{
  bool wet = digitalRead(PIN_S);  // (WET = 1) & (DRY = 0)

  Serial.print("Wet: ");
  Serial.println(wet);

  if (wet) 
  {
    // Basin has Water: (GREEN = ON) & (RED = OFF)
    digitalWrite(LED_G, HIGH);
    digitalWrite(LED_R, LOW);
  } 
  else 
  {
    // Basin Low:(GREEN = OFF) & (RED = ON)
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_R, HIGH);
  }

  delay(100);
}
