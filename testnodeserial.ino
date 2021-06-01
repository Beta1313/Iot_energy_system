#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
 
/*int pinValue1;
int pinValue2;
int pinValue3;
int pinValue4;
*/
 
 
char auth[] = "LAfGUqMmwbnuy2YqnNv5c2FyuMf9A6O2";
 
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "POCOX2";
char pass[] = "sherianna1234";
 
BlynkTimer timer;
 
String myString; // complete message from arduino, which consistors of snesors data
char rdata; // received charactors
 
//for temperature , pressure and altitude
float firstVal, secondVal, thirdVal, fourthVal, fifthVal, sixVal; // sensors 
// This function sends Arduino's up time every second to Virtual Pin (1).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.

 
 
void setup()
{
  // Debug console
  Serial.begin(9600);
 
  Blynk.begin(auth, ssid, pass);
 
    timer.setInterval(1000L,sensorvalue1); 
    timer.setInterval(1000L,sensorvalue2); 
    timer.setInterval(1000L,sensorvalue3);
    timer.setInterval(1000L,sensorvalue4); 
    timer.setInterval(1000L,sensorvalue6);
}
 
void loop()
{
   if (Serial.available() == 0 ) 
   {
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
   }
   
  if (Serial.available() > 0 ) 
  {
    rdata = Serial.read(); 
    myString = myString+ rdata; 
    
    if( rdata == '\n')
    {
   Serial.println(myString); 
  
// new code
String l = getValue(myString, ',', 0);
String m = getValue(myString, ',', 1);
String n = getValue(myString, ',', 2); 
String o = getValue(myString, ',', 3);
String p = getValue(myString, ',', 4);
String q = getValue(myString, ',', 5); 
 
Serial.println(l);
Serial.println(m);
firstVal = l.toFloat();
secondVal = m.toFloat();
thirdVal = n.toFloat();
fourthVal = o.toFloat();
fifthVal = p.toFloat();
sixVal = q.toFloat();

  myString = "";
// end new code
    }
  }
 
}
 
void sensorvalue1()
{
float sdata = firstVal;
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V1, sdata);
 
}
void sensorvalue2()
{
float sdata = secondVal;
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, sdata);
 
}
 
void sensorvalue3()
{
float sdata = thirdVal;
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V3, sdata);
 
}

void sensorvalue4()
{
float sdata = fourthVal;
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V4, sdata);
 
}

void sensorvalue6()
{
float sdata = sixVal;
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, sdata);
 
}
 
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;
 
    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
 
/*// in Blynk app writes values to the Virtual Pin 10
BLYNK_WRITE(V10)
{
   pinValue1 = param.asInt(); // assigning incoming value from pin V10 to a variable
 
  Serial.print(pinValue1);
 
}
 
 
// in Blynk app writes values to the Virtual Pin 11
BLYNK_WRITE(V11)
{
   pinValue2 = param.asInt(); // assigning incoming value from pin V10 to a variable
 
  Serial.print(pinValue2);
 
}*/
