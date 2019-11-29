#include <WiFi.h> 
#include <WebServer.h>

/* Put your SSID & Password */ 
const char* ssid= "msa";  // Enter SSID here 
const char* password = "123123321";  //Enter Password here

float vref=3.3;
float resolusi= vref/4095;
WebServer server(80);

uint8_t LM35=34;
float Temperature;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600); 
pinMode(LM35, INPUT); 
Serial.println("Connecting to ");
Serial.println(ssid); //connect to your local wi-fi network 
WiFi.begin(ssid, password); //check wi-fi is connected to wi-fi network 
while (WiFi.status() != WL_CONNECTED) { 
  delay(1000); Serial.print("."); 
  } 
  Serial.println(""); 
  Serial.println("WiFiconnected..!"); 
  Serial.print("Got IP: "); 
  Serial.println(WiFi.localIP()); 
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound); 
  server.begin(); 
  Serial.println("HTTP server started"); 
  }


void loop(){
server.handleClient();
}
void handle_OnConnect() {
  Temperature = analogRead(LM35); // Gets the values of the temperature 
  Temperature = (Temperature*resolusi);
  Temperature = (Temperature *100);
  server.send(200, "text/html", SendHTML(Temperature)); 
}
void handle_NotFound() { 
  server.send(404, "text/plain", "Not found"); 
  } 
  String SendHTML(float Temperaturestat) { 
  String ptr= "<!DOCTYPE html> <html>\n"; 
  ptr += "<title>ESP32 Weather Report</title>\n"; 
  ptr += "</head>\n"; 
  ptr+= "<body>\n"; 
  ptr += "<div id=\"webpage\">\n"; 
  ptr += "<h1>ESP32 Weather Report</h1>\n";
  ptr += "<p>Temperature: "; 
  ptr += (float)Temperaturestat;
  ptr += "°C</p>";
  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n"; 
  return ptr;
 }
 
