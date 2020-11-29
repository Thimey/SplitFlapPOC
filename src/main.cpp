#include <Arduino.h>
#include "config.h"
#include "SplitFlap/SplitFlap.h"

const int WORD_COUNT = 5;

const int SR_LATCH_PIN = 10;
const int SR_CLOCK_PIN = 11;
const int SR_DATA_PIN = 12;

const int DIR_PIN = 2;

SplitFlap splitflap1("splitFlap1");
SplitFlap splitflap2("splitFlap2");

SplitFlap splitFlapArray[NUMBER_OF_SPLIT_FLAPS] = {
  splitflap1,
  splitflap2
};

uint8_t flapDisplayCharacters[WORD_COUNT][NUMBER_OF_SPLIT_FLAPS] = {
  {'h', 'i'},
  {'a', '9'},
  {'3', 'f'},
  {'h', 'i'},
  {'o', 's'},
};


void setup() {
  Serial.begin(9600);

  pinMode(SR_LATCH_PIN, OUTPUT);
  pinMode(SR_CLOCK_PIN, OUTPUT);
  pinMode(SR_DATA_PIN, OUTPUT);

  pinMode(DIR_PIN, OUTPUT);

  digitalWrite(DIR_PIN, LOW);
}

uint8_t toShiftInput(bool shouldStepValues[NUMBER_OF_SPLIT_FLAPS])
{
  uint8_t shiftInput = 0;
  for (int i = 0; i < NUMBER_OF_SPLIT_FLAPS; ++i) {
    if (shouldStepValues[i]) {
      shiftInput |= 1 << (MAX_SPLIT_FLAPS - 1 - i);
    }
  }

  return shiftInput;
}

void setSplitFlapArrayTargets(SplitFlap splitFlapArray[], uint8_t displayCharacters[])
{
  // Set flap targets for each character in word
  for (int i = 0; i < NUMBER_OF_SPLIT_FLAPS; ++i) {
    uint8_t flapCharacter = displayCharacters[i];
    splitFlapArray[i].setFlapTarget(flapCharacter);
  }
}

bool hasSplitFlapArrayReachedTarget(SplitFlap splitFlapArray[])
{

  for (int i = 0; i < NUMBER_OF_SPLIT_FLAPS; ++i) {
    // Serial.print("Target for ");
    // Serial.print(splitFlapArray[i].name);
    // Serial.print(": ");
    // Serial.println(splitFlapArray[i].stepsToTarget);
    if (!splitFlapArray[i].isAtFlapTarget()) {
      return false;
    }
  }

  return true;
}

void shiftOutSteps(uint8_t shiftInput)
{
  // Prevent data from being released
  digitalWrite(SR_LATCH_PIN, LOW);
  // Write the step data to register
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, shiftInput);
  // Release the data
  digitalWrite(SR_LATCH_PIN, HIGH);
}

void stepSplitFlapArray(SplitFlap splitFlapArray[])
{
  bool splitFlapsToStep[NUMBER_OF_SPLIT_FLAPS];

  // Update the internal state of each split flap
  for (int i = 0; i < NUMBER_OF_SPLIT_FLAPS; ++i)
  {
    bool isFlapAtTarget = splitFlapArray[i].step();

    splitFlapsToStep[i] = !isFlapAtTarget;
  }

  const uint8_t shiftInput = toShiftInput(splitFlapsToStep);

  shiftOutSteps(shiftInput);
  delayMicroseconds(PULSE_DELAY);
  shiftOutSteps(0);
  delayMicroseconds(PULSE_DELAY);
}

void loop() {
  // for (int step = 0; step < STEPS_PER_REVOLUTION; ++step) {
  //   shiftOutSteps(B11111111);
  //   delayMicroseconds(PULSE_DELAY);
  //   shiftOutSteps(0);
  //   delayMicroseconds(PULSE_DELAY);
  // }

  delay(5000);

  for (int i = 0; i <= WORD_COUNT; ++i) {
    // Set flap targets
    setSplitFlapArrayTargets(splitFlapArray, flapDisplayCharacters[i]);

    while (!hasSplitFlapArrayReachedTarget(splitFlapArray)) {
      stepSplitFlapArray(splitFlapArray);
    }

    Serial.println("FINISHED WORD");

    delay(5000);
  }
}