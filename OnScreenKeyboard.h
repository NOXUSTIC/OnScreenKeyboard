/*
 * OnScreenKeyboard Library for Arduino/ESP32
 * 
 * A flexible on-screen keyboard for TFT displays
 * Supports QWERTY layout with special keys
 * 
 * Author: NOXUSTIC
 * Version: 1.0.0
 * License: MIT
 * 
 * Compatible with:
 * - TFT_eSPI
 * - Any display library with basic drawing functions
 */

#ifndef ONSCREENKEYBOARD_H
#define ONSCREENKEYBOARD_H

#include <Arduino.h>
#include <TFT_eSPI.h>

// Keyboard configuration
#define OSK_MAX_KEYS 50
#define OSK_MAX_INPUT_LENGTH 30

// Default color scheme (can be customized)
#define OSK_COLOR_BG        0x0020
#define OSK_COLOR_KEY       0x18C3
#define OSK_COLOR_KEY_SEL   0x07FF
#define OSK_COLOR_TEXT      0xFFFF
#define OSK_COLOR_TEXT_DIM  0x8410
#define OSK_COLOR_BORDER    0x39C7
#define OSK_COLOR_SPECIAL   0xFBE0

// Special key codes
#define OSK_KEY_BACKSPACE   8
#define OSK_KEY_ENTER       13
#define OSK_KEY_SPACE       32
#define OSK_KEY_CAPSLOCK    20
#define OSK_KEY_DOT         '.'
#define OSK_KEY_UNDERSCORE  '_'

struct KeyboardKey {
  char character;
  int x, y, w, h;
};

class OnScreenKeyboard {
  private:
    TFT_eSPI* _tft;
    KeyboardKey _keys[OSK_MAX_KEYS];
    int _total_keys;
    int _selected_key;
    bool _capslock;
    String _input_text;
    int _max_input_length;
    
    // Display settings
    int _keyboard_y;
    int _display_x;
    int _display_y;
    int _display_w;
    int _display_h;
    
    // Color scheme
    uint16_t _color_bg;
    uint16_t _color_key;
    uint16_t _color_key_selected;
    uint16_t _color_text;
    uint16_t _color_text_dim;
    uint16_t _color_border;
    uint16_t _color_special;
    
    // Navigation buttons (user-provided)
    int _btn_up;
    int _btn_down;
    int _btn_ok;
    
    // Internal methods
    void initQwertyLayout();
    void drawKey(int index, bool selected);
    void updateDisplay();
    void drawInputField();
    
  public:
    // Constructor
    OnScreenKeyboard(TFT_eSPI* tft);
    
    // Setup methods
    void begin(int btn_up, int btn_down, int btn_ok);
    void setKeyboardPosition(int y_start);
    void setInputFieldPosition(int x, int y, int w, int h);
    void setMaxInputLength(int length);
    void setColorScheme(uint16_t bg, uint16_t key, uint16_t key_sel, 
                        uint16_t text, uint16_t text_dim, uint16_t border, uint16_t special);
    
    // Main methods
    String getInput(String title = "", String default_text = "");
    void draw();
    void clear();
    bool update(); // Returns true if Enter pressed
    
    // Utility methods
    void setText(String text);
    String getText();
    void clearText();
    void toggleCapslock();
    bool isCapslock();
    int getSelectedKey();
    void setSelectedKey(int index);
    
    // Key navigation
    void navigateUp();
    void navigateDown();
    void navigateLeft();
    void navigateRight();
    void selectKey();
};

#endif
