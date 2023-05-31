// LED positive pin connected to Digital pins
int LED1 = 7;
int LED2 = 8;
int LED3 = 12;
int LED4 = 13;
//int LED5 = 10;
// IR Output pin connected to Digital pins
int IR1 = 2;
int IR2 = 3;
int IR3 = 4;
//int IR4 = 5;
int LDR = A0; // LDR Output pin connected to Analog pin "A0"
int MQ_135 = A1; // MQ 135 Output pin connect to Analog pin "A1"
int Brightness_Lvl = 10;

void setup()
{
  Serial.begin (9600);
  // initialize LED pins as an output
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
//  pinMode(LED5,OUTPUT);
  // initialize LED Pin as an input
  pinMode(LDR, INPUT);
  pinMode(MQ_135, INPUT);
}
void loop()
{
  // Read LDR value
  int LDRStatus = analogRead(LDR);
  // Read MQ_135 value
  int AQI = analogRead(MQ_135);
  // Serial.print("LDR Value = ");
  // Serial.println(LDRStatus);
   delay(1);

  if (LDRStatus < 400)
  {
    digitalWrite(LED1, HIGH);
    analogWrite(LED1, Brightness_Lvl);
    digitalWrite(LED2, HIGH);
    analogWrite(LED2, Brightness_Lvl);
    digitalWrite(LED3, HIGH);
    analogWrite(LED3, Brightness_Lvl);
    digitalWrite(LED4, HIGH);
    analogWrite(LED4, Brightness_Lvl);
    //  digitalWrite(LED5,HIGH);
    //  analogWrite(LED5,Brightness_Lvl);

    // IR  Sensor 1 CODE
    //*******************
    if (digitalRead(IR1) == 0) // Read IR Sensor 1 value
    {
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
    }
    // IR  Sensor 2 CODE
    //*******************
    if (digitalRead(IR2) == 0) // Read IR Senso 2 value
    {
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
    }
    // IR  Sensor 3 CODE
    //*******************
    if (digitalRead(IR3) == 0) // Read IR Senso 3 value
    {
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, HIGH);
    }
  }
  else
  {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
//    digitalWrite(LED5, LOW);
  }

  // Read MQ_135 Sensor for Checking Air Quality Index

  Serial.print("LDR Value = ");
  Serial.print(LDRStatus);
  Serial.print("\t |  ");
  Serial.print("AQI = ");
  Serial.print(AQI, DEC);
  Serial.print(" PPM -> ");

  if (AQI <= 350) {
    Serial.println("Fresh Air :)");
  } else if (AQI > 350 && AQI <= 600) {
    Serial.println("Poor Air :|");
  } else if (AQI > 600 && AQI <= 1000) {
    Serial.println("Very Poor :(");
  }
}
