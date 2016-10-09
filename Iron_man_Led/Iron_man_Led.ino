/*
 * Iron Man NeoPixel Ring Control Sketch
 * 
 * This sketch is to beused with a 24 pixel neopxiel ring and used to display number of effects and patterns:
 * -  The standard iron man whitish blue colour
 * -  Rainbow effects 
 * -  loading ring
 * 
 * The setup is fairly simple:
 * -  the neopixel is wired to power with a 100uF electrolytic in parallel to protect it from surges
 * -  TODO: there is a switch wired in series with the power supply
 * -  the signal wire is connected to pin 3
 * -  TODO: a pushbutton switch is wired to pin ___
 * 
 * 
 */

#include <Adafruit_NeoPixel.h>

#define PIXEL_PIN 4
#define BUTTON_PIN 3

Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

uint8_t count = 0;
uint8_t colour_index = 0;

int led_leader = 0;
int position_lag = 0;
int position_lag1 = 0;

boolean button_state = false;
#define BUTTON_DEBOUNCE_TIME 200
#define BUTON_PRESS_TIME 1000
uint8_t button_press_count = 0;
unsigned long button_timer = 0;
unsigned long button_press_timer = 0;

#define BRIGHTNESS_FACTOR 10


uint32_t iron_man = strip.Color(150,150,255);
uint32_t off = strip.Color(0,0,0);

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
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
  // update pixel ring
  
  // wait for specified delay time whilst checking state of button
  // check if button pressed and if so take millis
  if(!digitalRead(BUTTON_PIN) & button_state == false){
    button_timer = millis();
    button_state = true;
  }
  // check for button release
  else if(digitalRead(BUTTON_PIN) & button_state == true){
    button_timer = millis() - button_timer;
    if(button_timer > BUTTON_DEBOUNCE_TIME){
      if(button_press_count == 0){
        button_press_count++;
        button_press_timer = millis();
      }
      else{
        button_press_timer = millis() - button_press_timer;
        if(button_press_timer < BUTTON_PRESS_TIME){
          // finish this off...
        }
      
    }
  }
}

void allLedsOff(void){
  for(int n = 0; n<24; n++){
    strip.setPixelColor(n, off);
  }
  strip.show();
}

void allLedsOn(uint32_t colour){
    for(int n = 0; n<24; n++){
      strip.setPixelColor(n, colour);
    }
    strip.show();
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
