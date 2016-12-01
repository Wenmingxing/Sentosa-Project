/* Battery switch with a relay
   Coded by Luke wen
   05 Oct 2016
   
   Goal:
   The coding is aiming to switch the power supply for the boat system
   if there is no enough power left in the first choiced battery
   
   Configuration:
   Battery(+) -> diode -> voltage divider(R1,R2) -> relay(switch) -> boat ->battery(-)
   
   R1 -> A0 -> R2 -> GND*/
   
const int R1 = 200;
const int R2 = 100;
const int outpin = 5; // Power to the NPN transistor wokring as a switch for the Relay
const int pin = 13;
int state1;
int state2;
double voltage_of_battery = 11.0;

void setup() {
  Serial.begin(9600);
  if(!Serial){;}
  pinMode(outpin,OUTPUT);
  pinMode(pin,OUTPUT);
  digitalWrite(outpin,LOW);//Initialize the state of the battery system,specifically only battery one works
  digitalWrite(pin,LOW);
}

void loop() {
  digitalWrite(pin,LOW);
  //delay(5000);
  state1 = battery_switch( analogRead(A0),R1,R2 );
  state2 = battery_switch( analogRead(A1),R1,R2 );
  if (state1 == 1){
  digitalWrite(outpin,LOW);
  Serial.println("First Battery");
  }
  else{
  if (state2 == 0){
  Serial.println(" Warning:both batteries are low power!");
  digitalWrite(pin,HIGH);
  delay(5000);}
  else if(state2 == 1){
  digitalWrite(outpin,HIGH);
  Serial.println("Second battery!");}
}
 delay(1000);
}

int battery_switch(int ADCin,int m, int n){
    double v_battery;
    v_battery = (ADCin*5.0/1023.0)*(1+double(m)/n);
    if (v_battery > voltage_of_battery){
    return 1;}
    else{
    return 0;}}
