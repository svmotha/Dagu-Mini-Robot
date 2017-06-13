/*
|--------------------------------------------------------------------------
|
| Mini Warehouse Robot: Infrared Distance Sensor Calibration
| Author: Victor Motha as part of MECN3012 Group A2 (2017)
| University of the Witwatersrand - Copyright 2017
| Objective: Gather mean, minimum, and maximun voltage readings for 
|            incremental infrared distance sensor readings.
|
|--------------------------------------------------------------------------
*/

/*
|--------------------------------------------------------------------------
| Declaring Global variables & Running Setup Routine:
|--------------------------------------------------------------------------
|
| Creating empty array for mean value calculation and Initializing serial 
| communication at 9600 bits per second.
|--------------------------------------------------------------------------
*/
int accuracy = 45;
float distReadings[45];

void setup()
{
  Serial.begin(9600); // initialize serial communication at 9600 bits per second:
}

/*
|--------------------------------------------------------------------------
| Running Loop Function:
|--------------------------------------------------------------------------
|
| Creating array, calculating, mean, min, max, and displaying them on the 
| serial monitor.
|--------------------------------------------------------------------------
*/
void loop()
{
  for (int i = 0; i < accuracy; i++)
  {
    float voltage = infraDistance();
    float distance = infraSensorDist(voltage);
    distReadings[i] = distance;
  }

  for (int i = 0; i < accuracy; i++) 
  {
    Serial.print(distReadings[i]);
    Serial.println("cm");
  }
  // Printing mean value to serial monitor.
  Serial.print("Mean value is: ");
  Serial.println(meanFunction(distReadings, accuracy));
  delay(500);
  // Printing minimum value to serial monitor.
  Serial.print("Minimum value is: ");
  Serial.println(minFunction(distReadings, accuracy));
  delay(500);
  // Printing maximum value to serial monitor.
  Serial.print("Maximum value is: ");
  Serial.println(maxFunction(distReadings, accuracy));
  delay(500);

  exit(0);  // Ensuring that the loop only runs once
}

/*
|--------------------------------------------------------------------------
| Convert Analog Readings To Voltages:
|--------------------------------------------------------------------------
|
| Takes an anolog reading and uses a 10 bit scale to return a value 
| between 0 - 5 Volts.
|--------------------------------------------------------------------------
*/
float infraDistance()
{
  int sensorValue = analogRead(A0);
  float voltage = sensorValue * (5.0 / 1023.0);
  return voltage;
}

/*
|--------------------------------------------------------------------------
| Voltage To Distance Convertor:
|--------------------------------------------------------------------------
|
| Takes in a voltage value, and using the trendline equation found in 
| sesorCalibration.numbers to return an approximate actual distance 
| as is seen by the sensor.
|--------------------------------------------------------------------------
*/
float infraSensorDist(float voltage)
{
  float actualDistance = pow((23.669 / voltage), (1 / 0.749));
  return actualDistance;
}

/*
|--------------------------------------------------------------------------
| Mean function:
|--------------------------------------------------------------------------
|
| Takes in an array and its index as inputs and returns the average of 
| all its values.
|--------------------------------------------------------------------------
*/
float meanFunction(float voltagesArray[], int arrayIndex)
{
  float sum;
  for (int i = 0; i < arrayIndex; i++)
  {
    sum = sum + voltagesArray[i];
  }
  float mean = (sum/arrayIndex);
  return mean;
}

/*
|--------------------------------------------------------------------------
| Minimum Function:
|--------------------------------------------------------------------------
|
| Takes in an array and its index as inputs and returns the smallest value
| within it.
|--------------------------------------------------------------------------
*/
float minFunction(float distArray[], int arrayIndex)
{
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
| Maximum Function:
|--------------------------------------------------------------------------
|
| Takes in an array and its index as inputs and returns the largest value
| within it.
|--------------------------------------------------------------------------
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
