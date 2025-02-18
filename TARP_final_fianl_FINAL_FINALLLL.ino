#include <DHT.h>

#define DHTPIN 2           
#define SOIL_MOISTURE_PIN A0 
#define DHTTYPE DHT11       

DHT dht(DHTPIN, DHTTYPE);
int pumpPin = 2;  // Pin connected to the relay module for the water pump

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(pumpPin, OUTPUT);  // Set pumpPin as output
  digitalWrite(pumpPin, LOW);  // Make sure the pump is off initially
}

void loop() {
  // Read soil moisture
  int soilMoisture = analogRead(SOIL_MOISTURE_PIN);
  
  // Convert to moisture percentage (0 to 100)
  float moisturePercentage = 100 - (soilMoisture / 10.23); // Inverted calculation
  
  // Read temperature and humidity
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Send data to Serial
  if (!isnan(temperature) && !isnan(humidity)) {
    Serial.print(moisturePercentage);
    Serial.print(",");
    Serial.print(humidity);
    Serial.print(",");
    Serial.println(temperature);
  }

  // Check for incoming serial data (to control the pump)
  if (Serial.available()) {
    char command = Serial.read();  // Read the incoming byte
    if (command == '1') {
      digitalWrite(pumpPin, HIGH);  // Turn on the pump
      Serial.println(".");
    } else if (command == '0') {
      digitalWrite(pumpPin, LOW);   // Turn off the pump
      Serial.println(",");
    }
  }

  delay(5000);  // Wait a bit between readings
}
