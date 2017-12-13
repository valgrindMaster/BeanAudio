#include <Wire.h>
#include <Digital_Light_TSL2561.h>
#include "pitches.h"
#include "melody.h"

/* CONSTANTS */

//Pin Constants: INPUTS
#define MAGNETIC_SWITCH 0  //D2/D3
#define BUTTON 1           //D1/D2
/* Digital Light in Lib */

//Pin Constants: OUTPUTS
#define VIBRATION_MOTOR A1 //A1/A2
#define BUZZER A3          //A3/A4

#define ADC_REF 5 //5V voltage
#define GROVE_VCC 5
#define FULL_ANGLE 300

#define TEMPO

double* beats = (double*)malloc(sizeof(BEATS));
int* melody = (int*)malloc(sizeof(MELODY));
double velocity;
double songLength;

//Hooks up all inputs and outputs
void setup() {
  /* Digital Light in Lib */
  pinMode(BUTTON, INPUT);
  pinMode(MAGNETIC_SWITCH, INPUT);
  pinMode(VIBRATION_MOTOR, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  Wire.begin();
  TSL2561.init();

  beats = BEATS;
  melody = MELODY;
  velocity = VELOCITY;
  songLength = LENGTH;
}

void loop() {

  //One play-through of the song.
  for (int thisNote = 0; thisNote < songLength; thisNote++) {

    //Start song over
    if (digitalRead(BUTTON) == HIGH) {
      break;
    }

    //Tempo fluctuated based on proximity to light.
    double tempo_flux = calcTempoAdjustment();

    //Use a magnet to add "bass" to the music!
    if (digitalRead(MAGNETIC_SWITCH) == HIGH) {
      digitalWrite(VIBRATION_MOTOR, HIGH);
    }
    
    double noteDuration = (1000 / *(beats + thisNote)) * velocity * tempo_flux;
    tone(BUZZER, *(melody + thisNote), noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(BUZZER);
    digitalWrite(VIBRATION_MOTOR, LOW);
  }

  delay(1000);
}

//Gathers input from the light sensor and returns a value by which the tempo can be modified. 
double calcTempoAdjustment() {
  long lux = TSL2561.readVisibleLux();
  if (lux < 100) {
    return 1;
  } else if (lux < 150) {
    return 0.9;
  } else if (lux < 200) {
    return 0.8;
  } else if (lux < 250) {
    return 0.7;
  } else if (lux < 300) {
    return 0.6;
  } else {
    return 0.5;
  }
}

