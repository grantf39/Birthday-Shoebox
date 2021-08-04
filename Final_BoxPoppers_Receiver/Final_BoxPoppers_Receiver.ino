#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <Servo.h>

RF24 radio(9,10);

const byte address[6] = "00001";

Servo servo1;
Servo servo2;
Servo servo3;

int servoMax = 90;
int lidMax = 110;
int prevArr[5] = {0,0,0,0,0};

int radioArray[5] = {0,0,0,0,0};

void setup() {
  Serial.begin(115200);
  radio.begin(); // Checks if chip is connected to SPI bus
  radio.openReadingPipe(0,address); // Methods you can use to drive the chip in more advanced ways
  radio.setPALevel(RF24_PA_LOW); // Fetches the current @ref PALevel.
  radio.startListening(); // Stop listening for incoming messages, and switch to transmit mode.

  servo1.attach(8);
  servo1.write(0);
  servo2.attach(7);
  servo2.write(0);
  servo3.attach(6);
  servo3.write(0);

}

void loop() {
  //1st and 2nd signals are manual trigger buttons
  //3rd and 4th are auto buttons
  //5th is the lid switch.
  //int radioArray[5] = {0,0,0,0,0};

  //To stabilize this:
//  int radioArray[5] = prevArr;

  //Reset the auto buttons
  radioArray[2] = 0;
  radioArray[3]=0;

  delay(70);

  if(radio.available()){
    radio.read(&radioArray, sizeof(radioArray));
  }

  for(int i = 0; i < (sizeof(radioArray)/sizeof(int)); i++){
    Serial.print(radioArray[i]);
    Serial.print(" ");
  }
//    if (radioArray[0] == 1) {
//      servo1.write(servoMax);
//    } else{
//      servo1.write(0);
//    }
//    
//    if (radioArray[1] == 1){
//      servo2.write(servoMax);
//    }else{
//      servo2.write(0);
//    }
//    
//    if (radioArray[2] == 1){
//      servo3.write(lidMax);
//    } else {
//      servo3.write(0);
//    }
//  

  if(radioArray[4]==1){
    servo3.write(lidMax);//90 for sg90 test. in actual bot we use 110
    //delay(300); //this is an estimate to allow servo to complete turning
  }else{
    servo3.write(0);
  }


  //MANUAL BUTTONS
  if(radioArray[4]==1){
    if(radioArray[0]==1){
      servo1.write(servoMax);
    }else{
      servo1.write(0);
    }

    if(radioArray[1]==1){
      servo2.write(servoMax);
    }else{
      servo2.write(0);
    } 
  }else{
    servo1.write(0);
    servo2.write(0);
  }



  //5 AUTOS

 if(radioArray[4]==0){
  //if Lid is closed then we may begin auto firing sequence

  
  if(radioArray[2]==1){//if first auto button pressed begin sequence
    servo3.write(lidMax);//open lin
    delay(500);
    servo1.write(servoMax);//fire trigger1
    delay(500);
    servo1.write(0);//untrigger trigger1
    //delay(500);
    servo3.write(0);//close lid
    delay(500);
  }

  if(radioArray[3]==1){//if second auto button pressed begin sequence
    servo3.write(lidMax);//open lin
    delay(500);
    servo2.write(servoMax);//fire trigger2
    delay(500);
    servo2.write(0);//untrigger trigger2
    //delay(500);
    servo3.write(0);//close lid
    delay(500);
  }
  
 }
  
  Serial.println("");

  //Re-init in-case of fuck up
  radio.begin(); // Checks if chip is connected to SPI bus
  radio.openReadingPipe(0,address); // Methods you can use to drive the chip in more advanced ways
  radio.setPALevel(RF24_PA_HIGH); // Fetches the current @ref PALevel.
  radio.startListening(); // Stop listening for incoming messages, and switch to transmit mode.

}
