#include <Evo.h>
#include "arm.h"

EVOX1 evo;

// ===== TUNING =====
const int STEER_SPEED         = 6000;
const int FAST_SPEED          = 4000;

const int SIDE_WALL_THRESHOLD = 150;    // mm — trigger steer-away when a side wall gets this close, TODO: tune
const int STEER_AWAY_ANGLE    = 30;     // degrees to steer away from a close side wall, TODO: tune

const int HEADING_KP          = 4;      // steering degrees per degree of heading drift when going straight
const int MAX_STRAIGHT_CORRECTION = 15; // cap on straight-line IMU correction

const int CORNERS_PER_LAP     = 4;
const int TOTAL_LAPS          = 3;
const int TOTAL_CORNERS       = CORNERS_PER_LAP * TOTAL_LAPS;

const float CORNER_TURN_THRESHOLD = 70.0;  // degrees of heading change that counts as "a corner happened", TODO: tune
const unsigned long CORNER_COOLDOWN_MS = 800; // min time between corner counts
// ==================

int cornersCompleted = 0;
float lastCornerHeading = 0;
float straightHeadingRef = 0;
unsigned long lastCornerTime = 0;

void displayValues(){
  evo.clearDisplay();
  evo.writeLineToDisplay(imuu.getRelativeHeading(), 0);
  evo.writeLineToDisplay(steering.getAngle(), 1);
  evo.drawDisplay();
}

void straight(){
  steering.runTarget(3000, -90, MotorStop::HOLD, false);
  delay(1000);
  steering.runTarget(3000, 90, MotorStop::HOLD, false);
  delay(1000);
  steering.runTarget(3000, 0, MotorStop::HOLD, false);
}

void setup() {
  Serial.begin(115200);
  delay(500);
  pinMode(GPIO1, INPUT_PULLDOWN);

  Serial.println("=== BOOT START ===");

  evo.begin();
  hl.begin();
  hl.setMode(ALGORITHM_COLOR_RECOGNITION);
  steering.begin();
  drive.begin();
  tofFront.begin();
  tofLeft.begin();
  tofRight.begin();
  steering.coast();
  drive.brake();

  Serial.println("Calling imuu.begin()... make sure the bot is NOT being touched/moved right now.");
  imuu.begin();
  Serial.print("Heading immediately after imuu.begin(): ");
  Serial.println(imuu.getRelativeHeading());

  steering.resetAngle();

  Serial.println("Waiting for Programmable Button press...");
  unsigned long lastPrint = 0;
  while (evo.getButton() != PRESSED){
    displayValues();
    steering.runTarget(3000, imuu.getRelativeHeading()*4, MotorStop::HOLD, false);

    if (millis() - lastPrint > 1000) {
      Serial.print("Heading while waiting: ");
      Serial.println(imuu.getRelativeHeading());
      lastPrint = millis();
    }
  }

  Serial.println("Button pressed! Starting calibration wiggle...");
  evo.playTone(400,300);

  straight();
  Serial.println("Wiggle done, entering main loop.");
  Serial.print("Heading at start of loop: ");
  Serial.println(imuu.getRelativeHeading());

  lastCornerHeading = imuu.getRelativeHeading();
  straightHeadingRef = imuu.getRelativeHeading();
}

void loop() {
  if (cornersCompleted >= TOTAL_CORNERS) {
    drive.brake();
    steering.runTarget(STEER_SPEED, 0, MotorStop::BRAKE, true);
    evo.writeToDisplay("Done! 3 laps", 0, 0, true, true);
    return;
  }

  drive.run(FAST_SPEED);

  int left  = tofLeft.getDistance();
  int right = tofRight.getDistance();
  float heading = imuu.getRelativeHeading();

  float headingError = heading - straightHeadingRef;

  int steerAngle;
  if (left < SIDE_WALL_THRESHOLD) {
    steerAngle = STEER_AWAY_ANGLE;
  } else if (right < SIDE_WALL_THRESHOLD) {
    steerAngle = -STEER_AWAY_ANGLE;
  } else {
    steerAngle = constrain((int)(-headingError * HEADING_KP), -MAX_STRAIGHT_CORRECTION, MAX_STRAIGHT_CORRECTION);
  }

  steering.runTarget(STEER_SPEED, steerAngle, MotorStop::HOLD, false);

  static unsigned long lastDebugPrint = 0;
  if (millis() - lastDebugPrint > 200) {
    Serial.print("L:"); Serial.print(left);
    Serial.print(" R:"); Serial.print(right);
    Serial.print(" heading:"); Serial.print(heading);
    Serial.print(" headingError:"); Serial.print(headingError);
    Serial.print(" steerAngle:"); Serial.println(steerAngle);
    lastDebugPrint = millis();
  }

  if (fabs(heading - lastCornerHeading) >= CORNER_TURN_THRESHOLD &&
      (millis() - lastCornerTime > CORNER_COOLDOWN_MS)) {
    cornersCompleted++;
    lastCornerHeading = heading;
    lastCornerTime = millis();
    straightHeadingRef = heading;
    Serial.print("Corner ");
    Serial.print(cornersCompleted);
    Serial.print(" / ");
    Serial.println(TOTAL_CORNERS);
  }
}
