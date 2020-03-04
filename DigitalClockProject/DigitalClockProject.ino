#include <Clock.h>

#include <FastLED.h>

#define BUTTON_1_PIN 4
#define BUTTON_2_PIN 6
#define BUTTON_3_PIN 8



int m = 0;
int h = 0;

int left_in_second = 1000;

void add1(){
  m++;
  if(m >= 60){
    m = m % 60;
    h++;
  }
  if(h >= 60){
    h = 0;
  }
}

int previous_state = HIGH;
int current_state = HIGH;

void displaytime(){
  Clock::turnOnTwoPoints();
  Clock::displayMinutes(m);
  Clock::displayHours(h);
  Clock::updateScreen();
}

void setup() {
//  Serial.begin(9600);
  pinMode(13, OUTPUT);

  // button pin
  pinMode(BUTTON_1_PIN, INPUT_PULLUP);
  
  digitalWrite(13, HIGH);
  
  FastLED.addLeds<WS2812, LEDS_PIN, GRB>(Clock::leds, NUM_LEDS);
  // setting the mosfet pin as output to switch on and off the leds power

  pinMode(LEDS_SWITCH_PIN, OUTPUT);

  // turning on the screen
  Clock::turnOnScreen();

  // clearing the display of the clock
  Clock::clearScreenAndUpdate();

  digitalWrite(13, LOW);
}

int last_start_time = 0;
long long timeout = 5000;

void loop() {

  current_state = digitalRead(BUTTON_1_PIN);
  if(previous_state == HIGH && current_state == LOW){
   // m = 0;
    //h = 0;
    //left_in_second = 1000;
   // displaytime();
  //  add1();
    //Clock::turnOnScreen();
      
     digitalWrite(LEDS_SWITCH_PIN, HIGH);

    //Clock::ClockOn = true;
  }

  int delta = millis() - last_start_time;
  last_start_time = millis();
  //left_in_second -= delta;

  timeout -= delta;
/*  
  if(left_in_second <= 0){
    left_in_second += 1000;
    displaytime();
    add1();

  }
*/
  if(timeout <= 0){
    digitalWrite(LEDS_PIN, HIGH);
    digitalWrite(LEDS_SWITCH_PIN, LOW);
    //Clock::turnOffScreen();
          digitalWrite(13, HIGH);
  delay(50);
  digitalWrite(13, LOW);
  }
  
  previous_state = current_state;


  
 // delay(500);
}
