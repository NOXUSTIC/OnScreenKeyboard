/*
 * OnScreenKeyboard Basic Example
 * 
 * This example demonstrates basic usage of the OnScreenKeyboard library
 * 
 * Hardware required:
 * - ESP32 or Arduino board
 * - TFT display (compatible with TFT_eSPI)
 * - 3 buttons (UP, DOWN, OK)
 * 
 * Connections:
 * - Button UP: GPIO 32 (with pull-up)
 * - Button DOWN: GPIO 33 (with pull-up)
 * - Button OK: GPIO 13 (with pull-up)
 * 
 * Author: NOXUSTIC
 */

#include <TFT_eSPI.h>
#include <OnScreenKeyboard.h>

TFT_eSPI tft = TFT_eSPI();
OnScreenKeyboard keyboard(&tft);

// Button pins
#define BTN_UP 32
#define BTN_DOWN 33
#define BTN_OK 13

void setup() {
  Serial.begin(115200);
  
  // Initialize buttons
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_OK, INPUT_PULLUP);
  
  // Initialize display
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  
  // Initialize keyboard
  keyboard.begin(BTN_UP, BTN_DOWN, BTN_OK);
  
  Serial.println("OnScreenKeyboard Example Ready!");
}

void loop() {
  // Get user input with title
  String userInput = keyboard.getInput("Enter Name:", "");
  
  // Display result
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("You entered:", 120, 80);
  tft.setTextColor(TFT_CYAN);
  tft.drawString(userInput, 120, 110);
  tft.setTextDatum(TL_DATUM);
  
  // Print to serial
  Serial.print("User entered: ");
  Serial.println(userInput);
  
  // Wait before showing keyboard again
  delay(3000);
}
