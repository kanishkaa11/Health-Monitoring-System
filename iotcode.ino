#include <Wire.h>
#include <MAX30100.h>
#include <LiquidCrystal_I2C.h>

// Create an instance of the MAX30100 sensor
MAX30100 sensor;  // Create the sensor object
LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD object (I2C address is 0x27)

unsigned long lastReportTime = 0;
#define REPORTING_PERIOD_MS 1000  // 1 second update interval

void setup() {
  // Start serial communication
  Serial.begin(9600);

  // Initialize the MAX30100 sensor
  if (!sensor.begin()) {
    Serial.println("MAX30100 not detected! Please check the wiring.");
    while (1);
  }

  // Initialize the LCD
  lcd.begin(16, 2);
  lcd.setBacklight(1);  // Turn on backlight
  lcd.clear();
  lcd.print("Heart Rate & O2");

  delay(2000);  // Wait for initialization
}

void loop() {
  // Read the heart rate and SpO2 values from the sensor
  int heartRate = sensor.getHeartRate();
  int SpO2 = sensor.getSpO2();

  // Check if the values are valid
  if (millis() - lastReportTime >= REPORTING_PERIOD_MS) {
    lastReportTime = millis();

    if (heartRate == 0 || SpO2 == 0) {
      Serial.println("Invalid reading, check the sensor.");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error reading data");
    } else {
      // Print the values to the serial monitor
      Serial.print("Heart Rate: ");
      Serial.print(heartRate);
      Serial.print(" bpm, SpO2: ");
      Serial.print(SpO2);
      Serial.println(" %");

      // Display the values on the LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("HR: ");
      lcd.print(heartRate);
      lcd.print(" bpm");

      lcd.setCursor(0, 1);
      lcd.print("O2: ");
      lcd.print(SpO2);
      lcd.print(" %");
    }
  }
}
