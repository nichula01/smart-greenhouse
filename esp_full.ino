#include <WiFi.h>
#include <FirebaseESP32.h>

// WiFi credentials
#define WIFI_SSID "iPhone"
#define WIFI_PASSWORD "Cricket.8"

// Firebase configuration
FirebaseConfig config;
FirebaseAuth auth;
FirebaseData firebaseData;

// Variables to store sensor readings
float temperature = 0;
float humidity = 0;
float soilMoisture = 0;
int lightLevel = 0;
bool pumpStatus = false;
bool ledStatus = false;
bool relayStatus = false;
bool doorStatus = false;
bool rainStatus = false;  

String receivedData = "";

void setup() {
  Serial.begin(115200);  // Debug serial connection
  Serial2.begin(9600, SERIAL_8N1, 16, 17);  // RX2=GPIO16, TX2=GPIO17 (ESP32 Serial2 pins)

  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Set Firebase credentials
  config.host = "smart-farming-82577-default-rtdb.asia-southeast1.firebasedatabase.app";
  config.signer.tokens.legacy_token = "iNbg357YzfoC28aq54ockfSJ7Plw3o6aL8ePLg7C";

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Set database read timeout to 1 minute
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  // Set database write size limit to 1MB
  Firebase.setwriteSizeLimit(firebaseData, "tiny");

  Serial.println("ESP32 Ready to receive data from Arduino");
}

void loop() {
  // Read data from Arduino if available
  while (Serial2.available() > 0) {
    receivedData = Serial2.readStringUntil('\n');
    receivedData.trim();

    Serial.print("Received: ");
    Serial.println(receivedData);

    // Parse the received data based on prefixes
    if (receivedData.startsWith("TEMP:")) {
      temperature = receivedData.substring(5).toFloat();
      updateFirebase("temperature", temperature);
    } 
    else if (receivedData.startsWith("HUM:")) {
      humidity = receivedData.substring(4).toFloat();
      updateFirebase("humidity", humidity);
    } 
    else if (receivedData.startsWith("SOIL:")) {
      soilMoisture = receivedData.substring(5).toFloat();
      updateFirebase("soilMoisture", soilMoisture);
    } 
    else if (receivedData.startsWith("LIGHT:")) {
      lightLevel = receivedData.substring(6).toInt();
      updateFirebase("lightLevel", lightLevel);
    } 
    else if (receivedData.startsWith("PUMP:")) {
      pumpStatus = (receivedData.substring(5) == "ON");
      updateFirebase("pumpStatus", pumpStatus);
    } 
    else if (receivedData.startsWith("LED:")) {
      ledStatus = (receivedData.substring(4) == "ON");
      updateFirebase("ledStatus", ledStatus);
    } 
    else if (receivedData.startsWith("RELAY:")) {
      relayStatus = (receivedData.substring(6) == "ON");
      updateFirebase("relayStatus", relayStatus);
    } 
    else if (receivedData.startsWith("DOOR:")) {
      doorStatus = (receivedData.substring(5) == "OPEN");
      updateFirebase("doorStatus", doorStatus);
    } 
    else if (receivedData.startsWith("RAIN:")) {  // Added Rain Sensor Data
      rainStatus = (receivedData.substring(5) == "Detected");
      updateFirebase("rainStatus", rainStatus);
    }
  }

  // Upload a timestamp with each update cycle
  if (millis() % 60000 == 0) { // Update timestamp every minute
    updateFirebase("lastUpdate", String(millis()));
  }

  delay(100); // Small delay to prevent CPU hogging
}

// Helper function to update Firebase with different data types
template <typename T>
void updateFirebase(const String& field, T value) {
  String path = "/sensorData/" + field;

  if (Firebase.set(firebaseData, path, value)) {
    Serial.print("Successfully sent ");
    Serial.print(field);
    Serial.println(" to Firebase");
  } else {
    Serial.print("Failed to send ");
    Serial.print(field);
    Serial.print(" to Firebase: ");
    Serial.println(firebaseData.errorReason());
  }
}
