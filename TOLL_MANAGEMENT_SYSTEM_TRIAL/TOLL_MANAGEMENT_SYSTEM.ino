/*----------------------------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino        ESP8266
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro      NodeMCU
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin            Pin
 * ---------------------------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST           D3  (GPIO 0)
 * SPI SDA(SS) SDA(SS)      10            53        D10        10               10            D4  (GPIO 2)
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16            D7  (GPIO 13)
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14            D6  (GPIO 12)
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15            D5  (GPIO 14)
 *----------------------------------------------------------------------------------------------------------
*/

#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7); //Parameters: (rs, enable, d4, d5, d6, d7) 

Servo servo;
int servo_pin = 8;
int servoPos = 15; // Position at 15 Degree (Rest Position)

#define sensorPin1 A0
#define sensorPin2 A1
#define sensorPin3 A2

int sensorValue1 = 0;
int sensorValue2 = 0;
int sensorValue3 = 0;

#define RST_PIN 9
#define SS_PIN 10

int card1Balance = 500;
int card2Balance = 70;

MFRC522 mfrc522(SS_PIN, RST_PIN);

int state = 0;

void setup () {
  Serial.begin(9600);
  
  servo.attach(servo_pin);
  servo.write(servoPos);

  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);

  SPI.begin(); // Init Serial Peripheral Interface(SPI) bus
  mfrc522.PCD_Init(); // Init MFRC522

  Serial.println(" Automatic toll colection system");

  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print(" Toll Management");
  lcd.setCursor(0,1);
  lcd.print("     System");
  delay(3000);
  lcd.clear();
}
void loop()
{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("   Welcome to");
    lcd.setCursor(0,1);
    lcd.print(" Western Highway");
    
    sensorRead();
    rfid();
    
    if (sensorValue1 == 0)
    {
      servoDown();
      Serial.println("Sersor 1 Active & Servo Down");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Vehicle detected");
      delay(1500);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Put your card");
      lcd.setCursor(0,1);
      lcd.print("near to reader...");
      delay(3000);
      lcd.clear();
    }
    else if (sensorValue2 == 0 && state == 1) // state is 1 when amount is deducted from rfid
    {
      servoUp();
      Serial.println("Sersor 2 Active & Servo Up");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Have a safe and");
      lcd.setCursor(0,1);
      lcd.print("  happy journey");
      delay(3000);
      lcd.clear();
      state = 0;
    }
    else if (sensorValue3 == 0)
    {
      delay(2000);
      servoDown();
      Serial.println("Sersor 3 Active & Servo Down");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("  Thank You :)");
      lcd.setCursor(0,1);
      lcd.print("  Visit Again");
      delay(2000);
      lcd.clear();
    }
}

void servoDown()
{
  servo.attach(servo_pin);
//  servo.write(15);
  for (servoPos = 90; servoPos >= 15; servoPos -= 1)
  {
    servo.write(servoPos);
//    delay(5);
  }
}

void servoUp()
{
  servo.attach(servo_pin);
//  servo.write(90);
  for (servoPos = 15; servoPos <= 90; servoPos += 1)
  {
    servo.write(servoPos);
//    delay(5);
  }
}

void sensorRead()
{
  sensorValue1 = digitalRead(sensorPin1);
  sensorValue2 = digitalRead(sensorPin2);
  sensorValue3 = digitalRead(sensorPin3);
}

void rfid()
{
  if ( ! mfrc522.PICC_IsNewCardPresent()) // Look for new card
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) // Verify if the UID has been readed
  {
    return;
  }

  String content = "";
  
  for (byte i = 0; i < mfrc522.uid.size; i++) // Store UID into uidbyte array
  {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();

  if (content.substring(1) == "B4 4F 1C 1E") // 2E F5 4F A9
  {
    Serial.println((String)"RFID:"+content);
    
    if (card1Balance >= 100)
    {
      card1Balance = card1Balance - 100;
      lcdPrint();
      Serial.println(card1Balance);
      lcd.setCursor(9, 1);
      lcd.print("Rs.");
      lcd.print(card1Balance);
      delay(3000);
      lcd.clear();
      state = 1;
    }
    else
    {
      LcdPrint();
      Serial.println(card1Balance);
      lcd.setCursor(9, 1);
      lcd.print("Rs.");
      lcd.print(card1Balance);
      delay(3000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Please Recharge");
      delay(1500);
      lcd.clear();
      state = 0;
    }
  }
  else if (content.substring(1) == "52 1E 51 83") // 1E 0C 44 A9
  {
    Serial.println((String)"RFID:"+content);
 
    if (card2Balance >= 100)
    {
      lcdPrint();
      card2Balance = card2Balance - 100;
      Serial.println(card2Balance);
      lcd.setCursor(9, 1);
      lcd.print("Rs.");
      lcd.print(card2Balance);
      delay(3000);
      lcd.clear();
      state = 1;
    }
    else
    {
      LcdPrint();
      Serial.println(card2Balance);
      lcd.setCursor(9, 1);
      lcd.print("Rs.");
      lcd.print(card2Balance);
      delay(3000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Please Recharge");
      delay(1500);
      lcd.clear();
      state = 0;
    }
  }

  else   
  {
    Serial.println("Unknown Vehicle! Access Denied! ");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Unknown Vehicle");
    lcd.setCursor(0,1);
    lcd.print(" Access denied");
    delay(2000);
    lcd.clear();
  }
}

void lcdPrint()
{
  Serial.println("Successfully! your bill is paid");
  Serial.print("Your Remaining balance: ");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("  Successfully");
  lcd.setCursor(0,1);
  lcd.print("  bill is paid");
  delay(1500);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Your Remaining");
  lcd.setCursor(0,1);
  lcd.print("balance:");
}

void LcdPrint()
{
  Serial.println("Sorry! Your Balance is insufficent");
  Serial.print("Your Remaining balance: ");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("  Your balance");
  lcd.setCursor(0,1);
  lcd.print(" is insufficent");
  delay(1500);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Your Remaining");
  lcd.setCursor(0,1);
  lcd.print("balance:");
}
