<<<<<<< HEAD
# Arduino_Projects
Open source random Arduino  projects

using Processing and C++

enjoy :)
=======
# ESP32 SD Card Tester with OLED Interface


A professional-grade SD card testing tool built with ESP32, featuring OLED display, battery monitoring, and comprehensive card diagnostics.

## Features
- **SD Card Validation**
  - Read/write functionality test
  - Full card capacity verification
  - File system integrity check
- **Advanced Diagnostics**
  - Card type detection (SD/SDHC/MMC)
  - Storage capacity measurement
  - Used/free space analysis
- **Safety Features**
  - Format confirmation dialog
  - Recursive directory formatting
  - Battery level monitoring (3.2V-4.2V range)
- **User Interface**
  - OLED display menu system
  - Progress bar animations
  - Audible feedback via buzzer
  - Two-button navigation

## Hardware Requirements
| Component              | Specification                     |
|------------------------|-----------------------------------|
| Microcontroller        | ESP32 Dev Board                   |
| SD Card Module          | SPI Interface (3.3V logic)        |
| Display                | 128x64 OLED (SSD1306, I2C)        |
| Battery Monitor        | Voltage divider (10kΩ + 20kΩ)     |
| Buttons                | 2x Tactile switches               |
| Buzzer                 | Passive piezo buzzer              |

## Installation

### Software Requirements
1. [Arduino IDE](https://www.arduino.cc/en/software) 1.8.19+
2. Required Libraries:
   - `SD` (Built-in)
   - `SPI` (Built-in)
   - `SSD1306Wire` (OLED display)

### Hardware Connections
```plaintext
ESP32   | SD Card Module | OLED Display | Other
--------|----------------|--------------|---------
GPIO5   | CS             |              |
GPIO23  | MOSI           |              |
GPIO18  | CLK            |              |
GPIO19  | MISO           |              |
        |                |              |
GPIO21  |                | SDA          |
GPIO22  |                | SCL          |
GPIO25  |                |              | Buzzer
GPIO12  |                |              | Button A
GPIO14  |                |              | Button B
GPIO34  |                |              | Battery ADC


___________________________              

## Usage

Workflow

Initialization
Insert SD card
Hold Button A to begin
System performs automatic detection
Testing Phase
Write verification (test file creation)
Read verification (file contents check)
Capacity validation
Menu System
Button A: Show card info
Card type
Total capacity
Used/free space
Button B: Format card
Confirmation dialog
Full recursive format
Visual progress indicator
Battery Monitoring

Real-time voltage measurement
Percentage estimation (3.2V-4.2V range)
Low battery warning system
Troubleshooting

Issue	Solution
SD Card Not Detected	Check SPI connections
Formatting Fails	Verify card physical switch
Inaccurate Battery %	Confirm resistor values
Display Not Working	Verify I2C address (0x3C)
False Test Failures	Try different SD card
License

This project is licensed under the MIT License - see LICENSE for details.
>>>>>>> 9fdc9be366f1a0821cabe1b24d4b01657a544a7c
