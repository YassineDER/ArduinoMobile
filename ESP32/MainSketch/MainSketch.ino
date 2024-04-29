#include <MainSketch.h>
#include <Wire.h>
#include <U8x8lib.h>
#include <SparkFunHTU21D.h>
#include <WiFi.h>
#include <BH1750.h>
#include <ArduinoMqttClient.h>

const char *GENERAL = "arduino/network";
const char *DEBUG = "arduino/debug";

U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(15, 4, 16);
BH1750 lightMeter;
HTU21D myTempHumi;
WiFiClient client;
MqttClient mqtt(client);

// This function is called once at start up
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Wire.begin();
  u8x8.begin();
  u8x8.clear();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  myTempHumi.begin();
  lightMeter.begin();

  WiFi.disconnect(true);
  char out[50 + strlen(WIFI_SSID)];
  sprintf(out, "Connecting to %s", WIFI_SSID);
  show(out);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  delay(500);
  while (WiFi.status() != WL_CONNECTED)
    delay(750);

  publish("Connected to Wifi", DEBUG);
  connectToMqtt();
}

// This function is called repeatedly
void loop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    show("Wifi lost. Reconnecting");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(1500);
    return;
  }
  show("Wifi connected");

  mqtt.poll();
  if (!mqtt.connected())
  {
    show("Reconnecting to MQTT");
    connectToMqtt();
    return;
  }
  show("MQTT connected");
  digitalWrite(LED_BUILTIN, HIGH);
  
  publish("Calling sensors", DEBUG);
  float temp = myTempHumi.readTemperature();
  float humi = myTempHumi.readHumidity();
  float lux = lightMeter.readLightLevel();
  char data[100];
  sprintf(data, "{\"temp\":%.2f,\"humi\":%.2f,\"lux\":%.2f}", temp, humi, lux);
  publish(data, GENERAL);
  
  digitalWrite(LED_BUILTIN, LOW);
  delay(1500);
}


// This function is used to publish a message to the MQTT broker based on the topic provided
void publish(const char *message, const char *topic)
{
  mqtt.beginMessage(topic);
  mqtt.print(message);
  mqtt.endMessage();
}


// This function connects to the MQTT broker and subscribes to the topics
void connectToMqtt()
{
  mqtt.setId("esp32");
  mqtt.setUsernamePassword(MQTT_USERNAME, MQTT_PASSWORD);

  while (!mqtt.connect(MQTT_BROKER, MQTT_PORT))
  {
    show("MQTT error");
    delay(2500);
  }

  // mqtt.onMessage(onMqttMessage);
  mqtt.subscribe(GENERAL);
  mqtt.subscribe(DEBUG);
  publish("Connected and subscribed to MQTT", DEBUG);
}



// This function is not used in this sketch, but it's a good example of how to handle incoming MQTT messages
void onMqttMessage(int messageSize)
{
  // Light up the LED based on the message received (ON or OFF)
  char msg[6];
  int i = 0;
  while (mqtt.available() && i < 5){
    msg[i] = (char)mqtt.read();
    i++;
  }
  msg[i] = '\0';

  if (strcmp(msg, "ON") == 0)
    digitalWrite(LED_BUILTIN, HIGH);
  else if (strcmp(msg, "OFF") == 0)
    digitalWrite(LED_BUILTIN, LOW);
}


// This function is used to display a message on the OLED display
void show(const char *message)
{
  u8x8.clear();
  char lineBuffer[16]; // Buffer for one line of display, 15 chars + null terminator
  int msgLength = strlen(message);
  int lineStart = 0;
  int lineCount = 0;

  while (lineStart < msgLength)
  {
    if (msgLength - lineStart <= 15)
    {
      strcpy(lineBuffer, message + lineStart);
      u8x8.drawString(0, lineCount, lineBuffer);
      break;
    }
    int spaceIndex = -1;
    for (int i = 0; i < 15; i++)
    {
      if (message[lineStart + i] == ' ')
        spaceIndex = i;
    }

    if (spaceIndex == -1)
      spaceIndex = 14;
    strncpy(lineBuffer, message + lineStart, spaceIndex + 1);
    lineBuffer[spaceIndex + 1] = '\0';
    u8x8.drawString(0, lineCount, lineBuffer);

    lineStart += spaceIndex + 1;
    lineCount++;
    if (lineCount == 8)
      break;
  }
}
