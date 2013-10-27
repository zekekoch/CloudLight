#include <Adafruit_NeoPixel.h>

#define PIN 2

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
const byte ledCount = 88;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(ledCount, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  pinMode(0, INPUT);
  pinMode(1, INPUT);
}

void loopb()
{
  setAllColor(0, 0, 255);
  delay(random(500,5000));
  for(int i = 0;i<128;i++)
    flicker(240, 128);
  colorWipe(strip.Color(0, 0, 255), 1); // Blue
}

void loop()
{
  sunset(0);
  sunset(15);
  sunset(30);
  sunset(45);
  sunset(45);
}

void sunset(int  hue)
{
  const byte delayTime = 20;
  for(int i = 0;i<=255;i++)
  {
    setAllHSV(hue, i, 128);
    delay(delayTime);
  }
  for(int i = 128;i>=0;i--)
  {
    setAllHSV(hue, 255, i);
    delay(delayTime);
  }
}

int sunsetHue(int myHue)
{
  myHue--;
  if (myHue == 0)
  {
    myHue = 360;
  }
  
  if (myHue = 255)
  {
    myHue = 45;
  }
  return myHue;
}


void looped() {
  static boolean mode = false;
  static boolean modeLightning = false;
  // Some example procedures showing how to display to the pixels:
  if (digitalRead(0) == HIGH)
    mode = !mode;
    
  if (digitalRead(1) == HIGH)
    modeLightning = !modeLightning;

    if (modeLightning)
    {
      rainbow(5);
      modeLightning = !modeLightning;
    }
     
  if (mode)
    colorWipe(strip.Color(0, 0, 255), 1); // Blue
  else
  {
    {
      colorWipe(strip.Color(255, 0, 0), 1); // Red
    }
  }
  
  
//  colorWipe(strip.Color(0, 255, 0), 50); // Green
//  rainbow(20);
//  rainbowCycle(20);
}

void setAllColor(byte r, byte g, byte b)
{
  for (int i = 0; i< ledCount;i++)
  {
    strip.setPixelColor(i, r, g, b);
  }
}

void setAllHSV(unsigned int  h, byte  s, byte  v)
{
  for (int i = 0; i< ledCount;i++)
  {
    setHSV(i, h, s, v);
  }
  strip.show();
}

void flicker(int thishue, int thissat) {            //-m9-FLICKER EFFECT
  int random_bright = random(0,255);
  int random_delay = random(10,100);
  int random_bool = random(0,random_bright);
  if (random_bool < 10) {
    for(int i = 0 ; i < ledCount; i++ ) {
      setHSV(i, thishue, thissat, random_bright); 
    }
    strip.show();    
    delay(random_delay);
  }
}

void setHSV(int led, unsigned int hue, byte sat, byte val)
{
        unsigned char r,g,b;
        unsigned int H_accent = hue/60;
        unsigned int bottom = ((255 - sat) * val)>>8;
        unsigned int top = val;
        unsigned char rising  = ((top-bottom)  *(hue%60   )  )  /  60  +  bottom;
        unsigned char falling = ((top-bottom)  *(60-hue%60)  )  /  60  +  bottom;

        switch(H_accent) {
        case 0:
                r = top;
                g = rising;
                b = bottom;
                break;

        case 1:
                r = falling;
                g = top;
                b = bottom;
                break;

        case 2:
                r = bottom;
                g = top;
                b = rising;
                break;

        case 3:
                r = bottom;
                g = falling;
                b = top;
                break;

        case 4:
                r = rising;
                g = bottom;
                b = top;
                break;

        case 5:
                r = top;
                g = bottom;
                b = falling;
                break;
        }
        strip.setPixelColor(led, r, g, b);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

