# ArduinoMobile

This is a simple project to control an Arduino board using an Android device. The project is divided into two parts: the web app acting as an MQTT client by displaying a dashboard and the Arduino sketch that emits data to the MQTT broker.

Demo URL: [ArduinoMobile](https://yassineder.github.io/ArduinoMobile/)

For the arduino sketch, open the folder `ArduinoMobile` in the Arduino IDE and upload the sketch to the board to make sure the headers are included correctly.

The following libraries are required to compile the sketch:
- `ArduinoMqttClient` for MQTT communication
- `WiFi` for Wifi connection
- `BH1750` for the light sensor
- `SparkFunHTU21D` for the temperature and humidity sensor
- `U8g2` for the OLED display

Dont forget to install `esp32` board support in the Arduino IDE and select the correct board in the `Tools` menu which is `Heltec WiFi LoRa 32`.

## Requirements

- Arduino IDE to compile and upload the sketch to the board
- An ESP32 board of course, with the necessary libraries installed
- The Wifi credentials must be set in the sketch header `MainSketch.h` as well as the MQTT broker credentials if using a different server.

## Documentation

The project is documented [here](https://www.overleaf.com/read/kcbrkyfscrjk#a0183e) (read-only).
