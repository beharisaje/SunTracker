#include <Wire.h>

/* 
  IMPORTANT (prior to running):
  Align lines of elevation (as pictured in ReadMe)
  Setup up azimuth direction properly (in ReadMe)
*/


/**
 * Simplified A4988 Stepper Motor Control - 360° Rotation Both Ways
 * Motor specs: 17HS19-1684S-PG27 Nema 17 with 26.85:1 gearbox
 * Step angle: 0.067° per step (5373 steps per revolution)
 */


// Pin definitions
const int enPin = 12;      // Enable pin

const int ms1Pin = 11;     // Microstepping control 1
const int ms2Pin = 10;     // Microstepping control 2
const int ms3Pin = 9;      // Microstepping control 3
const int rstPin = 8;      // Reset pin
const int slpPin = 7;      // Sleep pin
const int stepPin = 6;     // Step pin
const int dirPin = 5;      // Direction pin

// Motor parameters for motor :: 0.067° per step = 5373 steps per revolution
const int stepsPerRevolution = 11290; // however, our system performs different because of gear ratios
const int stepDelaySimultaion = 1500;  // Microseconds between steps in simulation mode (speed control)



// function used instead of delay, that allows motor no strain driver during long delays
void sleep(int ms) {
  digitalWrite(slpPin, LOW);  // Enter sleep mode — chip shuts down
  delay(ms);
  digitalWrite(slpPin, HIGH); // Wake up
  delay(1);                   
}



void setup() {
  Serial.begin(9600);
  
  // Configure pins as outputs
  pinMode(enPin, OUTPUT);
  pinMode(ms1Pin, OUTPUT);
  pinMode(ms2Pin, OUTPUT);
  pinMode(ms3Pin, OUTPUT);
  pinMode(rstPin, OUTPUT);
  pinMode(slpPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  
  // Initialize the driver
  digitalWrite(enPin, LOW);    // Enable the driver (active low)
  digitalWrite(rstPin, HIGH);  // Not in reset mode
  digitalWrite(slpPin, HIGH);  // Not in sleep mode
  
  // Set Full step mode for more torque
  digitalWrite(ms1Pin, LOW);
  digitalWrite(ms2Pin, LOW);
  digitalWrite(ms3Pin, LOW);
      
}





void loop() {
  Serial.println("Motor sleeping. Enter 1 (Simulation Mode), 2 (Azimuth Rotation), or 3 (Elevation Rotation):");
  delay(10);
  // Put motor driver to sleep while waiting
  digitalWrite(slpPin, LOW);
  

  // Wait for user input
  while (Serial.available() == 0) {
    delay(10);
  }

  // wake the driver from sleep
  digitalWrite(slpPin, HIGH);  
  delay(1);  // Small delay to let the driver fully awake

  int option = Serial.parseInt();
  Serial.read();  

  if (option == 1) {
    daySimulation();
  }
  else if (option == 2) {
    azimuthCycle();
  }
  else if (option == 3) {
    elevationCycle();
  }
  else {
    Serial.println("Invalid option. Please enter 1, 2, or 3.");
  }
}

void azimuthCycle() {
    rotateMotor(11600, 0); //azimuth rotation
    sleep(30000);
}

void elevationCycle(){
    rotateMotor(11550, 1); //elevation rotation
    sleep(30000);
}


void daySimulation() {
  /////////////////////////////////////////
  ///////////Rotate Halfway up and 2 clicks azimuth /////////////
  rotateMotor(1250, 1);  
  delay(1000); // Wait 1 second

  rotateMotor(540, 0); 
  delay(1000); 

  rotateMotor(390, 0);  
  delay(1000);  

  Serial.println("Elevated Halfway | Rotated 24.8 degrees azimuth");


  ///////////////////////////////////////////
  ////////// Rotate rest of way up and 3 clicks azimuth///////////
  rotateMotor(1750, 1);  
  delay(1000);  

  rotateMotor(540, 0);  
  delay(1000);  
  rotateMotor(390, 0);  
  delay(1000);  
  rotateMotor(390, 0);  
  delay(1000);  

  Serial.println("Elevated Fully Up | Rotated 37.2 degrees azimuth");

  ///////////////////////////////////////////
  ////////// Cycle through top delay, and rotate 2 clicks azimuth///////////
  rotateMotor(1800, 1);  // 0 for clockwise
  delay(1000);  // Wait 1 second


  rotateMotor(540, 0);  // 0 for clockwise
  delay(1000);  // Wait 1 second

  rotateMotor(390, 0);  // 0 for clockwise
  delay(1000);  // Wait 1 second

  Serial.println("Cycle Through Elevation Delay | Rotated 24.8 degrees azimuth");

  ///////////////////////////////////////////
  ////////// Rotate halfway down and rotate  2 clicks ///////////
  rotateMotor(1500, 1);  // 0 for clockwise
  delay(1000);  // Wait 1 second

  rotateMotor(540, 0);  // 0 for clockwise
  delay(1000);  // Wait 1 second

  rotateMotor(390, 0);  // 0 for clockwise
  delay(1000);  // Wait 1 second


  Serial.println("Elevated Halfway Down | Rotated 24.8 degrees azimuth");

  ///////////////////////////////////////////
  ////////// Rotate rest of way down and 2 clicks ///////////
  rotateMotor(1500, 1);  // 0 for clockwise
  delay(1000);  // Wait 1 second

  rotateMotor(540, 0);  // 0 for clockwise
  delay(1000);  // Wait 1 second

  rotateMotor(390, 0);  // 0 for clockwise

  delay(1000);  // Wait 1 second

  Serial.println("Elevated Rest of Way Down | Rotated 24.8 degrees azimuth");

  ///////////////////////////////////////////
  ////////// Rotate through bottom delay and 1 click ///////////
  rotateMotor(4600, 1);  // 0 for clockwise

  delay(1000);  // Wait 1 second

  rotateMotor(540, 0);  // 0 for clockwise

  delay(1000);  // Wait 1 second

  Serial.println("Cycle Through Bottom Elevation Delay | Rotated 12.4 degrees azimuth");


  ///////////////////////////////////////////
  ////////// Rotate around to starting position ///////////
  rotateMotor(6610, 0);  // 0 for clockwise
  Serial.println("Tracker Returned to Starting Position");

  // 30 second sleep
  sleep(30000);  // Wait 1 second



}

// rotates motor depending on step input and direction input
void rotateMotor(int steps, int DIR) {
  // Set direction
  if(DIR == 1){
    digitalWrite(dirPin, HIGH);  // Counterclockwise (elevation)
  }
  else{
    digitalWrite(dirPin, LOW);   // Clockwise (azimuth)
  }

  // Execute steps
  for(int i = 0; i < steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(stepDelaySimultaion);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(stepDelaySimultaion);
  }
}
