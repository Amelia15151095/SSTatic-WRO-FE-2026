# SSTatic-WRO-FE-2026


Project Overview
Our autonomous vehicle was developed as an integrated mechanical, electrical, sensing and software system using an ESP32-S3-based EVOX1 controller. It began with a relatively simple vehicle architecture based on rear-wheel drive, Ackermann steering and colour recognition through a HuskyLens camera. As development progressed, testing exposed several mechanical, electrical and software limitations. Approximately one and a half months before the competition, the team decided to completely redesign the robot rather than continue modifying the original platform.
The redesigned vehicle was made smaller and faster; the steering mechanism was strengthened with a gear-based system instead of the original axle-based arrangement; the drive system was changed to differential drive; and a gyro/IMU was added to provide direct feedback on the vehicle's heading. The sensor architecture was also expanded to include three Time-of-Flight distance sensors.
The Challenge 1 software reflects the redesigned vehicle. The ESP32-S3/EVOX1 controller acts as the central control system. It communicates with an IMU via I2C5, a front ToF sensor via I2C6, a left ToF sensor via I2C7, a right ToF sensor via I2C8, and a HuskyLens via I2C1. The controller also operates the steering and drive motors through the EVO motor interface.
The Challenge 1 algorithm is primarily based on two forms of feedback. First, the IMU measures the vehicle's relative heading, allowing the software to detect drift and make proportional steering corrections. Second, the left and right ToF sensors measure the vehicle's distance from nearby walls. If a wall comes too close, the robot temporarily prioritises wall avoidance over its normal heading correction.
The software also includes a course-progress mechanism. The course is treated as four corners per lap, with three laps required, giving a total of twelve corners. A corner is detected when the vehicle's heading changes by at least a configured threshold of 70 degrees. A cooldown prevents the same physical corner from being counted multiple times.
The overall project demonstrates an iterative engineering process. The final vehicle was not the result of a single design being implemented exactly as originally planned. Instead, mechanical testing, electronic testing, software debugging, and autonomous driving experiments influenced later decisions. Problems discovered in the original vehicle, including steering-mechanism weakness, unreliable motor-port behaviour, motor compatibility issues, software-library problems and difficulties with the development environment, helped motivate the redesign.
The sections below describe the vehicle according to the five main judging criteria: Mobility & Mechanical Design; Power & Sensor Architecture; Software Architecture & Obstacle Strategy; Systems Thinking & Engineering Decisions; and Reproducibility.
1. Mobility & Mechanical Design
1.1 Initial mechanical architecture
At the beginning of the project, the vehicle used a conventional rear-wheel-drive architecture with Ackermann steering. The purpose of the original design was to create a small vehicle that behaved similarly to a conventional car: the rear wheels provided propulsion while a steering mechanism changed the direction of the front wheels.
The drive system used an EV3 Large Motor during the early development stage, while an EV3 Medium Motor was used for steering. The steering motor was connected to a mechanical Ackermann steering mechanism. This meant that steering could not be treated as simply turning two independent drive wheels in opposite directions. The software had to control an actual steering mechanism with physical limits.
This initial design was useful for establishing the fundamental relationship between the controller and the vehicle. The team learned how to initialise the motors, communicate with the EVOX1 controller, control steering, and combine motor commands with sensor information.
However, testing quickly demonstrated that the original mechanical arrangement had limitations.
The steering mechanism could not safely tolerate unrestricted motor movement. During testing, the steering motor was allowed to operate continuously at high power and the mechanism reached its physical limit. This placed excessive stress on the mechanism and temporarily damaged it. The incident demonstrated that the software and mechanical design had to be considered together.
A motor command that appears harmless from a software perspective can be dangerous when it is connected to a mechanical system with a limited range of movement.
The team therefore changed the steering strategy to use short controlled commands rather than allowing the motor to run indefinitely against its limits.
The original prototype also experienced problems with motor ports. The steering motor was tested on several EVOX1 ports because some ports appeared weak or unreliable. The team had to move the steering motor between ports while also considering physical cable length and the position of the other motor.
These experiences influenced the later decision to redesign the entire vehicle.
1.2 Complete redesign before competition
Approximately one and a half months before the competition, the team decided to completely redo the robot.
This was one of the largest engineering decisions in the project.
Instead of continuing to repair and modify the original vehicle, the team created a new platform that was smaller and faster. The steering system was strengthened using gears instead of the original axle-based mechanism, the drive system was changed to differential drive, and a gyro/IMU was added.
The decision involved a significant tradeoff. Rebuilding the robot so close to competition reduced the amount of time available for testing the final configuration. However, continuing to use the original architecture would have meant carrying known mechanical and control limitations into the competition.
The team therefore chose to accept the short-term development risk in exchange for a potentially more capable and reliable final platform.
The redesign also changed the control problem.
The original vehicle's Ackermann steering meant that the steering motor physically changed the angle of the vehicle's wheels. The redesigned differential-drive architecture instead allows vehicle direction to be influenced by the relative motion of the drive system.
The software therefore had to be adapted to the new mechanical system.
The smaller size also changed the vehicle's behaviour. A smaller robot can generally respond more quickly to changes in steering and can occupy less space on the course. At the same time, the faster vehicle means that errors accumulate more quickly. A small heading error at high speed can result in the vehicle moving significantly away from its intended path before a correction takes effect.
This was one reason the team added an IMU.
1.3 Differential-drive system
The redesigned vehicle uses differential drive rather than the original rear-wheel-drive/Ackermann architecture.
Differential drive provides a direct method of controlling vehicle movement through the drive system. Instead of depending entirely on a mechanically steered front axle, the vehicle's movement can be influenced by the relative motion of its drive system.
This is particularly useful for autonomous navigation because the controller can respond to environmental feedback without requiring a large mechanical steering movement for every correction.
The software defines the drive motor in arm.h as:
EvoMotor drive(M4, EV3MediumMotor, true);
The drive object is then initialised in setup() using:
drive.begin();
During autonomous operation, the drive motor is commanded using:
drive.run(FAST_SPEED);
where:
const int FAST_SPEED = 4000;
The use of a defined speed parameter means that the operating speed can be tuned without changing the main navigation algorithm.
The mechanical redesign therefore influenced the software architecture directly. The robot was intended to be smaller and faster, so the control system needed to provide sufficient feedback to keep its movement predictable.
1.4 Strengthened steering mechanism
The redesigned steering system uses a gear mechanism rather than the original axle-based arrangement.
The reason for this change was primarily mechanical strength and reliability.
The original system demonstrated that the steering mechanism could experience significant stress when the motor was driven toward its mechanical limit. A gear-based transmission provided a more robust mechanical connection between the motor and steering mechanism.
The steering motor is defined in the hardware configuration as:
EvoMotor steering(M3, EV3MediumMotor, true);
The steering is initialised using:
steering.begin();
and the steering angle is reset using:
steering.resetAngle();
The software then commands a target angle using:
steering.runTarget(
    STEER_SPEED,
    steerAngle,
    MotorStop::HOLD,
    false
);
The use of a target angle is important because it gives the software a way to request controlled steering positions rather than simply turning the motor on and leaving it running.
The steering speed is defined as:
const int STEER_SPEED = 6000;
The actual steering angle is calculated dynamically from sensor information.
This means that the mechanical steering system and the software control system operate as a feedback loop. The software calculates a desired steering angle, the motor moves the mechanical steering system toward that target, and the resulting vehicle movement is observed by the sensors.
1.5 Mechanical iteration as an engineering process
The mechanical evolution of the vehicle is one of the strongest examples of engineering iteration in the project.
The original design was not simply discarded because it was unsuccessful. It was used to identify specific weaknesses.
The early vehicle demonstrated:
the importance of mechanical steering limits;
the need for reliable motor connections;
the effect of cable length on hardware placement;
the difference between motor types;
the importance of matching motor commands to mechanical behaviour;
and the limitations of relying on predetermined steering movements.
The redesigned vehicle addressed several of these problems at the architectural level.
The steering was strengthened.
The vehicle was made smaller.
The drive system was changed.
A gyro/IMU was added.
The sensor architecture was expanded.
This shows that the team did not treat mechanical design and software as separate phases. Testing of the mechanical system directly affected software requirements, while software requirements also affected the mechanical redesign.
2. Power & Sensor Architecture
2.1 ESP32-S3 / EVOX1 controller
The central processing system is an EVOX1 platform based on the ESP32-S3.
The ESP32-S3 is responsible for running the autonomous-control program. It acts as the bridge between the sensing system and the electromechanical system.
The software begins with:
#include <Evo.h>
#include "arm.h"
and creates the EVOX1 controller object:
EVOX1 evo;
The EVO library provides the interface used by the program to control the motors and communicate with the sensors.
The hardware configuration is separated into arm.h. This file contains the objects representing the physical devices connected to the EVOX1.
This architecture makes the project easier to understand because there is a clear distinction between hardware configuration and autonomous behaviour.
2.2 Hardware configuration
The current Challenge 1 arm.h defines the following architecture:
EvoMotor steering(M3, EV3MediumMotor, true);

EvoMotor drive(M4, EV3MediumMotor, true);

EvoHuskyLens hl(I2C1);

EvoIMU imuu(I2C5);

EvoTOF tofFront(I2C6);

EvoTOF tofLeft(I2C7);

EvoTOF tofRight(I2C8);
This gives the following logical hardware map:
ComponentInterfaceSoftware objectPurpose
Steering motor	M3	steering	Controls vehicle steering
Drive motor	M4	drive	Provides vehicle propulsion
HuskyLens	I2C1	hl	Visual/colour recognition
IMU / gyro	I2C5	imuu	Measures relative heading
Front ToF	I2C6	tofFront	Front distance sensing
Left ToF	I2C7	tofLeft	Left-side distance sensing
Right ToF	I2C8	tofRight	Right-side distance sensing
Start input	GPIO1	GPIO1	Digital input
The final physical wiring diagram should be included alongside this table in the competition submission.
2.3 IMU / gyro architecture
The IMU was one of the major additions made during the final redesign.
The IMU is connected through I2C5:
EvoIMU imuu(I2C5);
The software initialises it with:
imuu.begin();
The robot then obtains its relative heading using:
imuu.getRelativeHeading();
The decision to add a gyro/IMU was motivated by the need for better directional stability.
A vehicle travelling quickly cannot rely on its mechanical system being perfectly straight. Small differences in wheels, motors, friction, alignment or the surface can cause the robot to gradually rotate away from its intended direction.
Without feedback, the robot would not necessarily know that this was happening.
The IMU solves this by providing a measurement of the vehicle's actual orientation.
At the beginning of autonomous operation, the program records:
straightHeadingRef = imuu.getRelativeHeading();
This value becomes the reference heading for the current straight section of the course.
The robot then continuously compares its current heading with this reference.
The error is calculated as:
float headingError = heading - straightHeadingRef;
This error becomes the basis of proportional steering correction.
2.4 Time-of-Flight sensors
The final Challenge 1 software defines three ToF sensors.
The front sensor is connected to I2C6:
EvoTOF tofFront(I2C6);
The left sensor is connected to I2C7:
EvoTOF tofLeft(I2C7);
The right sensor is connected to I2C8:
EvoTOF tofRight(I2C8);
All three are initialised during startup:
tofFront.begin();
tofLeft.begin();
tofRight.begin();
The active Challenge 1 navigation algorithm uses the left and right ToF sensors.
The readings are obtained using:
int left = tofLeft.getDistance();
int right = tofRight.getDistance();
The software defines:
const int SIDE_WALL_THRESHOLD = 150;
The code comments identify this value as millimetres.
If the measured distance from a side wall falls below the threshold, the robot treats the wall as sufficiently close to require an avoidance correction.
This gives the robot environmental awareness based on physical distance rather than assuming that the course is perfectly consistent.
2.5 HuskyLens architecture
The vehicle also contains a HuskyLens connected through I2C1:
EvoHuskyLens hl(I2C1);
During startup, the program calls:
hl.begin();
hl.setMode(ALGORITHM_COLOR_RECOGNITION);
The HuskyLens was especially important during the earlier stage of the project.
The original software used colour recognition to identify red and green blocks. Red and green detections were assigned different steering behaviours.
However, the Challenge 1 code provided here does not actively use the HuskyLens detection result inside its main navigation loop.
This is an important example of the project's evolution.
A component can remain physically connected to the vehicle while its role in the software changes as the design develops.
The early prototype was more dependent on visual colour recognition. The Challenge 1 architecture is instead primarily based on heading and distance sensing.
The HuskyLens can therefore be described as part of the vehicle's broader perception architecture, while the Challenge 1 navigation logic shown here primarily uses the IMU and ToF sensors.
2.6 Power architecture
The exact battery model, battery voltage, power distribution and regulator arrangement were not included in the information provided so far. These details should be added to the final submission rather than guessed.
The power section of the final document should therefore include a physical wiring diagram showing:
battery;
controller power;
motor power;
sensor power;
ground connections;
any voltage regulation;
and the relevant power distribution points.
This distinction is important for reproducibility. A future team should be able to determine not only which signals connect to which controller ports, but also how the vehicle is safely powered.
The current information establishes the signal/interface architecture, but not the complete electrical power architecture.
3. Software Architecture & Obstacle Strategy
3.1 Overall software structure
The Challenge 1 program is separated into a hardware-definition header and a main Arduino program.
The header is arm.h.
The main program is the Challenge 1 .ino file.
The header defines the hardware objects. The .ino file defines the behaviour.
This provides a simple form of hardware abstraction.
For example, the main program does not need to recreate the drive motor object. It can simply call:
drive.run(FAST_SPEED);
because drive has already been defined in arm.h.
Similarly, the main program can read:
tofLeft.getDistance();
without having to recreate the physical I2C configuration of the sensor.
This makes the software easier to maintain.
3.2 Startup sequence
The program begins by starting serial communication at 115200 baud:
Serial.begin(115200);
delay(500);
GPIO1 is configured as an input:
pinMode(GPIO1, INPUT_PULLDOWN);
The program then prints:
=== BOOT START ===
This provides a clear indication that the controller has begun executing the program.
The EVOX1 controller is initialised:
evo.begin();
The HuskyLens is initialised and configured for colour recognition.
The steering and drive motors are initialised.
The three ToF sensors are initialised.
The steering motor is placed in coast mode and the drive motor is braked.
The IMU is then initialised.
The program deliberately prints a warning before initialising the IMU:
make sure the bot is NOT being touched/moved right now.
This reflects the importance of IMU calibration. If the vehicle is moved during initialisation, the reference heading may not accurately represent the intended starting orientation.
The steering angle is then reset.
3.3 Pre-start state
The vehicle does not immediately start driving after power-up.
Instead, it waits for the EVOX1 programmable button to be pressed:
while (evo.getButton() != PRESSED)
While waiting, the program displays values from the IMU and steering system.
The display function is:
void displayValues(){
  evo.clearDisplay();
  evo.writeLineToDisplay(imuu.getRelativeHeading(), 0);
  evo.writeLineToDisplay(steering.getAngle(), 1);
  evo.drawDisplay();
}
This gives the team direct visibility into the vehicle's heading and steering angle.
During the waiting period, the program also commands steering based on the current heading:
steering.runTarget(
    3000,
    imuu.getRelativeHeading()*4,
    MotorStop::HOLD,
    false
);
Serial output is printed once per second to show the current heading.
This makes the pre-start stage useful for debugging and calibration.
3.4 Calibration movement
After the button is pressed, the robot plays a tone:
evo.playTone(400,300);
It then executes the straight() function.
The function commands three steering positions:
steering.runTarget(3000, -90, MotorStop::HOLD, false);
delay(1000);

steering.runTarget(3000, 90, MotorStop::HOLD, false);
delay(1000);

steering.runTarget(3000, 0, MotorStop::HOLD, false);
This produces a controlled steering movement before the autonomous run begins.
The routine can be considered a calibration or mechanical alignment movement.
Once the movement is complete, the program records the current IMU heading as the initial straight-line reference:
lastCornerHeading = imuu.getRelativeHeading();
straightHeadingRef = imuu.getRelativeHeading();
The vehicle is therefore starting its main navigation algorithm with a defined reference orientation.
3.5 Main autonomous loop
The main loop() function contains the autonomous behaviour.
The first condition checks whether the robot has completed the required number of corners:
if (cornersCompleted >= TOTAL_CORNERS)
If so, the drive motor is braked and the steering is returned to zero.
The display then shows:
Done! 3 laps
The robot does not continue driving once the target number of corners has been reached.
If the target has not been reached, the drive motor is commanded to the defined fast speed:
drive.run(FAST_SPEED);
The left and right ToF sensors are read.
The IMU heading is read.
The heading error is calculated.
The steering decision is then made.
3.6 Steering decision hierarchy
The most important part of the Challenge 1 algorithm is the hierarchy used to determine the steering angle.
The software first asks whether the left wall is too close:
if (left < SIDE_WALL_THRESHOLD)
If it is, the robot sets:
steerAngle = STEER_AWAY_ANGLE;
The configured angle is:
const int STEER_AWAY_ANGLE = 30;
If the left wall is not too close, the software checks the right wall:
else if (right < SIDE_WALL_THRESHOLD)
If the right wall is too close, the steering angle is:
steerAngle = -STEER_AWAY_ANGLE;
Only when neither side wall is too close does the software apply normal heading correction.
This priority structure can be expressed as:
Wall avoidance > heading correction
That is an important design decision.
A vehicle that is slightly off its desired heading can tolerate a small correction. A vehicle that is extremely close to a wall may require an immediate larger steering response.
The software therefore prioritises immediate environmental safety over minor orientation errors.
3.7 Proportional heading controller
When there is no immediate wall-avoidance condition, the robot calculates its steering angle from the IMU heading error.
The formula is conceptually:
steering correction = -heading error × heading gain
The code uses:
const int HEADING_KP = 4;
and:
steerAngle = constrain(
    (int)(-headingError * HEADING_KP),
    -MAX_STRAIGHT_CORRECTION,
    MAX_STRAIGHT_CORRECTION
);
The maximum correction is:
const int MAX_STRAIGHT_CORRECTION = 15;
This is a proportional controller.
Suppose the robot begins a straight section at a heading of zero degrees. If the vehicle later drifts to a positive heading, the heading error becomes positive. Multiplying by the negative proportional correction produces a steering command intended to push the vehicle back toward the reference orientation.
The correction is capped at ±15 degrees.
This cap is important because a large sensor error should not necessarily result in an unlimited steering command. An excessive correction could cause the vehicle to oscillate or turn too aggressively.
The controller therefore balances responsiveness and stability.
3.8 Corner detection
The software uses heading change to detect corners.
The relevant parameters are:
const float CORNER_TURN_THRESHOLD = 70.0;
const unsigned long CORNER_COOLDOWN_MS = 800;
The robot stores the heading at the previous corner:
float lastCornerHeading = 0;
During the main loop, it compares the current heading to this stored value.
A corner is considered detected when:
fabs(heading - lastCornerHeading) >= CORNER_TURN_THRESHOLD
and:
millis() - lastCornerTime > CORNER_COOLDOWN_MS
The first condition detects a sufficiently large change in orientation.
The second condition prevents the same corner from being detected repeatedly while the robot is still completing the turn.
Once the conditions are satisfied, the program increments:
cornersCompleted++;
and updates:
lastCornerHeading = heading;
lastCornerTime = millis();
straightHeadingRef = heading;
The last assignment is particularly important.
After a corner, the robot's desired straight-line heading is no longer the heading from before the turn. The new heading becomes the reference for the next section of the course.
Thus, the control system repeatedly establishes a new local heading reference after each detected corner.
3.9 Three-lap strategy
The course logic is configured using:
const int CORNERS_PER_LAP = 4;
const int TOTAL_LAPS = 3;
const int TOTAL_CORNERS = CORNERS_PER_LAP * TOTAL_LAPS;
The resulting target is twelve corners.
This is a simple and computationally efficient way of tracking course progress.
Rather than relying on an external timer or human input to determine when the run is finished, the vehicle uses its own sensor information.
The algorithm effectively assumes:
4 detected corners = 1 lap
and:
12 detected corners = 3 laps
Once the twelfth corner is detected, the vehicle stops.
This approach has the advantage of being independent of the exact time required to complete a lap. If the robot moves slightly faster or slower, the stopping condition remains based on course progress rather than a fixed time.
3.10 Debugging and telemetry
The software includes serial telemetry to make the autonomous system observable.
Every approximately 200 ms, it prints:
L:
R:
heading:
headingError:
steerAngle:
This allows the team to monitor the relationship between sensor input and control output.
For example, if the left distance suddenly drops below 150 mm, the serial data can show the change in the left sensor reading and the resulting steering command.
Similarly, if the heading gradually changes while travelling straight, the team can observe the heading error and the calculated correction.
Corner detection is also reported:
Corner X / 12
This is useful because autonomous failures can come from different layers.
A robot could:
physically steer incorrectly;
measure distance incorrectly;
measure heading incorrectly;
calculate the wrong steering angle;
detect a corner too early;
or fail to detect a corner.
Telemetry helps distinguish between these possibilities.
4. Systems Thinking & Engineering Decisions
4.1 The project as an integrated system
The vehicle demonstrates systems thinking because the final performance depends on the interaction of many subsystems.
The mechanical system determines how the robot moves.
The motors provide physical actuation.
The sensors provide information.
The ESP32-S3 processes the information.
The software determines the required response.
The vehicle then moves differently because of that response, which changes the next sensor measurements.
This creates a feedback system.
For example:
Robot drifts
→ IMU detects changed heading
→ software calculates heading error
→ software changes steering target
→ vehicle changes direction
→ IMU measures the new heading
→ software calculates the remaining error
→ correction continues or decreases.
The same principle applies to wall detection:
Robot approaches wall
→ ToF distance decreases
→ distance becomes less than threshold
→ wall-avoidance condition activates
→ steering angle changes
→ robot moves away from wall
→ measured distance increases.
This is a complete sensor-to-actuator feedback loop.
4.2 Why the team chose to redesign
The decision to completely rebuild the vehicle one and a half months before competition was a major systems-level tradeoff.
The team had two choices.
The first was to continue improving the original vehicle.
The second was to create a new platform.
Continuing with the original design would have preserved more existing work, but it would also have preserved the weaknesses already discovered during testing.
The redesign created more short-term workload, but it gave the team an opportunity to address multiple problems simultaneously.
The new design was smaller and faster.
The steering mechanism was strengthened.
The drive architecture was changed.
A gyro was added.
Additional ToF sensors were integrated.
These changes were mutually reinforcing.
The smaller vehicle required less physical space.
The faster vehicle increased the importance of accurate control.
The stronger steering system made more aggressive control safer.
The differential-drive system changed the movement model.
The gyro provided feedback about the result.
The ToF sensors provided environmental information.
The result was a more sensor-driven autonomous architecture.
4.3 Speed versus stability
Making the vehicle faster was useful because it could reduce the time required to complete the course.
However, speed also creates control challenges.
At higher speed:
the robot travels farther during each control interval;
steering errors can become larger before correction;
wall proximity can change more quickly;
mechanical vibration may become more significant;
and inaccurate heading can have a larger effect on the trajectory.
The team therefore did not simply increase speed without changing the control system.
The IMU was added to provide heading feedback.
The ToF sensors were incorporated to detect nearby walls.
The steering system was strengthened.
The software used proportional correction with a maximum steering limit.
These decisions show that performance was considered as a complete system rather than as a single parameter.
4.4 Sensor redundancy and complementary information
The sensors serve different purposes.
The IMU measures the robot's orientation.
The ToF sensors measure distance from nearby objects.
The HuskyLens provides visual information.
These sensors are complementary rather than interchangeable.
The IMU cannot directly tell the robot how far it is from a wall.
A ToF sensor cannot directly measure the robot's orientation.
The HuskyLens provides visual information that is fundamentally different from a distance measurement.
The Challenge 1 controller uses the sensors according to their strengths.
The IMU provides the normal straight-line correction.
The left and right ToF sensors provide immediate wall-proximity information.
The HuskyLens remains available for colour-based perception and was more important to the earlier version of the project.
This layered architecture allows the software to use the most relevant sensor for each decision.
4.5 Tuning and calibration
Autonomous robotics requires tuning because theoretical values rarely work perfectly on the first attempt.
The Challenge 1 code therefore places important values at the top of the file.
For example:
const int STEER_SPEED = 6000;
const int FAST_SPEED = 4000;
const int SIDE_WALL_THRESHOLD = 150;
const int STEER_AWAY_ANGLE = 30;
const int HEADING_KP = 4;
const int MAX_STRAIGHT_CORRECTION = 15;
const float CORNER_TURN_THRESHOLD = 70.0;
const unsigned long CORNER_COOLDOWN_MS = 800;
This arrangement allows the team to tune the robot without rewriting the main algorithm.
The side-wall threshold determines how close the vehicle can approach a wall before actively steering away.
The steering-away angle determines the strength of that response.
The heading proportional gain determines how strongly the robot reacts to orientation drift.
The maximum correction limits the response.
The corner threshold determines how much heading change is needed to classify movement as a corner.
The cooldown determines how quickly another corner can be counted.
These values interact with one another.
For example, increasing the drive speed without changing the wall threshold may cause the vehicle to approach walls more quickly than the control system can respond. Similarly, increasing the heading gain too much could cause oscillation.
The team therefore had to consider parameter interactions rather than tuning every value independently.
4.6 Mechanical and software failure as feedback
The early project experienced several failures and configuration problems.
These included:
unreliable motor ports;
incorrect motor declarations;
motor type mismatches;
steering mechanism stress;
missing header syntax;
ESP32/EVO library compatibility problems;
incorrect upload-port selection;
and use of a USB cable that needed to support data communication.
Rather than being unrelated inconveniences, these problems taught the team about the dependencies between hardware and software.
For example, an incorrect motor type is a software problem caused by the physical hardware configuration.
A motor-port issue affects both the physical wiring and the software declaration.
A steering-mechanism failure requires both a mechanical redesign and a change to the software's control behaviour.
A library compatibility issue can prevent otherwise correct application code from compiling.
These experiences strengthened the final design process.
5. Reproducibility
5.1 Development environment
The project was developed using Arduino IDE 2.x.
The ESP32 board support package from Espressif Systems is required.
The EVO library is also required because the project uses EVO-specific classes for the controller, motors and sensors.
The EVO library was manually installed as a ZIP package because it was not available through the standard Arduino Library Manager.
The final submission should record the exact ESP32 board-package and EVO library versions used by the final competition build.
This is important because version compatibility caused problems during early development.
Two ESP32 package versions investigated during development included 2.0.17 and 3.3.8. The project also encountered an issue involving Serial0 in EvoHC05.cpp, demonstrating that the EVO library and ESP32 framework versions could not simply be assumed to be interchangeable.
5.2 Source-code organisation
The project consists of the main Arduino .ino program and the hardware-definition file arm.h.
The basic structure is:
Challenge1/
├── Challenge1.ino
└── arm.h
The exact final filename should be replaced by the competition version used by the team.
The purpose of the separation is:
arm.h
Contains hardware definitions.
This includes:
steering motor;
drive motor;
HuskyLens;
IMU;
front ToF;
left ToF;
right ToF;
and the GPIO-based input helper.
Challenge 1 .ino
Contains:
tuning constants;
runtime variables;
display function;
steering calibration function;
setup;
main autonomous loop;
heading correction;
wall avoidance;
corner detection;
lap counting;
serial debugging.
This separation makes the code easier for judges or another engineering team to understand.
5.3 Hardware setup
The hardware connections defined in the software are:
M3   → steering motor
M4   → drive motor

I2C1 → HuskyLens
I2C5 → IMU / gyro
I2C6 → front ToF
I2C7 → left ToF
I2C8 → right ToF

GPIO1 → digital input / bump-style input
The physical wiring should match these declarations.
A change in physical wiring requires a corresponding change in arm.h.
This is an important reproducibility principle: the software configuration is a representation of the physical vehicle.
5.4 Compile procedure
The process for compiling the software begins with installing Arduino IDE 2.x.
The ESP32 board support package must then be installed through Arduino's board manager.
The EVO library must be installed manually if it is not available through the library manager.
The project files should be placed together in the same Arduino project.
The developer should then:
Open the Challenge 1 .ino file.
Confirm that arm.h is available.
Confirm that the EVO library is installed.
Select the correct ESP32-S3/EVOX1 board configuration.
Verify that the motor and sensor declarations match the actual robot.
Compile the program.
Check for missing library or board-version errors.
Correct any configuration problems before attempting to upload.
One of the early problems involved a missing #endif in arm.h. This prevented correct compilation and was fixed by completing the header guard.
Another problem occurred when motors were redeclared inside setup(). These local variables could shadow the global objects already created in arm.h. The solution was to use the existing hardware objects rather than creating conflicting local declarations.
Motor type mismatches were also encountered during development. The software must use the appropriate EVO motor type for the physical motor being controlled.
5.5 Upload procedure
The EVOX1 should be connected to the development computer using a USB data cable.
A charge-only cable is insufficient because it does not provide the data connection required for programming.
The correct upload port must also be selected.
During early development, selecting a Bluetooth port instead of the USB port caused upload problems.
The general procedure is:
Connect USB data cable → select correct board → select USB port → compile → upload → monitor serial output.
The serial monitor should use 115200 baud because the program begins with:
Serial.begin(115200);
Once uploaded, the developer should observe the startup messages to confirm that the controller reaches the expected stages.
5.6 Post-upload testing
The vehicle should be tested progressively.
First, the controller should be powered on while the robot is stationary.
The serial output should confirm:
=== BOOT START ===
The IMU should then be checked.
The program reports its heading immediately after IMU initialisation.
The steering angle should be reset and the steering motor tested.
The drive motor should be tested separately.
The ToF sensors should then be checked by observing their distance values.
The robot should be tested at low-risk conditions before being allowed to operate at the full configured speed.
The pre-start button provides an additional layer of control because the robot does not immediately enter autonomous driving after boot.
The calibration movement should also be observed to ensure that the steering mechanism can safely perform its programmed movement.
6. Code Documentation for Judges
The code should be understandable even to judges who did not use the same development tools.
The most important elements to explain are the hardware objects, tuning parameters and main control loop.
The following architecture should be made clear in the source code:
ESP32-S3 / EVOX1
       |
       +------------------+
       |                  |
       v                  v
    Sensors            Motors
       |                  |
       |                  +--> Steering
       |                  +--> Drive
       |
       +--> IMU
       +--> Left ToF
       +--> Right ToF
       +--> Front ToF
       +--> HuskyLens
The comments should explain not only what each line does, but why the operation exists.
For example, instead of only writing:
// read heading
a stronger comment would explain:
// Read the current relative heading so the robot can
// compare its orientation with the heading recorded at
// the beginning of the current straight section.
Similarly, the wall threshold should be explained as a physical decision:
// If a side wall is closer than 150 mm, prioritise
// steering away from it over normal heading correction.
This style is useful because judges may not know the EVO library or the team's exact development environment.
7. Reproducibility of the Engineering Process
Reproducibility does not only mean that someone can compile the code. It also means that another team can understand why the vehicle is configured the way it is.
The development history provides that context.
The original vehicle used rear-wheel drive and Ackermann steering.
The steering mechanism experienced mechanical stress.
The team experimented with different motor ports.
The development environment had library and board compatibility issues.
The team learned to distinguish the USB upload connection from Bluetooth communication.
The steering system was then redesigned.
The entire vehicle was rebuilt approximately one and a half months before competition.
The new design was smaller.
The new design was faster.
The steering was strengthened using gears.
The drive system became differential drive.
A gyro/IMU was added.
Three ToF sensors were incorporated into the new navigation architecture.
The software then evolved from the original colour-driven behaviour toward a heading-and-distance-based navigation system.
This progression is important because it shows that the final robot was the result of testing and decision-making rather than simply being assembled according to a fixed plan.
8. Relationship Between the Five Judging Criteria
Mobility & Mechanical Design
The vehicle's mechanical design evolved from RWD/Ackermann steering into a smaller, faster differential-drive platform with strengthened gear-based steering.
The redesign addressed physical weaknesses identified during testing.
Power & Sensor Architecture
The ESP32-S3/EVOX1 controller connects to the motors and sensor network.
The IMU provides heading.
The ToF sensors provide distance.
The HuskyLens provides visual recognition capability.
The sensor interfaces are explicitly defined through I2C connections.
Software Architecture & Obstacle Strategy
The software is divided into hardware definitions and autonomous behaviour.
The robot uses a hierarchical control strategy:
side-wall detection first, heading correction second.
The IMU provides proportional heading correction.
The ToF sensors provide wall-avoidance information.
Corner detection provides course-progress tracking.
Systems Thinking & Engineering Decisions
The complete vehicle was redesigned when the original architecture was judged insufficient.
Mechanical, electrical and software constraints were considered together.
The team accepted the risk of rebuilding close to competition in order to create a stronger and more controllable platform.
Reproducibility
The hardware definitions, software architecture, development environment and upload process can be documented so another team can recreate the system.
The final submission should add the exact final power configuration, physical dimensions, wiring photographs, mechanical drawings, final library versions and competition-specific calibration values.
9. Suggested System Diagram
The final report should include a diagram based on the following architecture:
                         ┌──────────────────────┐
                         │      ESP32-S3        │
                         │       EVOX1          │
                         └──────────┬───────────┘
                                    │
             ┌──────────────────────┼──────────────────────┐
             │                      │                      │
             │                      │                      │
             ▼                      ▼                      ▼
        I2C Sensors             Motor Control          GPIO Input
             │                      │                      │
     ┌───────┼────────┐             │                      │
     │       │        │             │                      │
     ▼       ▼        ▼             ▼                      ▼
  HuskyLens IMU    ToF Sensors   Steering + Drive      Start Input
             │          │
             │      ┌───┼────┐
             │      │   │    │
             │      ▼   ▼    ▼
             │    Front Left Right
             │     ToF  ToF  ToF
             │
             ▼
      Visual Recognition
The final physical wiring diagram should be more detailed than this logical diagram and should show the actual battery and power distribution.
10. Engineering Tradeoffs
Every autonomous vehicle design contains tradeoffs.
The first major tradeoff was the decision to prioritise a complete redesign despite the limited time remaining before competition.
The second was speed versus stability.
The team wanted a faster vehicle, but higher speed increased the need for accurate feedback.
The third was mechanical strength versus simplicity.
The original axle-based steering arrangement was simpler, but the gear-based redesign provided a stronger steering transmission.
The fourth was sensor complexity versus software complexity.
Adding more sensors increases the amount of information available to the controller, but it also increases the number of failure modes and tuning parameters.
The fifth was simplicity versus adaptability in the navigation algorithm.
The Challenge 1 program does not attempt to solve every possible environmental situation. Instead, it uses a relatively simple set of rules based on side distance, heading and corner count.
This makes the algorithm easier to understand and tune.
The hierarchical design is especially important.
Rather than combining every sensor into one complicated mathematical model, the program uses clear priorities:
Determine whether the vehicle is near a side wall.
If necessary, steer away from the wall.
Otherwise, use IMU feedback to maintain heading.
Monitor heading change for corner detection.
Update the reference heading after each corner.
Stop after the required number of corners.
This structure provides a clear relationship between sensor readings and actions.
11. Testing and Iteration
Testing was a major component of the project.
The earliest tests were focused on basic hardware operation.
The team needed to determine whether the motor ports worked reliably, whether the correct motor types were being used, whether the motors could be controlled through the EVO library and whether the steering mechanism could physically withstand the commands being sent to it.
The steering failure was particularly informative.
The motor was able to produce enough force to push the mechanism against its physical limit. The problem was therefore not that the motor was too weak. The problem was that the control strategy did not sufficiently account for the mechanical limits.
The team learned from this and later strengthened the steering mechanism using gears.
The software also underwent iteration.
The first software architecture was largely based around the HuskyLens and colour recognition.
The later Challenge 1 architecture uses the IMU and ToF sensors as primary navigation inputs.
This demonstrates a progression from event-based commands toward feedback-based autonomous control.
The vehicle therefore became less dependent on assumptions such as "turn for this amount of time" and more dependent on observations such as "the robot is currently this many degrees away from its reference heading" or "the wall is currently closer than the allowed threshold."
12. Final Challenge 1 Architecture
The Challenge 1 system can be summarised as five layers.
Layer 1 — Mechanical layer
The redesigned vehicle is smaller and faster, with differential drive and a strengthened gear-based steering mechanism.
Layer 2 — Actuation layer
The EVOX1 controls the drive and steering motors.
Layer 3 — Sensor layer
The vehicle uses:
IMU/gyro;
left ToF;
right ToF;
front ToF;
HuskyLens.
Layer 4 — Control layer
The ESP32-S3 processes:
heading;
heading error;
side distance;
corner progress.
Layer 5 — Behaviour layer
The vehicle:
waits for the start command;
calibrates;
drives forward;
avoids close side walls;
corrects heading;
detects corners;
updates its heading reference;
counts laps;
stops after three laps.
This layered architecture makes the system easier to understand and debug.
13. Limitations and Future Improvements
The documentation should also acknowledge limitations rather than presenting the engineering process as perfect.
The current Challenge 1 code includes a front ToF sensor:
EvoTOF tofFront(I2C6);
but the shown main loop does not use the front distance in its steering decision.
Therefore, the active wall-avoidance strategy shown in this version is based on the left and right sensors.
Similarly, the HuskyLens is initialised and configured for colour recognition but is not used for the primary Challenge 1 navigation decisions shown in the main loop.
These are not necessarily weaknesses; they reflect the fact that different sensors can be useful for different challenges or stages of the project.
Future software could integrate front-distance information into a dedicated obstacle state, allowing the robot to distinguish between side-wall correction and an object directly ahead.
The steering controller could also be improved through more advanced feedback, depending on the final vehicle mechanics and competition requirements.
The current controller uses proportional heading correction. A more advanced controller could potentially account for the rate of heading change as well as the heading error itself.
However, greater algorithmic complexity is not automatically better. In a competition robot, reliability, predictability and ease of tuning are also important.
The selected control system represents a deliberate balance between complexity and practical performance.
14. Final Reproducibility Checklist
A complete version of this documentation should be accompanied by the following information:
Mechanical documentation
Full vehicle photograph from the front.
Full vehicle photograph from the rear.
Full vehicle photograph from the left.
Full vehicle photograph from the right.
Photograph from above.
Photograph from below.
Photograph of the steering gear mechanism.
Photograph of the differential-drive system.
Diagram of the chassis.
Diagram showing sensor locations.
Diagram showing motor locations.
Electrical documentation
EVOX1/ESP32-S3 photograph.
Battery photograph.
Power wiring diagram.
Motor connections.
I2C connections.
GPIO connections.
Sensor wiring.
Final port table.
Software documentation
Complete .ino source.
arm.h.
Required EVO library.
ESP32 board package version.
Arduino IDE version.
Compile instructions.
Upload instructions.
Calibration instructions.
Tuning parameters.
Explanation of the control algorithm.
Competition evidence
Open Challenge video.
Obstacle Challenge video.
At least 30 seconds of actual autonomous driving in each video.
Public or link-accessible video URLs.
Full vehicle photographs.
Team photograph.
Conclusion
The development of our autonomous vehicle was an iterative process involving mechanical design, embedded programming, sensor integration and autonomous control.
The project began with a rear-wheel-drive vehicle using Ackermann steering. That design provided a useful foundation, but testing exposed practical limitations. The steering mechanism could be damaged by uncontrolled motor movement, several motor-port configurations were unreliable, motor types had to be correctly matched in software, and the development environment presented compatibility issues between the ESP32 platform and the EVO library.
These problems were not treated as isolated failures. They informed the development of the next version.
Approximately one and a half months before competition, the team made the significant decision to completely redesign the robot.
The new robot was made smaller and faster. The steering system was strengthened using gears rather than the previous axle-based arrangement. The drive system was changed to differential drive. A gyro/IMU was added so that the robot could measure its own heading and correct directional drift.
This redesign changed the software requirements.
The vehicle could now operate at a higher speed, but faster movement increased the importance of accurate control. A heading-feedback system was therefore implemented using the IMU. The robot records a reference heading at the beginning of each straight section and calculates the difference between that reference and the current heading. A proportional controller then converts this error into a steering correction, with the correction limited to a maximum of 15 degrees.
The robot also uses three ToF sensors. The left and right sensors are actively used in Challenge 1 to detect when the vehicle approaches a side wall. A threshold of 150 mm is used as the trigger point. When the vehicle is too close to a wall, wall avoidance takes priority over ordinary heading correction. This creates a clear hierarchy in the control system: immediate environmental constraints are addressed before smaller orientation errors.
The front ToF sensor is part of the hardware architecture but is not directly used in the primary steering logic shown in the Challenge 1 program. Similarly, the HuskyLens remains connected through I2C1 and is configured for colour recognition, reflecting the importance of visual sensing during earlier development, but the Challenge 1 navigation algorithm primarily relies on the IMU and side ToF sensors.
The software also tracks course progress. The expected course is represented as four corners per lap and three laps total. This produces a target of twelve corners. A corner is detected when the vehicle's heading changes by at least 70 degrees from the previous corner heading, subject to an 800 ms cooldown. After each detected corner, the current heading becomes the new reference heading for the next straight section.
This system combines low-level motor control with higher-level course logic.
At the lowest level, the EVOX1 controls the motors.
At the sensing level, the ESP32-S3 receives data from the IMU and ToF sensors.
At the control level, the software converts those measurements into steering commands.
At the navigation level, the software detects corners and counts progress.
At the completion level, the robot stops after the programmed twelve-corner objective.
The project therefore demonstrates systems thinking because no single component is responsible for autonomous performance. The mechanical design determines how the vehicle can move. The sensors provide information about the vehicle and its surroundings. The ESP32-S3 processes that information. The software determines what response is appropriate. The motors execute the response. The resulting movement changes the next sensor measurements, creating a feedback loop.
The redesign also demonstrates engineering decision-making under constraints. Rebuilding a vehicle approximately one and a half months before competition created a significant time risk. However, the team judged that continuing with the original architecture would leave important mechanical and control limitations unresolved. The decision to rebuild allowed the team to address several issues at once and produce a smaller, faster and more feedback-driven platform.
The project also places importance on reproducibility. The hardware configuration is separated into arm.h, while the autonomous behaviour is contained in the main Arduino program. This means that the relationship between software and hardware can be inspected directly. The motor and sensor interfaces are explicitly defined, and the main program uses named tuning constants for the most important control parameters.
The development environment itself also forms part of the reproducibility requirements. Arduino IDE 2.x, the appropriate Espressif ESP32 board package and the EVO library are required. The EVO library must be available to provide the EVOX1, motor, HuskyLens, IMU and ToF interfaces. During early development, library and board-package compatibility required troubleshooting, demonstrating why the final documentation should record the exact software versions used for the competition build.
The upload process also requires a USB data cable and the correct USB port rather than a Bluetooth communication port. These details may appear minor, but they were practical obstacles encountered during development and are therefore important for another team attempting to reproduce the system.
Overall, the autonomous vehicle represents an engineering process rather than simply a final collection of components. The first prototype provided the team with experience in hardware control and sensor integration. Testing exposed weaknesses. Those weaknesses led to mechanical changes, architectural changes and software changes. The final Challenge 1 system reflects those lessons through its smaller and faster vehicle, strengthened steering, differential-drive architecture, IMU-based heading correction, ToF-based wall detection and corner-counting navigation system.
The strongest characteristic of the project is therefore the relationship between iteration and engineering decisions. When the original steering system proved vulnerable, it was strengthened. When the original vehicle architecture was no longer considered suitable, the vehicle was rebuilt. When faster movement increased the need for control, an IMU was added. When environmental distance became important, ToF sensors were integrated. When the robot needed to maintain a stable direction, the software introduced proportional heading feedback. When the course needed to be completed without human intervention, the software added automatic corner and lap counting.
The result is a compact autonomous vehicle whose mechanical and software systems were developed together. The ESP32-S3/EVOX1 controller provides the computational core, the sensors provide environmental and motion information, the control algorithm interprets those measurements, and the motors translate the decisions into physical movement.
The final documentation should combine this engineering description with the team's photographs, final wiring and mechanical diagrams, complete source code, exact software versions, power architecture and competition videos. Together, these materials will allow judges to evaluate not only whether the robot can perform its challenge, but also how the team designed, tested, redesigned and documented the complete system.
