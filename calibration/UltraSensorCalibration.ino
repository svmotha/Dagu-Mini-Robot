/*
|--------------------------------------------------------------------------
|
| Mini Warehouse Robot: Ultrasonic Distance Sensor Calibration
| Author: MECN3102 Group A2
| University of the Witwatersrand - Copyright 2017
| Objective: Gather mean, minimum, and maximun pulse readings for 
|            incremental distance readings.
|
|--------------------------------------------------------------------------
*/

/*
|--------------------------------------------------------------------------
| Declaring Global variables & Running Setup Routine:
|--------------------------------------------------------------------------
|
| Creating empty array for mean value calculation, Setting Ultrasonic 
| sensor pin connections and Initializing serial communication at 
| 9600 bits per second.
|
*/
const int trigPin = 12;                 
const int echoPin = 11;
int accuracy = 45;
float distReadings[45];

void setup()
{  
  Serial.begin(9600); // initialize serial communication
}

/*
|--------------------------------------------------------------------------
| Running Loop Function:
|--------------------------------------------------------------------------
|
| Creating array, calculating, mean, min, max, and displaying them on the 
| serial monitor.
|
*/
void loop()
{ 
  for (int i = 0; i < accuracy; i++)
  {
    float distance = ultraSensorDist();
    distReadings[i] = distance;
  }

  for (int i = 0; i < accuracy; i++)
  {
    Serial.print(distReadings[i]);
    Serial.println("cm");
  }
  // Printing mean value to serial monitor.
  Serial.print("Mean Value is: ");
  Serial.print(meanFunction(distReadings, accuracy));
  Serial.println("cm");
  delay(500);
  // Printing maximum value to serial monitor.
  Serial.print("Maximum Value is: ");
  Serial.print(maxFunction(distReadings, accuracy));
  Serial.println("cm");
  delay(500);
  // Printing minimum value to serial monitor.
  Serial.print("Minimum value is: ");
  Serial.print(minFunction(distReadings, accuracy));
  Serial.println("cm");
  delay(500);

  exit(0);  // Ensuring that the loop only runs once
}

/*
|--------------------------------------------------------------------------
| Mean function:
|--------------------------------------------------------------------------
|
| Takes in an array and its index as inputs and returns the average of 
| all the values within the array.
|
*/
float meanFunction(float voltagesArray[], int arrayIndex)
{
  float sum = 0;
  for (int i = 0; i < arrayIndex; i++)
  {
    sum = sum + voltagesArray[i];
  }
  float mean = (sum/arrayIndex);
  return mean;
}

/*
|--------------------------------------------------------------------------
| Maximum Function:
|--------------------------------------------------------------------------
|
| Takes in an array and its index as inputs and returns the largest value
| within the array.
|
*/
float maxFunction(float distArray[], int arrayIndex)
{
  float maxVal = distArray[0];
  for (int i = 1; i < arrayIndex; i++)
  {
    if(maxVal < distArray[i])
    {
      maxVal = distArray[i];
    }
  }
  return maxVal;
}

/*
|--------------------------------------------------------------------------
| Minimum Function:
|--------------------------------------------------------------------------
|
| Takes in an array and its index as inputs and returns the smallest value
| within the array.
|
*/
float minFunction(float distArray[], int arrayIndex){
  float minVal = distArray[0];
  for (int i = 1; i < arrayIndex; i++)
  {
    if(minVal > distArray[i])
    {
      minVal = distArray[i];
    }
  }
  return minVal;
}

/*
|--------------------------------------------------------------------------
| Ultrasonic Distance Sensor Function:
|--------------------------------------------------------------------------
|
| This function obtains a reading in cm from the ultrasonic distance sensor
| The sensor is triggered by a HIGH pulse of 10 or more microseconds.
| Reading the signal from the sensor: 
|   a HIGH pulse whose duration is the time (in microseconds) from the sending
|   of the ping to the reception of its echo off of an object. 
|   To convert the time into a distance:
|   The speed of sound is 340 m/s or 29.4 microseconds per centimeter.
|   The ping travels out and back, so to find the distance of the
|   object we take half of the distance travelled.
|
| Please note, this code is modified from:
| https://gist.github.com/flakas/3294829#file-hc-sr04-ino
|
*/
float ultraSensorDist()
{
  pinMode(trigPin, OUTPUT);                 // Set trigPin as output
  digitalWrite(trigPin, LOW);               // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  delayMicroseconds(2);                     // Duration of LOW pulse
  digitalWrite(trigPin, HIGH);              // HIGH pulse to trigger sensor
  delayMicroseconds(10);                    // Duration of HIGH pulse
  digitalWrite(trigPin, LOW);               // Turn HIGH pulse off
  pinMode(echoPin, INPUT);                  // Set echoPin as input
  long duration = pulseIn(echoPin, HIGH);   // Read the signal from the sensor: 
  float cm = duration / 29.4 / 2.0;          // convert the time into a distance
  delay(30);
  return cm;
}

