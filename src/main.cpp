#include <WiFi.h>
#include <WiFiClient.h>
#include <ESP32Servo.h>

const char*  ssid = "TP-Link_21B5";
const char*  password = "26796666";
const int port = 1234; // The port number to listen on
const int maxDataLength = 256;

WiFiServer server(port);
WiFiClient client;
int recieved_u;
#define TopESC_PIN 14
Servo Topesc;
uint8_t rx_uint8 = 0;
uint8_t rx_byte[2] = {0};
uint16_t rx_uint16 = 0;

uint16_t AssembleUInt16(uint8_t highByte, uint8_t lowByte);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP:   ");
  Serial.println(WiFi.localIP());
  server.begin();

  Topesc.attach(TopESC_PIN, 1000, 2000);
  Topesc.writeMicroseconds(1500);
}

void loop() {
  if (client.connected()) {
    while (client.available()) {
      // Read incoming data from Simulink
      /* rx 8bit */
      rx_uint8 = client.read();
      Serial.printf("Rx: %4d\n",rx_uint8);
      /* rx 16bit(unverified) */
      // client.readBytes(rx_byte,2);
      // AssembleUInt16(rx_byte[0],rx_byte[1]);
      // Serial.printf("Rx: %4d\n",rx_uint16);
      Topesc.writeMicroseconds(rx_uint8);
    }
  } else {
    // Wait for a new client to connect
    client = server.available();
  }
}

uint16_t AssembleUInt16(uint8_t highByte, uint8_t lowByte) {
  return (static_cast<uint16_t>(highByte) << 8) | lowByte;
}
