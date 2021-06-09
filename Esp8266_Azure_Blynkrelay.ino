//espboard:2.5.2
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>

// WiFi settings
const char* ssid = "POCOX2";
const char* password = "sherianna1234";

char auth[] = "LAfGUqMmwbnuy2YqnNv5c2FyuMf9A6O2";

BlynkTimer timer;
SoftwareSerial arduino(D2,D3);

String myString,PostData;                                                  
char rdata;                                                          // received charactors
 
float firstVal, secondVal, thirdVal, fourthVal, fifthVal, sixVal;    // sensor reading


//Azure IoT Hub
const String AzureIoTHubURI="https://smartenergy.azure-devices.net/devices/myesp8266/messages/events?api-version=2020-03-13"; 
//openssl s_client -servername smartenergy.azure-devices.net -connect smartenergy.azure-devices.net:443 | openssl x509 -fingerprint -noout //
const String AzureIoTHubFingerPrint="DA:CC:26:E0:4D:91:4D:05:66:81:41:38:AB:86:62:C0:EA:93:CD:49"; 
//az iot hub generate-sas-token --device-id {YourIoTDeviceId} --hub-name {YourIoTHubName} 
const String AzureIoTHubAuth="SharedAccessSignature sr=smartenergy.azure-devices.net%2Fdevices%2Fmyesp8266&sig=UsVc14MVLAwdXnEfktVq%2FIlIh9hbxLPPaehQGXU6U0k%3D&se=1623546809";

void setup()
  {
     Serial.begin(9600);
     arduino.begin(9600);
     WiFi.begin(ssid, password);
    
     Serial.println(WiFi.localIP());          // Print the IP address

     Blynk.config(auth);

     timer.setInterval(1000L,sensorvalue1); 
     timer.setInterval(1000L,sensorvalue2); 
     timer.setInterval(1000L,sensorvalue3);
     timer.setInterval(1000L,sensorvalue4); 
     timer.setInterval(1000L,sensorvalue6);
  }

void loop()
  {
     if (arduino.available() == 0 ) 
       {
           Blynk.run();
           timer.run(); // Initiates BlynkTimer
       }
   
     if (arduino.available() > 0 ) 
       {
           rdata = arduino.read(); 
           myString = myString+ rdata; 
           if( rdata == '\n')
              {
                   String l = getValue(myString, ',', 0);
                   String m = getValue(myString, ',', 1);
                   String n = getValue(myString, ',', 2); 
                   String o = getValue(myString, ',', 3);
                   String p = getValue(myString, ',', 4);
                   String q = getValue(myString, ',', 5); 
  
                   firstVal = l.toFloat();
                   secondVal = m.toFloat();
                   thirdVal = n.toFloat();
                   fourthVal = o.toFloat();
                   fifthVal = p.toFloat();
                   sixVal = q.toFloat();
                   PostData="{ \"DeviceId\":\"ESP8266\",\"Voltage\":"+String(firstVal)+",\"Current\":"+String(secondVal)+",\"Power\":"+String(thirdVal)+",\"Energy\":"+String(fourthVal)+",\"Frequency\":"+String(fifthVal)+",\"Power Factor\":"+String(sixVal)+"}";
                   Serial.println(PostData);
                   
                   int returnCode=RestPostData(AzureIoTHubURI,AzureIoTHubFingerPrint,AzureIoTHubAuth,PostData);          // Send data to cloud
                   Serial.println(returnCode);
                   myString = "";
              }
      }
    
      
  }

int RestPostData(String URI, String fingerPrint, String Authorization, String PostData)
  {
      HTTPClient http;
      http.begin(URI, fingerPrint);
      
      http.addHeader("Authorization",Authorization);
      http.addHeader("Content-Type", "application/atom+xml;type=entry;charset=utf-8");
      int returnCode=http.POST(PostData);
      if(returnCode<0) 
         {  
            Serial.println("RestPostData: Error sending data: "+String(http.errorToString(returnCode).c_str()));
         }
      
      http.end();
      return returnCode;
  }

String getValue(String data, char separator, int index)
  {
      int found = 0;
      int strIndex[] = { 0, -1 };
      int maxIndex = data.length() - 1;
 
      for (int i = 0; i <= maxIndex && found <= index; i++) 
          {
               if (data.charAt(i) == separator || i == maxIndex) 
                 {
                     found++;
                     strIndex[0] = strIndex[1] + 1;
                     strIndex[1] = (i == maxIndex) ? i+1 : i;
                 }
          }
       return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
  }

void sensorvalue1()
  {
      float sdata = firstVal;
      Blynk.virtualWrite(V1, sdata);
  }
  
void sensorvalue2()
  {
      float sdata = secondVal;
      Blynk.virtualWrite(V2, sdata);
  }
 
void sensorvalue3()
  {
      float sdata = thirdVal;
      Blynk.virtualWrite(V3, sdata);
  }

void sensorvalue4()
  {
      float sdata = fourthVal;
      Blynk.virtualWrite(V4, sdata);
  }

void sensorvalue6()
  {
      float sdata = sixVal;
      Blynk.virtualWrite(V5, sdata);
  }

BLYNK_WRITE(V10)
  {
      int pinValue1 = param.asInt(); // assigning incoming value from pin V10 to a variable
      Serial.print("Switch Signal from Blynk");
      Serial.print(pinValue1);
      arduino.write(pinValue1);
  }
