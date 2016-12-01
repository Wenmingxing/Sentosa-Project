          /* Battery switch with a relay considering the battery type: lithium or lead acid battery
             Coded by Luke wen
             04/Nov/2016
             
             Goal:
             This coding is willing to imporve the battery switch system even the battery types are 
             different for both betteries.
             
             Configuration:
             Battery(+) --> diode --> voltage divider(R1,R2) --> relay(switch) -> Boat --> battery(-)
             
             R1 --> A0 -->R2 --> GND
             
             For the battery type:
             5v --> switch -->pin(6 or 7), when the switch is on, the state of pin is LOW, meaning the battery is Lithium battery,
             In contrast, when the switch is closed, the pin's state is HIGH so bettery has changed into lead acid.
         */
          
          
          const int R1 = 200;
          const int R2 = 100;// The resistences for voltage devider in 
                             //order to evaluate whether the battery can power the boat system.
          
          const int outpin = 5;// The output pin to power the relay, and change the battery for the boat system.
          const int pin = 13; // LED indicates the warning that both batteries are low power.
          const int pin_lithium = 6;// Battery type for the first battery.LOW:Lithium,HIGH:Lead acid.
          const int pin_leadacid = 7;// Battery type for the second battery.LOW:Lithium,HIGH:lead acid.
 
          int state1;
          int state2;
          double voltage_of_battery;
          double low_of_lithium;
          double low_of_lead_acid;
          
          void setup() {
            
            Serial.begin(9600);//Communicate with serial in computer
            
            if(!Serial){;}
            pinMode(outpin,OUTPUT);
            pinMode(pin,OUTPUT); // Define the outpins for relay and LED (for indicating).
            digitalWrite(outpin,LOW);// Relay is on the state:Normal Closed,that is, bettery one is on.
            
            digitalWrite(pin,LOW);// LED light is off.
          
          }
          
          void loop() {
            digitalWrite(pin,LOW);//Reinitiate the state of LED.
            state1 = battery_switch(analogRead(A0),R1,R2,digitalRead(pin_lithium));
            state2 = battery_switch(analogRead(A1),R1,R2,digitalRead(pin_leadacid));// Read the voltages of two batteries.
            
            if (state1 == 1){
            digitalWrite(outpin,LOW);//Relay : normal closed, battery one  is connected.
            Serial.println("First Battery is on!");
            }// First Battery is on based on this condition.
            else if(state1==0){
            digitalWrite(outpin,HIGH);// First is low power,turn on the second battery.
            if(state2 == 0){
            Serial.println("Both batteries are low power!");
            digitalWrite(pin,HIGH);
            delay(1000);}// Both batteries are not available(Emergency stop consideration).
            else if(state2==1){
            Serial.println("Second Battery is on!");// Second battery is on under this condition.
            }}
            delay(1000);
          }
          
          int battery_switch(int ACDin,int m,int n,int bt){
              double v_battery = (ACDin*5.0/1023.0)*(1+double(m/n));
              if (bt == LOW){
              voltage_of_battery = low_of_lithium;}
              else{
              voltage_of_battery = low_of_lead_acid;} 
              
              if (v_battery > voltage_of_battery){
              return 1;}
              else{
              return 0;}
            }
