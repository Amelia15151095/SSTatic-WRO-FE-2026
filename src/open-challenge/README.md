# Open Challenge Software

This directory contains the software used for development of Team SSTatic's WRO Future Engineers 2026 Open Challenge vehicle.

The main Arduino sketch contains the autonomous navigation logic.

The software interfaces with the vehicle's:

- rear-wheel drive motor;
- Ackermann steering motor;
- Time-of-Flight sensors;
- IMU / gyroscope;
- HuskyLens where used during development.

The program was developed iteratively through physical testing. Control values therefore reflect calibration performed on the actual vehicle.

For a detailed explanation of the software architecture and its relationship to the electromechanical components, see the main repository README.
