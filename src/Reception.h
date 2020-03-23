#ifndef RECEPTION_H_
#define RECEPTION_H_

#define FLYING_MODE_ANGLE 0
#define FLYING_MODE_ACCRO 1

#define CHANNELS_NB 7

#include "Arduino.h"

enum Mode { initialization, starting, safety, disarmed, accro, angle };

class Reception {
  public:
    // Reception setup
    const float MAX_ANGLE = 45;      // (°) Max roll and pitch angles reachable in angle mode
    const float MAX_ROT_SPEED = 135; // (°/s) Max roll and pitch speed in accro mode
    const float MAX_YAW_SPEED = 135; // (°/s) Max yaw speed in accro and angle modes

    // Channel 1: Ailerons 1.09 to 1.90 ms
    // Channel 2: Prof 1.09 to 1.90 ms
    // Channel 3: Throttle 1.09 to 1.90 ms
    // Channel 4: Rudder 1.09 to 1.90 ms

  private:
    // PWM computation
    bool initialized = false;
    int channel = 0;
    float PWM_Start = 0;
    float PWM_Stop = 0;
    float PWM_Width = 0;
    int cPPM[CHANNELS_NB] = {0, 0, 0, 0, 0, 0, 0}; // 6 channels plus separation

  public:
    void PrintCmd(void) {
        Serial.print(F("Aile: "));
        Serial.print(cPPM[0]);
        Serial.print(F(" Elev: "));
        Serial.print(cPPM[1]);
        Serial.print(F(" Throt: "));
        Serial.print(cPPM[2]);
        Serial.print(F(" Rudd: "));
        Serial.println(cPPM[3]);
        Serial.print(F("Switch1: "));
        Serial.print(cPPM[4]);
        Serial.print(F(" Switch2: "));
        Serial.println(cPPM[5]);
    }

    bool IsReady() {
        return initialized;
    }

    // Angle Mode:
    inline int GetAileronsAngle() {
        return -(map(cPPM[0], 1080, 1900, -MAX_ANGLE, MAX_ANGLE));
    }
    inline int GetElevatorAngle() {
        return (map(cPPM[1], 1080, 1900, -MAX_ANGLE, MAX_ANGLE));
    }

    // Accro mode:
    inline int GetAileronsSpeed() {
        return -(map(cPPM[0], 1080, 1900, -MAX_ROT_SPEED, MAX_ROT_SPEED));
    }
    inline int GetElevatorSpeed() {
        return (map(cPPM[1], 1080, 1900, -MAX_ROT_SPEED, MAX_ROT_SPEED));
    }
    inline int GetThrottle(const int _minPower, const int _maxThrottle) {
        return map(cPPM[2], 1080, 1900, _minPower, _maxThrottle);
    }

    inline int GetRudder() {
        return map(cPPM[3], 1080, 1900, -MAX_YAW_SPEED, MAX_YAW_SPEED);
    }
    inline int GetSwitchH() {
        if (cPPM[5] > 1500) {
            return true;
        } else {
            return false;
        }
    } // 1900 inter H en bas, 1090 inter H en haut

    inline int GetFlyingMode() {
        if (cPPM[4] > 1800)
            return disarmed;
        else if (cPPM[4] < 1200)
            return angle;
        else
            return accro;
    } // G switch: pos0=1900, pos1=1500, pos2=1092

    inline void GetWidth(void) {
        PWM_Stop = micros();
        PWM_Width = PWM_Stop - PWM_Start;
        PWM_Start = PWM_Stop;

        if (initialized) {
            if (channel < CHANNELS_NB)
                cPPM[channel] = PWM_Width;
        }

        if (PWM_Width > 4000) { // If delay more than 4ms, it is a new sequence
            channel = 0;
            initialized = true;
        } else if ((channel + 1) < CHANNELS_NB) {
            channel++;
        }
    }
};

#endif // RECEPTION_H_
