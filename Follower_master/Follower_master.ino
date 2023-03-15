
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>


#define SS_PIN 10
#define RST_PIN 9
#define echoPin_right 5
#define trigPin_right 4
#define trigPin_left 3
#define echoPin_left 2 

MFRC522 rfid(SS_PIN, RST_PIN);
int x;
int R = 0; 
int L = 0;
int duration_right = 0 ;
int duration_left = 0 ;
int distance_right = 0; 
int distance_left = 0 ;



void setup() {
  Serial.begin(9600);
  SPI.begin(); // init SPI bus
  rfid.PCD_Init(); // init MFRC522

  Wire.begin(); //include no input arguments to start arduino as master on i2c line

  pinMode(trigPin_left, OUTPUT);
  pinMode(trigPin_right, OUTPUT);
  pinMode(echoPin_left, INPUT);
  pinMode(echoPin_right, INPUT);


}

void loop() {

  
  if (rfid.PICC_IsNewCardPresent()) { // new tag is available
    if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
      MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
      Serial.print("RFID/NFC Tag Type: ");
      //Serial.println(rfid.PICC_GetTypeName(piccType));

      // print NUID in Serial Monitor in the hex format
      Serial.print("UID:");
      String content= "";
      for (int i = 0; i < rfid.uid.size; i++) {
        Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(rfid.uid.uidByte[i], HEX);
        content.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
        content.concat(String(rfid.uid.uidByte[i], HEX));
      }
      content.toUpperCase();
      if (content.substring(1) == "D9 6E 0A A3") //change here the UID of the card/cards that you want to give access
      {
        Serial.println("Authorized access");
        x = 1;
        Wire.beginTransmission(9); // transmit to device #9
        Wire.write(x);              // sends one byte
        Wire.endTransmission();    // stop transmitting
        Serial.println();
        delay(3000);
      }

      else {Serial.println("no");}

      rfid.PICC_HaltA(); // halt PICC
      rfid.PCD_StopCrypto1(); // stop encryption on PCD
    }
  }
  digitalWrite(trigPin_right, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin_right, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_right, LOW);
  duration_right = pulseIn(echoPin_right, HIGH);
  distance_right = duration_right * 0.034 / 2; 
  Serial.println(distance_right);

  digitalWrite(trigPin_left, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin_left, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_left, LOW);
  duration_left = pulseIn(echoPin_left, HIGH);
  distance_left = duration_left * 0.034 / 2; 
  Serial.println(distance_left);

  if (distance_right <=7){
    R = 2;
    Serial.println("Obstacle on right");
    Wire.beginTransmission(9);
    Wire.write(R);
    Wire.endTransmission(); 
    Serial.println();
    
  }
  
  else if (distance_left <=7){
    L = 3 ;
    Serial.println("Obstacle on left");
    Wire.beginTransmission(9);
    Wire.write(L);
    Wire.endTransmission(); 
    Serial.println();
  }

  else if (distance_left >7 && distance_right>7){
    Serial.println("No obstacle on left");
    Wire.beginTransmission(9);
    Wire.write(0);
    Wire.endTransmission(); 
    Serial.println();
}
} 
