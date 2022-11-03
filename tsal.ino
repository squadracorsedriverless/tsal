/*
  TSAL.ino

  Copyright Squadra Corse PoliTO
*/

// Pinout
#define TSAL_RED 13
#define TSAL_GREEN 12
#define BAT_SENSE A0
#define INV_SENSE A1
#define AIRP_SENSE 11
#define AIRN_SENSE 10
#define PRECH_SENSE 7
#define AIRP_CMD 9
#define AIRN_CMD 8
#define PRECH_CMD 7

#define VOLTAGE_THRESHOLD 102 // ADC reading 60V threshold

// Signals are active low
#define ON LOW
#define OFF HIGH

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(TSAL_RED, OUTPUT);
  pinMode(TSAL_GREEN, OUTPUT);
  pinMode(BAT_SENSE, INPUT);
  pinMode(INV_SENSE, INPUT);
}

// the loop function runs over and over again forever
void loop() {
  bool bat_over_threshold = analogRead(BAT_SENSE) > VOLTAGE_THRESHOLD;
  bool inv_over_threshold = analogRead(INV_SENSE) > VOLTAGE_THRESHOLD;
  bool airp = digitalRead(AIRP_SENSE);
  bool airn = digitalRead(AIRN_SENSE);
  bool prech = digitalRead(PRECH_SENSE);

  // Check for air implausibility (sense != cmd)
  bool air_implausibility = airp == digitalRead(AIRP_CMD) || \
                            airn == digitalRead(AIRN_CMD) || \
                            prech == digitalRead(PRECH_CMD);

  // check for implausibility and latch TSAL off
  if (bat_over_threshold != inv_over_threshold || air_implausibility)
  {
    digitalWrite(TSAL_RED, OFF);
    digitalWrite(TSAL_GREEN, OFF);
    while(1);
  }

  // if DC-link is >60V then blink red, else turn off red
  if(inv_over_threshold) {
    digitalWrite(TSAL_RED, digitalRead(TSAL_RED));   // blink TSAL
    digitalWrite(TSAL_GREEN, OFF);
  } else {
    digitalWrite(TSAL_RED, OFF);
  }
  // if DC-link is <=60v and all AIRs are open then turn on green, else green is off
  if (!inv_over_threshold && !airp && !airn && !prech) {
    digitalWrite(TSAL_GREEN, ON);
  } else {
    digitalWrite(TSAL_GREEN, OFF);
  }
  
  delay(200); // 2.5Hz
}
