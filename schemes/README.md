# System Architecture

The vehicle uses an EVOX1 controller to combine sensor inputs with motor control.

```text
HuskyLens --------\
                   \
ToF Sensors --------> EVOX1 Controller ---> Drive Motor ---> Rear Wheels
                   /
IMU / Gyroscope ---/                    ---> Steering Motor ---> Ackermann Front Steering
