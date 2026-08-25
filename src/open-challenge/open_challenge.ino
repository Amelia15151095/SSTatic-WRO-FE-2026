EVOX1 evo;
EvoVL53L0X *tofLeft;
EvoVL53L0X *tofRight;
EvoMotor *steering;

#ifndef ARM_H
#define ARM_H
#include <Evo.h>
EvoMotor steering(M3, EV3MediumMotor, true);
EvoMotor drive(M4, EV3MediumMotor, true);
EvoHuskyLens hl(I2C1);
HUSKYLENSResult result;
void waitForBump(){
  while (!digitalRead(GPIO1)){}
  while (digitalRead(GPIO1)){}
}
#endif

const int TRIGGER_DIST = 200;
const int TURN_ANGLE   = 50;   // degrees to turn for Left/Right
const int STEER_SPEED  = 300;  // tweak to taste

String lastMessage = "";
String lastDirection = ""; // tracks last commanded steering state

void displayState(const char* message) {
  if (lastMessage != message) {
    lastMessage = message;
    evo.clearDisplay();
    evo.writeToDisplay(message, 0, 0, true, true);
  }
}

void steerTo(const char* direction) {
  if (lastDirection == direction) return; // already there, don't re-command
  lastDirection = direction;

  if (strcmp(direction, "Left") == 0) {
    steering->runTarget(STEER_SPEED, -TURN_ANGLE, MotorStop::HOLD, false);
  } else if (strcmp(direction, "Right") == 0) {
    steering->runTarget(STEER_SPEED, TURN_ANGLE, MotorStop::HOLD, false);
  } else {
    // "Both" or "Neither" -> center
    steering->runTarget(STEER_SPEED, 0, MotorStop::HOLD, false);
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);
  evo.begin();
  delay(100);

  // Mapped Ports: Left=I2C8, Right=I2C7
  tofLeft  = new EvoVL53L0X(I2C8);
  tofRight = new EvoVL53L0X(I2C7);
  tofLeft->begin();
  tofRight->begin();

  steering = new EvoMotor(M3); // steering motor port
  steering->begin();
  steering->resetAngle();      // treat current position as 0

  displayState("Ready!");
  delay(1000);
}

void loop() {
  int left = tofLeft->getDistance();
  int right = tofRight->getDistance();

  // Serial debug print
  Serial.print("Left: "); Serial.print(left);
  Serial.print(" mm | Right: "); Serial.print(right);
  Serial.println(" mm");

  bool leftOpen = (left > TRIGGER_DIST);
  bool rightOpen = (right > TRIGGER_DIST);

  if (leftOpen && rightOpen) {
    displayState("Both");
    steerTo("Both");
  } else if (leftOpen) {
    displayState("Left");
    steerTo("Left");
  } else if (rightOpen) {
    displayState("Right");
    steerTo("Right");
  } else {
    displayState("Neither");
    steerTo("Neither");
  }

  delay(50);
}
