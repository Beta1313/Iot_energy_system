

//Azure IoT Hub + DHT11 + NodeMCU ESP8266 Experiment Done By Prasenjit Saha
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

// WiFi settings
const char* ssid = "POCOX2";
const char* password = "sherianna1234";

String myString,PostData;                                                     // complete message from arduino, which consistors of snesors data
char rdata;                                                          // received charactors
 
float firstVal, secondVal, thirdVal, fourthVal, fifthVal, sixVal;    // sensor reading


//Azure IoT Hub
const String AzureIoTHubURI="https://smartenergy.azure-devices.net/devices/myesp8266/messages/events?api-version=2020-03-13"; 
//openssl s_client -servername smartenergy.azure-devices.net -connect smartenergy.azure-devices.net:443 | openssl x509 -fingerprint -noout //
const String AzureIoTHubFingerPrint="B2:AA:F4:07:DA:8F:5D:1E:B7:7C:6B:3C:C4:7A:83:61:3B:68:C1:E1"; 
//az iot hub generate-sas-token --device-id {YourIoTDeviceId} --hub-name {YourIoTHubName} 
const String AzureIoTHubAuth="SharedAccessSignature sr=smartenergy.azure-devices.net%2Fdevices%2Fmyesp8266&sig=pXDT990zdiNUT7q7W4wwn5xsTNPgUU1XU5gmXjW3ZdQ%3D&se=1625575507";

void setup() {
  Serial.begin(9600);
  
  Serial.println("ESP8266 starting in normal mode");
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  if (Serial.available() == 0 ) 
    {
      delay(1000);
      
    }
   
   if (Serial.available() > 0 ) 
    {
      rdata = Serial.read(); 
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
          PostData="{ \"DeviceId\":\"ESP8266\",\"Voltage\":"+String(firstVal)+",\"Current\":"+String(secondVal)+",\"Power\":"+String(thirdVal)+",\"Energy\":"+String(fourthVal)+",\"Power Factor\":"+String(sixVal)+"}";
      Serial.println(PostData);
      // Send data to cloud
      int returnCode=RestPostData(AzureIoTHubURI,AzureIoTHubFingerPrint,AzureIoTHubAuth,PostData);
      Serial.println(returnCode);
          myString = "";
       }
   }
    
      
}

// Functions
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
