/*
  Carlib.h - Library for car controlling.
  Created by Loksly 2017 using some code that belongs to others.
  Released into the public domain.
*/
#ifndef Carlib_h
#define Carlib_h

#include "Arduino.h"
#include <Servo.h>


#define IN1 6
#define IN2 7
#define IN3 8
#define IN4 9
#define ENA 5
#define ENB 11
#define ABS 150

#define Echo A4
#define Trig A5

#define SERVOPIN 3
#define SERVOINC 2
#define MAXSERVO 180
#define MINSERVO 10

#define TICKTIME 100

class Carlib{

  public:
    Carlib();
    void tick(unsigned long);
    void forward();
    void backward();
    void stop();
    void rotateclockwise();
    void rotatecounterclockwise();
    int readDistance();
    int servovalue();
  private:
    boolean _servoascending;
	  int _servovalue;
    Servo _servo;
    unsigned long _lasttime;
};

#endif
