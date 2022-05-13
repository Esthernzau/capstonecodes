#define ledPin 2

#include <WiFi.h>
#include <WiFiClient.h>

const char* ssid = "Kavini";
const char* password = "Shiku@2018";
const char* host = "192.168.43.202";
  
String incomingString;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  Serial.println("starting");

//connecting to wifi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid,password);

  while (WiFi.status() != WL_CONNECTED){
    delay(50000);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
}

void loop() {
  // acquiring gps coordnates and prinint them
  if(Serial.available()) {
    incomingString = Serial.readString();
    String receivedlatitude = incomingString.substring(10,17); 
    String receivedlongitude = incomingString.substring(18,26);
    String receivedaltitude = incomingString.substring(27,33);
    String receivedsatellite = incomingString.substring(34,35);
    digitalWrite(ledPin, HIGH);
    Serial.println("working");
    Serial.print("latitude: ");
    Serial.println(receivedlatitude); 
    Serial.print("longitude: ");
    Serial.println(receivedlongitude); 
    Serial.print("altitude: ");
    Serial.println(receivedaltitude); 
    Serial.print("satellite: ");
    Serial.println(receivedsatellite); 


  Serial.print("Connecting to ");
  Serial.println(host);

//using wificlient class to creat TCP connections
   WiFiClient client;
   const int httpPort = 80;
   if (!client.connect(host, httpPort)) {
       Serial.println("connection failed");
       return;
    }

    //sending request to server
client.print(String("GET http://192.168.43.202/capstonedata/capstonefile1.php?")+
                         ("&Lat=")+receivedlatitude + 
                         ("&Lngd=")+receivedlongitude +
                         ("&Alt=")+receivedaltitude +
                         ("&Sat=")+receivedsatellite +
                          //("&CO2_Level=")+gassensorAnalog +
                          "HTTP/1.1\r\n" +
                "Host: " + host + "\r\n" +
                "Connection ; close\r\n\r\n");
   unsigned long timeout = millis();
   while(client.available() == 0){
    if(millis() - timeout > 1000){
      Serial.println(">>> Client Timeout!");
      client.stop();
      return;
    }
   }

   //reading lines of reply from server and printing to serial monitor
   while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
   }

   Serial.println();
   Serial.println("closing connection");
  // }
           
  }else{
  digitalWrite(ledPin, LOW);}
}
