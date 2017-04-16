#define S0 3
#define S1 4
#define S2 5
#define S3 6
#define sensorOut 2
void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  
  // Setting frequency-scaling to 20%
  digitalWrite(S0,LOW);
  digitalWrite(S1,HIGH);
  
  Serial.begin(9600);
}
void loop() {
  for (int i=0 ; i<20 ; i++)
  {
    digitalWrite(13, HIGH);
    delay(500);
    // Setting red filtered photodiodes to be read
    digitalWrite(S2,LOW);
    digitalWrite(S3,LOW);
    // Reading the output frequency
    int frequency_R = pulseIn(sensorOut, LOW);
    // Serial.print("R= ");//printing name
    // Serial.print(frequency);//printing RED color frequency
    // Serial.print("  ");
    // delay(100);
    // Setting Green filtered photodiodes to be read
    digitalWrite(S2,HIGH);
    digitalWrite(S3,HIGH);
    // Reading the output frequency
    int frequency_G = pulseIn(sensorOut, LOW);
  
    // Serial.print("G= ");//printing name
    // Serial.print(frequency);//printing RED color frequency
    // Serial.print("  ");
    // delay(100);
    // Setting Blue filtered photodiodes to be read
    digitalWrite(S2,LOW);
    digitalWrite(S3,HIGH);
    // Reading the output frequency
    int frequency_B = pulseIn(sensorOut, LOW);
  
    // Serial.print("B= ");//printing name
    // Serial.print(frequency);//printing RED color frequency
    // Serial.println("  ");
    digitalWrite(13, LOW);

    if ( ( (frequency_R - frequency_B) < 80 ) && ( (frequency_R < frequency_B) && (frequency_B < frequency_G) ) )
    {
     // colour = 1;
     Serial.println("pink");
    }
    else if ( frequency_B < frequency_G && frequency_G < frequency_R )
    {
     // colour = 2;
     Serial.println("blue");
    }
    else if ( frequency_R < frequency_B && frequency_B < frequency_G )
    {
     // colour = 3;
     Serial.println("green");
    }
    delay(500);
  }
  while(1);
}
