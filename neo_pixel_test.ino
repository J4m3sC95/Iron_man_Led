#include <Adafruit_NeoPixel.h>

#define PIN 3

Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, PIN, NEO_GRB + NEO_KHZ800);

uint8_t count = 0;
uint8_t colour_index = 0;

int led_leader = 0;
int position_lag = 0;
int position_lag1 = 0;

#define BRIGHTNESS_FACTOR 10

uint32_t iron_man = strip.Color(150,150,255);
uint32_t off = strip.Color(0,0,0);

void setup() {
  strip.begin();
  strip.show();
  strip.setBrightness(64);
  /*
  for(int n = 0; n<24; n++){
    strip.setPixelColor(n, iron_man);
  }
  strip.show();
  */
}

void loop() {
  randomPixelRing();
  randomRing();
}

void randomRing(void){
  uint32_t ringColour = Wheel(random(255));
  for(int n = 0; n<24; n++){
    strip.setPixelColor(n, ringColour);
  }
  strip.show();
  delay(200);
  for(int n = 0; n<24; n++){
    strip.setPixelColor(n, off);
  }
  strip.show();
  delay(100);
}

void randomPixelRing(void){
  for(int n = 0; n<24; n++){
    strip.setPixelColor(n, Wheel(random(255)));
  }
  strip.show();
  delay(200);
  for(int n = 0; n<24; n++){
    strip.setPixelColor(n, off);
  }
  strip.show();
  delay(100);
}


void loading(){
  for(int n = 0; n<24; n++){
    position_lag = n-led_leader;
    position_lag1 = (n-24)-led_leader;
    if(abs(position_lag1) < abs(position_lag)){
      position_lag = position_lag1;      
    }
    if((position_lag > -4) && (position_lag <= 0)){
      strip.setPixelColor(n,255/(BRIGHTNESS_FACTOR*position_lag),255/(BRIGHTNESS_FACTOR*position_lag),255/(BRIGHTNESS_FACTOR*position_lag));
    }
    else{
      strip.setPixelColor(n, 0,0,0);
    }
  }
  strip.show();
  led_leader++;
  if(led_leader == 24){
    led_leader = 0;
  }
  delay(100);
}

void rainbow_wheel(){
    for(int n=0; n<24; n++){
    colour_index = count + (n*10);
    strip.setPixelColor(n, Wheel(colour_index));
  }
  strip.show();
  delay(10);
  count++;
}

void rainbow(){
  for(uint8_t n = 0; n<=255; n++){
    for(int m = 0; m< 24; m++){
      strip.setPixelColor(m, Wheel(n));
    }
    strip.show();
    delay(10);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
