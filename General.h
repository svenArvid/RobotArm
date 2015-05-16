// Include some needed Arduino stuff and define general functions

#ifndef GENERAL_H
#define GENERAL_H

#define LIB_VERSION 1.0

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include <pins_arduino.h>
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

// Declare general functions
int Sign(float var);

#endif



