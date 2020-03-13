#include <Clock.h>

#include <FastLED.h>

#define BUTTON_1_PIN 4
#define BUTTON_2_PIN 6
#define BUTTON_3_PIN 8

#define BUZZER_PIN   3

#define ROTARY_CLK_PIN  A0
#define ROTARY_DT_PIN   A1
#define ROTARY_SW_PIN   A2

#define BUTTON_COOLOFF_TIME 10

class Button{

public:
  Button(int pin){
    m_pin = pin;
    m_state = HIGH;
    m_last_pressed = 0;
    pinMode(m_pin, INPUT_PULLUP);
  }

public:
  int m_pin;
  int m_state;
  int m_last_pressed;
public:

  int getStateNow(){
    return digitalRead(m_pin);
  }

  bool wasPressed(){

    if(millis() - m_last_pressed < BUTTON_COOLOFF_TIME) // too soon
      return false;

    bool wasPressed = (digitalRead(m_pin) == LOW && m_state == HIGH);
    m_state = digitalRead(m_pin);
    m_last_pressed = millis();
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
      addSeconds(1);
      updateScreen();

    }
  }

  void updateScreen(){
    Clock::turnOnTwoPoints();
    Clock::displayMinutes(m_m);
    Clock::displayHours(m_h);
    Clock::updateScreen();
  }

  void addSeconds(int s){
    m_s += s;

    if(m_s >= 60){
      m_m += m_s / 60;
      m_s = m_s % 60;
    }

    if(m_m >= 60){
      m_h += m_m / 60;
      m_m = m_m % 60;
    }
    if(m_h >= 24){
      m_h = m_h % 24;
    }
  }

};

enum class State{
  TIME,
  CHRONOMETER,
  TIMER
};

void checkButtons();
void button1_time_callback();
void button2_time_callback();
void button3_time_callback();

State myState = State::TIME;
Time* myTime;

Button* btn1;
Button* btn2;
Button* btn3;

void setup() {

  Serial.begin(9600);
  FastLED.addLeds<WS2812, LEDS_PIN, GRB>(Clock::leds, NUM_LEDS);
  // setting the mosfet pin as output to switch on and off the leds power

  pinMode(LEDS_SWITCH_PIN, OUTPUT);

  // turning on the screen
  Clock::turnOnScreen();

  // clearing the display of the clock
  Clock::clearScreenAndUpdate();

  Clock::turnOnTwoPointsAndUpdate();

  btn1 = new Button(BUTTON_1_PIN);
  btn2 = new Button(BUTTON_2_PIN);
  btn3 = new Button(BUTTON_3_PIN);

  myTime = new Time();

}

void loop() {

  checkButtons();

  myTime->update();

}

void checkButtons(){
  if(btn1->wasPressed()){
    switch(myState){
      case State::TIME:
        button1_time_callback();
      break;
    }
  }

  if(btn2->wasPressed()){
     switch(myState){
      case State::TIME:
        button2_time_callback();
      break;
    }
  }

  if(btn3->wasPressed()){
     switch(myState){
      case State::TIME:
        button3_time_callback();
      break;
    }
  }
}

void button1_time_callback(){
   myTime->addSeconds(60);
   myTime->updateScreen();
}
void button2_time_callback(){
   myTime->addSeconds(3600);
   myTime->updateScreen();
}
void button3_time_callback(){
  Clock::switchScreenOnOff();
  myTime->updateScreen(); //fix to delay when turning on
}
