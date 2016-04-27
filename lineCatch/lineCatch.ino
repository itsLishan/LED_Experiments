#include <Adafruit_NeoPixel.h>
const int LED_PIN = 13;
const int NUM_PIXELS = 288;
const int NUM_USERS = 6;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS
                          , LED_PIN, NEO_GRB + NEO_KHZ800);

#include "Win_Functions.h"

double speed = 1;    //pixels per second
float speedGrowthRate = 0.5;
int radius = 1;
int radiusgrowthRate = 2;
int userRadius = 5;
int maxLineRadius = 50;


double linePosition = 0;
uint32_t userColors[NUM_USERS + 1] = {strip.Color(102, 30, 217), strip.Color(0, 255, 0), strip.Color(0, 0, 255), strip.Color(255, 0, 255), strip.Color(255, 255, 0), strip.Color(255, 50, 0)};
uint32_t currentColor;
int userPositions[NUM_USERS];
int userSectionLength = NUM_PIXELS / NUM_USERS;

unsigned long timeOfLastIncrement = 0;

typedef enum {
  GM_SETUP = 0,
  GM_GAME = 1,
  GM_WIN = 2,
} GAME_MODE;

GAME_MODE gameState = GM_GAME;


void initializeGameMode () {
  speed = 1;
  radius = 1;
  linePosition = 0;
  currentColor = strip.Color(255, 255, 255);
}

void setup() {
  // put your setup code here, to run once:
  strip.begin();
  strip.setBrightness(30);
  strip.show(); // Initialize all pixels to 'off'
  Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
  for (int i = 0; i < NUM_USERS; i++) {
    userPositions[i] = i * userSectionLength + userSectionLength / 2;
  }
  initializeGameMode();

}

int gameLoop() {

  for (int i = 0; i < NUM_USERS; i++) {
    setPositionColor(userPositions[i], userColors[i]);
  }


  if (Serial.available() > 0) {

    // if (true){
    // read the incoming byte:
    byte incomingByte = Serial.read();
    // byte incomingByte = 1;

    if (abs(linePosition - userPositions[incomingByte]) <= (radius + userRadius)) {
      currentColor = userColors[incomingByte];
      radius += radiusgrowthRate;
      speed  += speedGrowthRate;
    }

    else {
      setPositionColor(userPositions[incomingByte], strip.Color(255, 0, 0));
    }

  }


  if (millis() - timeOfLastIncrement >= 1) { //if new time increment, move line
    timeOfLastIncrement = millis();

    linePosition = linePosition + speed;
    linePosition = linePosition > NUM_PIXELS ? 0 : linePosition;
  }


  for (int i = 0; i < NUM_PIXELS; i++) {
    if (abs(linePosition - i) <= radius) {
      strip.setPixelColor(i, currentColor);
    }
    else if ((i + userSectionLength / 2) % userSectionLength >= userRadius) {
      strip.setPixelColor(i, 0, 0, 0);
    }
  }
  strip.show();

  Serial.println(radius);

  return (radius >= maxLineRadius) ? GM_WIN : GM_GAME;
}

void winLoop() {


  theaterChaseRainbow(30);
}


void setPositionColor(int position, uint32_t newColor) {
  strip.setPixelColor(position, newColor);
  for (int j = 1; j <= userRadius; j++) {
    strip.setPixelColor(position + j, newColor);
    strip.setPixelColor(position - j, newColor);
  }
}

void loop() {
  switch (gameState) {
  case GM_SETUP:
    // do something
    break;
  case GM_GAME:
    gameState = (GAME_MODE)gameLoop();
    break;
  case GM_WIN:
    winLoop();
    initializeGameMode();
    gameState = GM_GAME;
    break;
  default:
    // do something
    break;
  }
}
