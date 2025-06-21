// Include required libraries
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Servo.h>

// Define pin numbers for System 1
const int ledPin = 3;
const int ldrPin = A1;

// Define pin numbers for System 2
LiquidCrystal_I2C lcd(0x27, 16, 2);
const int pumpPin = 2;

// Define pin numbers and setup for System 3
#define DHTPIN 4
#define RELAYPIN 5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Define pin numbers and setup for System 4
#define SERVO_PIN 9
#define RAIN_SENSOR_PIN 8  // Added rain sensor
Servo doorServo;

void setup() {
  // Setup for System 1
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(ldrPin, INPUT);

  // Setup for System 2
  lcd.init();
  lcd.backlight();
  lcd.clear();
  pinMode(pumpPin, OUTPUT);
  digitalWrite(pumpPin, HIGH);
  lcd.setCursor(0, 0);
  lcd.print("IRRIGATION");
  lcd.setCursor(0, 1);
  lcd.print("SYSTEM IS ON ");
  delay(3000);
  lcd.clear();

  // Setup for System 3
  dht.begin();
  pinMode(RELAYPIN, OUTPUT);
  digitalWrite(RELAYPIN, HIGH);

  // Setup for System 4
  doorServo.attach(SERVO_PIN);
  doorServo.write(0);
  pinMode(RAIN_SENSOR_PIN, INPUT); // Initialize rain sensor
}

void loop() {
  system1();
  system2();
  system3();
  system4();
}

void system1() {
  int ldrStatus = analogRead(ldrPin);

  if (ldrStatus <= 200) {
    digitalWrite(ledPin, HIGH);
    Serial.println("LDR is DARK, LED is ON");
    Serial.println("Light Condition: DARK");
    Serial.println("LIGHT:" + String(ldrStatus));
    Serial.println("LED:ON");
  } else {
    digitalWrite(ledPin, LOW);
    Serial.println("LDR is light, LED is Off");
    Serial.println("Light Condition: LIGHT");
    Serial.println("LIGHT:" + String(ldrStatus));
    Serial.println("LED:OFF");
  }
}

void system2() {
  int value = analogRead(A0);
  float moisturePercentage = map(value, 1023, 0, 0, 100);

  Serial.print("Soil Moisture: ");
  Serial.print(moisturePercentage);
  Serial.println("%");
  Serial.println("SOIL:" + String(moisturePercentage));

  if (value > 950) {
    digitalWrite(pumpPin, LOW);
    lcd.setCursor(0, 0);
    lcd.print("Water Pump is ON ");
    Serial.println("PUMP:ON");
  } else {
    digitalWrite(pumpPin, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("Water Pump is OFF");
    Serial.println("PUMP:OFF");
  }
}

void system3() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
  Serial.println("TEMP:" + String(temperature));

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");
  Serial.println("HUM:" + String(humidity));

  if (temperature > 30.0) {
    digitalWrite(RELAYPIN, LOW);
    Serial.println("Relay ON");
    Serial.println("RELAY:ON");
  } else {
    digitalWrite(RELAYPIN, HIGH);
    Serial.println("Relay OFF");
    Serial.println("RELAY:OFF");
  }

  delay(2000);
}

void system4() {
  float humidity = dht.readHumidity();
  int rainStatus = digitalRead(RAIN_SENSOR_PIN); // Read rain sensor data

  if (isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor! Retrying...");
    return;
  }

  Serial.print("Humidity: ");
  Serial.println(humidity);
  
  // Display rain sensor status
  if (rainStatus == LOW) {
    Serial.println("RAIN: Detected");
  } else {
    Serial.println("RAIN: No Rain");
  }

  // Control the door based on humidity
  if (humidity > 80) {
    doorServo.write(150);
    Serial.println("Opening door...");
    Serial.println("DOOR:OPEN");
  } else {
    doorServo.write(0);
    Serial.println("Closing door...");
    Serial.println("DOOR:CLOSED");
  }

  delay(3000);
}
