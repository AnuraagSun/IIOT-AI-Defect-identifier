#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WiFi and MQTT credentials
const char *ssid = "";
const char *password = "";
const char *mqtt_server = ""; // Replace with your Raspberry Pi's IP address
const int mqtt_port = 1883;

// Pin Definitions (ESP8266)
#define TEMP_SENSOR_PIN A0        // Temperature sensor on A0
#define RELAY1_PIN D1             // Relay 1 on D1
#define RELAY2_PIN D2             // Relay 2 on D2
#define RELAY3_PIN D5             // Relay 3 on D5
#define ALARM_RELAY_PIN D6        // Alarm relay (4th relay on module) on D6
#define FAN_RELAY_PIN D7          // Fan relay (loose relay) on D7

// Temperature threshold for alarm and fan activation (in Celsius)
const float TEMP_THRESHOLD = 35.0; // Adjust this value as needed

// WiFi and MQTT clients
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// Function prototypes
void setupWiFi();
void reconnectMQTT();
void mqttCallback(char *topic, byte *payload, unsigned int length);
void parseCommand(const String &topic, const String &message);
void checkTemperature();
void updateRelayStatus();
float readTemperature();

// Setup function
void setup() {
  Serial.begin(115200);
  
  // Initialize relay pins as outputs
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(RELAY3_PIN, OUTPUT);
  pinMode(ALARM_RELAY_PIN, OUTPUT);
  pinMode(FAN_RELAY_PIN, OUTPUT);

  // Set initial state of relays to OFF (assuming active LOW relays)
  digitalWrite(RELAY1_PIN, HIGH);
  digitalWrite(RELAY2_PIN, HIGH);
  digitalWrite(RELAY3_PIN, HIGH);
  digitalWrite(ALARM_RELAY_PIN, HIGH);
  digitalWrite(FAN_RELAY_PIN, HIGH);

  // Setup WiFi and MQTT
  setupWiFi();
  mqttClient.setServer(mqtt_server, mqtt_port);
  mqttClient.setCallback(mqttCallback);
}

// Loop function
void loop() {
  if (!mqttClient.connected()) {
    reconnectMQTT();
  }
  mqttClient.loop();

  // Check temperature and control alarm/fan relays
  checkTemperature();

  // Simulate other sensor readings (optional, for demonstration)
  float temperature = readTemperature();
  
  // Publish temperature to MQTT
  String tempPayload = String(temperature);
  mqttClient.publish("home/sensors/temperature", tempPayload.c_str());
  Serial.print("Temperature published: ");
  Serial.println(temperature);

  // Publish relay status
  updateRelayStatus();

  delay(5000); // Publish every 5 seconds
}

// Function to setup WiFi
void setupWiFi() {
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// Function to reconnect to MQTT broker
void reconnectMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect with a unique client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("connected");
      // Subscribe to command topics for normal relays only
      mqttClient.subscribe("home/relays/relay1");
      mqttClient.subscribe("home/relays/relay2");
      mqttClient.subscribe("home/relays/relay3");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

// Callback function to handle incoming MQTT messages
void mqttCallback(char *topic, byte *payload, unsigned int length) {
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.print("Message received on topic: ");
  Serial.print(topic);
  Serial.print(" -> ");
  Serial.println(message);

  // Parse the command based on the topic
  parseCommand(String(topic), message);
}

// Function to parse incoming MQTT commands
void parseCommand(const String &topic, const String &message) {
  int relayState = message.toInt(); // Convert message to integer (0 or 1)

  // Control the appropriate relay based on the topic
  if (topic == "home/relays/relay1") {
    digitalWrite(RELAY1_PIN, relayState ? LOW : HIGH); // Active LOW
    Serial.println("Relay 1 set to " + String(relayState));
  } else if (topic == "home/relays/relay2") {
    digitalWrite(RELAY2_PIN, relayState ? LOW : HIGH);
    Serial.println("Relay 2 set to " + String(relayState));
  } else if (topic == "home/relays/relay3") {
    digitalWrite(RELAY3_PIN, relayState ? LOW : HIGH);
    Serial.println("Relay 3 set to " + String(relayState));
  }
  // Note: Alarm and fan relays are not controllable via MQTT
}

// Function to read temperature (simulated or from sensor)
float readTemperature() {
  // Simulate temperature reading (replace with actual sensor code if needed)
  // For a real sensor, use analogRead(TEMP_SENSOR_PIN) and convert to Celsius
  float temperature = random(20, 40) + random(0, 100) / 100.0; // Simulated value
  return temperature;
}

// Function to check temperature and control alarm/fan relays
void checkTemperature() {
  float temperature = readTemperature();
  if (temperature > TEMP_THRESHOLD) {
    // Turn ON alarm and fan relays
    digitalWrite(ALARM_RELAY_PIN, LOW); // Active LOW
    digitalWrite(FAN_RELAY_PIN, LOW);   // Sync fan relay
    Serial.println("High temperature detected! Alarm and fan relays ON.");
  } else {
    // Turn OFF alarm and fan relays
    digitalWrite(ALARM_RELAY_PIN, HIGH);
    digitalWrite(FAN_RELAY_PIN, HIGH);
    Serial.println("Temperature normal. Alarm and fan relays OFF.");
  }
}

// Function to publish the current state of all relays
void updateRelayStatus() {
  // Read the state of all relays (inverted logic for active LOW)
  int relay1State = digitalRead(RELAY1_PIN) == LOW ? 1 : 0;
  int relay2State = digitalRead(RELAY2_PIN) == LOW ? 1 : 0;
  int relay3State = digitalRead(RELAY3_PIN) == LOW ? 1 : 0;
  int alarmRelayState = digitalRead(ALARM_RELAY_PIN) == LOW ? 1 : 0;
  int fanRelayState = digitalRead(FAN_RELAY_PIN) == LOW ? 1 : 0;

  // Create a comma-separated string of relay states
  String relayStatus = String(relay1State) + "," +
                       String(relay2State) + "," +
                       String(relay3State) + "," +
                       String(alarmRelayState) + "," +
                       String(fanRelayState);

  // Publish relay status
  mqttClient.publish("home/relays/status", relayStatus.c_str());
  Serial.print("Relay status published: ");
  Serial.println(relayStatus);
}
