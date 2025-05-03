/*
 * SD Card Tester
 * 
 * Copyright (C) 2024 Moutaz Baaj
 * 
 * Licensed under the MIT License.
 * You may obtain a copy of the License at
 * 
 *     https://opensource.org/licenses/MIT
 * 
 * This program is distributed "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * express or implied, including but not limited to the warranties of
 * merchantability, fitness for a particular purpose, and noninfringement.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * Author: Moutaz Baaj
 * Date: 2024-12-19
 */



#include <SD.h>
#include <SPI.h>
#include "SSD1306Wire.h"

#define SD_CS_PIN 5
#define SPEAKER_PIN 25

// Button settings
#define BUTTON_PIN_1 12

#define BUTTON_PIN_2 14

// ADC pin for battery monitoring
#define BATTERY_ADC_PIN 34 

// Resistor values (in ohms)
const float R1 = 10000.0; // 10k
const float R2 = 20000.0; // Two 10k resistors in series

// ADC reference voltage (ESP32 ADC range is 0 - 3.3V)
const float ADC_REF_VOLTAGE = 3.3;

// ADC resolution (12-bit ADC: 0 - 4095)
const int ADC_MAX_VALUE = 4095;


static unsigned long lastPress1 = 0;
static unsigned long lastPress2 = 0;
unsigned long debounceDelay = 200;
  
SSD1306Wire display(0x3C, 21, 22);



// Battery voltage calculation
float getBatteryVoltage() {
  int rawADC = analogRead(BATTERY_ADC_PIN);
  float voltageAtPin = (rawADC * ADC_REF_VOLTAGE) / ADC_MAX_VALUE;
  float batteryVoltage = voltageAtPin * ((R1 + R2) / R2);
  return batteryVoltage;
}

// Convert battery voltage to percentage
int getBatteryPercentage(float voltage) {
    if (voltage >= 3.0) return 100; // Fully charged (since it disconnects at 3.0V)
    else if (voltage >= 2.9) return map(voltage * 100, 290, 300, 90, 100);
    else if (voltage >= 2.8) return map(voltage * 100, 280, 290, 80, 90);
    else if (voltage >= 2.7) return map(voltage * 100, 270, 280, 70, 80);
    else if (voltage >= 2.6) return map(voltage * 100, 260, 270, 60, 70);
    else if (voltage >= 2.5) return map(voltage * 100, 250, 260, 50, 60);
    else if (voltage >= 2.4) return map(voltage * 100, 240, 250, 40, 50);
    else if (voltage >= 2.3) return map(voltage * 100, 230, 240, 30, 40);
    else if (voltage >= 2.2) return map(voltage * 100, 220, 230, 20, 30);
    else if (voltage >= 2.1) return map(voltage * 100, 210, 220, 10, 20);   
    else return 0; // Below 2.1V
}


float batteryVoltage = getBatteryVoltage();
int batteryPercentage = getBatteryPercentage(batteryVoltage);


// Draw battery icon with percentage
void drawBatteryIcon(int percentage) {
  // Icon position and size
  int x = display.getWidth() - 26; // Right corner 40
  int y = 0;
  int width = 20; //30
  int height = 12; //12

  // Draw battery outline
  display.drawRect(x, y, width, height);

  // Draw battery terminal
  display.fillRect(x + width, y + 3, 2, 6);

  // Calculate fill width based on percentage
  int fillWidth = map(percentage, 0, 100, 0, width - 2);

  // Draw fill
  display.fillRect(x + 1, y + 1, fillWidth, height - 2);

  // Show percentage
  String percentageText = String(percentage) + "%";
  display.drawString(x - display.getStringWidth(percentageText) - 5, y + 2, percentageText);
}

void beep() {
  tone(SPEAKER_PIN, 1000, 200);
  delay(300);
}

void restart() {

  if (digitalRead(BUTTON_PIN_2) == LOW && (millis() - lastPress2) > debounceDelay) {
    lastPress2 = millis();
    Serial.println("Restarting ESP32...");
    displayCenteredText("Restarting...");
    beep();
    delay(1000);
    ESP.restart();
  }

}

void showProgressBar(uint8_t progress) {
  display.clear();
  int16_t y = (display.getHeight() - 8) / 2;
  display.drawProgressBar(12, y, 100, 10, progress); // Adjust x, y, width, height as needed
  display.display();
}

void SDInit() {
  displayCenteredText("Initializing SD card...");

  for (int i = 0; i <= 100; i += 20) {
    showProgressBar(i); // Update progress
    delay(500);         // Simulate initialization steps
  }

  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD card initialization failed!");
    display.clear();
    display.drawString(28, 22, "SD init failed!");
    display.drawString(8, 38, "Hold Button B to Restart");
    display.display();
    beep();
   
     // Wait until Button B is pressed
     while (true) {
     restart();
     }
    return;
  }

  Serial.println("SD card initialized.");
  displayCenteredText("SD initialized.");
  beep();
  delay(2000);
}

void SDTest() {
  // Run the SD card test
  displayCenteredText("Testing SD card...");
  beep();
  delay(2000);

  // Write and Read Test
  File testFile = SD.open("/SDTester.txt", FILE_WRITE);
  if (testFile) {
    testFile.println("SD Card Tester");
    testFile.println("  ");
    testFile.println("  ");
    testFile.println("  ");
    testFile.println("SD card test successful.");
    testFile.println("  ");
    testFile.println("  ");
    testFile.println("  ");
    testFile.println("  ");
    testFile.println("  ");
    testFile.println("Copyright (C) 2024 Moutaz Baaj");
    testFile.println("Licensed under the MIT License.");
    testFile.println("For details, visit: https://opensource.org/licenses/MIT");
    testFile.close();

   for (int i = 0; i <= 100; i += 25) {
    showProgressBar(i); // Simulate formatting steps
    delay(500);         // Simulate delay for each step
  }
    displayCenteredText("Write Test Passed");
    beep();
    delay(1000);

    testFile = SD.open("/SDTester.txt");
    if (testFile) {
    for (int i = 0; i <= 100; i += 25) {
    showProgressBar(i); // Simulate formatting steps
    delay(500);         // Simulate delay for each step
      }
      displayCenteredText("Read Test Passed");
      beep();
      delay(1000);
      testFile.close();
    } else {
      display.clear();
      display.drawString(22, 22, "Read Test Failed!");
      display.drawString(8, 38, "Hold Button B to Restart");
      display.display();
      beep();
        while (true) {
    restart();
  }
      return;
    }
    
    displayCenteredText("Test Completed");
    beep();
    delay(2000);

  } else {
    display.clear();
    display.drawString(22, 22, "Write Test Failed!");
    display.drawString(8, 38, "Hold Button B to Restart");
    display.display();
    beep();
    while (true) {
    restart();
  }
    return;
  }
}

void formatSDCard() {
  beep();
  while (true) {
    display.clear();
    const char* text = "Formatting:";
    int16_t x = (display.getWidth() - display.getStringWidth(text)) / 2;
    display.drawString(x, 0, text);
    display.drawString(0, 18, "Confirm Formatting");
    display.drawString(0, 32, "A- Yes");
    display.drawString(0, 48, "B- No");
    display.display();

    if (digitalRead(BUTTON_PIN_1) == LOW && (millis() - lastPress1) > debounceDelay) {
      lastPress1 = millis();
      displayCenteredText("Formatting SD Card...");
      
      // Simulate formatting steps with progress
      for (int i = 0; i <= 100; i += 25) {
        showProgressBar(i);
        delay(500);  // Simulate delay for each step
      }

      File root = SD.open("/");
      if (root) {
        deleteFilesRecursive(root);
        root.close();
        displayCenteredText("Format Complete!");
        beep();
        delay(2000);
        ESP.restart();  // Restart the ESP32 after formatting

      } else {
        Serial.println("Error opening root directory!");
        displayCenteredText("Error opening root directory!");
        beep();
        delay(2000);
        displayMenu();  // Return to the main menu if error occurs
        return;
      }
    }

    if (digitalRead(BUTTON_PIN_2) == LOW && (millis() - lastPress2) > debounceDelay) {
      lastPress2 = millis();
      displayMenu();  // Return to the main menu
      return;
    }
  }
}

void deleteFilesRecursive(File dir) {
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      // No more files in this directory
      break;
    }
    if (entry.isDirectory()) {
      deleteFilesRecursive(entry);  // Recursively delete files and directories
    } else {
      Serial.print("Deleting file: ");
      Serial.println(entry.name());
      SD.remove(entry.name());  // Delete the file
     // delay(20);  // Optional: Wait for a short time to allow SD card to flush
    }
    entry.close();
  }

  // Once all files are deleted, remove the directory
  Serial.print("Removing directory: ");
  Serial.println(dir.name());
  SD.rmdir(dir.name());  // Delete the directory itself
}

void displayStartup() {

  display.clear();

    // Display battery icon and percentage
  drawBatteryIcon(batteryPercentage);
  display.drawString(0, 1, "SD Tester");
  display.drawString(22, 30, "Insert an SD Card");
  display.drawString(12, 48, "Hold Button A to Start");
  display.display();
  beep();
}

void displayCenteredText(const String& text) {
  display.clear();
  int16_t x = (display.getWidth() - display.getStringWidth(text)) / 2;
  int16_t y = (display.getHeight() - 10) / 2;
  display.drawString(x, y, text);
  display.display();
}

void displayMenu() {
  while (true) {
    display.clear();


  // Display battery icon and percentage
  drawBatteryIcon(batteryPercentage);

    display.drawString(0, 1, "Menu:");
    display.drawString(0, 18, "A- SD Card Info");
    display.drawString(0, 30, "B- Format SD");
    display.drawString(0, 48, "SD can be safely removed");
    display.display();
  

    // Wait for button press
    if (digitalRead(BUTTON_PIN_1) == LOW && (millis() - lastPress1) > debounceDelay) {
      lastPress1 = millis();
      displayCardInfo();  // Show SD card info
      delay(4000);        // Pause for readability
      continue;           // Return to the menu
    }

    if (digitalRead(BUTTON_PIN_2) == LOW && (millis() - lastPress2) > debounceDelay) {
      lastPress2 = millis();
      formatSDCard();  // Format SD card
      delay(2000);     // Pause for readability
      continue;        // Return to the menu
    }
  }
}

void displayCardInfo() {
  uint8_t cardType = SD.cardType();
  uint64_t cardSize = SD.cardSize() / (1024 * 1024); 
  uint64_t usedSpace = SD.usedBytes() / (1024 * 1024); 
  uint64_t freeSpace = (SD.totalBytes() - SD.usedBytes()) / (1024 * 1024);

  String cardTypeStr;
  switch (cardType) {
    case CARD_MMC:  cardTypeStr = "MMC"; break;
    case CARD_SD:   cardTypeStr = "SD"; break;
    case CARD_SDHC: cardTypeStr = "SDHC"; break;
    default:        cardTypeStr = "UNKNOWN"; break;
  }

  display.clear();
  const char* text = "Card Info:";
  int16_t x = (display.getWidth() - display.getStringWidth(text)) / 2;
  display.drawString(x, 0, text);
  display.drawString(0, 12, "Type: " + cardTypeStr);
  display.drawString(0, 24, "Size: " + String(cardSize) + " MB");
  display.drawString(0, 36, "Used: " + String(usedSpace) + " MB");
  display.drawString(0, 48, "Free: " + String(freeSpace) + " MB");
  display.display();
  beep();
}

void setup() {

  Serial.begin(115200);

  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);


  displayStartup();

  beep();

  // Wait for BUTTON_PIN_1 press before continuing
  while (digitalRead(BUTTON_PIN_1) == HIGH) { }

  // SD Init 
  SDInit();

  // Start SD Test 
  SDTest();

  // Display SD Card Info
  displayMenu();

  beep();

  delay(2000);

}

void loop() { }

