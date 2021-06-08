#include <SoftwareSerial.h>
#include <PZEM004Tv30.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
display.clearDisplay();
}
 
void loop()
{
if(nodemcu.available() == 0 )
{
    
    sdata1 = pzem.voltage();
    sdata2 = pzem.current();
    sdata3 = pzem.power();
    sdata4 = pzem.energy();
    sdata5 = pzem.frequency();
    sdata6 = pzem.pf();
 
   cdata = cdata + sdata1+","+sdata2+","+sdata3+","+sdata4+","+sdata5+","+sdata6;
   Serial.println(cdata); 
   nodemcu.println(cdata);
   vdata = vdata + "Voltage = " + sdata1 + "V";
   idata = idata + "Current = " + sdata2 + "A";
   edata = edata + "Energy = " + sdata4 + "kWh";
   pdata = pdata + "Power = " + sdata3 + "kWh";

   display.setCursor(0,0);
   display.setTextSize(1);
   display.setTextColor(WHITE);
   display.println(vdata);
   display.println(idata);
   display.println(pdata);
   display.println(edata);
   display.display();
 
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
