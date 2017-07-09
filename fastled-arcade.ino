/*This is a remix of Jason Coon's great FastLED Arcade software, originally written for a Teensy microcontroller. 
//https://github.com/jasoncoon/fastled-arcade
This could also be used with an ESP8266, but there are not enough pins on the ESP8266 to have 5 buttons with 
separate LEDs and a mode switch, but the pins all have pullup resistors built in. You could wire the LEDs to 
illuminate when pressed, but you'd lose some of the functionality of the "simon" game. I'll keep this option 
in mind for another project.

__Changes from Jason's FastLED Arcade code__
- Removed support for infrared remote to free up some memory. I am using a mode change button that ties a pin to ground when pressed.
- Added mode indication lights. Since Mode 1 is loaded on startup, the first time the mode button is pressed, it will 
show the lights for mode 2.
- Removed one of the games (launcher) because it used too much memory in global variables. Perhaps it could be added back with fewer balls.
- Reversed the logic for lighting arcade buttons in the main sketch and also on the "simon" game. I chose to wire the LEDS
so that the Arduino would sink current instead of source it.
- Changed the velocityDecay constant (Juggle game) from .0036 to .0044 to accomodate 112 LEDs instead of 144.
- Used PWM-capable pins for the arcade button LEDs so that fading effects using PWM could be used in the future
*/

#include <Bounce2.h> //https://github.com/thomasfredericks/Bounce2 This library is available in the Arduino IDE's library manager.
#include <FastLED.h>
//#include <IRremote.h>

FASTLED_USING_NAMESPACE

#define DATA_PIN    11
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    112

//#define MILLI_AMPS         1000     // IMPORTANT: set the max milli-Amps of your power supply (4A = 4000mA)

#define IR_RECV_PIN 2
#define MODE_PIN    4

CRGB leds[NUM_LEDS];

//IRrecv irReceiver(IR_RECV_PIN);

//#include "irCommands.h"

//InputCommand command;

bool modeInit = true;

//const uint8_t brightnessCount = 5;
//uint8_t brightnessMap[brightnessCount] = { 16, 32, 64, 128, 255 };
uint8_t brightness = 255;

const uint8_t buttonCount = 5;
//Original order = Red Grn Blu Yell Whit

uint8_t buttonPins[] = {
  16,  // red
  17, // green
  18, // blue
  15,  // yellow
  14,  // white (A0)
};

uint8_t ledPins[] = {
  6, // red (leds 43-68)
  9, // green (leds 69-92)
  10, // blue (leds 93-111
  5, // yellow (leds 19-42)
  3, // white (leds 0-18)
};

Bounce buttonMode = Bounce();

Bounce buttonRed = Bounce();
Bounce buttonGreen = Bounce();
Bounce buttonBlue = Bounce();
Bounce buttonYellow = Bounce();
Bounce buttonWhite = Bounce();

Bounce buttons[] = {
  buttonRed,
  buttonGreen,
  buttonBlue,
  buttonYellow,
  buttonWhite,
};

bool buttonChanged[5];

unsigned long buttonPressTimes[5];

String buttonNames[] = {
  "Red",
  "Green",
  "Blue",
  "Yellow",
  "White",
};

CHSV buttonColors[] = {
  CHSV(0, 255, 128),   // red
  CHSV(96, 255, 128),  // green
  CHSV(160, 255, 128), // blue
  CHSV(64, 255, 128),  // yellow
  CHSV(0, 0, 128),     // white
};

#include "ball.h"
#include "juggle.h"
//#include "launcher.h"
#include "colorInvaders.h"
#include "simon.h"
#include "shooter.h"
#include "fireworks.h"

void pulse() {
  shiftDown();

  fadeToBlackBy(leds, 1, 10);

  for (uint8_t i = 0; i < buttonCount; i++) {
    if (buttons[i].fell()) {
      leds[0] += buttonColors[i];
    }
  }
}

void add() {
  shiftDown();

  fadeToBlackBy(leds, 1, 10);

  for (uint8_t i = 0; i < buttonCount; i++) {
    if (!buttons[i].read()) {
      leds[0] += buttonColors[i];
    }
  }
}

// List of modes.  Each is defined as a separate function below.
uint8_t gHue = 0; // rotating "base color" used by the bpm pattern
typedef void (*SimpleModeList[])();
SimpleModeList modes = {
  bpm,
  shooter,
  fireworks,
// launcher,
  pulse,
  add,
  colorInvaders,
  simon,
  juggle
};

uint8_t currentModeIndex = 0; // Index number of which mode is current

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

const int modeCount = ARRAY_SIZE(modes);

void setup() {
  //Serial.begin(9600);

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(brightness);
  //  FastLED.setMaxPowerInVoltsAndMilliamps(5, MILLI_AMPS);

  for (uint8_t i = 0; i < buttonCount; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  pinMode(MODE_PIN, INPUT_PULLUP);
    
  for (uint8_t i = 0; i < buttonCount; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  buttonMode.attach(MODE_PIN);

  for (uint8_t i = 0; i < buttonCount; i++) {
    buttons[i].attach(buttonPins[i]);
  }

  // Initialize the IR receiver
//  irReceiver.enableIRIn();
//  irReceiver.blink13(true);
}

void loop() {
  random16_add_entropy(random());

  for (uint8_t i = 0; i < buttonCount; i++) {
    buttonChanged[i] = buttons[i].update();
    if (buttonChanged[i]) {
      buttonPressTimes[i] = millis();

  //     Serial.print(buttonNames[i]);
  //     Serial.println(" button");

      if (buttons[i].fell()) {
        // Serial.print(" pressed time: ");
        // Serial.println(buttonPressTimes[i]);
        digitalWrite(ledPins[i], LOW);
      } else {
        // Serial.println(" released");
        digitalWrite(ledPins[i], HIGH);
      }
    }
  }

  modes[currentModeIndex]();

  modeInit = false;

  FastLED.show();
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  handleInput();
}

void move(int delta) {
  moveTo(currentModeIndex + delta);
}

void moveTo(int index) {
  currentModeIndex = index;

  if (currentModeIndex >= modeCount)
    currentModeIndex = 0;
  else if (currentModeIndex < 0)
    currentModeIndex = modeCount - 1;

  modeInit = true;
//Serial.print("current mode = ");
//Serial.println(currentModeIndex);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  uint8_t i[] ={0,19,43,69,93,0,0,0}; //specific start points for the Wheat Sculpture 7 game modes, 1 standby mode
  uint8_t j[] ={19,24,26,24,19,43,69,93}; //specific ranges for the Wheat Sculpture
  fill_solid( &(leds[i[currentModeIndex]]), j[currentModeIndex], CHSV(currentModeIndex*30, 255, 255) ); //fills that range with a different color
  FastLED.show();
  FastLED.delay(1000); //shows mode selection for 1 second.
  fill_solid(leds, NUM_LEDS, CRGB::Black);
}
/*
int getBrightnessLevel() {
  int level = 0;
  for (int i = 0; i < brightnessCount; i++) {
    if (brightnessMap[i] >= brightness) {
      level = i;
      break;
    }
  }
  return level;
}

void adjustBrightness(int delta) {
  int level = getBrightnessLevel();

  level += delta;
  if (level < 0)
    level = 0;
  if (level >= brightnessCount)
    level = brightnessCount - 1;

  brightness = brightnessMap[level];
  FastLED.setBrightness(brightness);
}
*/
void handleInput() {
  if(buttonMode.update() && buttonMode.fell())   {
    move(1);
  }
  /*
  command = readCommand(defaultHoldDelay);

  if (command != InputCommand::None) {
    // Serial.print("command: ");
    // Serial.println((int) command);
  }

  if (command == InputCommand::Up) {
    move(1);
  }
  else if (command == InputCommand::Down) {
    move(-1);
  }
  else if (command == InputCommand::Left) {
    move(-1);
  }
  else if (command == InputCommand::Right) {
    move(1);
  }
  else if (command == InputCommand::Power) {
    powerOff();
  }
  else if (command == InputCommand::BrightnessUp) {
    adjustBrightness(1);
  }
  else if (command == InputCommand::BrightnessDown) {
    adjustBrightness(-1);
  }

  // pattern buttons

  else if (command == InputCommand::Pattern1) {
    moveTo(0);
  }
  else if (command == InputCommand::Pattern2) {
    moveTo(1);
  }
  else if (command == InputCommand::Pattern3) {
    moveTo(2);
  }
  else if (command == InputCommand::Pattern4) {
    moveTo(3);
  }
  else if (command == InputCommand::Pattern5) {
    moveTo(4);
  }
  else if (command == InputCommand::Pattern6) {
    moveTo(5);
  }
  else if (command == InputCommand::Pattern7) {
    moveTo(6);
  }
  else if (command == InputCommand::Pattern8) {
    moveTo(7);
  }
  else if (command == InputCommand::Pattern9) {
    moveTo(8);
  }
  else if (command == InputCommand::Pattern10) {
    moveTo(9);
  }
  else if (command == InputCommand::Pattern11) {
    moveTo(10);
  }
  else if (command == InputCommand::Pattern12) {
    moveTo(11);
  }
}

void powerOff()
{
  // clear the display
  // fill_solid(leds, NUM_LEDS, CRGB::Black);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
    FastLED.show(); // display this frame
    delay(1);
  }

  FastLED.show(); // display this frame

  while (true) {
    InputCommand command = readCommand();
    if (command == InputCommand::Power ||
        command == InputCommand::Brightness)

      // go idle for a while, converve power
      delay(250);
  }
*/
}
void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void shiftDown() {
  for (int i = NUM_LEDS - 1; i > 0; i--) {
    leds[i] = leds[i - 1];
  }
}
