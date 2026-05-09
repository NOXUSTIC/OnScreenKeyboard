# OnScreenKeyboard Library for Arduino/ESP32

A professional on-screen QWERTY keyboard library for TFT displays, perfect for WiFi configuration, user input, and interactive projects.

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Version](https://img.shields.io/badge/version-1.0.0-green.svg)
![Arduino](https://img.shields.io/badge/Arduino-Compatible-brightgreen.svg)

## Features

✨ **Full QWERTY Layout** - Standard keyboard with 4 rows of keys  
⌨️ **Special Keys** - Capslock, Space, Backspace, Enter, Dot, Underscore  
🎨 **Customizable Colors** - Full color scheme customization  
📱 **Responsive** - Works with 3-button navigation (UP/DOWN/OK)  
💾 **Lightweight** - Minimal memory footprint  
🔧 **Easy Integration** - Simple API, works with TFT_eSPI  

## Hardware Requirements

- **Arduino** or **ESP32** board
- **TFT Display** compatible with TFT_eSPI library
- **3 Buttons** for navigation (UP, DOWN, OK)

## Installation

### Method 1: Arduino Library Manager (Recommended)

1. Open Arduino IDE
2. Go to **Sketch → Include Library → Manage Libraries**
3. Search for "OnScreenKeyboard"
4. Click **Install**

### Method 2: Manual Installation

1. Download this repository as ZIP
2. Open Arduino IDE
3. Go to **Sketch → Include Library → Add .ZIP Library**
4. Select the downloaded ZIP file

### Method 3: Git Clone

```bash
cd ~/Arduino/libraries
git clone https://github.com/noxustic/OnScreenKeyboard.git
```

## Quick Start

```cpp
#include <TFT_eSPI.h>
#include <OnScreenKeyboard.h>

TFT_eSPI tft = TFT_eSPI();
OnScreenKeyboard keyboard(&tft);

#define BTN_UP 32
#define BTN_DOWN 33
#define BTN_OK 13

void setup() {
  // Initialize buttons
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_OK, INPUT_PULLUP);
  
  // Initialize display
  tft.init();
  tft.setRotation(0);
  
  // Initialize keyboard
  keyboard.begin(BTN_UP, BTN_DOWN, BTN_OK);
}

void loop() {
  // Get user input
  String userInput = keyboard.getInput("Enter Name:", "");
  
  // Use the input
  Serial.println(userInput);
  delay(3000);
}
```

## API Reference

### Initialization

#### `OnScreenKeyboard(TFT_eSPI* tft)`
Constructor - pass your TFT_eSPI instance.

```cpp
OnScreenKeyboard keyboard(&tft);
```

#### `void begin(int btn_up, int btn_down, int btn_ok)`
Initialize the keyboard with button pins.

```cpp
keyboard.begin(BTN_UP, BTN_DOWN, BTN_OK);
```

### Configuration

#### `void setKeyboardPosition(int y_start)`
Set the Y position where keyboard starts (default: 120).

```cpp
keyboard.setKeyboardPosition(150);
```

#### `void setInputFieldPosition(int x, int y, int w, int h)`
Set input field position and size.

```cpp
keyboard.setInputFieldPosition(10, 60, 220, 35);
```

#### `void setMaxInputLength(int length)`
Set maximum input length (default: 30).

```cpp
keyboard.setMaxInputLength(50);
```

#### `void setColorScheme(uint16_t bg, uint16_t key, uint16_t key_sel, uint16_t text, uint16_t text_dim, uint16_t border, uint16_t special)`
Customize all colors.

```cpp
keyboard.setColorScheme(
  TFT_BLACK,   // Background
  TFT_DARKGREY, // Key
  TFT_CYAN,    // Key selected
  TFT_WHITE,   // Text
  TFT_LIGHTGREY, // Text dim
  TFT_BLUE,    // Border
  TFT_YELLOW   // Special keys
);
```

### Main Methods

#### `String getInput(String title = "", String default_text = "")`
Display keyboard and get user input (blocking).

```cpp
String name = keyboard.getInput("Enter Name:", "");
String email = keyboard.getInput("Email:", "user@example.com");
```

#### `void draw()`
Manually draw the keyboard.

```cpp
keyboard.draw();
```

#### `void clear()`
Clear the keyboard area.

```cpp
keyboard.clear();
```

### Text Management

#### `void setText(String text)`
Set the current input text.

```cpp
keyboard.setText("Hello");
```

#### `String getText()`
Get the current input text.

```cpp
String current = keyboard.getText();
```

#### `void clearText()`
Clear the current input.

```cpp
keyboard.clearText();
```

### Utility Methods

#### `void toggleCapslock()`
Toggle capslock state.

```cpp
keyboard.toggleCapslock();
```

#### `bool isCapslock()`
Check if capslock is active.

```cpp
if (keyboard.isCapslock()) {
  // Capslock is on
}
```

#### `int getSelectedKey()`
Get currently selected key index.

```cpp
int selected = keyboard.getSelectedKey();
```

#### `void setSelectedKey(int index)`
Set the selected key.

```cpp
keyboard.setSelectedKey(10); // Select 'q' key
```

## Keyboard Layout

```
Row 1: [ 1 ][ 2 ][ 3 ][ 4 ][ 5 ][ 6 ][ 7 ][ 8 ][ 9 ][ 0 ]
Row 2:   [ q ][ w ][ e ][ r ][ t ][ y ][ u ][ i ][ o ][ p ]
Row 3:     [ a ][ s ][ d ][ f ][ g ][ h ][ j ][ k ][ l ]
Row 4:       [ z ][ x ][ c ][ v ][ b ][ n ][ m ][ CAPS ]
Row 5: [ . ][    SPACE    ][ DEL ][ _ ]
Row 6:          [ DONE ]
```

## Special Keys

| Key | Code | Function |
|-----|------|----------|
| SPACE | 32 | Insert space |
| BACKSPACE | 8 | Delete last character |
| ENTER | 13 | Confirm input (returns text) |
| CAPSLOCK | 20 | Toggle uppercase |
| DOT | '.' | Insert period |
| UNDERSCORE | '_' | Insert underscore |

## Examples

### Example 1: Basic Input

```cpp
String name = keyboard.getInput("Enter Name:", "");
Serial.println("Hello, " + name + "!");
```

### Example 2: WiFi Configuration

```cpp
String ssid = keyboard.getInput("WiFi SSID:", "");
String pass = keyboard.getInput("Password:", "");
WiFi.begin(ssid.c_str(), pass.c_str());
```

### Example 3: Custom Colors

```cpp
// Dark theme
keyboard.setColorScheme(
  0x0000,  // Black background
  0x2104,  // Dark gray keys
  0x07FF,  // Cyan selection
  0xFFFF,  // White text
  0x8410,  // Gray dim text
  0x4208,  // Dark border
  0xFBE0   // Orange special keys
);

String input = keyboard.getInput("Dark Theme:", "");
```

### Example 4: Form Input

```cpp
struct UserData {
  String name;
  String email;
  String phone;
};

UserData user;
user.name = keyboard.getInput("Name:", "");
user.email = keyboard.getInput("Email:", "");
user.phone = keyboard.getInput("Phone:", "");
```

## Pin Configuration

Default button configuration (customizable):

```cpp
#define BTN_UP 32    // Navigate up
#define BTN_DOWN 33  // Navigate down
#define BTN_OK 13    // Select key
```

Connect buttons with internal pull-ups:
```
Button → GND
Pin → INTERNAL_PULLUP
```

## Display Configuration

The library uses **TFT_eSPI**. Configure your display in:
- Arduino: `User_Setup.h` in TFT_eSPI library folder
- PlatformIO: `platformio.ini` build flags

Example for ST7789 240x320:
```cpp
#define ST7789_DRIVER
#define TFT_WIDTH  240
#define TFT_HEIGHT 320
```

## Memory Usage

Approximate memory usage:
- **Flash**: ~8KB
- **RAM**: ~2KB (includes key layout and text buffer)

## Compatibility

### Tested Boards
✅ ESP32 (all variants)  
✅ ESP8266  
✅ Arduino Mega 2560  
✅ Arduino Due  
⚠️ Arduino Uno (limited RAM - use short text only)  

### Tested Displays
✅ ST7789 (240x320, 240x240)

## Troubleshooting

### Keyboard not displaying?
- Check TFT_eSPI is properly configured
- Verify display dimensions match code
- Ensure `tft.init()` is called before `keyboard.begin()`

### Buttons not working?
- Verify pin numbers in `begin()`
- Check buttons are connected to GND
- Ensure `pinMode(pin, INPUT_PULLUP)` is set

### Text too long?
- Use `setMaxInputLength()` to limit length
- Default is 30 characters
- Display scrolls automatically for longer text

### Colors wrong?
- Use `setColorScheme()` to customize
- Ensure 16-bit color format (RGB565)
- Convert colors: `tft.color565(r, g, b)`

## Contributing

Contributions are welcome! Please:

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Submit a pull request

## License

MIT License - see LICENSE file for details.

## Credits

Created by **NOXUSTIC** for embedded systems and IoT projects.

Designed for easy WiFi configuration and user input in Arduino and ESP32 projects.

## Support

- **Issues**: [GitHub Issues](https://github.com/noxustic/OnScreenKeyboard/issues)
- **Discussions**: [GitHub Discussions](https://github.com/noxustic/OnScreenKeyboard/discussions)
- **Email**: noxustic@gmail.com

## Changelog

### Version 1.0.0 (2026)
- Initial release
- QWERTY layout with 4 rows
- Special keys support
- Color customization
- TFT_eSPI integration
- Example sketches

## Roadmap

Future features under consideration:
- [ ] Numeric-only keyboard mode
- [ ] Symbol/special character layout
- [ ] Multi-language support
- [ ] Touch screen support
- [ ] Password masking option
- [ ] Auto-complete suggestions
- [ ] Sound feedback

## Related Projects

- [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) - TFT display library

---

**Made with ❤️ by NOXUSTIC**
