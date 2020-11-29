#include <Arduino.h>
#include "../config.h"
#include "SplitFlap.h"

const int DEFAULT_FLAP_INDEX = 0;
const int NULL_FLAP_TARGET_INDEX = -1;
const int NULL_STEPS_TO_TARGET = -1;

SplitFlap::SplitFlap(String name) :
    name(name),
    currentFlapIndex(0),
    flapTargetIndex(NULL_FLAP_TARGET_INDEX),
    stepsToTarget(NULL_STEPS_TO_TARGET)
{
    // pinMode(stepPin, OUTPUT);
}

int SplitFlap::getFlapIndex(uint8_t flapCharacter)
{
    for (int i = 0; i++, i < NUMBER_OF_FLAPS;) {
        if (flaps[i] == flapCharacter) {
            return i;
        }
    }

    return 0;
}

int SplitFlap::getFlapsToRotate(int desiredIndex)
{
    int indexDiff = desiredIndex - SplitFlap::currentFlapIndex;

    if (indexDiff < 0) {
        return NUMBER_OF_FLAPS + indexDiff;
    }

    return indexDiff;
}

int SplitFlap::getStepsToRotate(int flapsToRotate)
{
    return flapsToRotate * (STEPS_PER_REVOLUTION / NUMBER_OF_FLAPS);
}


// Public

boolean SplitFlap::isAtFlapTarget()
{
    return SplitFlap::stepsToTarget < 1;
}

void SplitFlap::setFlapTarget(uint8_t flapCharacter)
{
    const int flapTargetIndex = SplitFlap::getFlapIndex(flapCharacter);
    const int flapsToTarget = SplitFlap::getFlapsToRotate(flapTargetIndex);

    SplitFlap::flapTargetIndex = flapTargetIndex;
    SplitFlap::stepsToTarget = SplitFlap::getStepsToRotate(flapsToTarget);
}

// Increment the virtual step of split flap. Returns true if step incremented, otherwise false.
bool SplitFlap::step()
{
    // Check if target index already reached, if so do nothing
    if (SplitFlap::isAtFlapTarget()) {
        return true;
    }

    // Rotate one step
    // digitalWrite(SplitFlap::stepPin, HIGH);
    // delayMicroseconds(PULSE_DELAY);
    // digitalWrite(SplitFlap::stepPin, LOW);
    // delayMicroseconds(PULSE_DELAY);

    // Decrement the steps to target
    SplitFlap::stepsToTarget = SplitFlap::stepsToTarget - 1;

    // If reached target flap, then set new current index and clear target
    if (SplitFlap::isAtFlapTarget()) {
        SplitFlap::currentFlapIndex = SplitFlap::flapTargetIndex;
        SplitFlap::flapTargetIndex = NULL_FLAP_TARGET_INDEX;
    }

    return false;
}


