# ESP32-Learning

A hands-on learning log for the **ESP32**, built one small project at a time.

The goal is to get comfortable with the board itself and every peripheral it has, starting from simple GPIO and working up to more advanced features. Wi-Fi and Bluetooth are intentionally left for the end, so the fundamentals come first and the wireless projects can be done properly later.

## Hardware

- **Board:** ESP32 DevKit V1 (ESP-WROOM-32, 30 pins)
- **Framework:** ESP-IDF, developed in VS Code with the Espressif ESP-IDF extension
- **Language:** C
- **Parts used so far:** LEDs, 330 Ω / 1 kΩ resistors, breadboard, jumper wires, MG90S servo, 1000 µF capacitor, and the on-board BOOT button

## Roadmap

| Level | Topic | Status |
|---|---|---|
| 1 | GPIO and tasks | Done |
| 2 | Timers and PWM | In progress |
| 3 | Analog input/output (ADC, DAC, touch) | Planned |
| 4 | Communication (UART, I2C, SPI) | Planned |
| 5 | ESP32-specific features (dual core, flash storage, deep sleep, pulse counter) | Planned |
| 6 | Combined projects | Planned |
| Final | Wi-Fi and Bluetooth projects | Planned |

## Level 1: GPIO and tasks

Folder: [`Level-1_GPIO`](Level-1_GPIO)

| Project | What it does | Concepts |
|---|---|---|
| [`Led_ControllWith_Button`](Level-1_GPIO/Led_ControllWith_Button) | An LED follows the button, then toggles on each press | GPIO input and output, pull-up, debounce |
| [`Led_Control_with_Interrupt`](Level-1_GPIO/Led_Control_with_Interrupt) | The same toggle, but the button wakes the code through an interrupt instead of being polled | Interrupts, FreeRTOS queue, task |
| [`Two_Leds_Two_Tasks`](Level-1_GPIO/Two_Leds_Two_Tasks) | Two LEDs blink at different speeds, each driven by its own task | FreeRTOS tasks, delays |

### Wiring used in Level 1

| Part | ESP32 pin | Notes |
|---|---|---|
| LED 1 | GPIO26 | Through a 1 kΩ resistor to GND |
| LED 2 | GPIO27 | Through a 1 kΩ resistor to GND |
| Button | GPIO0 | The on-board BOOT button, active low |

The BOOT button shares its pin with a boot-mode pin, so avoid holding it while the board powers up or resets. Once the program is running it behaves like any other input.

## Level 2: Timers and PWM

Folder: [`Level-2_Timer_PWM`](Level-2_Timer_PWM)

| Project | What it does | Concepts |
|---|---|---|
| [`Hardware_Timer_Led`](Level-2_Timer_PWM/Hardware_Timer_Led) | A hardware timer toggles an LED every 500 ms | GPTimer, alarm callback, auto-reload |
| [`PWM_Led_Fade`](Level-2_Timer_PWM/PWM_Led_Fade) | An LED smoothly fades in and out | LEDC, 5 kHz PWM, 10-bit duty cycle |
| [`Servo_Sweep`](Level-2_Timer_PWM/Servo_Sweep) | Drives a servo back and forth with 1000–2000 µs pulses | LEDC, 50 Hz PWM, 14-bit resolution, pulse width control |

Next up: a buzzer melody.

### Wiring used in Level 2

| Part | ESP32 pin | Notes |
|---|---|---|
| LED | GPIO26 | Through a 330 Ω or 1 kΩ resistor to GND (timer and fade projects) |
| Servo signal (orange) | GPIO13 | PWM output |
| Servo power (red) | VIN | 5 V from USB; only for a single unloaded servo |
| Servo ground (brown) | GND | |
| 1000 µF capacitor | VIN and GND | Across the servo supply; mind the polarity |

For a loaded servo, use an external 5 V supply and connect its ground to the ESP32 ground.

## How to build and flash a project

1. Install VS Code and the **Espressif IDF** extension, then let it set up ESP-IDF.
2. Open **one project folder** (for example `Level-1_GPIO/Two_Leds_Two_Tasks`) in VS Code, not the parent folder. The extension looks for `CMakeLists.txt` in the folder you open.
3. Set the target to `esp32` and select the board's serial port.
4. Use the build, flash and monitor buttons in the bottom bar. The flame button does all three at once.

## Repository layout

```
ESP32-Learning/
├── README.md
├── .gitignore
├── Level-1_GPIO/
│   ├── Led_ControllWith_Button/
│   ├── Led_Control_with_Interrupt/
│   └── Two_Leds_Two_Tasks/
└── Level-2_Timer_PWM/
    ├── Hardware_Timer_Led/
    ├── PWM_Led_Fade/
    └── Servo_Sweep/
```

Build output, local editor settings and generated configuration files are excluded through `.gitignore`.
