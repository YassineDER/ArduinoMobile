#include <Wire.h>
#include <U8x8lib.h>
#include <SparkFunHTU21D.h>
#include <WiFi.h>
#include <BH1750.h>
#include <ArduinoMqttClient.h>

U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(15, 4, 16);
BH1750 lightMeter;

const char *ssid = "It hertz when IP";
const char *pass = "culdevache";
const char *broker = "141.145.203.36";
const int port = 1883;
const char *GENERAL = "arduino/network";
const char *DEBUG = "arduino/debug";
HTU21D myTempHumi;
WiFiClient client;
MqttClient mqtt(client);

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
  char out[50 + strlen(ssid)];
  sprintf(out, "Connecting to %s", ssid);
  show(out);

  WiFi.begin(ssid, pass);
  delay(500);
  while (WiFi.status() != WL_CONNECTED)
    delay(750);

  publish("Connected to Wifi", DEBUG);
  connectToMqtt();
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    show("Wifi lost. Reconnecting");
    WiFi.begin(ssid, pass);
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

void publish(const char *message, const char *topic)
{
  mqtt.beginMessage(topic);
  mqtt.print(message);
  mqtt.endMessage();
}

void connectToMqtt()
{
  mqtt.setId("esp32");
  mqtt.setUsernamePassword("ev3", "omelette");

  while (!mqtt.connect(broker, port))
  {
    show("MQTT error");
    delay(2500);
  }

  // mqtt.onMessage(onMqttMessage);
  mqtt.subscribe(GENERAL);
  mqtt.subscribe(DEBUG);
  publish("Connected and subscribed to MQTT", DEBUG);
}

/* void onMqttMessage(int messageSize)
{
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
} */

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
