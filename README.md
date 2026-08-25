# WRO Future Engineers 2026

## Team SSTatic

**School of Science and Technology, Singapore**

This repository contains the engineering documentation for **Team SSTatic** for the **WRO Future Engineers 2026** competition.

### Team Members

* **Neah Shibil**
* **Amelia Ng**

This repository documents the development of our autonomous vehicle, including its mechanical design, propulsion and steering system, sensors, software architecture, autonomous navigation strategy, testing process, engineering decisions, source code, and development history.

Our vehicle was not created as one finished design. It was developed through repeated construction, testing, debugging, failure analysis, and redesign. Mechanical changes frequently affected software behaviour, while software testing often exposed mechanical problems that had to be corrected physically.

---

# 1. Vehicle Overview

Our vehicle is an autonomous four-wheel robot designed for the WRO Future Engineers competition.

The vehicle uses a **rear-wheel propulsion system** together with an **Ackermann-style front steering mechanism**. The rear drive system provides forward movement, while a separate steering motor changes the angle of the front wheels.

This arrangement was chosen because Ackermann steering operates similarly to the steering of a conventional road vehicle. When turning, the inner and outer front wheels follow different turning radii instead of being forced to travel through the same path.

The main controller is an **EVOX1-based controller**, programmed using Arduino-style C++.

The autonomous vehicle combines several sensing systems:

* **HuskyLens vision sensor** for recognising coloured objects;
* **Time-of-Flight (ToF) sensors** for measuring distance from walls and nearby objects;
* **IMU / gyroscope** for measuring the heading and orientation of the vehicle.

The controller combines these sensor inputs with commands to the propulsion and steering motors.

The software evolved from simple forward-driving and colour-recognition experiments into more advanced navigation involving steering correction, distance sensing, heading correction, turning, and lap-navigation logic.

---

# 2. Mobility & Mechanical Design

## 2.1 Initial Chassis Development

Construction of the vehicle began on **29 April 2026**.

During the first development session, we studied the competition requirements, researched vehicle designs, prepared a parts list, and started constructing the base of the robot.

We selected **Ackermann steering** as our steering concept from the beginning.

One of our first problems was designing a steering system using the available components. Suitable examples matching our exact requirements were difficult to find, and the steering gears initially did not align correctly.

As a result, the steering mechanism was developed through testing and modification rather than directly copying an existing design.

On **6 May**, the steering system was modified because the height of the front steering assembly did not properly match the rear wheels. Different combinations of structural components were tested until the front and rear parts of the chassis were aligned correctly.

The base and wiring were subsequently completed.

During later testing, we also found that chassis balance affected the behaviour of the robot. On **13 May**, we researched previous successful WRO Future Engineers vehicles and modified parts of our own vehicle to improve its balance.

This became an important theme throughout the project: mechanical reliability had to be achieved before software behaviour could become repeatable.

---

## 2.2 Rear-Wheel Propulsion

The vehicle uses rear-wheel propulsion.

The drive motor supplies forward movement while steering is performed independently by the front wheels.

The rear-wheel assembly required several rounds of troubleshooting.

During development, problems included:

* only one rear wheel moving correctly;
* unstable axle positioning;
* drivetrain components requiring temporary repair;
* mechanical alignment affecting vehicle movement;
* hardware inconsistencies causing identical software commands to produce different results.

On **1 July**, for example, only one rear wheel was moving properly. The rear-wheel mechanism had to be repaired before meaningful software testing could continue.

This taught us an important engineering lesson: software cannot reliably compensate for an inconsistent mechanical system.

If an axle moves or a wheel does not receive power correctly, changing steering values in the code does not solve the underlying mechanical problem. Therefore, we repaired and redesigned the mechanical system where necessary instead of attempting to correct every issue through software.

---

## 2.3 Ackermann Steering

The front wheels use an Ackermann-style steering system.

Ackermann steering allows the inside and outside front wheels to follow different turning radii. The inside wheel travels around a smaller radius while the outside wheel follows a larger radius.

A dedicated motor controls the steering mechanism.

The steering system was one of the most heavily modified parts of the robot.

The first version suffered from gear-alignment problems.

On **6 May**, the geometry and height of the steering system were changed.

On **29 May**, the steering mechanism was damaged and required a temporary repair.

On **1 July**, the steering broke again and had to be rebuilt.

By **10 July**, we had created a more permanent steering structure.

On **17 July**, the front steering was repaired and modified again to increase the available turning range.

Finally, on **29 July**, steering calibration was completed while the overall vehicle structure was rebuilt into its final mechanical form.

Software testing also revealed an important limitation. Early steering experiments showed that continuously driving the steering motor at high power could force the mechanism against its physical limit. This placed unnecessary load on the mechanism and could damage or move parts.

We therefore moved toward controlled steering movements and short steering corrections instead of continuously forcing the motor in one direction.

---

## 2.4 Mechanical and Software Co-Design

One of the most important lessons from the project was that mechanical design and software design cannot be treated independently.

For example:

* steering geometry affected how much steering input was required;
* loose or unreliable wheels affected navigation accuracy;
* increasing steering range changed the turning behaviour of the robot;
* sensor mounting affected detection reliability;
* chassis balance affected how consistently the vehicle travelled;
* mechanical instability made software calibration difficult.

On **29 July**, the entire structure of the robot was rebuilt.

This was not simply an aesthetic redesign. The hardware was modified specifically so that the software could control the vehicle more consistently.

This interaction between mechanical design and control software influenced many of our later engineering decisions.

---

# 3. Power & Sensor Architecture

## 3.1 Main Controller

The main controller used in the vehicle is an **EVOX1-based controller**.

The controller runs the autonomous navigation software and interfaces with the motors and sensors.

Its responsibilities include:

* controlling the propulsion motor;
* controlling the steering motor;
* communicating with the HuskyLens;
* reading ToF distance sensors;
* reading IMU heading information;
* executing autonomous navigation logic;
* providing serial output for debugging;
* providing display output during some stages of development.

The controller is programmed using Arduino-style C++.

---

## 3.2 Power Architecture

The vehicle is powered through its onboard battery and the EVOX1 hardware.

Electrical power is distributed to the controller, motors, and sensors.

Power and connection reliability became important during testing because intermittent hardware connections sometimes prevented the robot from uploading or operating correctly.

During later development, the robot occasionally had to be reset before new software could be uploaded. The BOOT button also became unreliable during some sessions.

These problems were treated separately from software logic because a successful compilation does not guarantee that the controller can physically receive the program.

---

## 3.3 HuskyLens Vision Sensor

The HuskyLens was originally added to the vehicle for the **Obstacle Challenge**, where it was intended to recognise coloured obstacles and trigger different steering responses.

During the development process, our team decided not to continue with the Obstacle Challenge and instead focused on completing the **Open Challenge** reliably.

Rather than removing the HuskyLens completely, we continued experimenting with it as part of the Open Challenge development. We explored whether its visual information could be used as an additional navigation input alongside the Time-of-Flight sensors and IMU.

This change reflects the iterative nature of our project. The HuskyLens began as a sensor for obstacle recognition, but after our competition strategy changed, we investigated how an existing component could still contribute to the final Open Challenge vehicle instead of redesigning the entire sensor architecture.

The final Open Challenge navigation strategy therefore focuses primarily on reliable lap navigation using steering control, distance sensing, and heading correction, while the HuskyLens remains part of our development and experimentation with visual sensing.


---

## 3.4 Time-of-Flight Sensors

Time-of-Flight sensors were introduced to measure the distance between the robot and surrounding walls or objects.

Unlike the HuskyLens, which provides visual information, the ToF sensors provide numerical distance measurements.

This allows the controller to estimate whether the vehicle is too close to or too far from a wall.

The basic concept is:

```text
Wall
 |
 | measured distance
 v
ToF Sensor
 |
 v
EVOX1 Controller
 |
 v
Steering Correction
```

Programming of the ToF sensors began in July.

By **10 July**, ToF programming had started.

On **21 July**, ToF sensing had been incorporated into navigation experiments.

The sensors did not work correctly immediately. During August testing, ToF readings were found to be unreliable.

On **14 August**, the team identified an issue affecting the ToF system and continued troubleshooting.

By **21 August**, ToF distance readings were being connected directly to steering behaviour.

A simplified steering strategy is:

```text
Read wall distance
       |
       v
Compare with desired position
       |
       +---- Too close ----> Steer away
       |
       +---- Too far ------> Steer toward wall
       |
       +---- Correct ------> Continue current path
```

The ToF sensors therefore became part of the feedback system used to keep the vehicle positioned correctly on the course.

---

## 3.5 IMU / Gyroscope

An **IMU / gyroscope** was added to measure the orientation and heading of the vehicle.

The IMU provides different information from the ToF sensors.

The ToF sensor answers:

> How far is the robot from the wall?

The IMU answers:

> In which direction is the robot pointing?

This heading information can be used to detect unwanted rotation and correct the vehicle's direction.

The IMU was physically introduced on **17 July**.

Further IMU programming was completed during August.

One significant software problem was discovered on **12 August**.

Our heading-correction logic was initially steering the robot back toward its original starting direction. This happened because the software continued treating the original direction as the desired heading even after the vehicle had intentionally completed a turn.

This meant the control system could not simply maintain one permanent heading.

Instead, the desired heading had to change after a deliberate turn.

The correction logic was therefore modified so that the robot could distinguish between:

* an unwanted heading error; and
* an intentional change in direction.

This was an important improvement to the autonomous navigation system.

---

## 3.6 Overall Sensor Architecture

The sensors provide complementary information:

```text
                    +----------------+
                    |   HuskyLens    |
                    | Colour Vision  |
                    +-------+--------+
                            |
                            v
+----------------+    +-------------+    +----------------+
| ToF Sensors    |--->|    EVOX1    |<---| IMU / Gyro     |
| Wall Distance  |    | Controller  |    | Heading        |
+----------------+    +------+------+    +----------------+
                            |
                    +-------+-------+
                    |               |
                    v               v
                Drive Motor    Steering Motor
```

The HuskyLens identifies visual information.

The ToF sensors measure distance.

The IMU provides orientation information.

The controller processes these inputs and commands the drive and steering motors.

---

# 4. Software Architecture & Obstacle Strategy

## 4.1 Software Structure

The vehicle software is written in Arduino-style C++.

During development, the code was organised around a main Arduino sketch together with a separate hardware-definition header.

Two important parts of the software architecture are:

### `arm.h`

The `arm.h` file contains hardware definitions and objects used to communicate with the electromechanical components of the robot.

These include hardware such as:

* the steering motor;
* the drive motor;
* the HuskyLens;
* detection-result structures;
* other hardware interfaces used by the vehicle.

Keeping these definitions separate reduces the amount of hardware-specific configuration inside the main navigation program.

### Main Arduino Sketch

The main `.ino` file contains the navigation behaviour.

Arduino programs use two main functions:

```cpp
void setup()
```

and:

```cpp
void loop()
```

`setup()` executes once after startup or reset.

It is used to initialise the controller and connected hardware.

Depending on the software version, this included:

* serial communication;
* EVOX1 functions;
* HuskyLens communication;
* motors;
* display output;
* ToF sensors;
* IMU functions.

`loop()` then repeats continuously while the robot is running.

This is where the autonomous navigation logic is executed.

---

## 4.2 Software-to-Hardware Relationship

The software directly interfaces with the electromechanical components of the vehicle.

```text
SOFTWARE                        ELECTROMECHANICAL COMPONENT

Drive command             ---> Rear propulsion motor

Steering command          ---> Ackermann steering motor

HuskyLens request         <--- Vision sensor

ToF reading               <--- Distance sensor

IMU heading reading       <--- IMU / gyroscope

Navigation decision       ---> Drive + steering response
```

Separating these responsibilities helped with debugging.

For example, if the HuskyLens successfully detected an object but the robot turned incorrectly, we could investigate the steering logic and steering mechanism separately from the camera.

---

## 4.3 Early Navigation Strategy

The earliest software concentrated on basic movement and colour recognition.

The vehicle was programmed to move forward while repeatedly checking the HuskyLens.

When the HuskyLens recognised a colour, the program applied an appropriate steering action.

A simplified form of the early logic was:

```text
START
  |
  v
Drive Forward
  |
  v
Check HuskyLens
  |
  +---- Nothing detected ---> Continue forward
  |
  +---- Colour detected ----> Select steering response
  |
  v
Continue
```

Early experiments used timed steering movements.

One version applied a steering pulse, allowed the vehicle to continue travelling, and then returned the steering toward its normal position.

---

## 4.4 Continuous Driving and Steering Corrections

One problem found during early development was that stopping the drive motor during every execution of the main loop prevented smooth forward movement.

The software was therefore changed so that the drive motor remained active continuously while steering was changed only when required.

The general strategy became:

```text
Drive continuously
       |
       v
Default steering = straight
       |
       v
Read sensors
       |
       +---- Correction required ---> Adjust steering temporarily
       |
       +---- No correction ---------> Continue straight
```

This produced smoother movement than repeatedly stopping the entire robot before each steering action.

The amount and duration of the steering correction had to be tuned experimentally because excessive steering could produce an unnecessarily large swerve.

---

## 4.5 Encoder / Target-Based Movement

During later development, we also experimented with `runTarget()`-based control.

Instead of relying entirely on a fixed time delay, target-based movement can command a motor to move by a defined amount.

This was used during development of the Open Challenge navigation logic.

The advantage of target-based movement is greater repeatability compared with using time alone.

However, the required values still have to be calibrated physically because actual vehicle movement depends on factors such as:

* wheel geometry;
* motor behaviour;
* steering geometry;
* drivetrain friction;
* surface conditions;
* mechanical tolerances.

---

## 4.6 Wall-Distance Correction

The ToF sensors were later incorporated into steering decisions.

Conceptually, the vehicle attempts to remain within a useful distance range from the wall.

```text
Measure distance
      |
      v
Is vehicle too close?
  |              |
 YES             NO
  |              |
Steer away       v
             Is vehicle too far?
                |          |
               YES         NO
                |          |
          Steer toward   Maintain path
```

This provides a feedback mechanism instead of relying only on a predetermined steering sequence.

By **21 August**, the team was actively connecting ToF distance readings to the turning of the wheels.

---

## 4.7 Heading Correction

The IMU is used to determine whether the vehicle's actual heading differs from its intended heading.

Conceptually:

```text
Desired Heading
       |
       v
Compare with IMU Heading
       |
       v
Calculate Heading Error
       |
       +---- Error left ----> Correct right
       |
       +---- Error right ---> Correct left
       |
       +---- Small error ---> Continue
```

The heading reference cannot remain constant for the entire course.

When the vehicle deliberately turns a corner, its new heading becomes intentional.

This was discovered experimentally on **12 August**, when the vehicle attempted to steer back toward its original starting direction.

The heading logic was modified so that navigation could continue after intentional turns.

---

## 4.8 Obstacle Strategy

For the Obstacle Challenge, the HuskyLens provides information about coloured obstacles.

When a relevant object is recognised, the software selects a steering response based on the detected colour.

The general sequence is:

```text
Drive
 |
 v
Read HuskyLens
 |
 v
Obstacle detected?
 |
 +---- No ----> Continue normal navigation
 |
 +---- Yes
          |
          v
     Identify colour
          |
          v
 Select steering response
          |
          v
Pass obstacle
          |
          v
Return to normal navigation
```

The obstacle response works together with the other navigation sensors rather than operating completely independently.

The camera provides information about the obstacle, while ToF and heading data help maintain the vehicle's overall position and direction.

---

# 5. Systems Thinking & Engineering Decisions

Our development process involved repeated trade-offs between mechanical simplicity, reliability, sensing accuracy, software complexity, and the limited time available for testing.

Several major engineering decisions resulted directly from problems discovered during testing.

---

## 5.1 Choosing Ackermann Steering

We selected Ackermann steering rather than relying on differential steering.

This increased mechanical complexity because the steering linkage had to be built, aligned, and calibrated.

However, it provided a vehicle architecture that behaves more like a normal car and allowed propulsion and steering to be controlled independently.

The disadvantage was that mechanical steering reliability became critical.

This resulted in several rebuilds, but ultimately gave us a controllable front-steering system.

---

## 5.2 Mechanical Reliability Before Software Compensation

A recurring issue was the temptation to adjust software whenever the robot did not travel correctly.

However, several failures were caused by hardware rather than code.

Examples included:

* a rear wheel not moving;
* damaged steering;
* unstable axles;
* incorrect gear alignment;
* chassis imbalance.

We therefore adopted the principle that hardware faults should first be corrected mechanically before software calibration is attempted.

The major chassis rebuild on **29 July** was an example of this approach.

---

## 5.3 Sensor Mounting as Part of Sensor Design

The HuskyLens initially required additional support to remain stable.

An axle was introduced to stabilise it, but the first arrangement prevented sufficient adjustment.

The mount was therefore changed.

This showed us that using a sensor is not only an electrical or programming problem. Its physical mounting affects the quality of the data received by the software.

---

## 5.4 Time-Based vs Target-Based Control

Early steering experiments used motor power together with fixed delays.

This approach was simple and easy to test.

However, time-based movement can change as mechanical conditions and battery behaviour change.

Later development therefore experimented with target-based motor control using `runTarget()`.

Target-based movement can provide more repeatable motor movement, although the required target values still need physical calibration.

---

## 5.5 Continuous Movement vs Stop-and-Turn

Another software decision involved whether to stop the vehicle before each steering movement.

Early approaches that repeatedly stopped the drive system made movement less smooth.

Later software kept the drive motor running and applied temporary steering corrections while the vehicle continued moving.

This improved continuity of motion and more closely matched the behaviour required for autonomous driving.

---

## 5.6 Combining Multiple Sensors

No single sensor provides all the information required for reliable navigation.

The HuskyLens provides visual information.

The ToF sensors provide distance information.

The IMU provides heading information.

Using these different sensing methods together reduces dependence on one type of measurement.

For example, a colour sensor may recognise an obstacle but cannot by itself tell the controller how far the vehicle is from a wall.

Similarly, an IMU can provide heading but cannot identify a coloured obstacle.

This led to a multi-sensor architecture where each sensor has a specialised role.

---

## 5.7 Debugging the Heading System

One of the most useful software failures occurred during IMU development.

The first heading-correction logic attempted to return the robot to its initial starting direction even after a deliberate corner.

The software was behaving according to its original rule, but the rule itself was incorrect for lap navigation.

We therefore changed the concept from:

> Always return to the original heading.

to:

> Maintain the current desired heading and update that desired heading after an intentional turn.

This was an example of changing the control model rather than simply tuning a numerical parameter.

---

## 5.8 Development Environment Reliability

Software development also involved significant toolchain troubleshooting.

Problems included:

* Arduino IDE version compatibility;
* ESP32 board-package compatibility;
* EVO library installation;
* compilation errors;
* USB connection issues;
* incorrect or unavailable serial ports;
* slow uploads;
* controller resets;
* BOOT-button problems;
* USB cables that did not provide reliable data connections.

At one point VS Code was also tested as an alternative environment, but it did not solve the underlying problem.

Eventually the upload procedure became more reliable.

These issues were documented because reproducibility requires not only source code but also an explanation of how that source code is actually transferred to the controller.

---

# 6. Build, Compile and Upload Process

The control software was developed using the Arduino development environment.

The following procedure was used during development.

## 6.1 Arduino Environment

We used **Arduino IDE 2.x**.

The ESP32 board support package from **Espressif Systems** was required because the controller is ESP32-based.

During development, ESP32 package compatibility caused problems with the EVO library, so package versions had to be tested when compilation errors occurred.

---

## 6.2 EVO Library

The EVO Arduino library was installed manually from the project's GitHub release package rather than relying only on Arduino Library Manager.

The library provides the interfaces used to communicate with EVO hardware such as motors, sensors, and display functionality.

During early setup, compatibility problems were encountered between some versions of the ESP32 package and the EVO library.

One library issue also involved serial communication definitions, which required troubleshooting before compilation could proceed correctly.

---

## 6.3 Project Files

The Arduino project contains the main `.ino` sketch and supporting header files such as `arm.h`.

The files must remain within the same Arduino project directory so that the compiler can include them correctly.

The general project structure is:

```text
RobotProject/
│
├── RobotProject.ino
└── arm.h
```

Later challenge-specific versions of the software can be stored separately in this repository.

---

## 6.4 Compilation

Before uploading, the Arduino IDE compiles the C++ source code and all required libraries.

During one development version, compilation used approximately:

* **369,295 bytes of program storage**
* **23,616 bytes of dynamic memory**

This showed that the controller still had significant memory available for additional functionality.

Compilation errors encountered during development included:

* an incorrectly terminated header file;
* local motor declarations conflicting with global hardware objects;
* motor-type mismatches;
* library compatibility problems;
* serial-interface errors.

These were corrected before upload testing continued.

---

## 6.5 Connecting the Controller

The EVOX1 controller is connected to the computer using a USB data cable.

A charge-only cable cannot be used for programming because it provides power without the data connection required for uploading software.

After connecting the controller, the correct serial port must be selected in Arduino IDE.

On macOS, the port appears as a `/dev/cu...` device.

A previously selected port may disappear after reconnecting the board, so the port must sometimes be selected again.

Bluetooth serial ports should not be selected for USB programming.

---

## 6.6 Uploading

The upload procedure is:

1. Connect the EVOX1 controller using a USB data cable.
2. Open the Arduino project.
3. Select the appropriate ESP32/EVO board configuration.
4. Select the correct USB serial port.
5. Compile the project.
6. Upload the compiled firmware to the controller.
7. Reset the controller if required.
8. Observe the robot and Serial Monitor output.
9. Modify the code or calibration values.
10. Compile and upload again.

During later testing, the controller sometimes had to be reset before accepting another upload.

The BOOT button was also unreliable during some sessions.

---

## 6.7 Serial Debugging

Serial communication was initialised at:

```cpp
Serial.begin(115200);
```

Serial output was used to observe values such as:

* colour-detection results;
* ToF readings;
* sensor status;
* program states;
* debugging messages.

The Serial Monitor therefore became an important tool for distinguishing software problems from hardware problems.

---

# 7. Reproducibility

This repository is designed to provide enough information for another team to understand the architecture and development process of our autonomous vehicle.

A team attempting to reproduce the system would require the following major components:

* EVOX1-compatible controller;
* four-wheel chassis;
* rear-wheel propulsion system;
* Ackermann-style front steering mechanism;
* propulsion motor;
* steering motor;
* HuskyLens vision sensor;
* Time-of-Flight distance sensors;
* IMU / gyroscope;
* appropriate battery and power connections;
* USB data cable for programming;
* structural parts, axles, gears, and wiring.

The software environment requires:

* Arduino IDE 2.x;
* ESP32 board support;
* EVO Arduino library;
* any sensor libraries required by the final ToF and IMU implementation.

The general reproduction process is:

```text
Build chassis
     |
     v
Construct Ackermann steering
     |
     v
Install propulsion system
     |
     v
Mount EVOX1 controller
     |
     v
Install HuskyLens + ToF + IMU
     |
     v
Complete wiring
     |
     v
Install Arduino environment
     |
     v
Install required libraries
     |
     v
Upload code
     |
     v
Train / calibrate sensors
     |
     v
Calibrate steering
     |
     v
Test navigation
     |
     v
Tune control values
```

Calibration is necessary because mechanical tolerances vary between physical builds.

Another robot using the same source code may therefore require different steering, timing, or target values.

---

# 8. Development Timeline

Our complete chronological development record is stored separately in:

[Development Timeline](documentation/development-timeline.md)

The timeline records work completed between April and August 2026, including:

* initial research;
* chassis construction;
* Ackermann steering development;
* HuskyLens mounting and programming;
* drivetrain repairs;
* steering rebuilds;
* ToF integration;
* IMU integration;
* heading correction;
* chassis redesign;
* autonomous navigation development;
* upload and hardware debugging.

This record demonstrates how the final design evolved through testing rather than being produced in one step.

---

# 9. Engineering Journal

A summary of our engineering-development process is available in:

[Engineering Journal](documentation/engineering-journal.md)

The journal focuses on the relationship between problems discovered during testing and the engineering changes made in response.

---

# 10. Vehicle Photographs

The competition documentation requires photographs showing the vehicle from the:

* front;
* rear;
* left side;
* right side;
* top;
* bottom.

It also requires a team photograph.

These photographs are part of the required competition evidence and should represent the actual competition vehicle and team.

---

# 11. Autonomous Driving Videos

The competition documentation requires autonomous-driving footage for both challenge categories:

## Open Challenge

The demonstration should contain at least 30 seconds of actual autonomous driving.

## Obstacle Challenge

The demonstration should contain at least 30 seconds of actual autonomous driving, including operation of the obstacle-navigation strategy.

Video evidence must show the real autonomous vehicle.

---

# 12. Source Code

The source code for the vehicle is organised separately from the written engineering documentation.

The intended repository structure is:

```text
src/
├── open-challenge/
└── obstacle-challenge/
```

The code is written in Arduino-style C++ and should be commented so that readers who are unfamiliar with our specific development environment can understand:

* hardware initialisation;
* motor control;
* steering control;
* HuskyLens colour detection;
* ToF distance measurement;
* IMU heading measurement;
* navigation decisions;
* turning behaviour;
* obstacle handling.

The separation between hardware definitions and main control logic also makes the relationship between software modules and physical vehicle components easier to understand.

---

# 13. Development Summary

The development of our WRO Future Engineers vehicle required extensive iteration.

The project began with chassis design and Ackermann steering construction.

The next stage involved basic motor movement and HuskyLens colour recognition.

Mechanical failures then required repeated work on the steering system and drivetrain.

As reliability improved, ToF distance sensing and IMU heading measurements were introduced.

The software progressed from basic timed steering actions toward continuous driving, sensor-based steering corrections, heading control, target-based motor movement, and autonomous lap-navigation logic.

Some of the most valuable lessons came from failures.

We learned that:

* mechanical reliability must come before precise software calibration;
* sensor mounting affects software performance;
* multiple sensors provide complementary information;
* heading control requires a changing reference after intentional turns;
* steering motors must respect mechanical limits;
* development tools and upload procedures are part of system reproducibility;
* major hardware redesign can sometimes solve a software-control problem more effectively than additional code.

The final robot is therefore the result of an iterative engineering process involving mechanical construction, electronics, programming, testing, debugging, and redesign.
