/*
  Temporisateur.cpp - Library for timing
  Created by Zied Miladi, January 2, 2022.
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <util/atomic.h>

#include "Arduino.h"
#include "Temporisateur.h"

// Initialize the static list of timers and count
Temporisateur* Temporisateur::timers[maxTimers];
int Temporisateur::timerCount = 0;

void temp_init(TimeUnit unit)
{
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0b00001100;
  TIMSK1 = 0b00000010;
  TCNT1 = 0;
  OCR1A = 62500/unit;
  interrupts();
}

Temporisateur::Temporisateur()
{
  ST = 10;
  CT = 0;
  Q = false;
  started = false;
  registerTimer();
}

void Temporisateur::Trigg()
{
  started = true;
}

void Temporisateur::Pause()
{
  started = false;
}

void Temporisateur::Reset()
{
  CT = 0;
  started = false;
  Q = false;
}

void Temporisateur::registerTimer()
{
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    if (timerCount < maxTimers) {
      timers[timerCount++] = this;
    }
  }
}

void Temporisateur::unregisterTimer()
{
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    for (int i = 0; i < timerCount; ++i) {
      if (timers[i] == this) {
        for (int j = i; j < timerCount - 1; ++j) {
          timers[j] = timers[j + 1];
        }
        --timerCount;
        break;
      }
    }
  }
}

void Temporisateur::processTimers()
{
  for (int i = 0; i < timerCount; ++i) {
    Temporisateur* timer = timers[i];
    if (timer->started) {
      if (timer->CT++ >= timer->ST-1) {
        timer->Q = true;
        timer->started =false;
        //timer->CT = 0; 
            // Reset time after overflow
      }
    }
  }
}

ISR(TIMER1_COMPA_vect)
{
  Temporisateur::processTimers();
}
