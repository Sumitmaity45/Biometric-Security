#include <Adafruit_Fingerprint.h> 
#include <SoftwareSerial.h> 
#include <LCD.h> 
#include <LiquidCrystal_I2C.h> 
#define I2C_ADDR 0x27 //LCD i2c stuff 
#define BACKLIGHT_PIN 3 
#define En_pin 2 
#define Rw_pin 1 
#define Rs_pin 0 
#define D4_pin 4 
#define D5_pin 5 
#define D6_pin 6 
#define D7_pin 7 
int in1 = 5; // In1 and In2 for the L298n H bridge module 
int in2 = 4; 
int But1=8; // In order to add a push button I got the 5v from the pin 8 instead of "5v" arduino pin 
int But2=9; 
 String Names[] = { "Amit", "Sumit", "Arijit", “Souvik” Dipra”};  //Those are the names affected to the fingertemplates IDs 
//The first on which is Names[0] : Amit has the ID 1 in the fingerprint sensor 
SoftwareSerial mySerial(2, 3) //Fingerprint sensor wiring RX 3, TX 2 
LiquidCrystal_I2C 
lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7 _pin); //LCD declaring 
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial); //Fingerprint sensor declaring 
void setup() 
{
pinMode(in1, OUTPUT); 
pinMode(in2, OUTPUT); 
pinMode(But1, OUTPUT); //Push button stuff, I made the pin 8 permanent HIGH level 
digitalWrite(But1,HIGH); 
pinMode(But2,INPUT); //And I read the button state on pin 9 
Serial.begin(9600); //Serial begin incase you need to see something on the serial monitor 
finger.begin(57600); //Sensor baude rate 
lcd.begin (16,2); 
lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE); 
lcd.setBacklight(HIGH); 
lcd.home (); 
finger.getTemplateCount(); //Counts the number of templates stored in the sensor flash memory 
delay(2000); 
lcd.setCursor(0,0); 
lcd.print("Place finger"); 
CloseDoor(); //As we don't know the state of the lock system, first we make sure it's closed 
}
void loop() 
{
getFingerprintIDez(); //This function keeps looping and waiting for a fingerprint to be put on the sensor 
delay(50); 
bool Button1=digitalRead(But2); //Reading the state of the pushbutton 
if(Button1==HIGH){ //If the button is pressed it opens the doorlock as the button is meant to be inside 
OpenDoor(); //And once more it depends on your system, mine requires an electrical signal to open 
delay(5000); //Opening delay 5s 
CloseDoor(); //Closing sequence requires electrical signal otherwise it stays open 
}
}
//Main function taken from the "fingerprint" example and modified 
//Only the modifications are commented 
int getFingerprintIDez() { 
uint8_t p = finger.getImage(); //Image scanning 
if (p != FINGERPRINT_OK) return -1; 
p = finger.image2Tz(); //Converting 
if (p != FINGERPRINT_OK) return -1; 
p = finger.fingerFastSearch(); //Looking for matches in the internal memory 
if (p != FINGERPRINT_OK){     //if the searching fails it means that the template isn't registered 
lcd.clear(); //And here we write a message or take an action for the denied template 
lcd.print("Access denied"); 
delay(2000); 
lcd.clear(); 
lcd.print("Place finger"); 
return -1; 
}
//If we found a match we proceed in the function 
lcd.clear(); 
lcd.print("Welcome"); //Printing a message for the recognized template 
lcd.setCursor(2,1); 
lcd.print(Names[finger.fingerID-1]); //Then print the name we gave it and the -1 is to remove the shift as the ID starts from "1" but the array from "0" 
OpenDoor(); //Opening and closing sequence 
delay(5000); 
CloseDoor(); 
lcd.clear(); 
lcd.print("Place finger"); 
return finger.fingerID; 
}
/*The two following functions depends on your locking system 
*mine has a DC motor that should turn in both ways for opening and 
closing 
*that's why I'm using H bridge, you could have a solenoid with a 
transistor 
*then you'll need only opening sequence but controlling the 
transistor 
*as the closing is spring loaded... 
*/ 
void OpenDoor(){ 
digitalWrite(in1, LOW); //Turn in a direction for 100ms and stops 
digitalWrite(in2, HIGH); //it's not the delay for the whole sequence 
delay(100); // the sequence delay is above (5000) 5s 
digitalWrite(in1, LOW); //If I stop the motor, the lock holds its place either open or closed 
digitalWrite(in2, LOW);
}
void CloseDoor(){ 
digitalWrite(in1, HIGH); //Turn in other direction for 100ms and stops 
digitalWrite(in2,LOW); 
delay(100); 
digitalWrite(in1,LOW); 
digitalWrite(in2,LOW);
}
