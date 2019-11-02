#include "servo.h"

#include "Particle.h"

#include "common.h"

Servo servo;

void servoSetup() {
    servo.attach(SERVO_PIN); // Servo control pin
}

void rotateServo() {
    servo.write(85); // Slow rotation clockwise
}

void stopServo() {
    servo.write(90); // Stop servo
}
