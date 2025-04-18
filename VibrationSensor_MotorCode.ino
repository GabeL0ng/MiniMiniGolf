#include <Servo.h>

// Motor driver pins
const int AIN1 = 13;
const int AIN2 = 12;
const int PWMA = 11;

// Vibration sensor and servo setup
const int vibPin = 7;
const int servoPin = 6;

Servo myServo;

// Timers
unsigned long previousMotorTime = 0;
const unsigned long motorInterval = 300; // 15 seconds
bool motorOn = false;

unsigned long vibrationTime = 0;
const unsigned long servoHoldTime = 10000; // Servo holds closed for 10 seconds
bool servoMoved = false;

void setup() {
  // Setup motor pins
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);

  // Setup vibration pin
  pinMode(vibPin, INPUT_PULLUP);

  // Setup servo
  myServo.attach(servoPin);
  myServo.write(0); // Start open

  Serial.begin(9600);
}

void loop() {
  unsigned long currentTime = millis();

  // --- Motor loop: toggle every 15 seconds ---
  if (currentTime - previousMotorTime >= motorInterval) {
    previousMotorTime = currentTime;
    motorOn = !motorOn;

    if (motorOn) {
      turnMotorOn();
    } else {
      turnMotorOff();
    }
  }

  // --- Vibration sensor and servo logic ---
  int vibState = digitalRead(vibPin);

  if (vibState == LOW && !servoMoved) {
    Serial.println("Vibration detected!");
    doorClose();
    vibrationTime = currentTime;
    servoMoved = true;
  }

  if (servoMoved && currentTime - vibrationTime > servoHoldTime) {
    doorOpen();
    servoMoved = false;
  }
}

// --- Motor control ---
void turnMotorOn() {
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, 75);
  Serial.println("Motor ON");
}

void turnMotorOff() {
  analogWrite(PWMA, 0);
  Serial.println("Motor OFF");
}

// --- Servo control ---
void doorOpen() {
  myServo.write(0);
  Serial.println("Door opened");
}

void doorClose() {
  myServo.write(90);
  Serial.println("Door closed");
}

