#include <WiFi.h>
#include <PubSubClient.h>

#ifdef __cplusplus
  extern "C" {
 #endif
 
  uint8_t temprature_sens_read();
 
#ifdef __cplusplus
}
#endif
 
uint8_t temprature_sens_read();
float temperature = 0;
const int touchPin= 4; 
float touchValue;
// Replace the next variables with your SSID/Password combination
const char* ssid = "Saparhadi";
const char* password = "ahahahaha";
// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "192.168.43.115";
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
// LED Pin
const int ledPin = 2;
void setup() {
Serial.begin(115200);
// default settings
setup_wifi();
client.setServer(mqtt_server, 1883);
client.setCallback(callback);
pinMode(ledPin, OUTPUT);
//pinMode(LDRPin, INPUT);
}
void setup_wifi() {
delay(10);
// We start by connecting to a WiFi network
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
}
void callback(char* topic, byte* message, unsigned int length) {
Serial.print("Message arrived on topic: ");
Serial.print(topic);
Serial.print(". Message: ");
String messageTemp;
for (int i = 0; i < length; i++) {
Serial.print((char)message[i]);
messageTemp += (char)message[i];
}
Serial.println();
// Feel free to add more if statements to control more GPIOs with MQTT
// If a message is received on the topic esp32/output, you check if the message is either "on"or "off".
// Changes the output state according to the message
if (String(topic) == "esp32/output") {
Serial.print("Device data received : ");
//Serial.println(messageTemp);
if(messageTemp == "on"){
Serial.println("on");
digitalWrite(ledPin, HIGH);
}
else if(messageTemp == "off"){
Serial.println("off");
digitalWrite(ledPin, LOW);
}
}
}
void reconnect() {
// Loop until we're reconnected
while (!client.connected()) {
Serial.print("Attempting MQTT connection...");
// Attempt to connect
if (client.connect("ESP32Client")) {
Serial.println("connected");
// Subscribe
client.subscribe("esp32/output");
} else {
Serial.print("failed, rc=");
Serial.print(client.state());
Serial.println(" try again in 5 seconds");
// Wait 5 seconds before retrying
delay(5000);
}
}
}
void loop() {
if (!client.connected()) {
reconnect();
}
client.loop();
long now = millis();
if (now - lastMsg > 1000) {
lastMsg = now;
temperature = ((temprature_sens_read() - 32) / 1.8);
touchValue= touchRead(touchPin); 
//intensity = analogRead(LDRPin);
// Convert the value to a char array
char temperatureString[8];
dtostrf(temperature, 1, 2, temperatureString);
touchValue= touchRead(touchPin); 
char touchString[8];
dtostrf(touchValue, 1, 2, touchString);
Serial.print("Send suhu: ");
Serial.print(temperatureString);
Serial.print("    ");
Serial.print("Send touch: ");
Serial.println(touchString);
String payloadStr = buildJson();
char payload[payloadStr.length()+1];
payloadStr.toCharArray(payload, payloadStr.length()+1);
// Serial.println("Sending payload: ");
// Serial.println(payload);
client.publish("esp32/SUHU", payload);
client.publish("esp32/touch", payload);
client.publish("esp32/SUHU", temperatureString);
client.publish("esp32/touch", touchString);
}
}
String buildJson() {
String data = "{";
data += "\n";
data += "\"timestamp\": \"";
data += lastMsg;
data += "\",";
data += "\n";
data += "\"SUHU\": ";
data += (int)temperature;
data += "  ||  ";
data += "\"touch\": ";
data += (int)touchValue;
data += "\n";
data += "}";
return data;
}
