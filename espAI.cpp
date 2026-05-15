#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid = "BAYSTREETPROPERTY";
const char* password = "Bay2025!";
const char* host = "myproj.up.railway.app";

String input_str = "                ";
String prev_input_str = "                ";

int prev_col_state = 0;
bool is_timer = false;
bool change_input = false;
unsigned long last_time = 0;
const unsigned long AMOUNT = 3000;

void setup() {
  Serial.begin(9600);   
  Serial1.begin(9600);
  delay(1000);

  // connect WiFi
  WiFi.begin(ssid, password);
  Serial.println("LCD_TOP:Connecting WiFi ");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println("LCD_TOP:WiFi Connected!");
  delay(1000);
  Serial.println("LCD_CLEAR");
}


void sendToServer(String value) {
  WiFiClientSecure client;
  client.setInsecure(); // skip cert verification for now

  if (!client.connect(host, 443)) {
    Serial.println("LCD_BOT:Server failed  ");
    return;
  }

  String payload = "{\"value\":\"" + value + "\"}";
  String http = "POST /update HTTP/1.1\r\n"
                "Host: " + String(host) + "\r\n"
                "Content-Type: application/json\r\n"
                "Content-Length: " + String(payload.length()) + "\r\n"
                "Connection: close\r\n"
                "\r\n" + payload;

  client.print(http);
  //Serial.println("LCD_BOT:Sent!           ");
}

void loop() {

   if (Serial.available()) {
    String msg = Serial.readStringUntil('\n');
    msg.trim();
    
    Serial.println("Received from Arduino:");
    Serial.println(msg);
    sendToServer(msg);

    // here you can use msg for search, wifi, etc
  }
  //sendToServer(input_str);
}