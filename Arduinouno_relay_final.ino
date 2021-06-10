#include <PZEM004Tv30.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

PZEM004Tv30 pzem(11, 12); 
Adafruit_SSD1306 display(-1);
 
const int relay = 8;
long int data;
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
   pinMode(9, OUTPUT);
   digitalWrite(9, HIGH);
   pinMode(relay, OUTPUT);
 
   display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
   display.clearDisplay();
 }
 
void loop()
 {
   if(Serial.available() == 0 )
     {
       sdata1 = pzem.voltage();
       sdata2 = pzem.current();
       sdata3 = pzem.power();
       sdata4 = pzem.energy();   
       sdata5 = pzem.frequency();
       sdata6 = pzem.pf();
 
       cdata = cdata + sdata1+","+sdata2+","+sdata3+","+sdata4+","+sdata5+","+sdata6;
       Serial.println(cdata); 

       vdata = vdata + "Voltage = " + sdata1 + "V";
       idata = idata + "Current = " + sdata2 + "A";
       edata = edata + "Energy = " + sdata4 + "W";
       pdata = pdata + "Power = " + sdata3 + "kWh";
 
       display.setCursor(0,0);
       display.setTextSize(1);
       display.setTextColor(WHITE);
       display.println(vdata);
       display.println(idata);
       display.println(pdata);
       display.println(edata);
       display.display();
       delay(1000); 
       display.clearDisplay();
       cdata = "";
       vdata = ""; 
       idata = "";
       edata = "";
       pdata = "";
    }
  if( Serial.available() > 0 ) 
    {
       data = Serial.read();
       delay(100);
       Serial.println("Relay Signal");
       Serial.println(data);
       if ( data == 0 )
           {
              digitalWrite(relay, LOW); 
           }
       if ( data == 1 )
           {
              digitalWrite(relay, HIGH); 
           }
    }
 }
