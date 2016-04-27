#include <Adafruit_NeoPixel.h>

const int LED_PIN = 13;
const int NUM_PIXELS = 288;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS
, LED_PIN, NEO_GRB + NEO_KHZ800);

int lightPositions[5] = {0,0,0,0,0};
uint32_t lightColors[5] = {strip.Color(255, 0, 0), strip.Color(0, 255, 0), strip.Color(0, 0, 255), strip.Color(255, 0, 255), strip.Color(255, 255, 0)};

void setup() {
  // put your setup code here, to run once:
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
  
}

void loop() {
  if (Serial.available() > 0) {
          // read the incoming byte:
          byte incomingByte = Serial.read();

          /* say what you got:
          Serial.print("I received: ");
          Serial.println(incomingByte, DEC);
          */
          
          strip.setPixelColor(lightPositions[(int) incomingByte], 0, 0, 0);
          lightPositions[(int) incomingByte]++;
          lightPositions[(int) incomingByte]%=NUM_PIXELS;

          
          strip.setPixelColor(lightPositions[(int) incomingByte], lightColors[incomingByte]);

//          for (int i = 0; i < NUM_PIXELS; i++) {
//            strip.setPixelColor(i, 0, 0, 0);
//          }
//          for (int i = 0; i < 5; i++) {
//            strip.setPixelColor(lightPositions[i], 0, 0, 255);
//          }          
//      
          strip.show();
        }
}
