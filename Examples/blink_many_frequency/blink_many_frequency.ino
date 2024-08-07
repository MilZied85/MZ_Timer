#include <Temporisateur.h>

Temporisateur timer1;
Temporisateur timer2;

void setup() {
  temp_init(_10Ms);  // set the resolution of timers 
  pinMode(13, OUTPUT);
  Serial.begin(9600);

  timer1.ST = 100; //set duration (setted time)
  timer2.ST = 500;

  timer1.Trigg();  //trigger the timers
  timer2.Trigg();

 
}

void loop() {
  // Main loop code
  // Check the state of timers

  if (timer2.Q && timer1.ST>10) {
    timer1.ST -= 10;
    timer2.Reset();
    timer2.Trigg();
  }

  if (timer1.Q) {
    bool a = digitalRead(13);
    digitalWrite(13, !a);
    timer1.Reset();
    timer1.Trigg();
  }
  Serial.println(timer2.CT);
}
