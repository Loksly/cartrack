/*
 *  Moisés Frutos Plaza
 *  Mario Losantos Albacete
 */

#include <Servo.h>


#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define DEFAULT_TIME 31539600 //1/1/1971 0:00:00 CET (Central European Time) en segundos

char getstr;
long tiempo_inicial;
long tiempo;
Servo servo;

#define SERVOPIN 3
#define TICKTIME 100
#define LED 13

volatile int luz = LOW;
volatile int servovalue = 90;
//volatile boolean connected_ = false;

int in1=6;
int in2=7;
int in3=8;
int in4=9;
int ENA=5;
int ENB=11;
int ABS=150;

int Echo = A4;
int Trig = A5;

void enviarRespuesta(){
    Serial.print("{\"c\": ");
    Serial.print(getstr);
    Serial.print(",\"d\": ");
    Serial.print(getDistancia());
    Serial.print(",\"s\": ");
    Serial.print(servovalue);
    Serial.print(",\"l\": ");
    Serial.print(luz);
    Serial.print(",\"t\": ");
    Serial.print(tiempo);
    Serial.println("}");
}

int getDistancia(){
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(20);
  digitalWrite(Trig, LOW);
  float Fdistance = pulseIn(Echo, HIGH, TICKTIME / 2);
  
  Fdistance = Fdistance/58;
  return (int) Fdistance;
}

void delante()
{ 
  digitalWrite(ENA,HIGH);
  digitalWrite(ENB,HIGH);
  digitalWrite(in1,HIGH);//digital output
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
  enviarRespuesta();
}
void atras()
{
  digitalWrite(ENA,HIGH);
  digitalWrite(ENB,HIGH);
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
  enviarRespuesta();
}
void izquierda()
{
  analogWrite(ENA,ABS);
  analogWrite(ENB,ABS);
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
  enviarRespuesta();
}
void derecha()
{
  analogWrite(ENA,ABS);
  analogWrite(ENB,ABS);
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
  enviarRespuesta();
}
void parar()
{
  digitalWrite(ENA,LOW);
  digitalWrite(ENB,LOW);
  enviarRespuesta();
}

void interruptorLuz()
{
  luz = !luz;
  digitalWrite(LED, luz);  
  enviarRespuesta();
}

void setup()
{ 
  bool connected = false;
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);

  servo.attach(SERVOPIN);

  servo.write(servovalue);

  while(Serial.available() == 0) {};
  //Serial.print("El tiempo en segundos es ");
  tiempo_inicial = sincronizarTiempoInicial();
  //Serial.println(tiempo);
  Serial.flush(); //Para esperar a que termine de sincronizar el tiempo inicial
  luz = false;
}

void cambiarAnguloServo(int incv){
  servovalue += incv;
  if (servovalue  > 180){
    servovalue = 180;
  } else if (servovalue < 5){
    servovalue = 5;
  }
  servo.write(servovalue);
}

long sincronizarTiempoInicial() {
  long t;
  if(Serial.find(TIME_HEADER)) {
     t = Serial.parseInt(); //Según la documentación oficial de Arduino retorna un long 2^32 con signo por lo que no podemos pasar milisegundos sino segundos
  } 
  return t;
}

long sincronizarTiempo() {
  long t;
  t = tiempo_inicial + (millis()/1000);
  return t;
}

void loop(){
    while(Serial.available() > 0)
        Serial.read();
    while(Serial.available() == 0){};
     getstr = Serial.read();
     switch(getstr) {
        case 'A': interruptorLuz(); break;
        case 'f': delante(); break;
        case 'b': atras(); break;
        case 'l': izquierda(); break;
        case 'r': derecha(); break;
        case 's': parar(); break;
        case 'z': cambiarAnguloServo(-5); break;
        case 'x': cambiarAnguloServo(5); break;
     }
     delay(200);
     tiempo = sincronizarTiempo();
}

