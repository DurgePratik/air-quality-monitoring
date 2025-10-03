#define BLYNK_TEMPLATE_ID "TMPL3CNM-E-Fp"
#define BLYNK_TEMPLATE_NAME "Smart Weather Station"
#define BLYNK_AUTH_TOKEN "iaMqmxY6Ap8EjreTZK4GoOyT1FzIiInQ"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <math.h>
#include <base64.h> // Include the correct Base64 library

// WiFi credentials
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "XYZ"; //Add your own ssid
char pass[] = "xyz";

BlynkTimer timer;

// Twilio credentials
const char* TWILIO_ACCOUNT_SID = "#####";
const char* TWILIO_AUTH_TOKEN = "######";
const char* TWILIO_PHONE_NUMBER = "+######"; // Your Twilio phone number

// Recipients
const char* userPhoneNumbers[] = {"X", "Y", "Z"}; // Replace with recipients' phone numbers
const int numberOfRecipients = sizeof(userPhoneNumbers) / sizeof(userPhoneNumbers[0]);

// DHT11 setup
#define DHTPIN D1
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// MQ-135 setup
#define MQ135_PIN A0
#define RL 10.0 // Load resistance in kilo-ohms
#define R0 110.63 // Calibration value (sensor resistance in clean air)

// Thresholds for notifications
const float TEMP_THRESHOLD = 20.0;    // °C
const float HUMIDITY_THRESHOLD = 90.0; // %
const float GAS_THRESHOLD = 300.0;    // ppm (for any gas)

// Gas curve parameters
float NH3_curve[3] = {1.3, 0.77, -1.68};
float CO2_curve[3] = {1.3, 0.72, -2.10};
float Toluene_curve[3] = {1.3, 0.71, -2.30};
float Ethanol_curve[3] = {1.3, 0.85, -1.95};
float Methanol_curve[3] = {1.3, 0.80, -2.00};

// Function to calculate RS
float calculateRS(float analogValue) {
  float voltage = analogValue * (5.0 / 1023.0);
  float rs = (5.0 - voltage) * RL / voltage;
  return rs;
}

// Function to calculate gas concentration (ppm)
float getGasConcentration(float rs_ro_ratio, float* curve) {
  return pow(10, (log10(rs_ro_ratio) - curve[1]) / curve[2] + curve[0]);
}

// Function to send SMS via Twilio
// Function to send SMS via Twilio
void sendSMS(const char* message) {
  WiFiClientSecure client;
  client.setInsecure(); // Ignore SSL certificate verification

  bool connectionSuccess = false;

  for (int i = 0; i < numberOfRecipients; i++) {
    if (client.connect("api.twilio.com", 443)) {
      connectionSuccess = true;

      // Prepare the POST data for each recipient
      String postData = "To=" + String(userPhoneNumbers[i]) +
                        "&From=" + String(TWILIO_PHONE_NUMBER) +
                        "&Body=" + String(message);

      // Base64 encode the credentials
      String credentials = String(TWILIO_ACCOUNT_SID) + ":" + TWILIO_AUTH_TOKEN;
      String encodedCredentials = base64::encode(credentials);

      // Send the POST request
      client.println("POST /2010-04-01/Accounts/" + String(TWILIO_ACCOUNT_SID) + "/Messages.json HTTP/1.1");
      client.println("Host: api.twilio.com");
      client.println("Authorization: Basic " + encodedCredentials);
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.print("Content-Length: ");
      client.println(postData.length());
      client.println();
      client.println(postData);

      // Read Twilio response
      String response = client.readString();
      Serial.println("Twilio Response for " + String(userPhoneNumbers[i]) + ": " + response);

    } else {
      Serial.println("Connection to Twilio failed for number: " + String(userPhoneNumbers[i]));
    }

    // Close the connection for this recipient
    client.stop();
  }

  if (!connectionSuccess) {
    Serial.println("Failed to connect to Twilio for all recipients.");
  }
}


// Function to check thresholds and send notifications
void checkThresholds(float temp, float humidity, float nh3, float co2, float toluene, float ethanol, float methanol) {
  if (temp > TEMP_THRESHOLD) {
    sendSMS(("Alert! Temperature exceeded threshold: " + String(temp) + " °C").c_str());
  }
  if (humidity > HUMIDITY_THRESHOLD) {
    sendSMS(("Alert! Humidity exceeded threshold: " + String(humidity) + " %").c_str());
  }
  if (nh3 > GAS_THRESHOLD || co2 > GAS_THRESHOLD || toluene > GAS_THRESHOLD || ethanol > GAS_THRESHOLD || methanol > GAS_THRESHOLD) {
    sendSMS("Alert! Gas concentration exceeded threshold!");
  }
}

// Function to send sensor data
void sendSensorData() {
  // Read from DHT11
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Read from MQ-135
  int sensorValue = analogRead(MQ135_PIN);
  float rs = calculateRS(sensorValue);
  float rs_ro_ratio = rs / R0;

  // Calculate gas concentrations
  float nh3_ppm = getGasConcentration(rs_ro_ratio, NH3_curve);
  float co2_ppm = getGasConcentration(rs_ro_ratio, CO2_curve);
  float toluene_ppm = getGasConcentration(rs_ro_ratio, Toluene_curve);
  float ethanol_ppm = getGasConcentration(rs_ro_ratio, Ethanol_curve);
  float methanol_ppm = getGasConcentration(rs_ro_ratio, Methanol_curve);

  // Send data to Blynk
  Blynk.virtualWrite(V0, t);         // Temperature
  Blynk.virtualWrite(V1, h);         // Humidity
  Blynk.virtualWrite(V2, nh3_ppm);  // NH3
  Blynk.virtualWrite(V3, co2_ppm);  // CO2
  Blynk.virtualWrite(V4, toluene_ppm); // Toluene
  Blynk.virtualWrite(V5, ethanol_ppm); // Ethanol
  Blynk.virtualWrite(V6, methanol_ppm); // Methanol

  // Check thresholds and send notifications
  checkThresholds(t, h, nh3_ppm, co2_ppm, toluene_ppm, ethanol_ppm, methanol_ppm);

  // Print to Serial Monitor
  Serial.println("Sensor Data:");
  Serial.print("Temperature: "); Serial.print(t); Serial.println(" °C");
  Serial.print("Humidity: "); Serial.print(h); Serial.println(" %");
  Serial.print("NH3: "); Serial.print(nh3_ppm); Serial.println(" ppm");
  Serial.print("CO2: "); Serial.print(co2_ppm); Serial.println(" ppm");
  Serial.print("Toluene: "); Serial.print(toluene_ppm); Serial.println(" ppm");
  Serial.print("Ethanol: "); Serial.print(ethanol_ppm); Serial.println(" ppm");
  Serial.print("Methanol: "); Serial.print(methanol_ppm); Serial.println(" ppm");

  Serial.println("--------------------------------");
}

void setup() {
  Serial.begin(115200);

  // Initialize Blynk, DHT, and Timer
  Blynk.begin(auth, ssid, pass);
  dht.begin();

  // Set interval to send sensor data
  timer.setInterval(2000L, sendSensorData);
}

void loop() {
  Blynk.run();
  timer.run();
}   