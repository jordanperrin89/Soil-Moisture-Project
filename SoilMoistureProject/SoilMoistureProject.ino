#include <WiFiNINA.h>
#include "arduino_credentials.h" // holds the values for the thingspeak API Key, channel ID and wifi details
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

char ssid[] = MY_SSID;    //  your network SSID (name) 
char pass[] = MY_PASS;   // your network password

WiFiClient  client;

unsigned long myChannelNumber = CH_ID;
const char * myWriteAPIKey = WRITE_APIKEY;


const int dry = 460; // dry soil value
const int wet = 117; // value when soil is freshly watered 

void setup() {
  Serial.begin(115200);  // Initialize Serial
  ThingSpeak.begin(client);  //Initialize ThingSpeak
  pinMode(13, OUTPUT);
}

void loop() {

  // Connect or reconnect to WiFi, I kept this in the void loop in case the user disconnects during use.
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(MY_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass); 
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
    

   }
// sets sensorVal to the A0 pin, where the soil moisture sensor input is.
int sensorVal = analogRead(A0);
// maps the highest and lowest possible sensor values to 100 and 0, making moistPercent 
int moistPercent = map(sensorVal, wet, dry, 100, 0);

// turns the LED on when the % of "moistness" is 50% or lower.
if (moistPercent < 51) {
  digitalWrite(13, HIGH);
} else {
  digitalWrite(13, LOW);
}

int x = ThingSpeak.writeField(myChannelNumber, 1, moistPercent, myWriteAPIKey); // writes the value of moistPercent to the thingspeak channel, value number one.
Serial.print("Value sent to ThingSpeak: "); // Serial prints to console to check it's working while plugged in direct to a PC.
Serial.print(moistPercent);
Serial.print("%\n");
delay(30000); // wait 30 seconds between readings, soil moisture doesnt change much over time.
}


