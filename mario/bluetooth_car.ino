#include <Servo.h>
#include <SPI.h>
#include "carlib.h"

#define LED 13

#define TIMEBETWEENSTATUS 200
#define TIMEBETWEENSERVO 20
#define TIMEBETWEENSERIAL 100

volatile int state = LOW;
unsigned long lasttime;
char getstr;
Carlib car;

boolean _servoascending;
int _servovalue = 90;
int _pin;
Servo _servo;

unsigned long statuslasttime = 0;
unsigned long servolasttime = 0;
unsigned long seriallasttime = 0;

void statustick(unsigned long time){
  if (statuslasttime + TIMEBETWEENSTATUS < time){
    Serial.print("{\"d\": ");
    Serial.print(car.readDistance());
    Serial.print(",\"s\": ");
    Serial.print(_servovalue);
    Serial.print(",\"c\": ");
    Serial.print(getstr);
    Serial.print(",\"l\": ");
    Serial.print(state);
    Serial.println("}");
    statuslasttime = time;
  }
}

void stateChange(){
  state = !state;
  digitalWrite(LED, state);
}

void setup(){ 
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  car.stop();

  _servo.attach(SERVOPIN);
  _servoascending = true;
  _servovalue;
  while (!Serial.available());
}

void servotick(unsigned long time){
   if (servolasttime + TIMEBETWEENSERVO < time){
    if (_servoascending){
      _servovalue += SERVOINC;
    } else {
      _servovalue -= SERVOINC;
    }
    if (_servovalue > MAXSERVO){
      _servovalue = MAXSERVO;
      _servoascending = false;
    } else if (_servovalue < MINSERVO){
      _servovalue = MINSERVO;
      _servoascending = true;
    }
    
    _servo.write(_servovalue);
    servolasttime = time;
  }
}

void serialtick(unsigned long time){
   if (seriallasttime + TIMEBETWEENSERIAL < time){
      if (Serial.available() > 0){
        getstr = Serial.read();
        if(getstr=='f'){
          car.forward();
        }else if(getstr=='b') {
          car.backward();
        }
        else if(getstr=='l')
        {
          car.rotatecounterclockwise();
        }
        else if(getstr=='r')
        {
          car.rotateclockwise();
        }
        else if(getstr=='s')
        {
           car.stop();     
        }
        else if(getstr=='A')
        {
          stateChange();
        }
      }
      seriallasttime = time;
   }
}


void loop(){
    unsigned long time = millis();
    statustick(time);
    servotick(time);
    serialtick(time);
}

