/*******************************************************/
/*            Project Name: IoT Platform               */
/*                Author: CSE Junkies                  */
/*                   Version: 1                        */
/*                Date: 3 Dec, 2023                    */
/*******************************************************/

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <WebSocketsClient.h>
#include <SocketIOclient.h>
#include <Hash.h>

/* Wi-Fi credentials */
const char* wifiSSID = "IEEE ZSB";
const char* wifiPassword = "for-the-stars";

unsigned long lastEmitTime = 0;
const int emitInterval = 2000; /* Emit every 2 seconds */

/* MPU6050 sensor */
Adafruit_MPU6050 mpuSensor;

/* Socket.IO client */
SocketIOclient socketIOClient;

/* Sensor events */
sensors_event_t accelerometerEvent, gyroscopeEvent, temperatureEvent;

/* LED pins */
#define LED_PIN_1 D7
#define LED_PIN_2 D8
#define LED_PIN_3 D5

/* Function prototypes */
void handleJsonData(const char *source, int value);
void handleSocketIoAction(const char *payload);
void socketIOEventCallback(socketIOmessageType_t type, uint8_t *payload, size_t length);
void updateMpuSensor();

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println("Adafruit MPU6050 test!");

  /* Initialize MPU6050 sensor */
  if (!mpuSensor.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  /* Connect to Wi-Fi */
  WiFi.begin(wifiSSID, wifiPassword);
  Serial.print("Connecting to ");
  Serial.print(wifiSSID);
  Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(++i);
    Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());

  /* Initialize Socket.IO client */
  socketIOClient.begin("192.168.8.136", 3001, "/socket.io/?EIO=4");
  socketIOClient.onEvent(socketIOEventCallback);

  /* Set up LED pins */
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  pinMode(LED_PIN_3, OUTPUT);
}

void loop() {
  socketIOClient.loop();
  unsigned long currentTime = millis();

  /* Emit data to the server every 2 seconds */
  if (currentTime - lastEmitTime >= emitInterval) {
    emitDataToServer();
    lastEmitTime = currentTime;
  }
}

void handleJsonData(const char *source, int value) {
  /* Implement your actions based on the source and value */
  if (strcmp(source, "switch1") == 0) {
    Serial.print("Switch1 is ");
    Serial.println(value ? "ON" : "OFF");
    digitalWrite(LED_PIN_1, value);
  } else if (strcmp(source, "switch2") == 0) {
    Serial.print("Switch2 is ");
    Serial.println(value ? "ON" : "OFF");
    digitalWrite(LED_PIN_2, value);
  } else if (strcmp(source, "slider") == 0) {
    Serial.print("Slider value: ");
    Serial.println(value);
    analogWrite(LED_PIN_3, map(value, 0, 20, 0, 1023));
  } else {
    Serial.println("Unknown source");
  }
}

void handleSocketIoAction(const char *payload) {
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, payload);

  /* Check for parsing errors */
  if (error) {
    Serial.print("JSON parsing error: ");
    Serial.println(error.c_str());
  } else {
    /* Check if the JSON data contains "action" key */
    if (doc[0] == "action") {
      const char *source = doc[1]["source"];
      int value = doc[1]["value"];

      /* Handle the action */
      handleJsonData(source, value);
    } else {
      Serial.println("Unknown JSON data");
    }
  }
}

void socketIOEventCallback(socketIOmessageType_t type, uint8_t *payload, size_t length) {
  switch (type) {
    case sIOtype_DISCONNECT:
      Serial.println("[IOc] Disconnected!");
      break;
    case sIOtype_CONNECT:
      Serial.printf("[IOc] Connected to url: %.*s\n", length, (const char *)payload);
      socketIOClient.send(sIOtype_CONNECT, "/");
      break;
    case sIOtype_EVENT:
      Serial.printf("[IOc] get event: %.*s\n", length, (const char *)payload);
      handleSocketIoAction((const char *)payload);
      break;
    case sIOtype_ACK:
      Serial.printf("[IOc] get ack: %u\n", length);
      hexdump(payload, length);
      break;
    case sIOtype_ERROR:
      Serial.printf("[IOc] get error: %u\n", length);
      hexdump(payload, length);
      break;
    case sIOtype_BINARY_EVENT:
      Serial.printf("[IOc] get binary: %u\n", length);
      hexdump(payload, length);
      break;
    case sIOtype_BINARY_ACK:
      Serial.printf("[IOc] get binary ack: %u\n", length);
      hexdump(payload, length);
      break;
  }
}

void emitDataToServer() {
  /* Create JSON message for Socket.IO (event) */
  DynamicJsonDocument doc(1024);
  JsonArray array = doc.to<JsonArray>();

  /* Add event name */
  /* Hint: socket.on('event_name', .... */
  array.add("data");

  /* Add payload (parameters) for the event */
  JsonObject payload = array.createNestedObject();
  
  /* Get new sensor events with the readings */
  mpuSensor.getEvent(&accelerometerEvent, &gyroscopeEvent, &temperatureEvent);

  payload["source"] = "sensor";
  payload["accelerationX"] = accelerometerEvent.acceleration.x;
  payload["accelerationY"] = accelerometerEvent.acceleration.y;
  payload["accelerationZ"] = accelerometerEvent.acceleration.z;

  /* JSON to String (serialization) */
  String output;
  serializeJson(doc, output);

  /* Send event */
  socketIOClient.sendEVENT(output.c_str(), output.length());

  /* Print JSON for debugging */
  Serial.println(output);
}

void updateMpuSensor() {
  /* Get new sensor events with the readings */
  mpuSensor.getEvent(&accelerometerEvent, &gyroscopeEvent, &temperatureEvent);
}
