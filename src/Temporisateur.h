/*
  Temporisateur.h - Library for timing.
  Created by Zied Miladi.
*/

#ifndef Temporisateur_h
#define Temporisateur_h

#include "Arduino.h"

enum TimeUnit {
  S = 1,
  _10Ms = 100
};

void temp_init(TimeUnit unit=_10Ms);

class Temporisateur
{
  public:
    Temporisateur();  // initialisation du temporisateur
    void Trigg();
    void Pause();
    void Reset();
    
    bool Q; // true aprés le débordement
    int CT;
    int ST;
    bool started;
    bool blinking;
    
    // Register the object with the ISR
    void registerTimer();
    
    // Unregister the object
    void unregisterTimer();

    // Static method to process all timers in the ISR
    static void processTimers();
    
  private:
    // Static list of all timers
    static Temporisateur* timers[];
    static int timerCount;
    static const int maxTimers = 10; // Adjust the size as needed
};

#endif // Temporisateur_h
