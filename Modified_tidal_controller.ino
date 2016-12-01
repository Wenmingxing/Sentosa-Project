 /* arduino UNO to read I,V and RPM of the tidal turbine generator
   
   I module:
   ACS712-20A hall effect sensor
   Configuration: Generator >> T1,T2 >> Generator 
   VCC <-> 5V
   GND <-> GND
   OUTPUT <-> A0
   OutputSensorVoltage = analogRead(sensorPin)*5.0/1023.0
   Current = (OutputSensorVolatge - 2.5)/0.100
   
   V module:
   Voltege divider by resistors
   Configuration: Generator >> R1*5 >> amplifier buffer>> R2 >> GND(confusion still exits: whether the GND is the same with the generator's GND)
   R1 = 200kohm,R2 = 20kohm 
   
   RPM module:
   WDG 58B encoder
   Co0nfiguration: 12vpp >> 20kohm >> pin 9 >>10kohm >>GND
   RPM Apin use interrupt:*/
   
#include <Wire.h>
#include <math.h>

#define sf 1000 // The sampling frequency
#define PPR 100 //pulses per revolution for encoder
#define IsensePin A0
#define VsensePin A1
#define AsensePin 2  // interrupt pin for A
#define BsensePin 4
#define D_Rpin  6 

volatile int encoderPos = 0; // tick counter

int R1 = 100;
int R2 = 20;
int R3 = 10;
   
void setup() {
  pinMode(AsensePin,INPUT);
  pinMode(BsensePin,INPUT);
  pinMode(D_Rpin,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(AsensePin),Aencoder,RISING);
  Serial.begin(9600);
  while(!Serial){;}
  Serial.println("Begin");
}

unsigned long previoustimer = 0;
double V;
double I;
double W;
double Pp = 0;
double Pc;
double D_Rp = 0;
double D_Rc = 0.5;
double D_Rn;


void loop() {


if (millis() - previoustimer >= sf){
  
  V = voltage_divider(analogRead(VsensePin),R1,R2);//Read the voltage
  
  I = current_converter(analogRead(IsensePin));// Read the current
  
  Pc = V * I;// The current power output from the generator.
  
  D_Rn = sign(D_Rc-D_Rp)*sign(Pc-Pp)*0.005 + D_Rc;// MPPT cntrol to determinate the next step duty ratio for the converter.
  
  D_Rn = constrain(D_Rn,0.0,1.0);
  
  Pp = Pc;
  
  D_Rp = D_Rc;
  
  D_Rc = D_Rn;
  
  int value = int(D_Rc*255.0);//Convert the duty ratio to the PWM 
  
  analogWrite(D_Rpin, value);// Output the PWM signal to control the converter.
    
  W = tick_to_rpm(encoderPos);// convert the tick number to the RPM
  
  //Serial.print(V);Serial.print(", ");
  //Serial.print(I);Serial.print(", ");
  //Serial.print(D_Rc);Serial.println("");
  Serial.print(W);Serial.println("");
  Serial.print(encoderPos);Serial.println("");
  encoderPos = 0;
  previoustimer = millis();
  
 }

}

void Aencoder(){
     if (digitalRead(AsensePin) == digitalRead(BsensePin))
      {encoderPos++;}
    else
      {encoderPos--;}
    }
    
    
double voltage_divider(int ADCin, int R1,int R2){
   return  (ADCin*5.0/1023.0)*(1+double(7*R1)/R2);
   

   }

double current_converter(int ADCin){

    return (ADCin*5.0/1023.0-2.5)/0.100;
}

double tick_to_rpm(int tick){
  return double(tick)/PPR*60.0;

}

double sign(double n){

   if (n>0){
     return 1.0;
   }
   else if (n<0)
   {
     return -1.0;
   }
   else {return 0.0;}
}

