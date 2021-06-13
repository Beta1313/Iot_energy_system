//espboard:2.5.2
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>

// WiFi settings
char ssid[] = "POCOX2";
char password[] = "sherianna1234";

char auth[] = "LAfGUqMmwbnuy2YqnNv5c2FyuMf9A6O2";

BlynkTimer timer;
SoftwareSerial arduino(D2,D3);

void setup()
  {
     Serial.begin(9600);
     arduino.begin(9600);
     Blynk.begin( auth, ssid, password);
     Serial.println(WiFi.localIP());          // Print the IP address
}

void loop()
  {
     if (arduino.available() == 0 ) 
       {
           Blynk.run();
           timer.run(); // Initiates BlynkTimer
       } 
  }


BLYNK_WRITE(V10)
  {
      String identfierrelay = "0";
      String pinValue1 = param.asStr(); // assigning incoming value from pin V10 to a variable
      String msg1 =identfierrelay+pinValue1;
      Serial.print("Switch Signal from Blynk");
      Serial.print(msg1);
      arduino.print(msg1);
  }

BLYNK_WRITE(V11)
  {
      String identfierled = "1";
      String pinValue2 = param.asStr(); // assigning incoming value from pin V11 to a variable
      String msg2 = identfierled+pinValue2;
      Serial.print("Switch Signal from Blynk");
      Serial.print(msg2);
      arduino.print(msg2);
      
  }
