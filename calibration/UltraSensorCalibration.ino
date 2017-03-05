// Ultrasonic distance sensor
const int trigPin = 12;                         // Ultrasonic sensor pin connections                       
const int echoPin = 11;
float distReadings[15];

// ******************************************************************************** 
// SETUP FUNCTION
// ********************************************************************************
void setup()
{  
  Serial.begin(9600);                            // initialize serial communication 
}

// ******************************************************************************** 
// LOOP FUNCTIONS 
// ********************************************************************************
void loop()
{ 
  for (int i = 0; i < 15; i++)
  {
    float distance = ultraSensorDist();
    distReadings[i] = distance;
  }

  for (int i = 0; i < 15; i++)
  {
    Serial.print(distReadings[i]);
    Serial.println("cm");
    delay(500);
  }
  // Printing mean value to serial monitor.
  Serial.print("Mean Value is: ");
  Serial.print(meanFunction(distReadings,15));
  Serial.println("cm");
  delay(500);
  // Printing maximum value to serial monitor.
  Serial.print("Maximum Value is: ");
  Serial.print(maxFunction(distReadings, 15));
  Serial.println("cm");
  delay(500);
  // Printing minimum value to serial monitor.
  Serial.print("Minimum value is: ");
  Serial.print(minFunction(distReadings, 15));
  Serial.println("cm");
  delay(500);
//  exit(0);
}

// ********************************************************************************
// Mean function: Find mean of all Values in an Array of known index
// ********************************************************************************
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

// ********************************************************************************
// Max function: Find maximum value amongst all values in an array of known index
// ********************************************************************************
float maxFunction(float distArray[], int arrayIndex){
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

// ********************************************************************************
// Min function: Find minimum value amongst all values in an array of known index
// ********************************************************************************
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

