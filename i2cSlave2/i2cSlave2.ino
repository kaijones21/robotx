/*
 * I2C master. This program takes the motor controller outputs within 
 * the range of 0-4096 (with 2048 being neutral), creates a string for 
 * each output, and sends it to the high current Arduino via I2C
 * communication protocol. 
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

// -------- CONSTANTS ----------------------
#define SLAVE_ADDR 9    // arbitrary address number to identify slave Arduino
#define ANSWERSIZE 28   // answer size of message to send

// -------- SYSTEM VARIABLES ---------------
int q1Out = 3000;    // Q1 motor command
int q2Out = 500;     // Q2 motor command
int q3Out = 3000;    // Q3 motor command
int q4Out = 500;     // Q4 motor command
int neutral = 2048;  // From 0-4096, 2048 is neutral

// -------- MOTOR SPEED COMMANDS -----------
char dir = 'N';       // variable for direction
String q1Msg = "Q1";
String q2Msg = "Q2";
String q3Msg = "Q3";
String q4Msg = "Q4";
String temp = ""; 
volatile String message = "";
bool flag = true; 

void setup() {
  Wire.begin(SLAVE_ADDR);   // join the I2C bus as a slave with address as SLAVE_ADDR
  Wire.onRequest(sendMsgs); // send motor command messages when master requests
  Serial.begin(9600);       // establish serial communication with baud rate as the parameter
}

void loop() {
  flag = !flag;
  if(flag == true){
    q1Out = 1000;
    q2Out = 1000; 
    q3Out = 1000;
    q4Out = 1000;
  }
  else{
    q1Out = 3000;
    q2Out = 500;
    q3Out = 3000;
    q4Out = 500;
  }

  getMsgs();
  sendMsgs();
  msgReset();
}
