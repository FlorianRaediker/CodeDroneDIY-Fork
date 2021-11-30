#ifndef CONTROLLOOPCONSTANTS_H_
#define CONTROLLOOPCONSTANTS_H_

#include "../Singleton.h"

typedef struct {
    float G;
    float Kp;
    float Kd;
    float Ki;
} Constants;

class ControlLoopConstants : public SingletonSimple<ControlLoopConstants> {
  public:
    // Angle mode
    // These parameters are very important for flight success
    // They must be tuned for each frame type, motors, and propeller used
    Constants anglePos = {.G = 0.010, .Kp = 268, .Kd = 0.5, .Ki = 0.0};  // 
    Constants angleSpeed = {.G = 0.010, .Kp = 192, .Kd = 0.0, .Ki = 0.0};

    // Accro mode
    // 450mm frame, 10x4.5" 2 blades propellers - Tested during flight test: OK
    Constants accroSpeed = {.G = 0.010, .Kp = 192, .Kd = 0.0, .Ki = 0.0};  // ÄNDERN für AcccroMode; wird in ControlLoop.cpp verwendet
                                                                           // .G: wird anscheinend letztendlich mit mixing (aus Stabilization.h) multipliziert, es reicht also einen dieser Parameter zu ändern
                                                                           // .Kp: Koeffizient für Proportional (je höher, desto mehr Korrektor), .Kd: Koeffizient für Ableitung ("derivative"), .Ki: Koeffizient für Integral
                                                                           // Da hier .Kd=0 und .Ki=0 ist die Gleichung letztendlich korrektur=mixing*.G*.Kp*error, also muss nur einer der Koeffizienten geändert werden

    // Yaw PID (common to both modes, yaw is always speed controlled)
    Constants yawSpeed = {.G = 0.010, .Kp = 150, .Kd = 0.0, .Ki = 0.0};  // ÄNDERN für AccroMode oder AngleMode
};
#endif // CONTROLLOOPCONSTANTS_H_
