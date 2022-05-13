
//gps and oled libraries
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPS++.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

//On ESP32: GPIO-21(SDA), GPIO-22(SCL)
#define OLED_RESET -1 //Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C //See datasheet for Address
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Alarm sys connection
#define BUTTON_PIN 19 // ESP32 GIOP16 pin connected to button's pin
#define BUZZER_PIN 18 // ESP32 GIOP21 pin connected to Buzzer's pin

//GPS module connection
#define RXD2 16
#define TXD2 17
HardwareSerial neogps(1);

TinyGPSPlus gps;


//lora
#define ledPin 2
#define pSwitch 13
int toggleState_1 = 0; //Define integer to remember the toggle state
String sendingString = "AT+SEND=1,36,";

//defining parameters
float Lat;
float Lng;
float Alt;
float Sat;



void setup() {
  // put your setup code here, to run once:
   //Begin serial communication Arduino IDE (Serial Monitor)
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(pSwitch, INPUT);  

  pinMode(BUTTON_PIN, INPUT); // set ESP32 pin to input pull-up mode
  pinMode(BUZZER_PIN, OUTPUT);       // set ESP32 pin to output mode


    //Begin serial communication Neo6mGPS
  neogps.begin(9600, SERIAL_8N1, RXD2, TXD2);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.display();
  //delay(2000);
}


void loop() {
  // put your main code here, to run repeatedly:
    Lat = gps.location.lat();
    Lng = gps.location.lng();
    Alt = gps.altitude.meters();
    Sat = gps.satellites.value();
   if (digitalRead(pSwitch) == HIGH){
    if (toggleState_1 == 0) {
      sendingString +=  String(Lat, 6)  +  String(Lng, 6)  +  String(Alt)  +  String(Sat, 0);
      Serial.println(sendingString);
      Serial.println("working");
      digitalWrite(ledPin, HIGH);
      toggleState_1 = 1;
      delay(500);

    }
    else {
      digitalWrite(ledPin, LOW);
      toggleState_1 = 0;
      delay(500);
    }
  }

  boolean newData = false;
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (neogps.available())
    {
      if (gps.encode(neogps.read()))
      {
        newData = true;
      }
    }
  }


  if(newData == true)
  {
    newData = false;
    Serial.print("Lat=");  Serial.println(gps.location.lat(), 6);
    Serial.print("Lng="); Serial.println(gps.location.lng(), 6);
    Serial.print("ALT=");  Serial.println(gps.altitude.meters());
    Serial.print("SAT=");  Serial.println(gps.satellites.value());
    Serial.print("Speed="); Serial.println(gps.speed.kmph());
    print_speed();
  }
  else
  {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.setTextSize(3);
    display.print("No Data");
    display.display();
  }  
  int buttonState = digitalRead(BUTTON_PIN); // read new state

  if (buttonState == LOW) {
    //Serial.println("The button is being pressed");
    digitalWrite(BUZZER_PIN, HIGH); // turn on
  }
  else
  if (buttonState == HIGH) {
    //Serial.println("The button is unpressed");
    digitalWrite(BUZZER_PIN, LOW);  // turn off
  }
}
void print_speed()
{
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
       
  if (gps.location.isValid() == 1)
  {
   //String gps_speed = String(gps.speed.kmph());
    display.setTextSize(1);
    
    display.setCursor(25, 5);
    display.print("Lat: ");
    display.setCursor(50, 5);
    display.print(gps.location.lat(),6);

    display.setCursor(25, 20);
    display.print("Lng: ");
    display.setCursor(50, 20);
    display.print(gps.location.lng(),6);

    display.setCursor(25, 35);
    display.print("Speed: ");
    display.setCursor(65, 35);
    display.print(gps.speed.kmph());
    
    display.setTextSize(1);
    display.setCursor(0, 50);
    display.print("SAT:");
    display.setCursor(25, 50);
    display.print(gps.satellites.value());

    display.setTextSize(1);
    display.setCursor(70, 50);
    display.print("ALT:");
    display.setCursor(95, 50);
    display.print(gps.altitude.meters(), 0);

    display.display();
    
  }
  else
  {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.setTextSize(3);
    display.print("No Data");
    display.display();
  }  

}
