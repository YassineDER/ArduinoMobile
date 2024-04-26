# ArduinoMobile

This is a simple project to control an Arduino board using an Android device. The project is divided into two parts: the web app acting as an MQTT client by displaying a dashboard and the Arduino sketch that emits data to the MQTT broker.

Demo URL: [ArduinoMobile](https://yassine.onthewifi.com)

## Requirements

- Arduino IDE to compile and upload the sketch to the board
- An ESP32 board of course, with the necessary libraries installed
- The Wifi credentials must be set in the sketch header `MainSketchSecrets.h` as well as the MQTT broker credentials if using a different server.

## Documentation

The project is documented [here](https://www.overleaf.com/read/kcbrkyfscrjk#a0183e) (read-only).
