int s0 = 3, s1 = 4, s2 = 5, s3 = 6;          // Colour sensor pin connections
int colourPin = 2;
int LED = 13;
int instructionsArray[3];
int colour = 0;
bool inArray = false;
// ****************************************************************************
void setup() {
  // Setup colour sensor pins as inputs and outputs
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(colourPin, INPUT);
  // Setting frequency-scaling to 2%
  digitalWrite(s0, LOW);
  digitalWrite(s1, HIGH);
  delay(100);
}
// ****************************************************************************
void loop() {
  int count = 0;
  while (count != 3)
  {
    inArray = false;
    int colourDetected = colourSensed();
    //Check if colour identified
    if (colourDetected != 0)
    {
      if (colourDetected == 1)
      {
        inArray = checkArray(colourDetected);
        count = count + storageBlinker(inArray, count, 1, colourDetected);
      }
      else if (colourDetected == 2)
      {
        inArray = checkArray(colourDetected);
        count = count + storageBlinker(inArray, count, 2, colourDetected);
      }
      else if (colourDetected == 3)
      {
        inArray = checkArray(colourDetected);
        count = count + storageBlinker(inArray, count, 3, colourDetected);
      }
    }
  }
  delay(3000);
  blinkLED(4);
  while(1);
}
// ****************************************************************************
// Storing identified colours in array
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
// ****************************************************************************
// Checking if colour already identified
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
// ****************************************************************************
// Making blue LED blink a specific number of times for 1 second at a time
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
// ****************************************************************************
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
// ****************************************************************************
//                                  END
// ****************************************************************************
