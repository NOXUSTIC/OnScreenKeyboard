/*
 * OnScreenKeyboard Library for Arduino/ESP32
 * Implementation file
 */

#include "OnScreenKeyboard.h"

// QWERTY keyboard layout
const char keyboard_row1[] = "1234567890";
const char keyboard_row2[] = "qwertyuiop";
const char keyboard_row3[] = "asdfghjkl";
const char keyboard_row4[] = "zxcvbnm";

OnScreenKeyboard::OnScreenKeyboard(TFT_eSPI* tft) {
  _tft = tft;
  _total_keys = 0;
  _selected_key = 0;
  _capslock = false;
  _input_text = "";
  _max_input_length = OSK_MAX_INPUT_LENGTH;
  
  // Default positions
  _keyboard_y = 120;
  _display_x = 10;
  _display_y = 60;
  _display_w = 220;
  _display_h = 35;
  
  // Default color scheme
  _color_bg = OSK_COLOR_BG;
  _color_key = OSK_COLOR_KEY;
  _color_key_selected = OSK_COLOR_KEY_SEL;
  _color_text = OSK_COLOR_TEXT;
  _color_text_dim = OSK_COLOR_TEXT_DIM;
  _color_border = OSK_COLOR_BORDER;
  _color_special = OSK_COLOR_SPECIAL;
  
  // Initialize keyboard layout
  initQwertyLayout();
}

void OnScreenKeyboard::begin(int btn_up, int btn_down, int btn_ok) {
  _btn_up = btn_up;
  _btn_down = btn_down;
  _btn_ok = btn_ok;
}

void OnScreenKeyboard::setKeyboardPosition(int y_start) {
  _keyboard_y = y_start;
  initQwertyLayout(); // Re-init with new position
}

void OnScreenKeyboard::setInputFieldPosition(int x, int y, int w, int h) {
  _display_x = x;
  _display_y = y;
  _display_w = w;
  _display_h = h;
}

void OnScreenKeyboard::setMaxInputLength(int length) {
  _max_input_length = length;
}

void OnScreenKeyboard::setColorScheme(uint16_t bg, uint16_t key, uint16_t key_sel, 
                                       uint16_t text, uint16_t text_dim, uint16_t border, uint16_t special) {
  _color_bg = bg;
  _color_key = key;
  _color_key_selected = key_sel;
  _color_text = text;
  _color_text_dim = text_dim;
  _color_border = border;
  _color_special = special;
}

void OnScreenKeyboard::initQwertyLayout() {
  _total_keys = 0;
  int keyW = 22, keyH = 18;
  int startY = _keyboard_y;
  
  // Row 1: Numbers
  for (int i = 0; i < 10; i++) {
    _keys[_total_keys].character = keyboard_row1[i];
    _keys[_total_keys].x = 2 + i * keyW;
    _keys[_total_keys].y = startY;
    _keys[_total_keys].w = keyW - 2;
    _keys[_total_keys].h = keyH;
    _total_keys++;
  }
  
  // Row 2: QWERTYUIOP
  for (int i = 0; i < 10; i++) {
    _keys[_total_keys].character = keyboard_row2[i];
    _keys[_total_keys].x = 12 + i * keyW;
    _keys[_total_keys].y = startY + keyH + 2;
    _keys[_total_keys].w = keyW - 2;
    _keys[_total_keys].h = keyH;
    _total_keys++;
  }
  
  // Row 3: ASDFGHJKL
  for (int i = 0; i < 9; i++) {
    _keys[_total_keys].character = keyboard_row3[i];
    _keys[_total_keys].x = 22 + i * keyW;
    _keys[_total_keys].y = startY + (keyH + 2) * 2;
    _keys[_total_keys].w = keyW - 2;
    _keys[_total_keys].h = keyH;
    _total_keys++;
  }
  
  // Row 4: ZXCVBNM
  for (int i = 0; i < 7; i++) {
    _keys[_total_keys].character = keyboard_row4[i];
    _keys[_total_keys].x = 32 + i * keyW;
    _keys[_total_keys].y = startY + (keyH + 2) * 3;
    _keys[_total_keys].w = keyW - 2;
    _keys[_total_keys].h = keyH;
    _total_keys++;
  }
  
  // CAPSLOCK
  _keys[_total_keys].character = OSK_KEY_CAPSLOCK;
  _keys[_total_keys].x = 186;
  _keys[_total_keys].y = startY + (keyH + 2) * 3;
  _keys[_total_keys].w = 35;
  _keys[_total_keys].h = keyH;
  _total_keys++;
  
  // Special keys row
  // DOT (.)
  _keys[_total_keys].character = OSK_KEY_DOT;
  _keys[_total_keys].x = 20;
  _keys[_total_keys].y = startY + (keyH + 2) * 4;
  _keys[_total_keys].w = 30;
  _keys[_total_keys].h = keyH;
  _total_keys++;
  
  // SPACE
  _keys[_total_keys].character = OSK_KEY_SPACE;
  _keys[_total_keys].x = 55;
  _keys[_total_keys].y = startY + (keyH + 2) * 4;
  _keys[_total_keys].w = 70;
  _keys[_total_keys].h = keyH;
  _total_keys++;
  
  // Backspace (DEL)
  _keys[_total_keys].character = OSK_KEY_BACKSPACE;
  _keys[_total_keys].x = 130;
  _keys[_total_keys].y = startY + (keyH + 2) * 4;
  _keys[_total_keys].w = 40;
  _keys[_total_keys].h = keyH;
  _total_keys++;
  
  // UNDERSCORE (_)
  _keys[_total_keys].character = OSK_KEY_UNDERSCORE;
  _keys[_total_keys].x = 175;
  _keys[_total_keys].y = startY + (keyH + 2) * 4;
  _keys[_total_keys].w = 30;
  _keys[_total_keys].h = keyH;
  _total_keys++;
  
  // DONE button (Enter)
  _keys[_total_keys].character = OSK_KEY_ENTER;
  _keys[_total_keys].x = 85;
  _keys[_total_keys].y = startY + (keyH + 2) * 5;
  _keys[_total_keys].w = 70;
  _keys[_total_keys].h = keyH;
  _total_keys++;
}

void OnScreenKeyboard::drawKey(int index, bool selected) {
  KeyboardKey k = _keys[index];
  
  // Clear the key area
  _tft->fillRect(k.x - 1, k.y - 1, k.w + 2, k.h + 2, _color_bg);
  
  // Special handling for capslock
  uint16_t keyColor = _color_key;
  if (k.character == OSK_KEY_CAPSLOCK && _capslock && !selected) {
    keyColor = 0x07E0; // Green when active
  }
  
  if (selected) {
    _tft->fillRoundRect(k.x, k.y, k.w, k.h, 3, _color_key_selected);
    _tft->setTextColor(_color_bg);
  } else {
    _tft->fillRoundRect(k.x, k.y, k.w, k.h, 3, keyColor);
    _tft->drawRoundRect(k.x, k.y, k.w, k.h, 3, _color_text_dim);
    _tft->setTextColor(_color_text);
  }
  
  _tft->setTextSize(2);
  _tft->setTextDatum(MC_DATUM);
  
  // Draw key label
  if (k.character == OSK_KEY_BACKSPACE) {
    _tft->setTextSize(1);
    _tft->drawString("DEL", k.x + k.w/2, k.y + k.h/2);
  } else if (k.character == OSK_KEY_ENTER) {
    _tft->setTextSize(1);
    _tft->drawString("DONE", k.x + k.w/2, k.y + k.h/2);
  } else if (k.character == OSK_KEY_SPACE) {
    _tft->setTextSize(1);
    _tft->drawString("SPACE", k.x + k.w/2, k.y + k.h/2);
  } else if (k.character == OSK_KEY_CAPSLOCK) {
    _tft->setTextSize(1);
    _tft->drawString("CAPS", k.x + k.w/2, k.y + k.h/2);
  } else {
    String charStr = String(k.character);
    if (_capslock) {
      charStr.toUpperCase();
    }
    _tft->drawString(charStr, k.x + k.w/2, k.y + k.h/2);
  }
  
  _tft->setTextDatum(TL_DATUM);
}

void OnScreenKeyboard::drawInputField() {
  // Draw input field background
  _tft->fillRoundRect(_display_x, _display_y, _display_w, _display_h, 4, _color_key);
  _tft->drawRoundRect(_display_x, _display_y, _display_w, _display_h, 4, _color_border);
  
  // Draw text with cursor
  _tft->setTextSize(2);
  _tft->setTextColor(_color_text);
  _tft->setCursor(_display_x + 5, _display_y + 10);
  
  // Display text (scroll if too long)
  String displayText = _input_text;
  if (displayText.length() > 18) {
    displayText = displayText.substring(displayText.length() - 18);
  }
  _tft->print(displayText);
  
  // Draw cursor
  int cursorX = _display_x + 5 + (displayText.length() > 18 ? 18 : displayText.length()) * 12;
  _tft->fillRect(cursorX, _display_y + 10, 2, 14, _color_key_selected);
}

void OnScreenKeyboard::draw() {
  // Draw input field
  drawInputField();
  
  // Draw all keys
  for (int i = 0; i < _total_keys; i++) {
    drawKey(i, i == _selected_key);
  }
}

void OnScreenKeyboard::clear() {
  _tft->fillRect(0, _display_y, 240, 240 - _display_y, _color_bg);
}

String OnScreenKeyboard::getInput(String title, String default_text) {
  _input_text = default_text;
  _selected_key = 0;
  _capslock = false;
  
  // Clear screen and draw title
  _tft->fillScreen(_color_bg);
  
  if (title.length() > 0) {
    _tft->setTextSize(2);
    _tft->setTextColor(_color_text);
    _tft->setTextDatum(TC_DATUM);
    _tft->drawString(title, 120, 10);
    _tft->setTextDatum(TL_DATUM);
  }
  
  // Draw keyboard
  draw();
  
  // Main input loop
  while (true) {
    // Check UP button
    if (!digitalRead(_btn_up)) {
      while (!digitalRead(_btn_up)) delay(10);
      drawKey(_selected_key, false);
      _selected_key--;
      if (_selected_key < 0) _selected_key = _total_keys - 1;
      drawKey(_selected_key, true);
    }
    
    // Check DOWN button
    if (!digitalRead(_btn_down)) {
      while (!digitalRead(_btn_down)) delay(10);
      drawKey(_selected_key, false);
      _selected_key++;
      if (_selected_key >= _total_keys) _selected_key = 0;
      drawKey(_selected_key, true);
    }
    
    // Check OK button
    if (!digitalRead(_btn_ok)) {
      uint32_t pressStart = millis();
      while (!digitalRead(_btn_ok)) {
        delay(10);
      }
      
      char selectedChar = _keys[_selected_key].character;
      
      if (selectedChar == OSK_KEY_ENTER) { // DONE
        return _input_text;
      }
      else if (selectedChar == OSK_KEY_BACKSPACE) { // Backspace
        if (_input_text.length() > 0) {
          _input_text.remove(_input_text.length() - 1);
        }
      }
      else if (selectedChar == OSK_KEY_CAPSLOCK) { // Capslock
        toggleCapslock();
        // Redraw all keys to show caps state
        for (int i = 0; i < _total_keys; i++) {
          drawKey(i, i == _selected_key);
        }
      }
      else { // Regular character
        if (_input_text.length() < _max_input_length) {
          char charToAdd = selectedChar;
          if (_capslock && selectedChar >= 'a' && selectedChar <= 'z') {
            charToAdd = selectedChar - 32;
          }
          _input_text += charToAdd;
        }
      }
      
      // Update display
      drawInputField();
    }
    
    delay(10);
  }
}

bool OnScreenKeyboard::update() {
  // Non-blocking version - returns true if Enter pressed
  // User must call this repeatedly in their loop
  return false; // Implement if needed
}

void OnScreenKeyboard::setText(String text) {
  _input_text = text;
  if (_input_text.length() > _max_input_length) {
    _input_text = _input_text.substring(0, _max_input_length);
  }
}

String OnScreenKeyboard::getText() {
  return _input_text;
}

void OnScreenKeyboard::clearText() {
  _input_text = "";
}

void OnScreenKeyboard::toggleCapslock() {
  _capslock = !_capslock;
}

bool OnScreenKeyboard::isCapslock() {
  return _capslock;
}

int OnScreenKeyboard::getSelectedKey() {
  return _selected_key;
}

void OnScreenKeyboard::setSelectedKey(int index) {
  if (index >= 0 && index < _total_keys) {
    _selected_key = index;
  }
}

void OnScreenKeyboard::navigateUp() {
  drawKey(_selected_key, false);
  _selected_key--;
  if (_selected_key < 0) _selected_key = _total_keys - 1;
  drawKey(_selected_key, true);
}

void OnScreenKeyboard::navigateDown() {
  drawKey(_selected_key, false);
  _selected_key++;
  if (_selected_key >= _total_keys) _selected_key = 0;
  drawKey(_selected_key, true);
}

void OnScreenKeyboard::navigateLeft() {
  navigateUp();
}

void OnScreenKeyboard::navigateRight() {
  navigateDown();
}

void OnScreenKeyboard::selectKey() {
  char selectedChar = _keys[_selected_key].character;
  
  if (selectedChar == OSK_KEY_BACKSPACE) {
    if (_input_text.length() > 0) {
      _input_text.remove(_input_text.length() - 1);
    }
  }
  else if (selectedChar == OSK_KEY_CAPSLOCK) {
    toggleCapslock();
    for (int i = 0; i < _total_keys; i++) {
      drawKey(i, i == _selected_key);
    }
  }
  else if (selectedChar != OSK_KEY_ENTER) {
    if (_input_text.length() < _max_input_length) {
      char charToAdd = selectedChar;
      if (_capslock && selectedChar >= 'a' && selectedChar <= 'z') {
        charToAdd = selectedChar - 32;
      }
      _input_text += charToAdd;
    }
  }
  
  drawInputField();
}
