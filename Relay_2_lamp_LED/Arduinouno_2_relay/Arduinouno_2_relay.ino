const int relay = 8;
const int led = 10;
int data; 
void setup()
 {
   Serial.begin(9600); 
   pinMode(9, OUTPUT);
   digitalWrite(9, HIGH);
   pinMode(relay, OUTPUT);
   pinMode(led, OUTPUT); 
 }
 
void loop()
 {
  if( Serial.available() > 0 ) 
    {
       data = Serial.parseInt();
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
       if ( data == 10 )
           {
              digitalWrite(led, LOW); 
           }
       if ( data == 11 )
           {
              digitalWrite(led, HIGH); 
           }    
    }
 }
