/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>
int tempPin = 0;
float tempCelsius =0;
float tempVolts;
float tempKelvin;
int pos;

Servo myservo;  // create servo object to control a servo

void setup() {
  myservo.attach(9); 
  Serial.begin(9600); // attaches the servo on pin 9 to the servo object
}

void loop() {
  // goes from 0 degrees to 180 degrees
    // in steps of 1 degree

      if( team a score > team b's){
        myservo.write(0);
        delay(1000);
        myservo.write(180);
        delay(1000);
        myservo.write(0);
        delay(1000);
        myservo.write(180);
        delay(1000);
        myservo.write(0);
        delay(1000);
        myservo.write(180);
        delay(1000);
        myservo.write(45);
      }
          
      

// tell servo to go to position in variable 'pos'
                             // waits 15ms for the servo to reach the position
  

}