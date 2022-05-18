#include <Servo.h> // servo library
#include <SoftwareSerial.h>

int GREEN = 12;
int RED = 7;

// --- Ultrasonic pins test

// person
const int trig1 = 5;
const int echo1 = 6; 

// top of bin
const int trig2 = 3;
const int echo2 = 4; 
// ---

Servo servo;
const int servoPin = 9;
#define SERVO_PIN 6
#define SERVO_CLOSED_POS 180 // horizontally closed to left
#define SERVO_OPEN_POS 80 // vertically open tilted to right

unsigned long dist1, average1;
unsigned long dist2, average2;
long aver[3];   //array for average
int FULL = 1; 
int PERSON_FAR = 1;
int SENT = 0;

void setup() {
    Serial.begin(9600);
    servo.attach(servoPin);
    pinMode(trig1, OUTPUT);
    pinMode(echo1, INPUT);
    pinMode(trig2, OUTPUT);
    pinMode(echo2, INPUT);
    servo.attach(SERVO_PIN);
    servo.write(0);     //close cap on power on
    
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);
    delay(100);

}

void loop() {
  // if person is near
  get_average_dist(trig1, echo1, dist1);
  get_average_dist(trig2, echo2, dist2);

  PERSON_FAR = check_if_far(dist1);
  FULL = check_if_full(dist2);
  if (!PERSON_FAR) {
    PERSON_FAR = false;
    if (!FULL) {
      light_green();
      openBin();
      if (PERSON_FAR) {
        closeBin();
      }
      delay(3000);
    } else {
      light_red();
      if (!SENT) {
        send_notification();
      }
      delay(1000);
    }
  } else {
    delay(3000);
  }
  delay(2000);



}

void measure(int trig, int echo, unsigned long dist) {
    // digitalWrite(led, HIGH); // why is it high!!???
    digitalWrite(trig, LOW);
    delayMicroseconds(5);
    digitalWrite(trig, HIGH);
    delayMicroseconds(15); //should be 10
    digitalWrite(trig, LOW);
    pinMode(echo, INPUT);
    unsigned long duration = pulseIn(echo, HIGH);
    
    // FIXME: wrong distance calculation
    // dist = (duration/2) /29.1; // obtain distance // FIXME: in w/c units?
    dist = 0.017 * duration;

}

void get_average_dist(int trig, int echo, unsigned long dist) {
    for (int i = 0; i <= 2; i++) {
        // average distance
        measure(trig, echo, dist);
        aver[i] = dist;
        delay(10); // delay between measurements
    }
    dist = (aver[0]+aver[1]+aver[2])/3;
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

int check_if_far(unsigned long dist) {
  return dist > 10;
}

int check_if_full(unsigned long dist) {
  return dist < 1;
}

void light_red() {
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, LOW);
}

void light_green() {
  digitalWrite(GREEN, HIGH);
  digitalWrite(RED, LOW);
}

void send_notification() {
  delay(10);
}

