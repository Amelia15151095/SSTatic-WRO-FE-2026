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

Our vehicle is an autonomous four-wheel robot designed for the WRO Future Engineers competition. It uses a rear-wheel propulsion system together with an Ackermann front steering mechanism, where the rear drive system provides forward movement and a separate steering motor controls the angle of the front wheels. We chose this arrangement because Ackermann steering operates similarly to a conventional road vehicle, with the inner and outer front wheels following different turning radii during a turn. The main controller is an EVOX1-based controller programmed using Arduino-style C++. The vehicle combines several sensing systems, including the HuskyLens vision sensor for recognising coloured objects, Time-of-Flight (ToF) sensors for measuring distances from walls and nearby objects, and an IMU/gyroscope for measuring the vehicle’s heading and orientation. The controller combines these sensor inputs with commands to the propulsion and steering motors. Throughout development, the software progressed from simple forward-driving and colour-recognition experiments to more advanced navigation involving steering corrections, distance sensing, heading control, turning, and autonomous lap-navigation logic.

---

# 2. Mobility & Mechanical Design

## 2.1 Initial Chassis Development

Construction of the vehicle began on 29 April 2026. During the first development session, we studied the competition requirements, researched different vehicle designs, prepared a parts list, and began constructing the base of the robot. We selected Ackermann steering as our steering concept from the beginning, but one of our first challenges was designing a suitable steering system using the available components. Since it was difficult to find existing designs that matched our exact requirements, and the steering gears initially did not align correctly, the mechanism had to be developed through testing and modification rather than simply copying an existing design. On 6 May, we modified the steering system because the height of the front steering assembly did not properly match the rear wheels. We tested different combinations of structural components until the front and rear sections of the chassis were correctly aligned, after which the base and wiring were completed. During later testing, we also found that the balance of the chassis affected the vehicle’s behaviour. On 13 May, we researched previous successful WRO Future Engineers vehicles and modified parts of our own vehicle to improve its balance. This became an important theme throughout the project, as we learned that mechanical reliability had to be established before the software could produce consistent and repeatable behaviour.

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

On 1 July, for example, only one rear wheel was moving properly. The rear-wheel mechanism had to be repaired before meaningful software testing could continue.

This taught us an important engineering lesson: software cannot reliably compensate for an inconsistent mechanical system.

If an axle moves or a wheel does not receive power correctly, changing steering values in the code does not solve the underlying mechanical problem. Therefore, we repaired and redesigned the mechanical system where necessary instead of attempting to correct every issue through software.

---

## 2.3 Ackermann Steering

The front wheels use an Ackermann-style steering system, which allows the inside and outside front wheels to follow different turning radii, with the inside wheel travelling around a smaller radius and the outside wheel following a larger one. A dedicated motor controls the steering mechanism, which became one of the most heavily modified parts of the vehicle throughout development. The first version experienced gear-alignment problems, leading to changes in the steering geometry and height on 6 May. On 29 May, the steering mechanism was damaged and required a temporary repair, before breaking again on 1 July and needing to be rebuilt. By 10 July, we had developed a more permanent steering structure, and on 17 July the front steering was repaired and modified again to increase the available turning range. Finally, on 29 July, steering calibration was completed while the overall vehicle structure was rebuilt into its final mechanical form. Software testing also revealed that continuously driving the steering motor at high power could force the mechanism against its physical limit, placing unnecessary stress on the system and potentially causing damage or unwanted movement. We therefore moved towards controlled steering movements and short steering corrections instead of continuously forcing the motor in one direction. This improved the reliability and control of the steering system during autonomous driving.

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

On 29 July, the entire structure of the robot was rebuilt.

This was not simply an aesthetic redesign. The hardware was modified specifically so that the software could control the vehicle more consistently.

This interaction between mechanical design and control software influenced many of our later engineering decisions.

---

# 3. Power & Sensor Architecture

## 3.1 Main Controller

The main controller used in the vehicle is an EVOX1-based controller.

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

The vehicle is powered through its onboard battery and EVOX1 hardware, which distributes electrical power to the controller, motors, and sensors. Power and connection reliability became increasingly important during testing because intermittent hardware connections sometimes prevented the robot from uploading or operating correctly. During later development, the robot occasionally had to be reset before new software could be uploaded, and the BOOT button also became unreliable during some testing sessions. We treated these issues separately from the software logic because successful compilation does not always mean that the controller can physically receive or run the program.

---

## 3.3 HuskyLens Vision Sensor

The HuskyLens was originally added to the vehicle for the Obstacle Challenge, where it was intended to recognise coloured obstacles and trigger different steering responses. During development, however, our team decided not to continue with the Obstacle Challenge and instead focused on completing the Open Challenge reliably. Rather than removing the HuskyLens completely, we continued experimenting with it as an additional source of navigation information alongside the Time-of-Flight sensors and IMU. This change reflected the iterative nature of our project, as we explored how an existing component could still contribute to the final vehicle instead of redesigning the entire sensor system. The final Open Challenge navigation strategy therefore focused mainly on reliable lap navigation using steering control, distance sensing, and heading correction, while the HuskyLens remained part of our development and experimentation with visual sensing.

---

## 3.4 Time-of-Flight Sensors

Time-of-Flight sensors were introduced to measure the distance between the robot and surrounding walls or objects. Unlike the HuskyLens, which provides visual information, the ToF sensors provide numerical distance measurements. This allows the controller to estimate whether the vehicle is too close to or too far from a wall.

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

Programming of the ToF sensors began in July, with development starting by 10 July and the sensors being incorporated into navigation experiments by 21 July. However, the ToF sensors did not work reliably from the beginning, and testing in August revealed issues with the distance readings. On 14 August, we identified a problem affecting the ToF system and continued troubleshooting to improve its reliability. By 21 August, the ToF distance readings were being directly connected to the vehicle’s steering behaviour, allowing the sensors to play a more active role in autonomous navigation.

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

An IMU/gyroscope was added to measure the vehicle’s orientation and heading, providing different information from the ToF sensors. While the ToF sensors determine how far the robot is from a wall, the IMU determines the direction in which the robot is pointing. This heading information can then be used to detect unwanted rotation and correct the vehicle’s direction while driving. The IMU was physically introduced to the vehicle on 17 July and became an important part of the navigation system as development progressed.

We had a pseudocode to plan out what we needed to code:

main(){
loop(if left tof > xcm or right tof > xcm)(
	Steer target(imu)
	Check left tof if there is a wall within x cm  
		steer right 	
	Check right tof I there is a wall within x cm
		steer left
	Check if left tof is greater than x cm	
)


Further IMU programming was completed during August, with one significant software problem discovered on 12 August. Our heading-correction logic was initially steering the robot back towards its original starting direction because the software continued treating the starting heading as the desired heading even after the vehicle had intentionally completed a turn. This showed that the control system could not rely on one permanent heading throughout the course. Instead, the desired heading needed to be updated after each deliberate turn. We therefore modified the correction logic so that the robot could distinguish between an unwanted heading error and an intentional change in direction. This was an important improvement to the autonomous navigation system and allowed the vehicle to continue navigating more effectively after making turns.

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

The HuskyLens identifies visual information, while the ToF sensors measure distance and the IMU provides orientation information

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

The earliest software focused on basic movement and colour recognition. The vehicle was programmed to move forward while continuously checking the HuskyLens for recognised colours. When the HuskyLens detected a specific colour, the program responded by applying the appropriate steering action. This provided a simple starting point for developing the vehicle’s autonomous navigation system and allowed us to test the interaction between movement, sensor input, and steering control.

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

During later development, we also experimented with runTarget()-based control as part of the Open Challenge navigation logic. Instead of relying entirely on fixed time delays, target-based movement allows the motor to be commanded to move by a defined amount. This provided greater control over the vehicle’s movement and improved repeatability compared with using time-based control alone. However, the target values still needed to be tested and calibrated to ensure that the commanded movement matched the actual movement of the vehicle.

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

By 21 August, the team was actively connecting ToF distance readings to the turning of the wheels.

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

The heading reference could not remain constant throughout the entire course because the vehicle’s direction changes intentionally when it turns a corner. We discovered this experimentally on 12 August, when the vehicle attempted to steer back towards its original starting direction instead of continuing in the new direction. This showed that the original heading should not always be treated as the target. We therefore modified the heading logic so that the desired heading could be updated after intentional turns, allowing the vehicle to continue navigating correctly around the course.

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

We selected Ackermann steering instead of relying on differential steering because it provided a vehicle architecture that behaved more like a conventional car and allowed propulsion and steering to be controlled independently. However, this increased the mechanical complexity, as the steering linkage had to be carefully built, aligned, and calibrated. The main disadvantage was that the reliability of the mechanical steering system became critical, which resulted in several rebuilds and adjustments throughout development. Despite this, the final design gave us a controllable front-steering system that suited the requirements of the vehicle.

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

We therefore adopted the principle that hardware faults should first be corrected mechanically before software calibration is attempted, hence the major chassis rebuild on 29 July.

---

## 5.3 Sensor Mounting as Part of Sensor Design

The HuskyLens initially required additional support to remain stable during operation. We introduced an axle to improve its stability, but the first mounting arrangement limited the amount of adjustment available. The mount was therefore redesigned to provide better positioning and flexibility. This showed us that using a sensor is not only an electrical or programming challenge, as its physical mounting can also affect the quality and consistency of the data received by the software.

---

## 5.4 Time-Based vs Target-Based Control

Early steering experiments used motor power together with fixed delays, which provided a simple and easy way to test the vehicle’s movement. However, time-based movement could vary depending on factors such as mechanical conditions and battery behaviour. As development progressed, we experimented with target-based motor control using runTarget(). This approach allowed us to control the motors based on a specific target position, making movement more repeatable. However, the target values still needed to be physically calibrated to ensure that the vehicle moved the required distance accurately.

---

## 5.5 Continuous Movement vs Stop-and-Turn

Another important software decision was whether the vehicle should stop before each steering movement. In our earlier approaches, repeatedly stopping the drive system made the vehicle’s movement less smooth and interrupted its motion. We later changed the software so that the drive motor remained running while temporary steering corrections were applied as the vehicle continued moving. This resulted in smoother and more continuous movement, while also being more suitable for the behaviour required during autonomous driving.

---

## 5.6 Combining Multiple Sensors

No single sensor can provide all the information needed for reliable navigation, so we combined multiple sensing methods, with each sensor having a specific role. The HuskyLens provides visual information, the ToF sensors provide distance measurements, and the IMU provides heading information. Using these sensors together reduces the vehicle’s dependence on any one type of measurement. For example, the HuskyLens can recognise an obstacle but cannot determine the exact distance between the vehicle and a wall, while the IMU can provide heading information but cannot identify a coloured obstacle. This led us to develop a multi-sensor architecture where each sensor contributes different information to improve the overall reliability of the navigation system.

---

## 5.7 Debugging the Heading System

One of the most useful software failures occurred during the development of the IMU heading control. Our first heading-correction system attempted to return the robot to its original starting direction even after it had intentionally made a corner. Although the software was following the rule we had programmed, the rule itself was not suitable for lap navigation. We therefore changed the control approach so that the robot maintained a current desired heading and updated this heading after each intentional turn. This showed us that improving the system was not always about adjusting numerical values, but sometimes required changing the control model itself.

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

The EVO Arduino library was installed manually using the project’s GitHub release package instead of relying only on the Arduino Library Manager. The library provides the interfaces needed to communicate with EVO hardware, including the motors, sensors, and display. During the initial setup, we encountered compatibility issues between certain versions of the ESP32 package and the EVO library. There was also an issue involving serial communication definitions within the library, which required troubleshooting before the code could compile correctly. These setup problems helped us understand the importance of using compatible library and board package versions.

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

The EVOX1 controller is connected to the computer using a USB data cable, as a charge-only cable only provides power and does not support the data connection needed to upload software. After connecting the controller, the correct serial port must be selected in Arduino IDE. On macOS, the controller appears as a /dev/cu... device. The selected port may sometimes disappear after the board is disconnected and reconnected, meaning it has to be selected again. For USB programming, Bluetooth serial ports should not be selected.

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

# 10. Source Code

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

# 11. Development Summary

The development of our WRO Future Engineers vehicle involved a lot of testing and iteration. We started with the chassis design and construction of the Ackermann steering system, before moving on to basic motor movement and HuskyLens colour recognition. After encountering several mechanical failures, we repeatedly modified and improved the steering system and drivetrain. As the vehicle became more reliable, we introduced ToF distance sensing and IMU heading measurements. At the same time, the software developed from basic timed steering actions to continuous driving, sensor-based steering corrections, heading control, target-based motor movement, and eventually autonomous lap-navigation logic. Throughout the project, some of the most valuable lessons came from the failures we experienced and the improvements we made from them.

We learned that:

* mechanical reliability must come before precise software calibration;
* sensor mounting affects software performance;
* multiple sensors provide complementary information;
* heading control requires a changing reference after intentional turns;
* steering motors must respect mechanical limits;
* development tools and upload procedures are part of system reproducibility;
* major hardware redesign can sometimes solve a software-control problem more effectively than additional code.

The final robot is therefore the result of an iterative engineering process involving mechanical construction, electronics, programming, testing, debugging, and redesign.
