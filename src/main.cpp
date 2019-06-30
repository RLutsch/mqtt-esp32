#include <SPI.h>
#include <WiFi.h>
#include <MQTT.h>

const char ssid[] = "roomwithaview";
const char pass[] = "xingchaocarly";
const int LEDPIN = 22;
const int PushButton = 15;

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("arduino", "try", "try")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("topic/state");
  // client.unsubscribe("/hello");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  if (payload == "OFF"){
    digitalWrite(LEDPIN, LOW);
  }
  if (payload == "ON"){
    digitalWrite(LEDPIN, HIGH);
  }

}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  pinMode(LEDPIN, OUTPUT);
  pinMode(PushButton, INPUT);
  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported by Arduino.
  // You need to set the IP address directly.
  client.begin("192.168.0.40", net);
  client.onMessage(messageReceived);

  connect();
}

void loop() {
  client.loop();
  int Push_button_state = digitalRead(PushButton);
  if (!client.connected()) {
    connect();
  }

  // publish a message roughly every second.
  if (Push_button_state == HIGH) {
    client.publish("topic/state", "ON");
    digitalWrite(LEDPIN, HIGH);
    delay(500);
  }

}
