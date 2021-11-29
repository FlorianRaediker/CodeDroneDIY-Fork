#ifndef UNIT_TEST
#include <avr/wdt.h>
#include <math.h>

#define BUFFER_LENGTH 32

#include "customLibs/CustomTime.h"
#include "stabilization/hardware/RadioReception.h"
#include "stabilization/Stabilization.h"
#include "stateMachine/StateMachine.h"

CustomTime time;
Stabilization stabilization;
StateMachine stateMachine;
void PrintConfig();

// Initialiaze all sensors and communication pipes
void setup() {
    //CustomSerialPrint::begin(230400); // Console print: initialize serial communication
    CustomSerialPrint::begin(9600);

    stabilization.Init();
    time.Init();
    stateMachine.Init();

    //wdt_enable(WDTO_1S); // Set watchdog reset
    PrintConfig();
}

uint16_t loopNb = 0;
float meanLoopTime = 0.0;

// Main loop
void loop() {
    float loopTimeSec = time.GetloopTimeMilliseconds();

    // State Machine Initializing -> Ready -> AngleMode/AccroMode -> Safety -> Disarmed -> AngleMode/AccroMode
    stateMachine.Run(loopTimeSec);

    // Compute mean loop time and complementary filter time constant
    int flyingMode = stabilization.GetFlyingMode();
    if ((flyingMode == angleMode) || (flyingMode == accroMode)) {
        if (!stabilization.IsThrottleIdle()) {
            time.ComputeMeanLoopTime(loopTimeSec, meanLoopTime, loopNb);
        }
    }
    //wdt_reset();
}

void PrintConfig() {
    if ((stabilization.GetMotorsMaxPower() == 1860)
        && (stabilization.GetMotorsMaxThrottle() >= (1860 * 0.8)))
        CustomSerialPrint::println(
                F("!!!!!!!!!!!!!!!!!!!!FLYING MODE "
                  "POWER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! "));
    else if ((stabilization.GetMotorsMaxPower() <= 1300))
        CustomSerialPrint::println(F("DEBUG MODE POWER!!! "));
    else
        CustomSerialPrint::println(F("UNEXPECTED POWER "));

    CustomSerialPrint::print(F("MAX_POWER: "));
    CustomSerialPrint::print(stabilization.GetMotorsMaxPower());
    CustomSerialPrint::print(F(" MAX_THROTTLE_PERCENT: "));
    CustomSerialPrint::println(stabilization.GetMotorsMaxThrottlePercent());
    CustomSerialPrint::println(F("Setup Finished"));
}

/*
// Initialize static members
unsigned long cPPM[6] = {
        0, 0, 0, 0, 0, 0,
};
//bool RadioReception::initialized = false;
int nbSpacingEncountered = 0;
int channel = 0;
//unsigned long RadioReception::PWM_Start = 0;
//unsigned long RadioReception::PWM_Stop = 0;
//unsigned long RadioReception::PWM_Width = 0;
unsigned long PWM_Starts[6] = {
        0, 0, 0, 0, 0, 0,
};

void PrintCmd(void) {
    CustomSerialPrint::print(F("Aile: "));
    CustomSerialPrint::print(cPPM[0]);
    CustomSerialPrint::print(F(" Elev: "));
    CustomSerialPrint::print(cPPM[1]);
    CustomSerialPrint::print(F(" Throt: "));
    CustomSerialPrint::print(cPPM[2]);
    CustomSerialPrint::print(F(" Rudd: "));
    CustomSerialPrint::print(cPPM[3]);
    CustomSerialPrint::print(F(" Switch1: "));
    CustomSerialPrint::print(cPPM[4]);
    CustomSerialPrint::print(F(" Switch2: "));
    CustomSerialPrint::println(cPPM[5]);
}

bool isInitialized() {
    //for (int i=0; i<CHANNELS_NB; i++) {
    PrintCmd();
    for (int i=0; i<4; i++) {
        if (cPPM[i] == 0) return false;
    }
    return true;
}

void GetWidth(int index) {
    unsigned long stop = micros();
    unsigned long width = stop - PWM_Starts[index];
    cPPM[index] = width;

    PWM_Starts[index] = stop;
}

bool Init() {
    pinMode(2, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
    pinMode(18, INPUT_PULLUP);
    pinMode(19, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(2), [](){ GetWidth(0); }, CHANGE);   // Aile:  D2,  PE4
    attachInterrupt(digitalPinToInterrupt(3), [](){ GetWidth(1); }, CHANGE);   // Elev:  D3,  PE5
    attachInterrupt(digitalPinToInterrupt(18), [](){ GetWidth(2); }, CHANGE);  // Throt: D18, PD3
    attachInterrupt(digitalPinToInterrupt(19), [](){ GetWidth(3); }, CHANGE);  // Rudd:  D19, PD2

    CustomSerialPrint::print(F("RadioReception - ...: "));
    CustomSerialPrint::println(isInitialized());
    while (!isInitialized()) {
        //CustomSerialPrint::print(F("RadioReception not ready, try again, please wait. "));
        delay(20);
        / *if (timeout.IsTimeout(2000)) {
            CustomSerialPrint::println(F("RadioReception - Timeout during initialization!!"));
            return false;
        }* /
    }
    CustomSerialPrint::println(F("RadioReception - Initialized!"));
    return true;
}

void setup() {
    CustomSerialPrint::begin(9600);
    Init();
}

void loop() {
    CustomSerialPrint::print(F("loop - "));
    PrintCmd();
}*/

#endif