/*
 * I2C slave. This program reads the motor controller commands coming
 * in from the main Arduino via i2C. The low current Arduino sends 
 * the four motor commands in their own respective message.
 * 
 * Msg Template: 
 *  Q    : start flag
 *  1-4  : thruster number
 *  XXXX : thruster speed magnitude
 *  F/R  : direction & end flag
 * 
 * Example Output per line: Q13000FQ2500R Q33000FQ4500R 
 * 
 */

#include <Wire.h>

// ------------ CONSTANTS ------------------
#define SLAVE_ADDR 9
bool debug = false;

// -------- SYSTEM VARIABLES ---------------
const int messageSize = 31;
volatile char message[messageSize]; 
int i = 0; 

// -------- MESSAGE VARIABLES --------------
String q1Msg;
String q2Msg;
String q3Msg;
String q4Msg;
int q1Motor; 
int q2Motor; 
int q3Motor; 
int q4Motor; 
char q1Dir;
char q2Dir; 
char q3Dir;
char q4Dir;

void setup() {
 Wire.begin(SLAVE_ADDR);
 Wire.onReceive(readMotorCmds);
 Serial.begin(9600);
}

void loop() {
  getMotorCmds(); 
  getMotorDirs();
}
