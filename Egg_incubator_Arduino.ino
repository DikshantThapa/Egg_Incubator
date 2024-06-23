#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 2
#define DHTTYPE DHT22

#define LED_PIN 4
#define FAN_PIN 3

DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  
  dht.begin();
  
  pinMode(LED_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  
  lcd.init();
  lcd.backlight();
  
  lcd.setCursor(0, 0);
  lcd.print("Temperature:");
  lcd.setCursor(0, 1);
  lcd.print("LED Brightness:");
}

void loop() {
  float temperature = dht.readTemperature();

  if (isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    lcd.setCursor(0, 0);
    lcd.print("DHT Sensor Error");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" *C");

  if (temperature < 27.0) {
    int brightness = mapTemperatureToBrightness(temperature);
    analogWrite(LED_PIN, brightness); // Adjust the light brightness
    analogWrite(FAN_PIN, 0);          // Turn off the fan

    lcd.setCursor(15, 1);
    lcd.print(brightness);            // Display the LED brightness
  } else if (temperature >= 27.0 && temperature <= 28.0) {
    int brightness = mapTemperatureToBrightness(temperature);
    int fanSpeed = mapTemperatureToFanSpeed(temperature);
    analogWrite(LED_PIN, brightness); // Adjust the light brightness
    analogWrite(FAN_PIN, fanSpeed);   // Adjust the fan speed

    lcd.setCursor(15, 1);
    lcd.print(brightness);            // Display the LED brightness
  } else {
    analogWrite(LED_PIN, 0); // Turn off the light
    int fanSpeed = mapTemperatureToFanSpeed(temperature);
    analogWrite(FAN_PIN, fanSpeed); // Adjust the fan speed

    lcd.setCursor(15, 1);
    lcd.print("   ");  // Clear the LED brightness display
  }

  lcd.setCursor(13, 0);
  lcd.print(temperature);
  lcd.print(" C");

  delay(1000);
}

int mapTemperatureToBrightness(float temp) {
  int minTemp = 17;  
  int maxTemp = 27;  
  int minBrightness = 255;   
  int maxBrightness = 0; 

  int brightness = map(temp, minTemp, maxTemp, minBrightness, maxBrightness);

  if (brightness < maxBrightness) {
    brightness = maxBrightness;
  } else if (brightness > minBrightness) {
    brightness = minBrightness;
  }

  return brightness;
}

int mapTemperatureToFanSpeed(float temp) {
  int minTemp = 27;    // Minimum temperature to start the fan
  int maxTemp = 38.0;  // Maximum temperature for egg incubation
  int minSpeed = 0;    // Minimum fan speed
  int maxSpeed = 255;  // Maximum fan speed

  int speed = map(temp, minTemp, maxTemp, minSpeed, maxSpeed);

  if (speed < minSpeed) {
    speed = minSpeed;
  } else if (speed > maxSpeed) {
    speed = maxSpeed;
  }

  return speed;
}
