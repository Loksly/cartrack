#include <Servo.h>

#include "Arduino.h"
#include "carlib.h"



Carlib::Carlib(){
	pinMode(IN1,OUTPUT);
	pinMode(IN2,OUTPUT);
	pinMode(IN3,OUTPUT);
	pinMode(IN4,OUTPUT);
	pinMode(ENA,OUTPUT);
	pinMode(ENB,OUTPUT);
	stop();
}

void Carlib::tick(unsigned long time){
  return;
  if (_lasttime + TICKTIME < time){
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
    _lasttime = time;
  }
}

int Carlib::readDistance(){
	digitalWrite(Trig, LOW);
	delayMicroseconds(2);
	digitalWrite(Trig, HIGH);
	delayMicroseconds(20);
	digitalWrite(Trig, LOW);
	float Fdistance = pulseIn(Echo, HIGH, TICKTIME / 2);

	Fdistance= Fdistance/58;

	return (int) Fdistance;
}

void Carlib::forward(){
  digitalWrite(ENA,HIGH);
  digitalWrite(ENB,HIGH);

  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
}

void Carlib::backward(){
  digitalWrite(ENA,HIGH);
  digitalWrite(ENB,HIGH);

  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
}

void Carlib::rotateclockwise(){
  analogWrite(ENA,ABS);
  analogWrite(ENB,ABS);

  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW); 
}

void Carlib::rotatecounterclockwise(){
  analogWrite(ENA,ABS);
  analogWrite(ENB,ABS);

  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
}

void Carlib::stop(){
  digitalWrite(ENA,LOW);
  digitalWrite(ENB,LOW);
}

int Carlib::servovalue(){
  return _servovalue;
}

