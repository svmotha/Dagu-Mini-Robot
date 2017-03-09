#include "Constants.h"
#include "IOpins.h"

// Colour sensor 
const int s0 = 3, s1 = 4, s2 = 5, s3 = 6;       // Colour sensor pin connections
const int colourPin = 2;

// Ultrasonic distance sensor
const int trigPin = 12;                         // Ultrasonic sensor pin connections                       
const int echoPin = 11;

// Servo motor
const int srvoMotorPin = 18;                    // Servo motor digital pin connection 

// Battery
int volts;                                      // battery voltage
int pause = -1;                                 // a value of -1 means the battery voltage is ok, 
                                                // 0 and 1 are used to flash LED on D13 when the battery is flat 
                                                                             
// ******************************************************************************** 
// SETUP and LOOP FUNCTIONS 
// ********************************************************************************

void setup()
{  
  //Serial.begin(9600);                            // initialize serial communication

  // Setup motor direction pins as outputs
  pinMode(Ldirpin,OUTPUT);                       // set left  direction pin as an output pin
  pinMode(Rdirpin,OUTPUT);                       // set right direction pin as an output pin

  // Setup colour sensor pins as inputs and outputs
  pinMode(s0,OUTPUT);
  pinMode(s1,OUTPUT);
  pinMode(s2,OUTPUT);
  pinMode(s3,OUTPUT);
  pinMode(colourPin,INPUT);
    
  // Set colour sensor output frequency
  digitalWrite(s1,HIGH);
  digitalWrite(s0,LOW);
  
  // Set servo control pin as an output pin
  pinMode(srvoMotorPin,OUTPUT);        

  delay(1000);                              // Wait 1 second before starting the loop function.  
}

void loop()
{ 
  // -------------------------------------------------------------------------
  /* Battery Check.
   * !Low battery voltages will damage your controller! 
   * The robot will stop moving if the battery voltage is below a certain value. 
   * The LED on pin D13 will also flash on and off. 
   * !Do not tamper with this code!
   */
  // This code is modified from:
  // https://sites.google.com/site/daguproducts/home/Software/Doodle_Bot_Text_Demo.zip?attredirects=0  
  // -------------------------------------------------------------------------
  
  volts = analogRead(Battery)/10;           // Battery voltage reading
  
  if((volts < lowbat) || (pause != -1))     // once voltage falls below lowbat value
  {
    // Stop the robot
    digitalWrite(Ldirpin,0);                // set left direction 
    analogWrite (Lpwmpin,0);                // set left speed to zero
    digitalWrite(Rdirpin,0);                // set right direction 
    analogWrite (Rpwmpin,0);                // set right speed to zero

    pause++;                                // increment pause
    if(pause > 1)                           // toggle pause between 0 and 1 once battery is flat
    {
      pause = 0;                           
    }
    
    digitalWrite(13,pause);                 // flash LED on D13
    delay(200);                             // set flashing LED speed
    return;                                 // reset loop
  }   
                                            
  robotProject();                           // The robotProject function contains your project code
}

// ******************************************************************************** 
// ROBOTPROJECT FUNCTION 
// ******************************************************************************** 
void robotProject()
{
  goSomewhere(0, 0, 45, 90, 2000);
  goSomewhere(0, 0, 0, 0, 1000);
  goSomewhere(1, 1, 45, 90, 2000);
  goSomewhere(0, 0, 0, 0, 1000);
  goSomewhere(0, 0, 0, 90, 500);
  goSomewhere(0, 0, 0, 0, 1000);
  goSomewhere(0, 0, 45, 0, 500);
  goSomewhere(0, 0, 0, 0, 1000);
}

// ******************************************************************************** 
// FUNCTIONS 
// ******************************************************************************** 
 
// ********************************************************************************
// Robot speed and direction functions
// ******************************************************************************** 

void goSomewhere(int dirBinaryLeft, int dirBinaryRight, int leftSpeed, int rightSpeed, int delayTime)
{
  /*
  | -------------------------------------------------------------------------
  | This function moves the robot in any desired direction based on user 
  | inputs. It can move:
  |                     Forward
  |                     Backwards
  |                     Turn Left
  |                     Turn Right
  |                     Stop
  | -------------------------------------------------------------------------
  */
  if (leftSpeed > 255)                          // limit maximum motor speed value 
  {
    leftSpeed = 255;
  }
  if (rightSpeed > 255)                         // limit maximum motor speed value 
  {
    rightSpeed = 255;
  }

  digitalWrite(Ldirpin, dirBinaryLeft);          // set left direction
  analogWrite (Lpwmpin, leftSpeed);              // set left speed 
  digitalWrite(Rdirpin, dirBinaryRight);         // set right direction 
  analogWrite (Rpwmpin, rightSpeed);             // set right speed
  delay(delayTime)
}

void goForward(int leftSpeed, int rightSpeed)
{  
  // -------------------------------------------------------------------------
  // This function moves your robot forward 
  // -------------------------------------------------------------------------
  
  int forwLeftSpeed = abs(leftSpeed);      // create temporary speed variable
  int forwRightSpeed = abs(rightSpeed);      // create temporary speed variable
  
  if(forwLeftSpeed > 255)                     // limit maximum motor speed value 
  {
    forwLeftSpeed = 255;
  }
  
  if(forwRightSpeed > 255)                     // limit maximum motor speed value 
  {
    forwRightSpeed = 255;
  }
   
  digitalWrite(Ldirpin,0);                // set left direction (0 is forward, 1 is reverse)
  analogWrite (Lpwmpin,leftSpeed);        // set left speed 
  digitalWrite(Rdirpin,0);                // set right direction 
  analogWrite (Rpwmpin,rightSpeed);        // set right speed
}

void stopRobot()
{
  // -------------------------------------------------------------------------
  // This function stops your robot wheels 
  // -------------------------------------------------------------------------

  digitalWrite(Ldirpin,0);                // set left direction 
  analogWrite (Lpwmpin,0);                // set left speed to zero
  digitalWrite(Rdirpin,0);                // set right direction 
  analogWrite (Rpwmpin,0);                // set right speed to zero
}


// ********************************************************************************
// Ultrasonic Distance Sensor Function
// ******************************************************************************** 

// This code is modified from:
// https://gist.github.com/flakas/3294829#file-hc-sr04-ino

float ultraSensorDist()
{
  // -------------------------------------------------------------------------
  /* This function obtains a reading in cm from the ultrasonic distance sensor
   * The sensor is triggered by a HIGH pulse of 10 or more microseconds.
   * Read the signal from the sensor: 
   * a HIGH pulse whose duration is the time (in microseconds) from the sending
   * of the ping to the reception of its echo off of an object. 
   * To convert the time into a distance:
   * The speed of sound is 340 m/s or 29.4 microseconds per centimeter.
   * The ping travels out and back, so to find the distance of the
   * object we take half of the distance travelled.
   */
   // -------------------------------------------------------------------------
   
  long duration;
  float cm;                           // variables for duration of the ping and the distance result in centimeters

  pinMode(trigPin, OUTPUT);           // Set trigPin as output
  digitalWrite(trigPin, LOW);         // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  delayMicroseconds(2);               // Duration of LOW pulse
  digitalWrite(trigPin, HIGH);        // HIGH pulse to trigger sensor
  delayMicroseconds(10);              // Duration of HIGH pulse
  digitalWrite(trigPin, LOW);         // Turn HIGH pulse off
 
  pinMode(echoPin, INPUT);            // Set echoPin as input
  duration = pulseIn(echoPin, HIGH);  // Read the signal from the sensor: 

  cm = duration / 29.4 / 2.0;         // convert the time into a distance

  delay(30);
  return cm;
}


// ********************************************************************************
// Colour Sensor Functions
// ********************************************************************************

/*  This code is modified from: 
 * http://www.dfrobot.com/wiki/index.php/TCS3200_Color_Sensor_(SKU:SEN0101)
 * and
 * http://www.toptechboy.com/arduino/lesson-15-super-cool-arduino-color-sensor-project/
 */

unsigned int colourRedVal() 
{
  // -------------------------------------------------------------------------
  /* This function obtains Red colour intensity reading from the colour sensor.
   * An object placed in front of the sensor will output values 
   * for it's red, green and blue color intensity. 
   * The three colour intensities together determine the colour of the object. 
   */
  // -------------------------------------------------------------------------
  
  digitalWrite(s2,LOW);                   // To read red value of object
  digitalWrite(s3,LOW); 
  
  unsigned int valRed = pulseIn(colourPin, LOW);   // Read the length (microseconds) of the pulse from the colourPin
  delay(100);
  
  return valRed;
}

unsigned int colourGreenVal() 
{
  // -------------------------------------------------------------------------
  /* This function obtains Green colour intensity reading from the colour sensor.
   * An object placed in front of the sensor will output values 
   * for it's red, green and blue color intensity. 
   * The three colour intensities together determine the colour of the object. 
   */
  // -------------------------------------------------------------------------
   
  digitalWrite(s2,HIGH);                    // To read green value of object   
  digitalWrite(s3,HIGH);  
  
  unsigned int valGreen = pulseIn(colourPin, LOW);   // Read the length (microseconds) of the pulse from the colourPin
  delay(100);   
  
  return valGreen; 
}

unsigned int colourBlueVal() 
{
  // -------------------------------------------------------------------------
  /* This function obtains Blue colour intensity reading from the colour sensor.
   * An object placed in front of the sensor will output values 
   * for it's red, green and blue color intensity. 
   * The three colour intensities together determine the colour of the object. 
   */
  // -------------------------------------------------------------------------
  
  digitalWrite(s2,LOW);                    // To read blue value of object  
  digitalWrite(s3,HIGH);
  
  unsigned int valBlue = pulseIn(colourPin, LOW);   // Read the length (microseconds) of the pulse from the colourPin
  delay(100); 
  
  return valBlue;
}


// ********************************************************************************
// Servomotor position function
// ********************************************************************************

// This code is modified from:
// https://sites.google.com/site/daguproducts/home/download-page

void setServoPos(int servoPin, int posMicro) 
{
  // ------------------------------------------------------------------------- 
  /* The angle of rotation of the servo motor is dependant on the length of the pulse sent to it
   * A pulse of 1500uS corresponds to 90 degrees
   * A pulse of 700uS corresponds roughly to 0 degrees
   * A pulse of 2300uS corresponds roughly to 180 degrees
   * Do not set your servo motor to the limits of its angular position range as this will damage the servo motor
   * If your servo motor starts to vibrate, you have reached its position limit 
   */
  // -------------------------------------------------------------------------
   
  if(posMicro < 700)
  {
    posMicro = 700;                         // set minimum pulse width 
  }
 
  if(posMicro > 2300)
  {
    posMicro = 2300;                        // set maximum pulse width 
  }
  
  for(int i = 0; i < 20; i++)               // send 20 pulses to the servo to allow time for the servo to reach the new position
  {
    digitalWrite(servoPin,1);               // set servo control pin high to start pulse
    delayMicroseconds(posMicro);            // hold pulse high for 'posMicro'uS
    digitalWrite(servoPin,0);               // set servo control pin low to finish pulse
    int lenDelay = 20 - posMicro/1000;
    delay(lenDelay);                        // wait 20mS before repeating the pulse
  } 
}
