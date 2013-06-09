/*
Melting point apparatus for the OpenChemLab project by chaac

Visit openchemlab.org , check out the forums.

Donations welcome

Bitcoin: 18mG29fRKU4NdhPRfP98nMV4fXMJ73gbjn
Litecoin: Lb2ABK6Zy5dnza14FaKDTNQsvrXTMXoEgL
*/

#include <PID_v1.h>
#include <math.h>

float Temperature(int AnalogInputNumber)
{
  float R,T;
  
  R=1024.0f*4700.0f/float(analogRead(AnalogInputNumber))-4700.0f;
  T=1.0f/(0.003354016435f+0.0002516356316f*log(R/98400));
  T-=273.15f;
  
  return T;
}

double Setpoint, Input, Output;
int sett;
int i;
char buffer[100];

PID myPID(&Input, &Output, &Setpoint,10,.1,5, DIRECT);

void setup()
{
  Serial.begin(9600);
  Setpoint = 20;
  Input = Temperature(0);
  myPID.SetMode(AUTOMATIC);
}

void loop()
{
  i = 0;
  
  memset(buffer,0,sizeof(buffer));
      
  if (Serial.available()) {
    delay(10);
    while (Serial.available() && i < 99) {
      buffer[i++]=Serial.read();
    }
  }
  
  if (atoi((char*)buffer) > 0) {sett = atoi((char*)buffer);}
  if (Setpoint < sett) {Setpoint += .01;}
  if (Setpoint > sett) {Setpoint -= .01;}

  Input = Temperature(0);
  myPID.Compute();
  analogWrite(11,Output);
  
  Serial.print("MELTINGPOINT,");
  Serial.print(Input);
  Serial.print(",");
  Serial.print(sett);
  Serial.print(",");
  Serial.print(map(Output,0,255,0,100));
  Serial.print("\r\n"); 
}



