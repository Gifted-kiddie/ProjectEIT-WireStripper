// Libraries import
#include <Stepper.h>
#include <Servo.h>

// Object instanciation
Stepper feedingStepper(200,3,4,5,6); // Feeding stepper is connected to pin 3,4,5,6
Stepper cuttingStepper(200,8,9,10,11); // Cutting stepper is connected to pin 8,9,10,11
Servo guideServo;

void setup() {
  cuttingStepper.setSpeed(30); // Set the speed of the feeding motor
  feedingStepper.setSpeed(30); // Set the speed of feeding motor
  // Pinout setup for the feeding stepper. This will allow to stop current going in the motor while he's in idle state
  // as stepper library don't allow it
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  // Pinout setup for the cutting stepper for the same reason as the feeding stepper
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  // Pinout of the servo used to chose the position of the cable in the stripper
  guideServo.attach(7);
  // Allow to print information
  Serial.begin(9600);
}

void loop(){
  // Variable instanciation
  // !!! USER SHOULD ONLY CHANGE THESE PARAMETERS !!! 
  int N = 5; // Number of cables needed
  float L = 0.1; // Length of the cables needed [m]

  // Constant instanciation
  int strippingAngle = 10;
  int cuttingAngle = 2;
  
  Serial.print("code started");
  Serial.print("\n");
  int stepNeeded = (int) lengthToRev(L); // Compute the needed step from the wanted length
  for(int i=1;i<=N+1;i++){ // Loop N times to have N cables
    Serial.print("Cable ");
    Serial.print(i);
    Serial.print("\n");
    feed(30); // Call to the feed function to move the cable of 30 steps
    guideServo.write(strippingAngle); // Place the cable in the stripping part
    delay(500); // Delay to ensure cable is well placed
    cut(); // Call to the cut function to move the stripper
    feed(stepNeeded - 60); // Move to the other stripping place
    cut(); // Call to the cut function to move the stripper
    feed(30); // Move the cable to the end
    guideServo.write(2); // Move the cable to the cutting part
    delay(500); // Delay to ensure the cable is well placed
    cut(); // Cut the cable
  }
  exit(0); // Exit the loop when all the cables are cut
}

void feed(int val){ // Feeding function which move the cable of a given value
  feedingStepper.step(val); // Move the stepper
  delay(250); // Delay to ensure no error
  // set all motor pinout LOW to stop current from flowing when idle
  // to ensure the other motor can draw the 2A of the power supply
  // and ensure no overheating
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  delay(500);
  }

void cut(){ // Cutting function move the stripper up and down
  // Reversing value is lower as cutting the current in the motor
  // make it move back with the tension
  cuttingStepper.step(100); // tighten the stripper
  delay(250); // delay to ensure no error
  // set all motor pinout LOW to stop current from flowing when idle
  // to ensure the other motor can draw the 2A of the power supply
  // and ensure no overheating
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  delay(500); // Delay to ensure no error
  
  cuttingStepper.step(-40); // Loosen the stripper
  delay(250); // delay to ensure no error
  // set all motor pinout LOW to stop current from flowing when idle
  // to ensure the other motor can draw the 2A of the power supply
  // and ensure no overheating
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  delay(500); // delay to ensure no error
  }


float lengthToRev(float l){ // Function that transform the length needed in [m] to the number of step needed
  // 1 step = 1.8°
  float R = 0.0055; // Radius of the feeding wheel [m]
  float correspL = 2*PI*R; // Corresponding length for one full revolution
  float a = l/correspL; // Ratio of the lentgth to make vs length of 1 revolution
  return a*200; // Number of step needed to do the desired length
}
