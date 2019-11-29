#include "BluetoothSerial.h"
//declare suhu internal
#ifdef __cplusplus
  extern "C" {
 #endif
 
  uint8_t temprature_sens_read();
 
#ifdef __cplusplus
}
#endif
 
uint8_t temprature_sens_read();
// define bluetooth
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

const int LED=2;
const int LED2=4;

BluetoothSerial SerialBT;

String message = ""; 
String messages = ""; 
int incomingint;
char incomingChars; 
String temperatureString= "";
// Timer: auxiliarvariables
unsigned long previousMillis= 0;    // Stores last time temperature was published
const long interval = 10000;         // interval at which to publish sensor readings

void setup() {
  pinMode(LED, OUTPUT);
   pinMode(LED2, OUTPUT);
  Serial.begin(115200);
  // Bluetooth device name
  SerialBT.begin("ESP32");
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop() {
  unsigned long currentMillis = millis();
  // Send temperature readings
  if (currentMillis - previousMillis >= interval){
    previousMillis = currentMillis;
    temperatureString = ((temprature_sens_read() - 32) / 1.8);
    Serial.println( temperatureString );
//    SerialBT.print("Suhu : "); 
//    SerialBT.println(temperatureString); 
    
  }
  // Read received messages (LED control command)
if (Serial.available()) {
  
  incomingint= Serial.parseInt();
    Serial.println(incomingint);  
}
if (incomingint== 1){
  digitalWrite(LED, HIGH);
}
else if (incomingint==2){
  digitalWrite(LED, LOW);
}
  
  if (SerialBT.available()){
    char incomingChar = SerialBT.read();
    if (incomingChar != '\n'){
      message += String(incomingChar);
    }
    else{
      message = "";
    }
    Serial.write(incomingChar);  
  }
  // Check received message and control output accordingly
  
  if (messages =="a"){
    digitalWrite(LED, HIGH);
  }
 if (messages =="c"){
    digitalWrite(LED, LOW);
  }
  
//  if (message =="led_on"){
//    digitalWrite(LED, HIGH);
//  }
//  else if (message =="led_off"){
//    digitalWrite(LED, LOW);
//  }
// delay(20);
// if (message =="led2_on"){
//    digitalWrite(LED2, HIGH);
//  }
//  else if (message =="led2_off"){
//    digitalWrite(LED2, LOW);
//  }
//  delay(20);
}

