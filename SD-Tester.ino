#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET -1
Adafruit_SSD1306 display(OLED_RESET);

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define SD_CS_PIN 10

void setup() {
  Serial.begin(9600);

  // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Display the header
  display.setCursor(0, 0);
  display.println("NECORONICS SD TESTER");
  display.display();
  delay(2000);

  display.clearDisplay();
  
  // Initialize the SD card
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD card initialization failed!");
    display.setCursor(0, 0);
    display.println("SD init failed!");
    display.display();
    return;
  }

  Serial.println("SD card initialized.");
  display.setCursor(0, 0);
  display.println("SD init successful!");
  display.display();

  delay(2000);

  display.clearDisplay();
  listFiles("/");
}

void loop() {
  // Nothing to do here
}

void listFiles(const char *dirname) {
  File root = SD.open(dirname);

  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }

  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  root.rewindDirectory();

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Files:");
  display.display();

  Serial.println("Files in root:");

  int fileIndex = 0;

  File file = root.openNextFile();
  while (file) {
    if (fileIndex < 5) {
      display.setCursor(0, fileIndex * 10 + 10);
      display.println(file.name());
      display.display();
    }

    Serial.println(file.name());

    fileIndex++;
    file.close();
    file = root.openNextFile();
  }

  Serial.println("End of file listing");
}
