SolarNav
"Smart Transit for a Cooler Commute"

SolarNav is an intelligent, low-cost hardware module designed to recommend the cooler, more comfortable side of a vehicle to passengers. By combining light intensity, temperature, and vehicle orientation data, it calculates a real-time comfort score to improve the transit experience.

Project Description
SolarNav measures environmental conditions using two sets of sensors placed on the left and right sides of a vehicle. It utilizes an Arduino-based logic system to process:

Light Intensity: Via BH1750 sensors to track sunlight exposure.

Ambient Temperature: Via DHT22 sensors to measure heat variance.

Orientation: Via HMC5883L magnetometers to track the vehicle's heading.

This data is processed to provide a "LEFT," "RIGHT," or "SIMILAR" recommendation on an OLED display. We envision this project integrating with public transit systems like the KSRTC Chalo app to provide passengers with "Shade-Ready" seating recommendations before they even board.

Hardware Requirements (BOM)
Assembly Instructions

Component Placement: Mount sensors symmetrically on the left and right sides of the chassis.


Power/Ground: Connect all modules to the 5V and GND rails on your breadboard.


I2C Connections: Connect all I2C sensors (BH1750, HMC5883L, OLED) to the A4 (SDA) and A5 (SCL) pins on the Arduino.


Data Connections: Connect DHT22 data pins to digital pins 2 and 3.

Setup Guide
IDE: Install the .

Libraries: Install the following via the Library Manager: Adafruit GFX, Adafruit SSD1306, DHT sensor library, BH1750, and HMC5883L.


Upload: Open code/SolarNav.ino, select your board, and click Upload.


Input Time: Open the Serial Monitor (9600 baud) and enter the time as HH MM (e.g., 14 30) to calibrate the solar angle logic.

How It Works
The system normalizes sensor inputs—Temperature (35%), Light (30%), Solar Impact (20%), and Humidity (15%)—to compare sides. It calculates the sun's position based on the input time and compares it with the current compass heading.

Team Members
Suzanne Riju Issac

Reema Maria Jilson

License
This project is licensed under the MIT License.


![SolarNav](https://github.com/user-attachments/assets/678c97fd-eef7-477d-b502-a3e0e8bcaf77)
![SolarNav 2](https://github.com/user-attachments/assets/3208db7d-fda2-4a44-aa4d-5d2f82c3b70f)
![SolarNav 3](https://github.com/user-attachments/assets/353fa904-0989-429d-8b49-5c6501cbaefa)



