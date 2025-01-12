//----------------------------------------------------------------
//-- SpecTron version 1.0
//-- Spectrum OUTR-B
//-- By Om Prakash Barik
//----------------------------------------------------------------

#ifndef SPECTRON_H
#define SPECTRON_H

#include <Arduino.h>
#include <ESP32Servo.h>

class SpecTron {
public:
    void init(int YL, int YR, int RL, int RR);
    void attachServos();
    void detachServos();
    void home();
    bool getRestState();
    void setRestState(bool state);
    void walk(float steps, int T, int dir);
    void moonwalk(float steps, int T);

private:
    Servo servo[4];
    int servo_pins[4];
    int servo_position[4];
    bool isSpecTronResting;
    float increment[4];
    unsigned long final_time;
    unsigned long partial_time;

    void _moveServos(int time, int servo_target[]);
    void _execute(int A[4], int O[4], int T, double phase_diff[4], float steps);
    void oscillateServos(int A[4], int O[4], int T, double phase_diff[4], float cycle_fraction = 1.0);
};

#endif // SPECTRON_H
