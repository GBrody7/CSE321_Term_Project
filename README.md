# Overview
This project is an automated plant-watering system built using an Arduino uno. It uses a capacitive soil-moisture sensor to monitor soil conditions and a digital water-level sensor to ensure the water reservoir in the 3D print is safe to use. A small pump—controlled through an L298N motor driver, automatically waters the plant when the soil is too dry and the basin has enough water. LEDs provide visual status, and the Serial Monitor is used for debugging.

This README explains how to set up, run, and use this project.

---

# 1. Hardware

## Required Components
- Arduino Uno  
- Capacitive Soil-Moisture Sensor (Analog Output)  
- Basin Water-Level Sensor (Digital HIGH/LOW Output)  
- L298N motor driver  
- Small 5–12V water pump  
- 9V Battery Supply
- LED indicators  
  - Green LED (Basin OK)  
  - Red LED (Basin Empty)  
- SSD1306 OLED display

---

## Wiring Guide

### Soil Moisture Sensor
| Sensor Pin | Arduino Pin |
|------------|-------------|
| VCC        | 5V          |
| GND        | GND         |
| AOUT       | A0          |

### Basin (Water Level) Sensor
| Sensor Pin | Arduino Pin |
|------------|-------------|
| VCC        | 5V          |
| GND        | GND         |
| DOUT       | D2          |

### Pump using L298N
| L298N Pin | Arduino Pin |
|-----------|-------------|
| IN1       | D4          |
| ENA        | D3          |
| GND       | Arduino GND |
| Motor Out | Pump Wires |

### LED Indicators
| LED  | Arduino Pin |
|------|-------------|
| Green | D8 |
| Red   | D9 |

### SSD1306 OLED
| OLED Pin | Arduino Pin |
|----------|-------------|
| VCC      | 3.3V |
| GND      | GND |
| SDA      | A4 |
| SCL      | A5 |

---

# 2. Software

## Libraries Required:
Install in Arduino Library Manager:

- **Adafruit_GFX**
- **Adafruit_SSD1306**

## Uploading Code
1. Open the .ino sketch  
2. Connect Arduino  
3. Select Board + Port  
4. Click Upload 
5. Open Serial Monitor

---

# 3. How the System Works

## Main Logic
The system continuously polls:

- Soil Moisture (Analog Value)
- Basin Water Level (Digital Value)

The pump turns (ON) only if both:
1. Soil is Dry (Below Chosen Threshold)  
2. Basin has Water (Sensor HIGH)

## LED Behavior
- Green LED ON (Basin Full)  
- Red LED ON (Basin Low)  
- Soil Moisture does (NOT) affect LED Status

## Pump Behavior
- Soil Dry AND Basin Full (Pump ON)  
- Soil Wet OR Basin Empty (Pump OFF)

## Serial Monitor Output (SSD1306)

Soil Level: 3482
Basin: FULL
Pump: ON


---

# 4. Using This Project

## Day to Day Use
Once Connected:
- LEDs show Basin Status  
- Soil Moisture is Monitored  
- Pump Activates Automatically
- OLED Gives At-A-Glance Plant Updates

---

# 5. Troubleshooting

| Issue | Fix |
|-------|-----|
| OLED libraries missing | Install (Adafruit_GFX) & (Adafruit_SSD1306) |
| Pump not running | Check L298N Wiring & Power |
| LEDs always off | Check Polarity & Resistors |
| Soil readings incorrect | Ensure Sensor Fully Inserted & Check A0 Wiring |
| Basin always reads LOW | Check 5V Power & D2 Pull Up Resistor |
