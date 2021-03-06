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
#define SLAVE_ADDR 9     // arbitrary address number to identify slave Arduino

// -------- SYSTEM VARIABLES ---------------
int q1Out = 3000;    // Q1 motor command
int q2Out = 500;     // Q2 motor command
int q3Out = 3000;    // Q3 motor command
int q4Out = 500;     // Q4 motor command
int neutral = 2048;  // From 0-4096, 2048 is neutral

// -------- MOTOR SPEED COMMANDS -----------
char dir = 'N';       // variable for direction
String q1Msg = "Q1";  // Q1 Serial message
String q2Msg = "Q2";  // Q2 Serial message
String q3Msg = "Q3";  // Q3 Serial message
String q4Msg = "Q4";  // Q4 Serial message
String message = "";  // message coming from low current Arduino 
bool flag = true; 
int numOfBits = 30;   // number of bits to request from slave

void setup() {
  Wire.begin();        // join the I2C bus as a master (master device has no paramter)
  Serial.begin(9600);  // establish serial communication with baud rate as the parameter
}

void loop() {
//  flag = !flag;
//  if(flag == true){
//    q1Out = 1000;
//    q2Out = 1000; 
//    q3Out = 1000;
//    q4Out = 1000;
//  }
//  else{
//    q1Out = 3000;
//    q2Out = 500;
//    q3Out = 3000;
//    q4Out = 500;
//  }
//  q1Msg += commandToMsg(q1Out);  // "Q1" + "3000F" = "Q13000F"
//  q2Msg += commandToMsg(q2Out);  // "Q2" + "500R"  = "Q20500R"
//  q3Msg += commandToMsg(q3Out);  // "Q3" + "3000F" = "Q33000F"
//  q4Msg += commandToMsg(q4Out);  // "Q4" + "500R"  = "Q40500R"
//  sendMsgs();
//  msgReset();

  message = requestFromSlave();
  Serial.println(message);
}


// --------- FUNCTIONS -----------------
String commandToMsg(int motor){
  /***
   *  Converts motor commands into serial messages. Checks to see if
   *  the motor command is greater/less than neutral point to determine
   *  whether F/R character should be used. 
   *  
   *  Inputs:
   *   - motor <int>: motor command
   *   
   *  Return:
   *   - msg <String>: motor magnitude and direction as a String
   */
  String msg; 
  if (motor<1000){
    msg = "0" + String(motor);
  }
  else{
    msg = String(motor);
  }
  
  if (motor < neutral){
    dir = 'R';
    return msg += dir;
  }
  if (motor > neutral && motor <= 4096){
    dir = 'F';
    return msg += dir;
  }
  else{
    dir = 'N';
    return msg += dir;
  }
}

void msgReset(){
  /***
   * Function to reset the serial messages after each loop. 
   */
  q1Msg = "Q1";
  q2Msg = "Q2";
  q3Msg = "Q3";
  q4Msg = "Q4";
}

void sendMsgs(){
  /***
   * Function to send serial messages for all four motor commands.
   * It iterates over the serial message Strings because Serial.write
   * cannot take Strings as input.
   */

  Wire.beginTransmission(SLAVE_ADDR);
  // Send Q1 messages
  for(int i=0; i<=7; i++){
    Wire.write(q1Msg[i]);
    Serial.print(q1Msg[i]);
  }
  // Send Q2 messages
  for(int i=0; i<=7; i++){
    Wire.write(q2Msg[i]);
    Serial.print(q2Msg[i]);
  }  
  // Send Q3 messages
  for(int i=0; i<=7; i++){
    Wire.write(q3Msg[i]);
    Serial.print(q3Msg[i]);
  }
  // Send Q4 messages
  for(int i=0; i<=7; i++){
    Wire.write(q4Msg[i]);
    Serial.print(q4Msg[i]);
  }
  Wire.endTransmission();
  Serial.println();
}

String requestFromSlave(){
  // Function to request motor commands to send to motors from
  // Arduino in low current box. 
  String newMessage = "";
  Wire.requestFrom(SLAVE_ADDR, numOfBits); // request numOfBits (30) from SLAVE_ADDR (9)

  while(Wire.available()){
    char c = Wire.read();  // receive bit as a character
    newMessage += c;       // add new character into message string
  }
  
  return newMessage;
}
