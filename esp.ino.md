# ESP8266 Relay Control System

This project uses an ESP8266 to control a relay module and monitor temperature, with MQTT communication for remote control and monitoring. Below are the details of the pin definitions, MQTT topics, and important notes.

## Pin Definitions (ESP8266)

Below are the pins used in the code, mapped to their MCU board names (printed on the ESP8266 board) and their code names:

| **Function**         | **MCU Board Pin (Printed Name)** | **Code Name**         |
|-----------------------|----------------------------------|-----------------------|
| Temperature Sensor   | A0 (Analog Pin)                 | `TEMP_SENSOR_PIN`     |
| Relay 1 (Normal)     | D1                              | `RELAY1_PIN`          |
| Relay 2 (Normal)     | D2                              | `RELAY2_PIN`          |
| Relay 3 (Normal)     | D5                              | `RELAY3_PIN`          |
| Alarm Relay (4th)    | D6                              | `ALARM_RELAY_PIN`     |
| Fan Relay (Loose)    | D7                              | `FAN_RELAY_PIN`       |

> **Note**: Ensure your relay module is active LOW (i.e., setting the pin LOW turns the relay ON). If your relay module is active HIGH, you'll need to invert the logic in the code (e.g., use `HIGH` to turn ON and `LOW` to turn OFF).

## MQTT Topics to Create on Raspberry Pi

You need to set up the following MQTT topics on your Raspberry Pi MQTT broker (e.g., Mosquitto):

### Publish Topics

These are the topics to which the ESP8266 publishes data:

- **`home/sensors/temperature`**: Publishes the temperature sensor reading.
- **`home/relays/status`**: Publishes the current state of all relays (comma-separated, including alarm and fan relays for monitoring).

### Subscribe Topics

These are the topics to which the ESP8266 subscribes to receive commands:

- **`home/relays/relay1`**: Receives commands to control Relay 1 (e.g., publish `"1"` to turn ON, `"0"` to turn OFF).
- **`home/relays/relay2`**: Receives commands to control Relay 2.
- **`home/relays/relay3`**: Receives commands to control Relay 3.

> **Note**: There are no subscribe topics for the alarm relay or fan relay, as they are reserved for temperature-based automation and not controllable via MQTT.
