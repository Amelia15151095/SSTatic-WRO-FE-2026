# Engineering Journal

This engineering journal explains the major design decisions, testing process, failures, modifications, and lessons learned during the development of our WRO Future Engineers 2026 autonomous vehicle.

Our vehicle was developed iteratively. Mechanical design, electronics, sensors, and software were tested together, and changes to one subsystem often required changes to another.

For example, the steering system was redesigned several times because the original geometry and gear alignment were not sufficiently reliable. The final steering system was calibrated after rebuilding the vehicle structure in July.

The sensing system also evolved over time. The HuskyLens was first used for colour recognition, followed by the integration of Time-of-Flight sensors for distance measurement and an IMU for heading correction.

Software development followed the same iterative process. Early code focused on basic motor movement and colour detection. Later versions introduced continuous steering corrections, sensor-based wall detection, heading control, and autonomous lap navigation.

A detailed chronological record of each development session is available here:

[Development Timeline](development-timeline.md)
