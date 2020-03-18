#include <Clock.h>

#include <FastLED.h>

#define BUTTON_1_PIN 4
#define BUTTON_2_PIN 6
#define BUTTON_3_PIN 8

#define BUZZER_PIN   3

#define ROTARY_CLK_PIN  A0
#define ROTARY_DT_PIN   A1
#define ROTARY_SW_PIN   A2

#define BUTTON_COOLOFF_TIME 200
#define ROTARY_COOLOFF_TIME 50

class Button{

public:
  int m_pin;
  int m_state;
  unsigned long m_last_pressed;

public:
  Button(int pin){
    m_pin = pin;
    m_state = HIGH;
    m_last_pressed = 0;
    pinMode(m_pin, INPUT_PULLUP);
  }

public:

  int getStateNow(){
    return digitalRead(m_pin);
  }

  bool wasPressed(){

    int newState = digitalRead(m_pin);

    bool wasPressed = ( newState == LOW && m_state == HIGH);
    m_state = newState;
    
    if(millis() - m_last_pressed < BUTTON_COOLOFF_TIME) // too soon
      return false;

    if(wasPressed)
      m_last_pressed = millis();
      
    return wasPressed;
  }

};

class RotaryEncoder{

public:
  enum{
    NOT_ROTATED,
    CLK,
    CCLK
  };

public:
  int m_pin_dt;
  int m_pin_clk;
  int m_state_dt;
  unsigned long m_last_rotated;

public:
  RotaryEncoder(int pin_dt, int pin_clk){
    m_pin_dt = pin_dt;
    m_pin_clk = pin_clk;
    m_state_dt = HIGH;
    m_last_rotated = 0;
    pinMode(m_pin_dt, INPUT_PULLUP);
    pinMode(m_pin_clk, INPUT_PULLUP);
}

public:

  int wasRotated(){

    int newState = digitalRead(m_pin_dt);

    bool wasPressed = ( newState == LOW && m_state_dt == HIGH);
    m_state_dt = newState;
    
    if(millis() - m_last_rotated < ROTARY_COOLOFF_TIME) // too soon
      return false;

    if(wasPressed)
      m_last_rotated = millis();

    if(wasPressed){
      if(digitalRead(m_pin_clk) == LOW)
        return CLK;
      else
        return CCLK;
    }
    else
      return NOT_ROTATED;
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
Button* rotaryBtn;
RotaryEncoder* rotary;

void setup() {

  Serial.begin(250000);
  FastLED.addLeds<WS2812, LEDS_PIN, GRB>(Clock::leds, NUM_LEDS);
  // setting the mosfet pin as output to switch on and off the leds power

  pinMode(LEDS_SWITCH_PIN, OUTPUT);
  pinMode(13, OUTPUT);
  // turning on the screen
  Clock::turnOnScreen();

  // clearing the display of the clock
  Clock::clearScreenAndUpdate();

  Clock::turnOnTwoPointsAndUpdate();

  btn1 = new Button(BUTTON_1_PIN);
  btn2 = new Button(BUTTON_2_PIN);
  btn3 = new Button(BUTTON_3_PIN);
  rotaryBtn = new Button(ROTARY_SW_PIN);
  rotary = new RotaryEncoder(ROTARY_DT_PIN, ROTARY_CLK_PIN);

  myTime = new Time();

}


void loop() {

  checkInputs();

  myTime->update();

}

void checkInputs(){
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

  if(rotaryBtn->wasPressed()){
     switch(myState){
      case State::TIME:
        rotaryButton_time_callback();
      break;
    }
  }

  int rotation = rotary->wasRotated();

  if(rotation == RotaryEncoder::CLK){
    switch(myState){
      case State::TIME:
        rotation_clk_time_callback();
      break;
    }
  }

  if(rotation == RotaryEncoder::CCLK){
    switch(myState){
      case State::TIME:
        rotation_cclk_time_callback();
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
  myTime->updateScreen(); //fix to delay when turning on screen
}
void rotaryButton_time_callback(){

}
void rotation_clk_time_callback(){

}
void rotation_cclk_time_callback(){

}
