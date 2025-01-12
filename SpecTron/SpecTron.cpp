#include "SpecTron.h"

// Conversion macro
#define DEG2RAD(deg) ((deg) * PI / 180.0)  

void SpecTron::init(int YL, int YR, int RL, int RR) {
    servo_pins[0] = YL;
    servo_pins[1] = YR;
    servo_pins[2] = RL;
    servo_pins[3] = RR;

    attachServos();
    isSpecTronResting = false;
    for (int i = 0; i < 4; i++) servo_position[i] = 90;
}

void SpecTron::attachServos() {
    for (int i = 0; i < 4; i++) {
        servo[i].attach(servo_pins[i]);
        servo[i].write(90);  // Initialize servos to the 90-degree position
    }
}

void SpecTron::detachServos() {
    for (int i = 0; i < 4; i++) {
        servo[i].detach();
    }
}

void SpecTron::home() {
    if (!isSpecTronResting) {
        int homes[4] = {90, 90, 90, 90}; // All servos at rest position
        _moveServos(500, homes);         // Move the servos in half a second

        detachServos();
        isSpecTronResting = true;
    }
}

bool SpecTron::getRestState() {
    return isSpecTronResting;
}

void SpecTron::setRestState(bool state) {
    isSpecTronResting = state;
}

void SpecTron::walk(float steps, int T, int dir) {
    //-- Oscillator parameters for walking
    int A[4] = {30, 30, 20, 20};
    int O[4] = {90, 90, 90, 90};  // Set offsets to 90 degrees
    double phase_diff[4] = {0, 0, DEG2RAD(dir * -90), DEG2RAD(dir * -90)};

    //-- Execute the walking motion
    _execute(A, O, T, phase_diff, steps);
}

void SpecTron::moonwalk(float steps, int T) {
    //-- Oscillator parameters for moonwalking
    int A[4] = {20, 20, 10, 10};
    int O[4] = {90, 90, 85, 95};  // Set offsets around 90 degrees for moonwalk
    double phase_diff[4] = {0, 0, DEG2RAD(180), DEG2RAD(180)};

    //-- Execute the moonwalking motion
    _execute(A, O, T, phase_diff, steps);
}

void SpecTron::_moveServos(int time, int servo_target[]) {
    attachServos();
    if (getRestState() == true) {
        setRestState(false);
    }

    if (time > 10) {
        for (int i = 0; i < 4; i++)
            increment[i] = ((servo_target[i]) - servo_position[i]) / (time / 10.0);
        final_time = millis() + time;

        for (int iteration = 1; millis() < final_time; iteration++) {
            partial_time = millis() + 10;
            for (int i = 0; i < 4; i++) servo[i].write(servo_position[i] + (iteration * increment[i]));
            while (millis() < partial_time);
        }
    } else {
        for (int i = 0; i < 4; i++) servo[i].write(servo_target[i]);
    }
    for (int i = 0; i < 4; i++) servo_position[i] = servo_target[i];
}

void SpecTron::_execute(int A[4], int O[4], int T, double phase_diff[4], float steps) {
    attachServos();
    if (getRestState() == true) {
        setRestState(false);
    }

    int cycles = (int)steps;

    //-- Execute complete cycles
    if (cycles >= 1)
        for (int i = 0; i < cycles; i++) oscillateServos(A, O, T, phase_diff);

    //-- Execute the final not complete cycle
    oscillateServos(A, O, T, phase_diff, (float)steps - cycles);
}

void SpecTron::oscillateServos(int A[4], int O[4], int T, double phase_diff[4], float cycle_fraction) {
    unsigned long start_time = millis();
    while ((millis() - start_time) < (T * cycle_fraction)) {
        for (int i = 0; i < 4; i++) {
            float angle = A[i] * sin(2 * PI * (millis() - start_time) / T + phase_diff[i]) + O[i];
            servo[i].write(angle);
        }
    }
}
