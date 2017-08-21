#include <ESPert.h>

ESPert espert;

const char* mqtt_server = "mqtt.espert.io";
const char* ssid = "20 Laboratory";
const char* password = "mg;o9uhld^Ux";
String inTopic = "UP_Team_A/LED";
int PIN_LED = 12;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  WiFi.begin (ssid, password);
  espert.init();
  espert.oled.init();
  delay(2000);

  espert.oled.clear();
  espert.oled.println(espert.info.getId());
  espert.oled.println();

  int mode = espert.wifi.init();

  if (mode == ESPERT_WIFI_MODE_CONNECT) {
    espert.println(">>> WiFi mode: connected.");
    espert.oled.println("WiFi: connected.");
    espert.oled.print("IP..: ");
    espert.oled.println(espert.wifi.getLocalIP());
    espert.mqtt.init(mqtt_server, 1883, callback);
  } else if (mode == ESPERT_WIFI_MODE_DISCONNECT) {
    espert.println(">>> WiFi mode: disconnected.");
    espert.oled.println("WiFi: not connected.");
  }
}

void loop() {
  if (espert.mqtt.connect()) {
    espert.println("MQTT: Connected");
    espert.mqtt.subscribe(inTopic);
    Serial.println("MQTT: Subscribed " + inTopic);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String strPayload = String((char*)payload).substring(0, length);
  espert.println("Receive: " + strPayload);
  if (strPayload == "on") {
    espert.println("on led");
    digitalWrite(PIN_LED, HIGH);
  } else if (strPayload == "off") {
    espert.println("off led");
    digitalWrite(PIN_LED, LOW);
  }
}
