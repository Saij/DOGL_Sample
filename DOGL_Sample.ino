#include "DOG_7565R/dog_7565R.h"
#include "DOG_7565R/font_16x32nums.h"
#include "DOG_7565R/font_6x8.h"
#include "DOG_7565R/font_8x16.h"
#include "DOG_7565R/font_8x8.h"
#include "DOG_7565R/logo_BLH.h"

dog_7565R display;

//the following port definitions are used by our demo board "EA PCBARDDOG7565"
int led_red   = 3;
int led_green = 5;
int led_blue  = 6;

byte red        = 255;
byte green      = 255;
byte blue       = 255;
byte brightness = 100;
byte invert     = VIEW_NORMAL;

void initBacklight();
void rgbBacklight();
void printHelp();
void runSerialCommand(char cmd, int16_t value);
void sampleScreen();

/// State definition for this radio implementation.
enum COMMAND_STATE {
  STATE_PARSECOMMAND, // waiting for a new command character.
  STATE_PARSEINT,     // waiting for digits for the parameter.
  STATE_EXEC          // executing the command.
};

COMMAND_STATE state;

void setup() {
  // open the Serial port
  Serial.begin(57600);
  Serial.print("DOGL Sample");
  delay(500);

  initBacklight();
  display.initialize(10, 0, 0, 9, 4, DOGL128);
  display.contrast(63);
  display.view(VIEW_BOTTOM);
  display.invert(invert);

  sampleScreen();

  Serial.write('>');
  state = STATE_PARSECOMMAND;  
  runSerialCommand('?', 0);
}

void loop() {
  // some internal static values for parsing the input
  static char command;
  static int16_t value;
  
  char c;
  if (Serial.available() > 0) {
    // read the next char from input.
    c = Serial.peek();

    if ((state == STATE_PARSECOMMAND) && (c < 0x20)) {
      // ignore unprintable chars
      Serial.read();
    } else if (state == STATE_PARSECOMMAND) {
      // read a command.
      command = Serial.read();
      state = STATE_PARSEINT;
    } else if (state == STATE_PARSEINT) {
      if ((c >= '0') && (c <= '9')) {
        // build up the value.
        c = Serial.read();
        value = (value * 10) + (c - '0');
      } else {
        // not a value -> execute
        runSerialCommand(command, value);
        command = ' ';
        state = STATE_PARSECOMMAND;
        value = 0;
      }
    }
  }
}

void initBacklight() {
  pinMode(led_blue,  OUTPUT);
  pinMode(led_green, OUTPUT);
  pinMode(led_red,   OUTPUT);
  rgbBacklight();
}

void rgbBacklight() {
  analogWrite(led_red,    red * brightness / 100);
  analogWrite(led_green,  green * brightness / 100);
  analogWrite(led_blue,   blue * brightness / 100);
}

void printHelp() {
  Serial.println();
  Serial.println("? Help");
  Serial.println("l Set brightness (0 to 100)");
  Serial.println("c Set contrast (0 to 63)");
  Serial.println("w Wipe display");
  Serial.println("n Normal (Bottom) view");
  Serial.println("t Top View");
  Serial.println("r Backlight color red");
  Serial.println("g Backlight color green");
  Serial.println("b Backlight color blue");
  Serial.println("i Toggle invert");
}

void sampleScreen() {
  display.clear();  //clear whole display
  display.picture(0,0,ea_logo);
  display.string(71,0,font_8x16,"DOGL128");
  display.rectangle(71,2,127,2,0x03);
  display.string(0,3,font_6x8,"-3.3V single supply");
  display.string(0,4,font_6x8,"-ST7565R controller");
  display.string(0,5,font_6x8,"-different backlights");
  display.string(0,6,font_6x8,"-extrem low power");
  display.string(0,7,font_8x8,"lcd-module.com");
}

void runSerialCommand(char cmd, int16_t value) {
  if (cmd == '?') {
    printHelp();
  } else if (cmd == 'l') {
    brightness = constrain(value, 0, 100);
    rgbBacklight();
  } else if (cmd == 'c') {
    display.contrast(value);
  } else if (cmd == 'w') {
    display.clear();
    sampleScreen();
  } else if (cmd == 'n') {
    display.view(VIEW_BOTTOM);
  } else if (cmd == 't') {
    display.view(VIEW_TOP);
  } else if (cmd == 'r') {
    red = constrain(value, 0, 255);
    rgbBacklight();
  } else if (cmd == 'g') {
    green = constrain(value, 0, 255);
    rgbBacklight();
  } else if (cmd == 'b') {
    blue = constrain(value, 0, 255);
    rgbBacklight();
  } else if (cmd == 'i') {
    invert = (invert == VIEW_NORMAL ? VIEW_INVERT : VIEW_NORMAL);
    display.invert(invert);
  }
}
