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
 * -  TODO: there is a switch wired in series with the power supply??
 * -  the signal wire is connected to pin PIXEL_PIN
 * -  a pushbutton switch is wired to pin BUTTON_PIN for control
 * 
 * note: this version has been developed using an arduino mega and serial comms however final version intended for attiny85
 * 
 * 
 */

#include <Adafruit_NeoPixel.h>

#define PIXEL_PIN 4
#define BUTTON_PIN 8

Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

// effect numbers
#define EFFECT_COUNT          6
#define IRON_MAN              1
#define LOADING               2
#define RAINBOW               3
#define RAINBOW_WHEEL         4
#define RANDOM_COLOUR         5
#define RANDOM_PIXEL_COLOUR   6

int delay_array [EFFECT_COUNT + 1] = {1000, 100, 10, 10, 100, 100};

uint8_t count = 0;
uint8_t colour_index = 0;

int position_lag = 0;
int position_lag1 = 0;

uint32_t ringColour = 0;

boolean button_state = false;
#define BUTTON_DEBOUNCE_TIME 50
#define BUTTON_RESET_TIME 2000
#define BUTTON_PRESS_TIME 1000
uint8_t button_press_count = 0;
uint8_t previous_button_press_count = 0;
unsigned long button_timer = 0;
unsigned long button_press_timer = 0;
boolean button_timeout = false;
boolean button_press_timer_active = false;
int effect_delay = 1000;
unsigned long effect_timer = 0;

#define BRIGHTNESS_FACTOR 10

uint32_t iron_man = strip.Color(150,150,255);
uint32_t off = strip.Color(0,0,0);

void setup() {
  Serial.begin(115200);
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  strip.begin();
  strip.show();
  strip.setBrightness(64);
}

void loop() {
  // wait for specified delay time whilst checking state of button
  // check if button pressed and if so take millis
  if(!digitalRead(BUTTON_PIN) && button_state == false){
    button_timer = millis();
    button_state = true;
    Serial.print("Button pressed\t");
  }
  // check for button release
  else if(digitalRead(BUTTON_PIN) && button_state == true){
    button_state = false;
    button_timer = millis() - button_timer;
    Serial.print("button released after ");
    Serial.print(button_timer);
    Serial.println("ms\t");
    // if longer than off time, turn LEDs off
    if(button_timer > BUTTON_RESET_TIME){
      previous_button_press_count = 0;
      effect_delay = 1000;
      count = 0;
      effect_timer = 0;
      button_press_count = 0;
      Serial.println("Resetting!!\n");
    }
    // if longer than debounce time find out if within button_press_time and increment count if so else set timeout bit 
    else if(button_timer > BUTTON_DEBOUNCE_TIME){
      if(button_press_count == 0){
        button_press_timer_active = true;
      }
      button_press_count++;
      button_press_timer = millis();    
    }
  }
  else if(((millis() - button_press_timer) > BUTTON_PRESS_TIME) && (button_press_timer_active == true)){
    button_press_timer_active = false;
    button_timeout = true;
    Serial.print("Button press timeout, count = ");
    Serial.println(button_press_count);
    Serial.println("");
  }

  // check if button_timeout is true and respond by setting new display parameters
  if(button_timeout){
    button_timeout = false;
    count = 0;
    previous_button_press_count = button_press_count;
    button_press_count = 0;
    effect_timer = 0;
    if(previous_button_press_count <= EFFECT_COUNT){
      effect_delay = delay_array[previous_button_press_count - 1];
    }
    else{
      effect_delay = 1000;
    }
    
  }

  if((millis() - effect_timer) > effect_delay){
    // update pixel ring based on the previous button press count
    switch(previous_button_press_count){
      case IRON_MAN:
      {
        allLedsOn(iron_man);
      }
      break;
      case LOADING:
      {
        loading();
      }
      break;
      case RAINBOW:
      {
        rainbow();
      }
      break;
      case RAINBOW_WHEEL:
      {
        rainbow_wheel();
      }
      break;
      case RANDOM_COLOUR:
      {
        randomRingFlash();
      }
      break;
      case RANDOM_PIXEL_COLOUR:
      {
        randomPixelRingFlash();
      }
      break;
      default:
      {
        allLedsOff();
      }
      break;
    }
    effect_timer = millis();
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

void randomRingFlash(void){
  if(count == 0){
    ringColour = Wheel(random(255));
    allLedsOn(ringColour);
  }
  else if(count == 2){
    allLedsOff();
  }

  count++;
  if(count == 3){
    count = 0;
  }
  //delay(100);
}

void randomPixelRingFlash(void){
  if(count == 0){
    for(int n = 0; n<24; n++){
      strip.setPixelColor(n, Wheel(random(255)));
    }
    strip.show();
  }
  else if(count == 2){
    allLedsOff();
  }

  count++;
  if(count == 3){
    count = 0;
  }

  //delay(100);
}


void loading(void){
  for(int n = 0; n<24; n++){
    position_lag = n-count;
    position_lag1 = (n-24)-count;
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
  count++;
  if(count == 24){
    count = 0;
  }
  //delay(100);
}

void rainbow_wheel(){
  for(int n=0; n<24; n++){
    colour_index = count + (n*10);
    strip.setPixelColor(n, Wheel(colour_index));
  }
  strip.show();
  count++;
  //delay(10);
}

void rainbow(){
  for(int m = 0; m< 24; m++){
    strip.setPixelColor(m, Wheel(count));
  }
  strip.show();
  count++;
  //delay(10);
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
