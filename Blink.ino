const int pingPin1 = 22; // Trigger Pin of Ultrasonic Sensor
const int echoPin1 = 48; // Echo Pin of Ultrasonic Sensor
const int pingPin2 = 42; // Trigger Pin of Ultrasonic Sensor
const int echoPin2 = 40; 
const int ENA_PIN = 7; // the Arduino pin connected to the EN1 pin L298N
const int IN1_PIN = 6; // the Arduino pin connected to the IN1 pin L298N
const int IN2_PIN = 4; // the Arduino pin connected to the IN2 pin L298N


/*
PINOUT:
RC522 MODULE    Uno/Nano     MEGA
SDA             D10          D9
SCK             D13          D52
MOSI            D11          D51
MISO            D12          D50
IRQ             N/A          N/A
GND             GND          GND
RST             D9           D8
3.3V            3.3V         3.3V
*/
/* Include the standard Arduino SPI library */
#include <SPI.h>
#include <MFRC522.h>
/* Include the RFID library */
#include <RFID.h>

#define SS_PIN 53
#define RST_PIN 5
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.



void setup() {
     Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  pinMode(26,INPUT);
  
}

void loop() {
   ////////////////////////////////////////// Actuator code stops /////////////////////////////////////
 // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "23 AC 23 13") //change here the UID of the card/cards that you want to give access
  {
    int ir;
  
  ir = digitalRead(26);
  Serial.println(ir);
    
    pinMode(ENA_PIN, OUTPUT);
   pinMode(IN1_PIN, OUTPUT);
   pinMode(IN2_PIN, OUTPUT);

   digitalWrite(ENA_PIN, HIGH);

   
  /////////////////////////////////////////// Ultrasonic Code starts ////////////
   long duration1,duration2, inches1, inches2;
   pinMode(pingPin1, OUTPUT);
   pinMode(pingPin2, OUTPUT);
   digitalWrite(pingPin1, LOW);
   delayMicroseconds(2);

   digitalWrite(pingPin2, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin1, HIGH);
   delayMicroseconds(10);

   digitalWrite(pingPin2, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin1, LOW);

   pinMode(echoPin1, INPUT);
   duration1 = pulseIn(echoPin1, HIGH);
   inches1 = microsecondsToInches(duration1);
   digitalWrite(pingPin2, LOW);
   pinMode(echoPin2, INPUT);
   duration2 = pulseIn(echoPin2, HIGH);
   inches2 = microsecondsToInches(duration2);
   
   ///////////////////////////// Ultrasonic Code ends //////////////////////////////////

   ///////////////////////////////////// Actuator code starts //////////////////////////
   if (inches1 <=5 && inches2 <=5){
    
      digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, HIGH);

  delay (100); ////////////////////////// actuator will stop extending automatically when reaching the limit
   }
  else{ 
    // retracts the actuator
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);

   delay(100);//////////////////////////// actuator will stop retracting automatically when reaching the limit
  }
  
   Serial.print(inches1);
   Serial.print("in, ");
   Serial.print(inches2);
   Serial.print("in, ");

   Serial.println();
   delay(100);
  }
 
 else   {
    Serial.println(" Access denied");
    delay(3000);
  }
}

long microsecondsToInches(long microseconds) {
   return microseconds / 74 / 2;
}
