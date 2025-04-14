#include <Arduino.h>

hw_timer_t *timer = timerBegin(0, 80, true); // Timer 0, prescaler 80 (1 tick = 1us)
bool reset_button = false;
int second_counter = 0;


void IRAM_ATTR onTimer() {
  // This function will be called every second
  Serial.println("Timer interrupt triggered!");
  second_counter++;
  if (second_counter <= 60) {
    Serial.println(second_counter);
  } 
}

void IRAM_ATTR button_interrupt(){  
  reset_button = true;
  Serial.println("Button pressed!");
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(D5, INPUT_PULLUP); 
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000000, true);  
  timerAlarmEnable(timer);
  attachInterrupt(digitalPinToInterrupt(D5), button_interrupt, FALLING);
}


void loop() {
  static unsigned long last_time = 0;  // Variable to track the last time we checked
  unsigned long current_time = millis();

  // Check if 1 minute has passed (60000 ms)
  if (current_time - last_time >= 60000) {
    last_time = current_time;  // Reset the last_time after 1 minute has passed
    Serial.println("1 minute passed!");
    second_counter = 0;  // Reset the second counter
  }

  // Reset the timer if the button was pressed
  if (reset_button) {
    last_time = current_time;  // Reset the last_time after 1 minute has passed
    reset_button = false;
    Serial.println("Resetting timer!");
    second_counter = 0;  // Reset the counter immediately
  }

  delay(100); // Small delay to avoid flooding the serial output
}
