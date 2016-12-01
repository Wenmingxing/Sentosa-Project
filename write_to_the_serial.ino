/* SERIAL*/

int led = 13;
void setup() {
  
Serial.begin(9600);
// set the baud rate
Serial.write("Ready"); // print "Ready" once
pinMode(led,OUTPUT);

}
int state = 0;
void loop() {
  


if(Serial.available()){ 
  // only send data back if data has been sent
state = Serial.read('\n'); // read the incoming data

if (state == 0){
    digitalWrite(led,LOW);
    state = 1;}
else{digitalWrite(led,HIGH);
state = 0;}// send the data back in a new line so that it is not all one long line
}

delay(100); // delay for 1/10 of a second
}

