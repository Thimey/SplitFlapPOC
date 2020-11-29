#pragma once

class SplitFlap {
    private:

        int currentFlapIndex;
        int flapTargetIndex;


        int getFlapIndex(uint8_t flapCharacter);
        int getFlapsToRotate(int desiredIndex);
        int getStepsToRotate(int flapsToRotate);
        void incrementFlapIndex();

    public:
        String name;
        int stepsToTarget;
        SplitFlap(String name);

        boolean isAtFlapTarget();

        void setFlapTarget(uint8_t flapCharacter);
        bool step();
};
