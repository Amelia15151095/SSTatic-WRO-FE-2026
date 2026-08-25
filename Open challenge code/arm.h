#ifndef ARM_H
#define ARM_H

#include <Evo.h>

EvoMotor steering(M3, EV3MediumMotor, true);
EvoMotor drive(M4, EV3MediumMotor, true);
EvoHuskyLens hl(I2C1);
HUSKYLENSResult result;
EvoIMU imuu(I2C5);
EvoTOF tofFront(I2C6);
EvoTOF tofLeft(I2C7);
EvoTOF tofRight(I2C8);

void waitForBump(){
  while (!digitalRead(GPIO1)){}
  while (digitalRead(GPIO1)){}
}

#endif
