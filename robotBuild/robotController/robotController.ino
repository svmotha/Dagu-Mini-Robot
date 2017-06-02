/*
|--------------------------------------------------------------------------
|
| Mini Warehouse Robot: Robot Controller
| Author: MECN3012 Group A2 (2017)
| University of the Witwatersrand - Copyright 2017
| Objective: Detect colour sequence, Deliver cups to dispatch bay in 
|            presented sequence, return back to starting bay. 
|
|--------------------------------------------------------------------------
*/

// Including motor pins and bttery voltage constants
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

// Coordinate Array
;float cupPoints[3][6] = 
{
  {27.0, 140.0, 27.0, 27.0, 30.0, 27.0},        // Pink
  {27.0, 54.0, 70.0, 25.0, 30.0, 25.0},         // Blue
  {27.0, 88.0, 70.0, 25.0, 30.0, 25.0}          // Green
};

// Motion
float accuracyRange = 2.0;
int turnIncrement = 0;

// Servo motor
const int srvoMotorPin = 18;              // Servo motor digital pin connection

// Battery
int volts;                                // battery voltage
int pause = -1;                           // a value of -1 means the battery voltage is ok,
                                          // 0 and 1 are used to flash LED on D13 when the battery is flat

// -------------------------------------------------------------------------
// SETUP and LOOP FUNCTIONS
// -------------------------------------------------------------------------

void setup()
{
  // Setup motor direction pins as outputs
  pinMode(Ldirpin, OUTPUT);                    // set left  direction pin as an output pin
  pinMode(Rdirpin, OUTPUT);                    // set right direction pin as an output pin

  // Setup colour sensor pins as inputs and outputs
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(colourPin, INPUT);

  // Set colour sensor output frequency to frequency-scaling of 2%
  digitalWrite(s0, LOW);
  digitalWrite(s1, HIGH);

  // Set servo control pin as an output pin
  pinMode(srvoMotorPin, OUTPUT);

  delay(1000);                              // Wait 1 second before starting the loop function.
}

void loop()
{
  /*
    | -------------------------------------------------------------------------
    | Battery Check.
    | -------------------------------------------------------------------------
    | !Low battery voltages will damage your controller!
    | The robot will stop moving if the battery voltage is below a certain value.
    | The LED on pin D13 will also flash on and off.
    | !Do not tamper with this code!
    | This code is modified from:
    | https://sites.google.com/site/daguproducts/home/Software/Doodle_Bot_Text_Demo.zip?attredirects=0
    | -------------------------------------------------------------------------
  */

  volts = analogRead(Battery) / 10;         // Battery voltage reading

  if ((volts < lowbat) || (pause != -1))    // once voltage falls below lowbat value
  {
    // Stop the robot
    digitalWrite(Ldirpin, 0);               // set left direction
    analogWrite (Lpwmpin, 0);               // set left speed to zero
    digitalWrite(Rdirpin, 0);               // set right direction
    analogWrite (Rpwmpin, 0);               // set right speed to zero

    pause++;                                // increment pause
    if (pause > 1)                          // toggle pause between 0 and 1 once battery is flat
    {
      pause = 0;
    }

    digitalWrite(13, pause);                // flash LED on D13
    delay(200);                             // set flashing LED speed
    return;                                 // reset loop
  }

  robotProject();                          // The robotProject function contains your project code
  exit(0);                                 // Ensuring code runs only once
}
// -------------------------------------------------------------------------
// Voltage Stabilising function
// -------------------------------------------------------------------------
int voltageFunction(int mtrSpeed)
{
  int batteryLvl = analogRead(Battery);
  int newSpeed = round(((7.5*mtrSpeed)/255)*(34782/batteryLvl));
  return newSpeed;
}

// -------------------------------------------------------------------------
// ROBOTPROJECT FUNCTION
// -------------------------------------------------------------------------
void robotProject()
{
  colourSequence();         // Check & Store colour sequence 
  openServo();              // Raise cup latching mechanism
  getCup();                 // Get cups in array sequence
  blinkLED(4);              // Blink LED to signal completion
}

// -------------------------------------------------------------------------
// Robot speed and direction Functions                                     |
// -------------------------------------------------------------------------

// ------------------------------------
// Setting motion (turning or moving) |
// ------------------------------------
void goSomewhere(int dirBinaryLeft, int dirBinaryRight, int leftSpeed, int rightSpeed, int delayTime)
{
  /*
    | -------------------------------------------------------------------------
    | This function moves the robot in any desired direction based on user
    | inputs. It can move: Forward, Backwards, Turn Left and Right, and
    | Stop completely.
    | -------------------------------------------------------------------------
  */
  if (leftSpeed > 255)                      // limit maximum left motor speed value
  {
    leftSpeed = 255;
  }
  if (rightSpeed > 255)                     // limit maximum right motor speed value
  {
    rightSpeed = 255;
  }

  digitalWrite(Ldirpin, dirBinaryLeft);     // set left direction
  digitalWrite(Rdirpin, dirBinaryRight);    // set right direction
  analogWrite (Rpwmpin, rightSpeed);        // set right speed
  analogWrite (Lpwmpin, leftSpeed);         // set left speed
  delay(delayTime);                         // Hold desired motion for specific amount of time

  // Stop wheels after moving
  digitalWrite(Ldirpin, 0);                 // set left direction
  digitalWrite(Rdirpin, 0);                 // set right direction
  analogWrite (Rpwmpin, 0);                 // set right speed
  analogWrite (Lpwmpin, 0);                 // set left speed
}

// --------------------------------
// Keep motion in a straight line |
// --------------------------------
void straightLine(int leftMtr, int rightMtr, float desiredDist, float knownWall, bool BOF)
{
  if (knownWall == 0.0)
  {
    knownWall = ultraSensorDist();
  }

  bool distReached = true;
  while (distReached)
  {
    if ( (BOF == true) && (infraDistance() <= desiredDist) )
    {
      distReached = false;
    }

    else if ( (BOF == false) && (infraDistance() >= desiredDist) )
    {
      distReached = false;
    }

    if (BOF == true)
    {
      obstructionStop();
      break;
    }

    int mtrSpeed = voltageFunction(80);
    goSomewhere(leftMtr, rightMtr, mtrSpeed, mtrSpeed, 100);
    delay(300);

    int rs = 0;
    int ls = 0;
    float ping = ultraSensorDist();
    mtrSpeed = voltageFunction(60);
    if (ping < knownWall)                 // Shifted Right
    {
      for (int rs=0; rs < 3; rs++)
      {
        if (BOF == true)
        {
          goSomewhere(0, 0, 0, mtrSpeed, 30);
        }
        else if (BOF == false)
        {
          goSomewhere(1, 1, mtrSpeed, 0, 30);
        }

        ping = ultraSensorDist();
        if ( (knownWall < ping) && (ping < knownWall + accuracyRange) )
        {
          break;
        }
        delay(100);
      }
    }

    else if ( ping > knownWall)           // Shifted left
    {
      for (int ls=0; ls < 3; ls++)
      {
        if (BOF == true)
        {
          goSomewhere(0, 0, mtrSpeed, 0, 30);
        }
        else if (BOF == false)
        {
          goSomewhere(1, 1, 0, mtrSpeed, 30);
        }

        ping = ultraSensorDist();
        if ( (knownWall < ping) && (ping < knownWall + accuracyRange) )
        {
          break;
        }
        delay(100);
      }
    }

    if (rs == 2)
    {
      for (int j=0; j < 6; j++)
      {
        if (BOF == true)
        {
          goSomewhere(0, 0, mtrSpeed, 0, 30);
        }
        else if (BOF == false)
        {
          goSomewhere(1, 1, 0, mtrSpeed, 30);
        }
        delay(100);
      }
    }
    if (ls == 2)
    {
      for (int k=0; k < 6; k++)
      {
        if (BOF == true)
        {
          goSomewhere(0, 0, 0, mtrSpeed, 30);
        }
        else if (BOF == false)
        {
          goSomewhere(1, 1, mtrSpeed, 0, 50);
        }
        delay(100);
      }
    }
    delay(100);
  }
  delay(1000);
}

// ---------
// Reverse |
// ---------
void reverse()
{
  for (int k=0; k < 5; k++)
  {
    goSomewhere(1, 1, 80, 85, 90);
  }
  delay(1000);
}

// ---------
// Get cup |
// ---------
void getCup()
{
  float cupGap = 0;
  for (int i=0; i<3; i++)
  {
    turnIncrement = 0;

    int mtrSpeed = voltageFunction(70);
    straightLine(0, 0, cupPoints[instructionsArray[i]][0], 0.0, true);          // point 1
    turnLR(0, 0, 0, mtrSpeed, cupPoints[instructionsArray[i]][0]);              // turn A
  
    straightLine(0, 0, cupPoints[instructionsArray[i]][1], 0.0, true);          // point 2
    turnLR(0, 0, 0, mtrSpeed, cupPoints[instructionsArray[i]][1]);              // turn B
    
    straightLine(0, 0, cupPoints[instructionsArray[i]][2], 0.0, true);          // point 3
    closeServo();                                                               // Grab cup
    reverse();
    reverse();
    reverse();
    reverse();
    reverse();
    turnLR(0, 0, mtrSpeed, 0, cupPoints[instructionsArray[i]][2]);              // turn C
  
    mtrSpeed = voltageFunction(60);
    straightLine(0, 0, cupPoints[instructionsArray[i]][3] + cupGap, 0.0, true); // point 5
    openServo();                                                                // leave cup
    reverse();                                                                  // point 6 (reverse)
    turnLR(0, 0, 0, mtrSpeed, cupPoints[instructionsArray[i]][3] + 5.0);        // turn D
    closeServo();
  
    straightLine(0, 0, cupPoints[instructionsArray[i]][4], 0.0, true);          // point 7
    turnLR(0, 0, 0, mtrSpeed, cupPoints[instructionsArray[i]][4]);              // turn E
  
    straightLine(0, 0, cupPoints[instructionsArray[i]][5], 0.0, true);          // point 8
    turnLR(0, 0, 0, mtrSpeed, cupPoints[ instructionsArray[i] ][5]);            // turn F
  
    cupGap = cupGap + 10;
    openServo();
  
    delay(3000);
  }

}

// ---------------------------------
// Stop if obstructuction detected |
// ---------------------------------
bool obstructionStop()
{
  float ping = infraDistance();
  if (ping < 22)
  {
    goSomewhere(0, 0, 0, 0, 0);
  }
}

// --------------------
// Turn left or right |
// --------------------
void turnLR(int leftDir, int RightDir, int leftSpeed, int rightSpeed, float wallDist)
{
  wallDist = infraDistance() - 3.0;
  delay(1000);
  goSomewhere(leftDir, RightDir, leftSpeed, rightSpeed, 480 + turnIncrement);
  turnIncrement = turnIncrement + 15;
  delay(2000);
  for (int l=0; l < 15; l++)
  {
    goSomewhere(leftDir, RightDir, leftSpeed, rightSpeed, 60);
    float wallPing = ultraSensorDist();
    if( (wallPing > wallDist) && ( wallPing <= (wallDist + 3.0) ) )
    {
      break;
    }
    delay(300);
  }
  delay(1000);
}

// --------------------------------------------------------------------------
// Infrared Distance Sensor Function
// --------------------------------------------------------------------------
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

  float tempArray[5];
  for (int i=0; i<5; i++)
  {
    int sensorValue = analogRead(A0);
    float voltage = sensorValue * (5.0 / 1024.0);                     // Get sensor voltage reading
    float trendline1 = pow((23.669 / voltage), (1 / 0.749));          // Use calibration voltage to get distance reading
    float trendline2 = ( (trendline1 + 11.588) / (1.4165) );          // Use trendline one to get distance reading
    float tempDistance = pow(  (trendline2 / 0.7163), (1 / 1.089) );  // Use trendline two to get distance reading
    
    tempArray[i] = tempDistance;
    delay(5);
  }

  float sum = 0.0;
  for (int j=0; j<5; j++)
  {
    sum = sum + tempArray[j];
  }
  sum = sum / 5;       // Ensure all readings are averages of at least 5 readings
  return sum;
}

// -------------------------------------------------------------------------
// Ultrasonic Distance Sensor Function
// -------------------------------------------------------------------------
float ultraSensorDist()
{
  /*
    | -------------------------------------------------------------------------
    | This function obtains a reading in cm from the ultrasonic distance sensor
    | The sensor is triggered by a HIGH pulse of 10 or more microseconds.
    |
    | Reading the signal from the sensor:
    | a HIGH pulse whose duration is the time (in microseconds) from the sending
    | of the ping to the reception of its echo off of an object.
    | To convert the time into a distance:
    | The speed of sound is 340 m/s or 29.4 microseconds per centimeter.
    | The ping travels out and back, so to find the distance of the
    | object we take half of the distance travelled.
    |
    | This code is modified from:
    | https://gist.github.com/flakas/3294829#file-hc-sr04-ino
    | -------------------------------------------------------------------------
  */
  float tempArray[5];
  for (int i=0; i<5; i++)
  {
    pinMode(trigPin, OUTPUT);                               // Set trigPin as output
    digitalWrite(trigPin, LOW);                             // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
    delayMicroseconds(2);                                   // Duration of LOW pulse
    digitalWrite(trigPin, HIGH);                            // HIGH pulse to trigger sensor
    delayMicroseconds(10);                                  // Duration of HIGH pulse
    digitalWrite(trigPin, LOW);                             // Turn HIGH pulse off
  
    pinMode(echoPin, INPUT);                                // Set echoPin as input
    long duration = pulseIn(echoPin, HIGH);                 // Read the signal from the sensor:
  
    float cm = (duration / 29.4) / 2.0;                     // convert the time into a distance result in centimeters
  
    float finalDist = pow( (cm / 1.0014), (1 / 1.0013) );   // Convert distance using trendline equation
    tempArray[i] = finalDist;
    delay(5);
  }
  float sum = 0.0;
  for (int j=0; j<5; j++)
  {
    sum = sum + tempArray[j];
  }
  sum = sum / 5;
  return sum;
}

// -------------------------------------------------------------------------
// Colour Sensor Functions:
// -------------------------------------------------------------------------

// -------------------------------------------------------
// Checking all colours and setting cup storage sequence |
// -------------------------------------------------------
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
  int count = 0;                                                                // Keep track of number of colours detected
  while (count != 3)
  {
    inArray = false;
    int colourDetected = colourSensed();
    if (colourDetected != 0)                                                    // Check if colour identified
    {
      if (colourDetected == 1)                                                  // If colour detected is Pink
      {
        inArray = checkArray(colourDetected);                                   // Check if colour already in array
        count = count + storageBlinker(inArray, count, 1, colourDetected);      // Append or increment count if colour successfully stored in array
      }
      else if (colourDetected == 2)                                             // If colour detected is Blue
      {
        inArray = checkArray(colourDetected);                                   // Check if colour already in array
        count = count + storageBlinker(inArray, count, 2, colourDetected);      // Append or increment count if colour successfully stored in array
      }
      else if (colourDetected == 3)                                             // If colour detected is Green
      {
        inArray = checkArray(colourDetected);                                   // Check if colour already in array
        count = count + storageBlinker(inArray, count, 3, colourDetected);      // Append or increment count if colour successfully stored in array
      }
    }
  }
  delay(3000);
  blinkLED(4);                                                                  // Blink 4 times to signal colour sensing process complete
}
// -------------------------------------
// Storing identified colours in array |
// -------------------------------------
int storageBlinker(bool inArray, int count, int blinkNumber, int colour)
{
  if (inArray == false)                   // If item not already in colour sequence array
  {
    instructionsArray[count] = colour;    // Append colour into current count slot in colour sequence array
    delay(4000);                          // Because colour sensing process blinks continuously at 300 ms intervals,
                                          // the delay allows for recognizable blinking once a colour is identified and stored
    blinkLED(blinkNumber);                // Blink according to identified colour
    delay(3000);
    return 1;                             // Return integer to increase count by
  }
  else                                    // Else if item already in colour seequence array
  {
    return 0;                             // Return zero, so count value stays the same
  }
}
// ---------------------------------------
// Checking if colour already identified |
// ---------------------------------------
bool checkArray(int colourDetected)
{
  for (int i = 0; i < 3; i++)                     // Iterate throughout entire array length
  {
    if (instructionsArray[i] == colourDetected)   // Check array slot for detected colour
    {
      inArray = true;                             // If detected colour already in array set inArray value to True
    }
  }
  return inArray;                                 // Return inArray value,
                                                  // which equals false if detected colour not in colour sequence array
}
// --------------------------------------------------
// Making blue LED blink a specific number of times |
// --------------------------------------------------
int blinkLED(int blinkCount)
{
  for (int i = 0; i < blinkCount; i++)    // Iterate required number of blinks
  {
    digitalWrite(LED, HIGH);              // Turn LED on
    delay(1000);                          // Keep on for 1 second
    digitalWrite(LED, LOW);               // Turn LED off
    delay(1000);                          // Keep off for 1 second
  }
}
// -----------------------------------------
// Detecting and returning colour detected |
// -----------------------------------------
int colourSensed()
{
  // Turning colour sensor LED on
  digitalWrite(LED, HIGH);
  delay(200);

  // Setting red filtered photodiodes to be read
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  // Reading the output frequency
  int redCheck = pulseIn(colourPin, LOW);

  // Setting Green filtered photodiodes to be read
  digitalWrite(s2, HIGH);
  digitalWrite(s3, HIGH);
  // Reading the output frequency
  int greenCheck = pulseIn(colourPin, LOW);

  // Setting Blue filtered photodiodes to be read
  digitalWrite(s2, LOW);
  digitalWrite(s3, HIGH);
  // Reading the output frequency
  int blueCheck = pulseIn(colourPin, LOW);

  // Turning colour sensor LED on
  digitalWrite(LED, LOW);
  delay(100);

  /*
    |--------------------------------------------------------------------------
    | Detect, append, and respond accordingly:
    |--------------------------------------------------------------------------
    | 
    | Detection values used are calibrated using ColourSensorCalibration.ino
    | file found 
    |
    |--------------------------------------------------------------------------
  */
  if ( (redCheck < 600) && (redCheck < blueCheck) && (blueCheck < greenCheck) )
  {
    colour = 1;                   // Setting detected colour to Pink
  }
  else if ( (redCheck > 600) && (blueCheck < greenCheck) && (greenCheck < redCheck) )
  {
    colour = 2;                   // Setting detected colour to Green
  }
  else if ( (redCheck < 600) && (blueCheck < greenCheck) && (greenCheck < redCheck) )
  {
    colour = 3;                   // Setting detected colour to Blue
  }
  else
  {
    colour = 0;                   // No colour detected
  }
  return colour;
}

// -------------------------------------------------------------------------
// Servomotor position function
// -------------------------------------------------------------------------
void setServoPos(int servoPin, int posMicro)
{
  /* 
    | -------------------------------------------------------------------------
    |
    | This code is modified from:
    | https://sites.google.com/site/daguproducts/home/download-page
    |
    | The angle of rotation of the servo motor is dependant on the length of 
    | the pulse sent to it.
    | A pulse of 1500uS corresponds to 90 degrees
    | A pulse of 700uS corresponds roughly to 0 degrees
    | A pulse of 2300uS corresponds roughly to 180 degrees
    | Do not set your servo motor to the limits of its angular position 
    | range as this will damage the servo motor.
    | If your servo motor starts to vibrate, you have reached its position limit
    |
    | -------------------------------------------------------------------------
  */

  if (posMicro < 700)
  {
    posMicro = 700;                         // set minimum pulse width
  }

  if (posMicro > 2300)
  {
    posMicro = 2300;                        // set maximum pulse width
  }

  for (int i = 0; i < 20; i++)              // send 20 pulses to the servo to allow time for the servo to reach the new position
  {
    digitalWrite(servoPin, 1);              // set servo control pin high to start pulse
    delayMicroseconds(posMicro);            // hold pulse high for 'posMicro'uS
    digitalWrite(servoPin, 0);              // set servo control pin low to finish pulse
    int lenDelay = 20 - posMicro / 1000;
    delay(lenDelay);                        // wait 20mS before repeating the pulse
  }
}
// ------------------------------
// Raise cup latching mechanism |
// ------------------------------
void openServo()
{
  setServoPos(18, 700);                     // Lift Cup latching Mechanism
  delay(500);
}
// ------------------------------
// Lower cup latching mechanism |
// ------------------------------
void closeServo()
{
  setServoPos(18, 1900);                    // Lower cup latching mechanism
  delay(500);
}