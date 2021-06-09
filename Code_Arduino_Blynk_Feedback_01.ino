/**********************************************************************************
 *  Code for Arduino.
 *  TITLE: Arduino ESP8266 control Relay Module with Blynk and IR remote | Real-Time Feedback
 *  Click on the following links to learn more. 
 *  YouTube Video: https://youtu.be/Nu2pQ-BNvEo
 *  Related Blog : https://easyelectronicsproject.com/projects/arduino-projects/
 *  by Tech StudyCell
 *  
 *  Download the libraries
 *  https://github.com/contrem/arduino-timer
 *  https://github.com/z3t0/Arduino-IRremote
 **********************************************************************************/

#include <IRremote.h>
#include <arduino-timer.h>

auto timer = timer_create_default(); // create a timer with default settings

//Define PIN constant
const int RELAY_PIN_1 = 12;
const int RELAY_PIN_2 = 11;
const int RELAY_PIN_3 = 10;
const int RELAY_PIN_4 = 9;
const int smode = 7;
const int irled = 6;
const int wifiled = 5;

int RECV_PIN = 8;

char toggleState_1 = 'a'; //Define integer to remember the toggle state for switch 1
char toggleState_2 = 'b'; //Define integer to remember the toggle state for switch 2
char toggleState_3 = 'c'; //Define integer to remember the toggle state for switch 3
char toggleState_4 = 'd'; //Define integer to remember the toggle state for switch 4
int ModeFlag = 1;

String pinStatus = "abcd";

//Define IR receiver and Result Objects
IRrecv irrecv(RECV_PIN);
decode_results results;

void sendStatus(){
  Serial.print(pinStatus);
  //true;
}

void relayOnOff(int relay){

    switch(relay){
      case 1: 
             if(toggleState_1 == 'a'){
              digitalWrite(RELAY_PIN_1, HIGH); // turn on relay 1
              toggleState_1 = 'A';
              }
             else{
              digitalWrite(RELAY_PIN_1, LOW); // turn off relay 1
              toggleState_1 = 'a';
              }
      break;
      case 2: 
             if(toggleState_2 == 'b'){
              digitalWrite(RELAY_PIN_2, HIGH); // turn on relay 2
              toggleState_2 = 'B';
              }
             else{
              digitalWrite(RELAY_PIN_2, LOW); // turn off relay 2
              toggleState_2 = 'b';
              }
      break;
      case 3: 
             if(toggleState_3 == 'c'){
              digitalWrite(RELAY_PIN_3, HIGH); // turn on relay 3
              toggleState_3 = 'C';
              }
             else{
              digitalWrite(RELAY_PIN_3, LOW); // turn off relay 3
              toggleState_3 = 'c';
              }
      break;
      case 4: 
             if(toggleState_4 == 'd'){
              digitalWrite(RELAY_PIN_4, HIGH); // turn on relay 4
              toggleState_4 = 'D';
              }
             else{
              digitalWrite(RELAY_PIN_4, LOW); // turn off relay 4
              toggleState_4 = 'd';
              }
      break;
      default : break;      
      } 
      delay(200);
}

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Enable the IR receiver
   pinMode(RELAY_PIN_1, OUTPUT);
   pinMode(RELAY_PIN_2, OUTPUT);
   pinMode(RELAY_PIN_3, OUTPUT);
   pinMode(RELAY_PIN_4, OUTPUT);
   pinMode(irled,       OUTPUT);
   pinMode(wifiled,     OUTPUT);
   pinMode(smode,       INPUT);

   digitalWrite(irled, HIGH); // turn on IRLED
   digitalWrite(wifiled, HIGH);

  // call the toggle_led function every 1000 millis (1 second)
  timer.every(2000, sendStatus);
}

void loop() {

if (digitalRead(smode) == HIGH){  
  if (ModeFlag == 0){
    ModeFlag = 1;
    digitalWrite(irled, HIGH); // turn on IRLED
    digitalWrite(wifiled, HIGH); //turn on WIFILED
    delay(200);
  }
  else if (ModeFlag == 1) {
    ModeFlag = 0;
    digitalWrite(wifiled, LOW); // turn off WIFILED
    digitalWrite(irled, HIGH); // turn on IRLED
    delay(200);
  }
  //delay(1000);
}
if (ModeFlag == 1) {  //Both WiFi & IR control
  //WiFi Control
  if(Serial.available()>0)
   {     
      char c_val= Serial.read(); // reading the data received from the bluetooth module
      switch(c_val)
      {
        case 'A': digitalWrite(RELAY_PIN_1, HIGH);  toggleState_1 = 'A'; break; // when A is pressed on the app Turn on Pin 12
        case 'a': digitalWrite(RELAY_PIN_1, LOW);   toggleState_1 = 'a'; break; // when a is pressed on the app Turn off Pin 12
        case 'B': digitalWrite(RELAY_PIN_2, HIGH);  toggleState_2 = 'B'; break; // when B is pressed on the app Turn on Pin 11
        case 'b': digitalWrite(RELAY_PIN_2, LOW);   toggleState_2 = 'b'; break; // when b is pressed on the app Turn off Pin 11
        case 'C': digitalWrite(RELAY_PIN_3, HIGH);  toggleState_3 = 'C'; break; // when C is pressed on the app Turn on Pin 10
        case 'c': digitalWrite(RELAY_PIN_3, LOW);   toggleState_3 = 'c'; break; // when c is pressed on the app Turn off Pin 10
        case 'D': digitalWrite(RELAY_PIN_4, HIGH);  toggleState_4 = 'D'; break; // when D is pressed on the app Turn on Pin 9
        case 'd': digitalWrite(RELAY_PIN_4, LOW);   toggleState_4 = 'd'; break; // when d is pressed on the app Turn off Pin 9
        default : break;
      }
      pinStatus = String(toggleState_1) + String(toggleState_2) + String(toggleState_3) + String(toggleState_4);
   }
   delay(100);
  //IR Remote Control
   if (irrecv.decode(&results)) {
    switch(results.value){
      case 0x80BF49B6: relayOnOff(1); break;
      case 0x80BFC936: relayOnOff(2); break;
      case 0x80BF33CC: relayOnOff(3); break;
      case 0x80BF718E: relayOnOff(4); break;
      default : break;      
      }
    pinStatus = String(toggleState_1) + String(toggleState_2) + String(toggleState_3) + String(toggleState_4);
    irrecv.resume(); // Receive the next value
  }
}

else if (ModeFlag == 0) {  //only IR control
 //IR Remote Control
   if (irrecv.decode(&results)) {
    switch(results.value){
      case 0x80BF49B6: relayOnOff(1); break;      
      case 0x80BFC936: relayOnOff(2); break;
      case 0x80BF33CC: relayOnOff(3); break;
      case 0x80BF718E: relayOnOff(4); break;
      default : break;      
      }
    pinStatus = String(toggleState_1) + String(toggleState_2) + String(toggleState_3) + String(toggleState_4);
    irrecv.resume(); // Receive the next value
  }
}
timer.tick(); // tick the timer
}
