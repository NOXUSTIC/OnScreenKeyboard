/*
 * OnScreenKeyboard WiFi Configuration Example
 * 
 * This example shows how to use the keyboard for WiFi setup
 * 
 * Hardware required:
 * - ESP32 board
 * - TFT display (compatible with TFT_eSPI)
 * - 3 buttons (UP, DOWN, OK)
 * 
 * Author: NOXUSTIC
 */

#include <TFT_eSPI.h>
#include <OnScreenKeyboard.h>
#include <WiFi.h>
#include <Preferences.h>

TFT_eSPI tft = TFT_eSPI();
OnScreenKeyboard keyboard(&tft);
Preferences preferences;

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
  
  // Initialize keyboard with custom colors
  keyboard.begin(BTN_UP, BTN_DOWN, BTN_OK);
  keyboard.setColorScheme(
    0x0020,  // Background
    0x18C3,  // Key
    0x07FF,  // Key selected
    0xFFFF,  // Text
    0x8410,  // Text dim
    0x39C7,  // Border
    0xFBE0   // Special
  );
  
  // Load preferences
  preferences.begin("wifi_config", false);
  
  Serial.println("WiFi Configuration Ready!");
  
  // Run WiFi config
  wifiConfig();
}

void loop() {
  // Main loop - WiFi is configured
  tft.fillScreen(TFT_BLACK);
  
  if (WiFi.status() == WL_CONNECTED) {
    tft.setTextSize(2);
    tft.setTextColor(TFT_GREEN);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("WiFi Connected!", 120, 100);
    tft.setTextColor(TFT_WHITE);
    tft.drawString(WiFi.localIP().toString(), 120, 130);
    tft.setTextDatum(TL_DATUM);
  } else {
    tft.setTextSize(2);
    tft.setTextColor(TFT_RED);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("Not Connected", 120, 120);
    tft.setTextDatum(TL_DATUM);
  }
  
  delay(5000);
  
  // Press OK to reconfigure
  if (!digitalRead(BTN_OK)) {
    while (!digitalRead(BTN_OK)) delay(10);
    wifiConfig();
  }
}

void wifiConfig() {
  tft.fillScreen(TFT_BLACK);
  
  // Show title
  tft.setTextSize(2);
  tft.setTextColor(TFT_CYAN);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("WIFI CONFIG", 120, 10);
  tft.setTextDatum(TL_DATUM);
  
  // Get SSID
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(10, 35);
  tft.print("Enter WiFi SSID:");
  
  String ssid = keyboard.getInput("", "");
  
  if (ssid.length() == 0) {
    showError("SSID required!");
    return;
  }
  
  // Get Password
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_CYAN);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("WIFI CONFIG", 120, 10);
  tft.setTextDatum(TL_DATUM);
  
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(10, 35);
  tft.print("Enter Password:");
  
  String password = keyboard.getInput("", "");
  
  // Save to preferences
  preferences.putString("wifi_ssid", ssid);
  preferences.putString("wifi_pass", password);
  
  // Show connecting screen
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_YELLOW);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("CONNECTING...", 120, 100);
  tft.setTextColor(TFT_WHITE);
  tft.drawString(ssid, 120, 130);
  tft.setTextDatum(TL_DATUM);
  
  // Connect to WiFi
  WiFi.begin(ssid.c_str(), password.c_str());
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    tft.fillCircle(90 + (attempts % 5) * 12, 160, 4, TFT_CYAN);
    attempts++;
  }
  
  // Show result
  tft.fillScreen(TFT_BLACK);
  
  if (WiFi.status() == WL_CONNECTED) {
    tft.setTextSize(3);
    tft.setTextColor(TFT_GREEN);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("SUCCESS!", 120, 90);
    
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    tft.drawString(WiFi.localIP().toString(), 120, 140);
    tft.setTextDatum(TL_DATUM);
  } else {
    tft.setTextSize(3);
    tft.setTextColor(TFT_RED);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("FAILED", 120, 90);
    
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE);
    tft.drawString("Check password", 120, 140);
    tft.setTextDatum(TL_DATUM);
  }
  
  delay(3000);
}

void showError(String msg) {
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_RED);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("ERROR", 120, 100);
  tft.setTextColor(TFT_WHITE);
  tft.drawString(msg, 120, 130);
  tft.setTextDatum(TL_DATUM);
  delay(2000);
}
