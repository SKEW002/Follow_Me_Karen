#include <Wire.h>

#define echoPin 4
#define trigPin 3
#define RIGHT 5
#define LEFT 7
#define buzzer 8
#define sharp A3
#define limit 2

int x = 0;
int y = 0;
long duration;
int distance;

int in1 = 10;
int in2 = 11;
int in3 = 12;
int in4 = 13;
int ena = 9;
int enb = 6;

void receiveEvent(int bytes) {
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  x = Wire.read();    // receive byte as an integer
  delay(10);
}


void dir(int choice){ //3forward 1back 4left 5right  2stop
  int power_1_3 = 200;
  int power_4_5 = 115;
  
  if (choice == 1){ //back
    analogWrite(ena, power_4_5); 
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enb, power_4_5); 
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW); 
  }

  else if (choice == 2){ //stop
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW); 
  }

  else if (choice == 3){ ///forward
    analogWrite(ena, power_4_5); 
    digitalWrite(in3, LOW);  
    digitalWrite(in4, HIGH);
    analogWrite(enb, power_4_5); 
    digitalWrite(in1, LOW);   
    digitalWrite(in2, HIGH);
  }

  else if (choice == 4){//left sharp
    analogWrite(ena, power_1_3); 
    digitalWrite(in3, LOW);  
    digitalWrite(in4, HIGH);
    analogWrite(enb, power_1_3); 
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  }
  
  else if (choice == 5){//right sharp
    analogWrite(ena, power_1_3); 
    digitalWrite(in1, LOW);  
    digitalWrite(in2, HIGH);
    analogWrite(enb, power_1_3); 
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }
  
  else if (choice == 6){//left
    analogWrite(ena, 0); 
    digitalWrite(in3, LOW);  
    digitalWrite(in4, HIGH);
    analogWrite(enb, 200); 
    digitalWrite(in1, LOW);   
    digitalWrite(in2, HIGH);
  }
  
  else if (choice == 7){//right
    analogWrite(ena, 200); 
    digitalWrite(in3, LOW);  
    digitalWrite(in4, HIGH);
    analogWrite(enb, 0); 
    digitalWrite(in1, LOW);   
    digitalWrite(in2, HIGH);
  }
}

void setup() {
  pinMode(ena, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(ena, INPUT);
  pinMode(enb, INPUT);

  pinMode(buzzer, OUTPUT);
  
  Wire.begin(9); // Start the I2C Bus as Slave on address 9
  Wire.onReceive(receiveEvent); // Attach a function to trigger when some data is received.
  
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT

  pinMode(limit, INPUT);
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
}


void loop() {
    Serial.println(x);
    if (x == 1) {
    tone(buzzer, 2000); // Send 1KHz sound signal...
    delay(200);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    delay(50);        // ...for 1sec
    tone(buzzer, 2000); // Send 1KHz sound signal...
    delay(200);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...    
    Serial.println("Access");
    x = 0;
    
    while(1){
      if (x == 1) {
        tone(buzzer, 2000); // Send 1KHz sound signal...
        delay(200);        // ...for 1 sec
        noTone(buzzer);     // Stop sound...
        delay(50);        // ...for 1sec
        tone(buzzer, 2000); // Send 1KHz sound signal...
        delay(200);        // ...for 1 sec
        noTone(buzzer);     // Stop sound...
        delay(2000);
        x = 0;
        dir(2);
        break;
      }    
      x = 0;
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH);
      distance = duration * 0.034 / 2; 
      
      int Right_Value = digitalRead(RIGHT);
      int Left_Value = digitalRead(LEFT);
      Serial.print("RIGHT");
      Serial.println(Right_Value);
      Serial.print("LEFT");
      Serial.println(Left_Value);
      
      Serial.print("Distance");
      Serial.println(distance);
      
      Serial.print("x ");
      Serial.println(x);
      Serial.println(" "); 

/*      rate = abs(distance - prev_distance);

      if (rate > 65 && (Right_Value == Left_Value)){
          dir(2);
          tone(buzzer, 1000); // Send 1KHz sound signal...
          delay(1000);        // ...for 1 sec
          noTone(buzzer);     // Stop sound...
          delay(500);        // ...for 1sec
          tone(buzzer, 1000); // Send 1KHz sound signal...
          delay(1000);        // ...for 1 sec
          noTone(buzzer);     // Stop sound...
          delay(500);        // ...for 1sec   
          tone(buzzer, 1000); // Send 1KHz sound signal...
          delay(1000);        // ...for 1 sec
          noTone(buzzer);     // Stop sound...
          break;
      }
*/

      
      if(distance>=13 && distance<=30){ //forward
        if(Right_Value == Left_Value){
          dir(3);
        }
    
        else if((Right_Value > Left_Value) && x == 0){
          dir(6);
        }
    
        else if((Right_Value < Left_Value) && x == 0){
          dir(7);
        }
      }
    
      
      else if((distance >= 5 && distance < 13) || (distance >30 && distance <= 50)) { //stop 
        if((Right_Value==0) && (Left_Value==1) && (x != 2)) { //right
          dir(5);
        }
  
        else if ((Right_Value==1)&&(Left_Value==0) && (x != 3)) { //left
          dir(4);
        }
        
        else if ((Right_Value==0)&&(Left_Value==1) && (x ==2)){
          dir(2);
        }
        
        else if ((Right_Value==1)&&(Left_Value==0) && (x ==3)){
          dir(2);
        }
        else{
         dir(2);
        }
      }
        
      else if(distance < 5) { //back
        if (digitalRead(limit) == HIGH){dir(2);}
        else {dir(1);}
      }
      else if(distance == 0){
        dir(2);
      }
      else if(distance >50){
        if((Right_Value==0) && (Left_Value==1) && (x == 0 || x != 2)) { //right
          dir(5);
        }
  
        else if ((Right_Value==1)&&(Left_Value==0) && (x == 0 || x != 3)) { //left
          dir(4);
        }
        
        else if ((Right_Value==1)&&(Left_Value==1) && (x ==2)){
          dir(3);
          delay(1400);
          dir(5);
          delay(1000);
        }
        
        else if ((Right_Value==1)&&(Left_Value==1) && (x ==3)){
          dir(3);
          delay(1400);
          dir(4);
          delay(1000);
        }
        else if ((Right_Value == 1 && Left_Value == 1) && (x==0)){
          dir(2);
          tone(buzzer, 1000); 
          delay(500);    
          noTone(buzzer); 
          delay(500);     
          tone(buzzer, 1000); 
          delay(500);       
          noTone(buzzer);     
          delay(500);        
          tone(buzzer, 1000); 
          delay(500);       
          noTone(buzzer);    
          break;
        }
        
      }
      
      
      /*else if(distance >60 && x != 0){
        if ((Right_Value==0) && (Left_Value==1) && x == 3) { //right
          y = 3;
          dir(3);
        }

        else if((Right_Value==1) && (Left_Value==0) && x == 2){ //left
          y = 2;
          dir(3);
        }
        else if (Right_Value == Left_Value && x == 0){
        dir(2);
        tone(buzzer, 1000); // Send 1KHz sound signal...
        delay(1000);        // ...for 1 sec
        noTone(buzzer);     // Stop sound...
        delay(500);        // ...for 1sec
        tone(buzzer, 1000); // Send 1KHz sound signal...
        delay(1000);        // ...for 1 sec
        noTone(buzzer);     // Stop sound...
        delay(500);        // ...for 1sec   
        tone(buzzer, 1000); // Send 1KHz sound signal...
        delay(1000);        // ...for 1 sec
        noTone(buzzer);     // Stop sound...
        break;
        }
      }
        else if(distance >60 && x == 0){
          if (y == 3){dir(5);delay(1000);y = 0;}
          else if (y == 2){dir(4);delay(1000);y = 0;}
          else{dir(2);}
        }
*/
      }
    }
 }

  
