# Convyor esp8266 setup 
  - Downloading esp8266 lib: https://github.com/esp8266/Arduino
  - Youute tutorial for Downloading esp8266 lib : Downloading esp8266 lib

## ESP8266 Stepper Motor Control with ULN2003APG (Bare IC)

**Connections:**
**ULN2003APG PIN DIAGRAM :**

![ssss](https://github.com/user-attachments/assets/89812cd6-715e-4041-b18e-189c7cd13992)

**1. ESP8266 -> ULN2003APG:**

| ESP8266 Pin | ULN2003APG Pin | Description       |
| :---------- | :------------- | :---------------- |
| D1          | 1 (IN1)        | Input 1           |
| D2          | 2 (IN2)        | Input 2           |
| D3          | 3 (IN3)        | Input 3           |
| D4          | 4 (IN4)        | Input 4           |
| GND         | 8 (GND)        | Ground            |
|             |                | **ALSO connect ESP8266 GND to Stepper Motor Power Supply `-` (Negative)** |

**2. ULN2003APG -> Stepper Motor (Common Color Scheme 1):**

| ULN2003APG Pin | Stepper Motor Wire (Possible Color) | Description |
| :------------- | :--------------------------------- | :---------- |
| 16 (OUT1)      | Orange                             | Coil 1      |
| 15 (OUT2)      | Yellow                             | Coil 2      |
| 14 (OUT3)      | Brown                              | Coil 3      |
| 13 (OUT4)      | Black                              | Coil 4      |
| 9 (COM)       | Stepper Motor Power Supply `+`      | Flyback     |

**3. Stepper Motor -> Stepper Motor Power Supply:**

| Stepper Motor Wire (Possible Color) | Connection                               | Description    |
| :--------------------------------- | :--------------------------------------- | :------------- |
| Red (and often a second common, maybe White) | Stepper Motor Power Supply `+` (Positive) | Common Wires |

**Alternative Stepper Motor Wiring (Common Color Scheme 2 - if Scheme 1 doesn't work):**

| ULN2003APG Pin | Stepper Motor Wire (Possible Color) |
| :------------- | :--------------------------------- |
| 16 (OUT1)      | Blue                              |
| 15 (OUT2)      | Orange                             |
| 14 (OUT3)      | Yellow                             |
| 13 (OUT4)      | Green                             |
| 9 (COM)        | Stepper motor Power supply `+`    |

**Keep Red (and White if present) connected to Stepper motor Power Supply `+`**

---

