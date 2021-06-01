#include <SoftwareSerial.h>
#include <Wire.h>
#include <PZEM004Tv30.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define VCC2 5

SoftwareSerial nodemcu(2,3);
PZEM004Tv30 pzem(11, 12); 
Adafruit_SSD1306 display(-1);
 
//long int data;

float sdata1 = 0;
float sdata2 = 0;
float sdata3 = 0;
float sdata4 = 0;
float sdata5 = 0;
float sdata6 = 0;

 
String cdata, idata, vdata, edata,pdata; // complete data
 
void setup()
{
Serial.begin(9600); 
nodemcu.begin(9600);
pinMode(VCC2, OUTPUT);
digitalWrite(VCC2, HIGH); 
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
display.clearDisplay();
}
 
void loop()
{
if(nodemcu.available() == 0 )
{
      Serial.print("Voltage = ");
    Serial.print(pzem.voltage());
    sdata1 = pzem.voltage();
    Serial.println(" V");
    
    Serial.print("Current ");
    Serial.print(pzem.current());
    sdata2 = pzem.current();
    Serial.println(" A");
 
    Serial.print("Power");
    Serial.print(pzem.power());
    sdata3 = pzem.power();
    Serial.println(" W");  

    Serial.print("Energy");
    Serial.print(pzem.energy());
    sdata4 = pzem.energy();
    Serial.println(" kWh"); 

    Serial.print("Frequency");
    Serial.print(pzem.frequency());
    sdata5 = pzem.frequency();
    Serial.println(" Hz"); 

    Serial.print("Power Factor");
    Serial.println(pzem.pf());
    sdata6 = pzem.pf();
 
   cdata = cdata + sdata1+","+sdata2+","+sdata3+","+sdata4+","+sdata5+","+sdata6;
   Serial.println(cdata); 
   nodemcu.println(cdata);
   vdata = vdata + "Voltage = " + sdata1 + "V";
   idata = idata + "Current = " + sdata2 + "A";
   edata = edata + "Energy = " + sdata4 + "kWh";
   pdata = pdata + "Power = " + sdata3 + "kWh";
   /*display.setCursor(0,0);
   display.setTextSize(1);
   display.setTextColor(WHITE);
   display.println("Meter readings");*/
   display.setCursor(0,0);
   display.setTextSize(1);
   display.setTextColor(WHITE);
   display.println(vdata);
   display.println(idata);
   display.println(pdata);
   display.println(edata);
   display.display();
   //display.startscrollright(0x00,0x00);
   delay(1000); // 100 milli seconds
   display.clearDisplay();
   cdata = "";
   vdata = ""; 
   idata = "";
   edata = "";
   pdata = "";
}
}
/*if ( nodemcu.available() > 0 ) 
{
  data = nodemcu.parseInt();
  delay(100); 
  Serial.println(data); 
 
  if ( data == 10 )
  {
    digitalWrite(relay1, LOW); 
  }
 
    if ( data == 11 )
  {
    digitalWrite(relay1, HIGH); 
  }
 
  // relay2 
 
    if ( data == 12 )
  {
    digitalWrite(relay2, LOW); 
  }
 
    if ( data == 13 )
  {
    digitalWrite(relay2, HIGH); 
  }
}
 */
