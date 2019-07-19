/*
This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
It won't work with v1.x motor shields! Only for the v2's with built in PWM
control

For use with the Adafruit Motor Shield v2 
---->  http://www.adafruit.com/products/1438
*/
/*

 * timer_blink

 *

 * Blinks the built-in LED every second using the arduino-timer library.

 *

 */



#include <timer.h>


#include <Wire.h>
#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *myMotor = AFMS.getMotor(1);
// You can also make another motor on port M2
Adafruit_DCMotor *myOtherMotor = AFMS.getMotor(2);

const int trigPin=9;
const int echoPin=10 ;


long duration;
int distance;


unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 535;  //the value is a number of milliseconds, 
int Pos = 0;
int state = 1;

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("BSE Floor Cleaning Robot");

  AFMS.begin(1600);  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  
  // Set the speed to start, from 0 (off) to 255 (max speed)
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

}

void loop() {
myMotor->setSpeed(138);
myOtherMotor->setSpeed(123);
  
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW); 
  
  duration = pulseIn(echoPin,HIGH);
  distance = (duration*0.034)/2;

 Serial.print("Distance: ");
 Serial.println(distance);
 
 if (state == 1) {
    myMotor->run(FORWARD);                             
    myOtherMotor->run(FORWARD);
     if (distance < 17) {
      if (distance >0) {
      if (Pos == 0) {
        state = 2;
       } else if (Pos == 1) {
        state = 3;
       }
      }
     } else if (distance >= 17) {
       if (Pos == 0 || Pos == 1) {
       state = 1;
       }
     }
 } else if (state == 2) {
   if (Pos == 0) {
  currentMillis = millis();     //Get the time for current Millis
  startMillis = millis();
     while (startMillis - currentMillis < period) {
     myOtherMotor->setSpeed(255);     
     myOtherMotor->run(BACKWARD);     //Spin right motor back (to turn the robot right)
     myMotor->run(FORWARD);           //Spin left motor forward (to turn the robot right)
     startMillis = millis();          //initial start time
     }
  myOtherMotor->setSpeed(123);
  myMotor->(FORWARD);           //make the robot move forward because it has already turned
  myOtherMotor->run(FORWARD);
  }
  if (distance < 17) {
    state = 2;            //Set the state so that next time, the robot will turn LEFT
   } else {
    state = 1;           //Make the robot go forward if it doesnt sense anything 
   }
 Pos = 1;              //Making sure that next time, the robot will turn LEFT
 } else if(state == 3) {      //This is the 2nd state, where the robot will turn to the left
    if (Pos == 1) {
   currentMillis = millis();     //Get the time for current Millis
   startMillis = millis();
     while (startMillis - currentMillis < period) {
     myMotor->setSpeed(255);     
     myOtherMotor->run(FORWARD);     //Spin right motor back (to turn the robot right)
     myMotor->run(BACKWARD);           //Spin left motor forward (to turn the robot right)
     startMillis = millis();          //initial start time
     }
 myMotor->setSpeed(138);
 myMotor->run(FORWARD);
 myOtherMotor->run(FORWARD);
 }
 if (distance < 17) {
     state = 3;            //Set the state so that next time, the robot will turn LEFT
   } else {
     state = 1;           //Make the robot go forward if it doesnt sense anything 
   }
Pos = 0;              //Making sure that next time, the robot will turn LEFT
}
Serial.println(state);
Serial.println(Pos);
}
