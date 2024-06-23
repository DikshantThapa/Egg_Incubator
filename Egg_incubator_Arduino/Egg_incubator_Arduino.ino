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
  lcd.print("Temp: ");
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.setCursor(0, 2);
  lcd.print("LED: ");
  lcd.setCursor(9, 2);
  lcd.print("Fan: ");
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    lcd.setCursor(6, 0);
    lcd.print("Sensor Error   ");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" *C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  lcd.setCursor(6, 0);
  lcd.print(temperature);
  lcd.print(" C  ");

  lcd.setCursor(10, 1);
  lcd.print(humidity);
  lcd.print(" %  ");

  if (temperature <= 26.0) {
    digitalWrite(LED_PIN, HIGH); // Turn on the LED
    digitalWrite(FAN_PIN, LOW);  // Turn off the fan

    lcd.setCursor(5, 2);
    lcd.print("ON ");
    lcd.setCursor(13, 2);
    lcd.print("OFF");

    Serial.println("LED is ON, Fan is OFF");
  } else {
    digitalWrite(LED_PIN, LOW);  // Turn off the LED
    digitalWrite(FAN_PIN, HIGH); // Turn on the fan

    lcd.setCursor(5, 2);
    lcd.print("OFF");
    lcd.setCursor(13, 2);
    lcd.print("ON ");

    Serial.println("LED is OFF, Fan is ON");
  }

  delay(2000); // Wait for 2 seconds
}
