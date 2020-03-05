#include <Clock.h>

#include <FastLED.h>

#define BUTTON_1_PIN 4
#define BUTTON_2_PIN 6
#define BUTTON_3_PIN 8

#define BUZZER_PIN   3

#define ROTARY_CLK_PIN  A0
#define ROTARY_DT_PIN   A1
#define ROTARY_SW_PIN   A2

class Button{

public:
  Button(int pin){
    m_pin = pin;
    m_state = HIGH;
    pinMode(m_pin, INPUT_PULLUP);
  }

public:
  int m_pin;
  int m_state;
public:

  int getStateNow(){
    return digitalRead(m_pin);
  }

  bool wasPressed(){
    bool wasPressed = (digitalRead(m_pin) == LOW && m_state == HIGH);
    m_state = digitalRead(m_pin);
    return wasPressed;
  }

};

class Time{

public:
  Time() : m_s(0), m_m(0), m_h(0), m_millis_to_second(1000) {
    m_millis_last_update = millis();
  }

public:
  int m_s;
  int m_m;
  int m_h;
  int m_millis_to_second;
  int m_millis_last_update;

  void update(){
    int delta = millis() - m_millis_last_update;
    m_millis_last_update = millis();
    m_millis_to_second -= delta;

    if(m_millis_to_second <= 0){
      m_millis_to_second += 1000;
      addSecond();
      updateScreen();

    }
  }

  void updateScreen(){
    Clock::turnOnTwoPoints();
    Clock::displayMinutes(m_m);
    Clock::displayHours(h_m);
    Clock::updateScreen();
  }

  void addSecond(){
    m_s++;

    if(m_s >= 60){
      m_s = 0;
      m_m++;
    }

    if(m_m >= 60){
      m_m = 0;
      m_h++;
    }
    if(m_h >= 24){
      m_h = 0;
    }
  }

};

void checkButtons(){
  if(btn1.wasPressed()){
    button1_callback();
  }

  if(btn2.wasPressed()){
    button1_callback();
  }

  if(btn3.wasPressed()){
    button1_callback();
  }
}

void button1_callback(){
     Clock::turnOffScreen();
}
void button2_callback(){}
void button3_callback(){}


void setup() {

  FastLED.addLeds<WS2812, LEDS_PIN, GRB>(Clock::leds, NUM_LEDS);
  // setting the mosfet pin as output to switch on and off the leds power

  pinMode(LEDS_SWITCH_PIN, OUTPUT);

  // turning on the screen
  Clock::turnOnScreen();

  // clearing the display of the clock
  Clock::clearScreenAndUpdate();


  Button btn1(BUTTON_1_PIN);
  Button btn2(BUTTON_2_PIN);
  Button btn3(BUTTON_3_PIN);

  Time myTime;

}

void loop() {

  checkButtons();

  myTime.update();

}
