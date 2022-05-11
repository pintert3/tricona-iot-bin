// Check if the bin is full
// if full, light RED and send notification
// if not full, check if bin is half
// if half, light ORANGE and send notification
// if not half, continue
//
// wait till someone is standing at the bin, 
// open, and wait till he's gone.
// if the person is gone: close.
// LOOP

#include <Servo.h>

// int GREEN = 7;
// int RED = 6;

// --- Ultrasonic pins
// U1:
const int trig1 = 13;
const int echo1 = 12;
// U2:
const int trig2 = 9;
const int echo2 = 8;
// ---

// --- Servo Motor
#define SERVO_PIN 6
#define SERVO_CLOSED_POS 180 // horizontally closed to left
#define SERVO_OPEN_POS 80 // vertically open tilted to right
Servo servo;
// ---


void setup() {

  pinMode(trig1, OUTPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(echo2, INPUT);

  Serial.begin(9600);
  Serial.println("---Testing Distance---");
  
  // Servo setup
  servo.attach(SERVO_PIN);
  servo.write(SERVO_CLOSED_POS); // rest position
}

void loop() {
  // float current_distance1;
  // float current_distance2;
  int current_distance1;
  int current_distance2;
  current_distance1 = ultrasonicDistance(trig1, echo1);
  current_distance2 = ultrasonicDistance(trig2, echo2);
  printDistance(current_distance1);
  printDistance(current_distance2);

  delay(1000);
  // Test Servo open and close
  // openBin();
  // delay(3000);
  // closeBin();
  // delay(3000);
}

float ultrasonicDistance(int trig, int echo) {
  digitalWrite(trig,LOW);
  delayMicroseconds(5);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  unsigned long duration = pulseIn(echo, HIGH);
  Serial.print("Duration obtained: "); // DEBUG
  Serial.println(duration); // DEBUG
  int distanceMeasured = 0.017 * duration;
  return distanceMeasured;

}

void printDistance(float distance){
  Serial.print("Distance : ");
  Serial.print(distance);
  Serial.println(" cm");
}

void openBin() {
  for(int angle = SERVO_CLOSED_POS; angle > SERVO_OPEN_POS; angle--) {
    servo.write(angle);
    delay(10);
  }
}

void closeBin() {
  for(int angle = SERVO_OPEN_POS; angle < SERVO_CLOSED_POS; angle++) {
    servo.write(angle);
    delay(10);
  }
}
