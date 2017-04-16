#include "Constants.h"
#include "IOpins.h"

// Colour sensor 
const int s0 = 3, s1 = 4, s2 = 5, s3 = 6;  // Colour sensor pin connections
const int colourPin = 2;
int LED = 13;
int instructionsArray[3];                  // Creating empty colour sequence array
int colour = 0;                            // Colour varibale (1 = pink, 2 = Blue, 3 = Green)
bool inArray = false;                      // Setting array checking boolean variable

// Ultrasonic distance sensor
const int trigPin = 12;                    // Ultrasonic sensor pin connections                       
const int echoPin = 11;

// Servo motor
const int srvoMotorPin = 18;               // Servo motor digital pin connection 

// Battery
int volts;                                 // battery voltage
int pause = -1;                            // a value of -1 means the battery voltage is ok, 
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
    
  // Set colour sensor output frequency to frequency-scaling of 2%
  digitalWrite(s0, LOW);
  digitalWrite(s1, HIGH);
  
  // Set servo control pin as an output pin
  pinMode(srvoMotorPin,OUTPUT);        

  delay(1000);                              // Wait 1 second before starting the loop function.  
}

void loop()
{ 
  // -------------------------------------------------------------------------
  /* Battery Check.
  | !Low battery voltages will damage your controller! 
  | The robot will stop moving if the battery voltage is below a certain value. 
  | The LED on pin D13 will also flash on and off. 
  | !Do not tamper with this code!
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
                                            
  robotProject();                          // The robotProject function contains your project code
  while(1);
}

// ******************************************************************************** 
// ROBOTPROJECT FUNCTION 
// ******************************************************************************** 
void robotProject()
{
  colourSequence();
  goSomewhere(1, 1, 60, 90, 3);
//  goSomewhere(0, 0, 45, 90, 2);
//  goSomewhere(0, 0, 0, 0, 1);
//  goSomewhere(0, 0, 0, 0, 1);
//  goSomewhere(0, 0, 0, 90, 0.5);
//  goSomewhere(0, 0, 0, 0, 1);
//  goSomewhere(0, 0, 45, 0, 0.5);
//  goSomewhere(0, 0, 0, 0, 1);
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
  | inputs. It can move: Forward, Backwards, Turn Left and Right, and 
  | Stop completely.
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
  delay(delayTime*1000);
}

// ********************************************************************************
// Infrared Distance Sensor Function
// ********************************************************************************
float infraDistance()
{
  /*
  |--------------------------------------------------------------------------
  | Convert Analog Readings To Voltages:
  |--------------------------------------------------------------------------
  |
  | Takes an anolog reading and uses a 10 bit scale to return a value 
  | between 0 - 5 Volts. Then takes the voltage value, and using the 
  | trendline equation found in sesorCalibration.numbers to return 
  | an approximate actual distance as is seen by the sensor.
  |
  */
  int sensorValue = analogRead(A0);
  float voltage = sensorValue * (5.0 / 1024.0);
  float actualDistance = pow((23.694 / voltage), (1 / 0.7478));
  return actualDistance;
}

// ********************************************************************************
// Ultrasonic Distance Sensor Function
// ******************************************************************************** 

// This code is modified from:
// https://gist.github.com/flakas/3294829#file-hc-sr04-ino

float ultraSensorDist()
{
  /*
  | -------------------------------------------------------------------------
  | This function obtains a reading in cm from the ultrasonic distance sensor
  | The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  | Reading the signal from the sensor: 
  | a HIGH pulse whose duration is the time (in microseconds) from the sending
  | of the ping to the reception of its echo off of an object. 
  | To convert the time into a distance:
  | The speed of sound is 340 m/s or 29.4 microseconds per centimeter.
  | The ping travels out and back, so to find the distance of the
  | object we take half of the distance travelled.
  | -------------------------------------------------------------------------
  */

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

  cm = (duration / 29.4) / 2.0;         // convert the time into a distance

  delay(30);
  return cm;
}


// ********************************************************************************
// Colour Sensor Functions:
// ********************************************************************************

// ------------------------------------------------------
// Checking  all colours and setting cup storage sequence
// ------------------------------------------------------
void colourSequence()
{
  /*
  |--------------------------------------------------------------------------
  | Detect, append, and respond accordingly:
  |--------------------------------------------------------------------------
  | 1. Check if a known cup colour is detected i.e. colourDetected is not 
  |    equal to zero.
  | 2. Set response for each possible colour detected i.e. for Pink = 1, 
  |    Blue = 2, Green = 3, and "Not cup colour" = 0.
  | 3. For each response check if colour not already in array. If it is
  |    not, append that colour into the corresponding array slot using 
  |    the count variable and increment the count variable by 1.
  | 4. For each appended colour, blink LED a specific number of times 
  |    i.e. Pink = once, Blue = twice, Green = Thrice.
  | 5. Once all three colours identified, blink four times.
  |--------------------------------------------------------------------------
  */
  int count = 0;                          // Keep track of number of colours detected
  while (count != 3)
  {
    inArray = false;
    int colourDetected = colourSensed();
    if (colourDetected != 0)              // Check if colour identified
    {
      if (colourDetected == 1)            // If colour detected is Pink
      {
        inArray = checkArray(colourDetected);  // Check if colour already in array
        count = count + storageBlinker(inArray, count, 1, colourDetected);
      }
      else if (colourDetected == 2)       // If colour detected is Blue
      {
        inArray = checkArray(colourDetected); // Check if colour already in array
        count = count + storageBlinker(inArray, count, 2, colourDetected);
      }
      else if (colourDetected == 3)        // If colour detected is Green
      {
        inArray = checkArray(colourDetected); // Check if colour already in array
        count = count + storageBlinker(inArray, count, 3, colourDetected);
      }
    }
  }
  delay(3000);
  blinkLED(4);       // Blink 4 times to signal colour sensing process complete
}

// -----------------------------------
// Storing identified colours in array
// -----------------------------------
int storageBlinker(bool inArray, int count, int blinkNumber, int colour)
{
  if (inArray == false)
  {
    instructionsArray[count] = colour;
    delay(4000);
    blinkLED(blinkNumber);
    delay(3000);
    return 1;
  }
  else
  {
    return 0;
  }
}
// -------------------------------------
// Checking if colour already identified
// -------------------------------------
bool checkArray(int colourDetected)
{
  for (int i = 0; i < 3; i++)
  {
    if (instructionsArray[i] == colourDetected)
    {
      inArray = true;
    }
  }
  return inArray;
}
// ------------------------------------------------
// Making blue LED blink a specific number of times
// ------------------------------------------------
int blinkLED(int blinkCount)
{
  for (int i = 0; i < blinkCount; i++)
  {
    digitalWrite(LED, HIGH);
    delay(1000);
    digitalWrite(LED, LOW);
    delay(1000);
  }
}
// ---------------------------------------
// Detecting and returning colour detected
// ---------------------------------------
int colourSensed() 
{
  digitalWrite(LED, HIGH);
  delay(200);
  // Setting red filtered photodiodes to be read
  digitalWrite(s2,LOW);
  digitalWrite(s3,LOW);
  // Reading the output frequency
  int redCheck = pulseIn(colourPin, LOW);

  // Setting Green filtered photodiodes to be read
  digitalWrite(s2,HIGH);
  digitalWrite(s3,HIGH);
  // Reading the output frequency
  int greenCheck = pulseIn(colourPin, LOW);

  // Setting Blue filtered photodiodes to be read
  digitalWrite(s2,LOW);
  digitalWrite(s3,HIGH);
  // Reading the output frequency
  int blueCheck = pulseIn(colourPin, LOW);
  digitalWrite(LED, LOW);
  delay(100);

  if ( (redCheck < 600) && (redCheck < blueCheck) && (blueCheck < greenCheck) )
  {
   colour = 1;
  }
  else if ( (redCheck > 600) && (blueCheck < greenCheck) && (greenCheck < redCheck) )
  {
   colour = 2;
  }
  else if ( (redCheck < 600) && (blueCheck < greenCheck) && (greenCheck < redCheck) ) 
  {
   colour = 3;
  }
  else
  {
    colour = 0;
  }
  return colour;
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