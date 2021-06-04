#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <String.h>
#include "sha256.h"
#include "Base64.h"

// START: Azure Evet Hub settings
   
const char* KEY = "Your event hub primary key";    // event hub access key 
const char* KEY_NAME = "RootManageSharedAccessKey";                 // event hub key name  ( policy name) 
const char* HOST = "your evnt hub namespace.servicebus.windows.net";              // event hub name (name of service bus)
const char* END_POINT = "/ your event hub name /messages";                 // name of the evnthub which we create inside eventhub namespace
// END: Azure Evet Hub settings 
// START: WiFi settings
const char* SSID = "wifi ssid";
const char* PASSWORD = "wifi password";
// END: WiFi settings
String request;
String data;
String fullSas;
 WiFiClientSecure client;
void setup() {
  Serial.begin(115200);

  // START: Naive URL Encode
  String url = "https://" + (String)HOST + (String)END_POINT;
  url.replace(":", "%3A");
  url.replace("/", "%2F");
  Serial.println(url);
  // END: Naive URL Encode

  // START: Create SAS
  // https://azure.microsoft.com/en-us/documentation/articles/service-bus-sas-overview/
  // Where to get secods since the epoch: local service, SNTP, RTC
  int expire = 1711104241;
  String stringToSign = url + "\n" + expire;

  // START: Create signature
  Sha256.initHmac((const uint8_t*)KEY, 44);
  Sha256.print(stringToSign);
  char* sign = (char*) Sha256.resultHmac();
  

  //Serial.println(String (Sha256.resultHmac));
  int signLen = 32;
  // END: Create signature

  // START: Get base64 of signature
  int encodedSignLen = base64_enc_len(signLen);
  char encodedSign[encodedSignLen];
  base64_encode(encodedSign, sign, signLen); 
  String encodedSas = (String) encodedSign;

  // Naive URL encode
  encodedSas.replace("=", "%3D");
  //Serial.println(encodedSas);
  // END: Get base64 of signature

  // SharedAccessSignature
   fullSas = "sr=" + url + "&sig="+ encodedSas + "&se=" + expire +"&skn=" + KEY_NAME;
  // END: create SAS
//Serial.println("SAS below");
//Serial.println(fullSas);
//Serial.println();
  // START: Wifi connection
  Serial.print("connecting to ");
  Serial.println(SSID);
  
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  // END: Wifi connection

}

void loop() {
  //delay(100);
   WiFiClientSecure client;
  if (!client.connect(HOST, 443)) {
    Serial.println("connection failed");
    return;
  }
  int r=random(100,10000);
  int temp=random(25 , 45);
  int light= random(10 , 100);
  int soilm = random (10 , 70 );
  data = "{'deviceid':'esp8266'";
   data+= ", 'temperature' : ";
   data+= temp;
   data+=" , 'soilmoisture' : ";
   data+=soilm;
   data+=" , 'lightintensity' :";
   data+=light;
   data+="}"; 
   request = String("POST ") + END_POINT + " HTTP/1.1\r\n" +
               "Host: " + HOST + "\r\n" +
               "Authorization: SharedAccessSignature " + fullSas + "\r\n" +                
               "Content-Type: application/atom+xml;type=entry;charset=utf-8\r\n" + 
               "Content-Length: " + data.length() + "\r\n\r\n" +
               data;
    Serial.println(request);
  client.print(request);
 delay(100);
 /*
  String response = "";
  while (client.connected()) {
    response += client.readStringUntil('\n');
  }

  Serial.println();
  Serial.print("Response code: ");
  Serial.println(response.substring(9, 12));
  
*/
}
