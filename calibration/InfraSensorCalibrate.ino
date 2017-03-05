/*
|--------------------------------------------------------------------------
|
| Mini Warehouse Robot: Infrared Distance Sensor Calibration
| Author: MECN3102 Group A2
| University of the Witwatersrand | Copyright 2017
| Objective: Gather mean, minimum, and maximun voltage readings for 
|            incremental distance readings.
|
|--------------------------------------------------------------------------
*/

/*
|--------------------------------------------------------------------------
| Declaring Global variables & Running Setup Routine:
|--------------------------------------------------------------------------
|
| Creating empty array for mean value calculation. and Initial erial 
| communication at 9600 bits per second.
|
*/
float distReadings[15];

void setup() {
  Serial.begin(9600);
}

/*
|--------------------------------------------------------------------------
| Running Loop:
|--------------------------------------------------------------------------
|
| Creating array, calculating, mean, min, max, and displaying them on the 
| serial monitor.
|
*/
void loop()
{
  for (int i = 0; i < 15; i++)
  {
    float voltage = infraDistance();
    float distance = infraSensorDist(voltage);
    distReadings[i] = distance;
  }
  delay(100);

  for (int i = 0; i < 15; i++) 
  {
    Serial.print(distReadings[i]);
    Serial.println("cm");
    delay(100);
  }

  Serial.print("Mean value is: ");
  Serial.println(meanFunction(distReadings,15));
  delay(500);
  Serial.print("Minimum value is: ");
  Serial.println(minFunction(distReadings,15));
  delay(500);
  Serial.print("Maximum value is: ");
  Serial.println(maxFunction(distReadings,15));
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
|
*/
float infraDistance() {
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
|
*/
float infraSensorDist(float voltage){
  float actualDistance = pow((23.694 / voltage), (1 / 0.7478));
  return actualDistance;
}

/*
|--------------------------------------------------------------------------
| Find Mean Value In An Array:
|--------------------------------------------------------------------------
|
| Takes in an array and its index as inputs and returns the average of 
| all its values.
|
*/
float meanFunction(float voltagesArray[], int arrayIndex){
  float sum = 0;
  for (int i = 0; i < arrayIndex; i++) {
    sum = sum + voltagesArray[i];
  }
  float mean = (sum/arrayIndex);
  return mean;
}

/*
|--------------------------------------------------------------------------
| Find Minimum Value In An Array:
|--------------------------------------------------------------------------
|
| Takes in an array and its index as inputs and returns the smallest value
| within it.
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
| Find Maximum Value in an Array:
|--------------------------------------------------------------------------
|
| Takes in an array and its index as inputs and returns the largest value
| within it.
|
*/
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
