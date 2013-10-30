#include <Adafruit_NeoPixel.h>

#define PIN 2

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
#define ledCount 88
Adafruit_NeoPixel strip = Adafruit_NeoPixel(ledCount, PIN, NEO_GRB + NEO_KHZ800);

enum mode {modeSunrise, modeSunset, modeDayStorm, modeNightStorm, modeRainbow};
mode currentMode = modeDayStorm;

#ifndef __AVR_ATtiny85__
  const byte btnRainbow = 6;
  const byte btnChangeDay = 7;
#endif

#ifdef  __AVR_ATtiny85__
  const byte btnRainbow = 0;
  const byte btnChangeDay = 1;  
#endif  

void println(String s = "")
{
  #ifndef __AVR_ATtiny85__
  Serial.println(s);
  #endif  
}

void print(int s)
{
  #ifndef __AVR_ATtiny85__
  Serial.print(String(s));
  #endif
}
void print(String s)
{
  #ifndef __AVR_ATtiny85__
  Serial.print(s);
  #endif  
}


void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  pinMode(btnRainbow, INPUT);
  pinMode(btnChangeDay, INPUT);
  
  //Serial.begin(9600);
}

void loop()
{
  switch(currentMode)
  {
    case modeSunrise:
      sunrise();
      break;
    case modeSunset:
      sunset();
      break;
    case modeDayStorm:
      storm(); // daystorm
      break;
    case modeNightStorm:
      storm(); // daystorm
      break;
    case modeRainbow:
      rainbowLoop();
      break;
    default:
      break;
  }
  
  checkForEvent();
}

void storm()
{
  //println(("in storm"));

  static int stormCount = 0;
  static bool inLightning = false;
  
  if (stormCount == 10)
  {
    // rainbows only happen in the day
    if (currentMode == modeNightStorm)
      currentMode = modeSunrise;
    else
      currentMode = modeRainbow;
    stormCount=0;
  }
  
  if (currentMode == modeDayStorm)
    setAllHSV(40, 1, 128); // daystorm
  else
    setAllHSV(244, 255, 20);  // nightstorm

  strip.show();
  
  if (random(500) == 0)
  {
    inLightning = true;
    stormCount++;
  }
  
  if (inLightning)
  {
    for(int i = 0;i<128;i++)
    {
      flicker(240, 128);
      strip.show();
    }
    inLightning = false;
  }
}

void checkForEvent()
{
  //print(btnRainbow);print(" ");print(digitalRead(btnRainbow));print(" ");print(digitalRead(btnStorm));println();
  
  if (digitalRead(btnRainbow) == HIGH)
  {
    //println("btn high in modeRainbow");
    currentMode = modeRainbow;
  } 
  if (digitalRead(btnChangeDay) == HIGH)
  {
    //println("btn high in modeStorm");
    switch(currentMode)
    {
      case modeRainbow:
      case modeDayStorm:
        currentMode = modeSunset;
        break;
      case modeNightStorm:
        currentMode = modeSunrise;
        break;
      default:
        break;
    }      
  }  
}

void sunset()
{
  int hue = 45;
  //print("sunset ");print(hue);println();
  const byte delayTime = 15;
  for(int i = 0;i<=255;i++)
  {
    if (i % 5 == 0)
      hue = sunsetHue(hue);
    //print("HSVt ");print(hue);print(" ");print(i);print(" ");print(128);println();

    setAllHSV(hue, i, 128);
    strip.show();
    delay(delayTime);
  }
  for(int i = 128;i>=20;i--)
  {
    if (i % 1 == 0)
      hue = sunsetHue(hue);
    //Serial.print("HSVt ");Serial.print(hue);Serial.print(" ");Serial.print(255);Serial.print(" ");Serial.print(i);Serial.println();
    setAllHSV(hue, 255, i);
    strip.show();
    delay(delayTime);
  }
  
  currentMode = modeNightStorm;
}

void sunrise()
{
  int hue = 45;
  //print("begin sunrise ");print(hue);println();
  const byte delayTime = 15;
  for(int i = 20;i<=128;i++)
  {
    if (i % 1 == 0)
      hue = sunriseHue(hue);
    //Serial.print("HSVt ");Serial.print(hue);Serial.print(" ");Serial.print(255);Serial.print(" ");Serial.print(i);Serial.println();
    setAllHSV(hue, 255, i);
    strip.show();
    delay(delayTime);
  }
  for(int i = 255;i>=0;i--)
  {
    if (i % 5 == 0)
      hue = sunriseHue(hue);
    //Serial.print("HSVt ");Serial.print(hue);Serial.print(" ");Serial.print(i);Serial.print(" ");Serial.print(128);Serial.println();

    setAllHSV(hue, i, 128);
    strip.show();
    delay(delayTime);
  }
  //print("end sunrise ");print(hue);println();
  
  currentMode = modeDayStorm;
}


int sunsetHue(int myHue)
{
  if (myHue == 0)
  {
    myHue = 360;
  }
  
  if (myHue == 240)
  {
    myHue = 45;
  }
  myHue--;
  return myHue;
}

int sunriseHue(int myHue)
{
  if (myHue == 360)
  {
    myHue = 0;
  }
  
  if (myHue == 45)
  {
    myHue = 240;
  }
  myHue++;
  return myHue;
}

void rainbowLoop() {              //-m3-LOOP HSV RAINBOW
  static int hue = 0;
  static byte currentFrame = 0;
  currentFrame++;
  //print(currentFrame);println();
  
  for (int pixel = 0;pixel < ledCount;pixel++)
  {
    hue += 360/ledCount;
    if (hue > 360)
      hue = 0;    
    setHSV(pixel, hue, 255, 255);
  } 
  strip.show();
  delay(15);
  hue++;
  if(currentFrame == 255)
  {
    currentMode = modeSunset;
  }
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
}

void flicker(int thishue, int thissat) {            //-m9-FLICKER EFFECT
  int random_bright = random(0,255);
  int random_delay = random(10,100);
  int random_bool = random(0,random_bright);
  if (random_bool < 10) {
    delay(random_delay);
    for(int i = 0 ; i < ledCount; i++ ) {
      setHSV(i, thishue, thissat, random_bright); 
    }
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
