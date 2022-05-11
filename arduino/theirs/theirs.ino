#include <Servo.h> // servo library
Servo servo;
const int trigPin = 5; // FIXME: Add const
const int echoPin = 6; // FIXME: Add const
const int servoPin = 7; // FIXME: Add const
// const int led = 10; // FIXME: Add const
long duration, dist, average; // FIXME: duration has to be unsigned long
long aver[3];   //array for average

void setup() {
    Serial.begin(9600);
    servo.attach(servoPin);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    servo.write(0);     //close cap on power on
    
    delay(100);
    servo.detach();

}

void loop() {
    for (int i = 0; i <= 2; i++) {
        // average distance
        measure();
        aver[i] = dist;
        delay(10); // delay between measurements
    }
    dist = (aver[0]+aver[1]+aver[2])/3;

    Serial.print("Distance: ");
    Serial.println(dist); // changed to println
    delay(2000);
}

void measure() {
    // digitalWrite(led, HIGH); // why is it high!!???
    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(15); //should be 10
    digitalWrite(trigPin, LOW);
    pinMode(echoPin, INPUT);
    duration = pulseIn(echoPin, HIGH);
    
    // FIXME: wrong distance calculation
    // dist = (duration/2) /29.1; // obtain distance // FIXME: in w/c units?
    dist = 0.017 * duration;

}
