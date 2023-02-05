#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

#include <SoftwareSerial.h>
#define TX 7
#define RX 8

SoftwareSerial bluetooth(TX, RX);

#define RST_PIN         9         
#define SS_PIN          10 

int IP[4],i;
int IP1[4] = {160, 58, 237, 32};

int Moter = A0;
int LedRed = A1;
int LedGreen = A2;
int Voice = A3;

char input_value;
Servo myservo;
MFRC522 mfrc522(SS_PIN, RST_PIN);  
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
	Serial.begin(9600);
  bluetooth.begin(9600);
  while (!Serial);
  mfrc522.PCD_Init();
	SPI.begin();
  lcd.begin();		
  lcd.backlight();
  pinMode(Voice, OUTPUT);
  myservo.attach(Moter);
  
  myservo.write(0); 
  analogWrite(LedRed, 255);
  analogWrite(LedGreen, 0);

  LCD_default();
}
void LCD_default(){
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("CARD CHECK");
}

void voice() {
  tone(Voice, 493, 300);
  delay(500);
  tone(Voice, 493, 300);
}

void LedOn(){
  analogWrite(LedGreen, 255);
  analogWrite(LedRed, 0);
}
void LedOff(){
  analogWrite(LedGreen, 0);
  analogWrite(LedRed, 255);
}

void Time(){

  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("RIGHT CARD");
  delay(1000);

  for (int s = 5; s > 0; s--){
    lcd.setCursor(0,0);
    lcd.print("CLOSE AFTER:  ");
    lcd.setCursor(13,0);
    lcd.print(s,DEC);
    lcd.print("S");
    delay(1000);
  }
}
void loop() {
  if (bluetooth.available() > 0 ) {
    input_value = bluetooth.read();
    Serial.println(input_value);
    switch (input_value){
      case '1':
      voice();
      LedOn(); 
      myservo.write(90);
      break;
      case '2':
      voice();
      LedOff();
      LCD_default();
      myservo.write(0);
      break;
    }
     
  }

  if ( ! mfrc522.PICC_IsNewCardPresent()) {
		return;
	}

	
	if ( ! mfrc522.PICC_ReadCardSerial()) {
		return;
	}

  Serial.print("INPUT ID: ");
  for (byte i = 0; i< mfrc522.uid.size; i++ ){
    IP[i] = mfrc522.uid.uidByte[i];
    Serial.print(IP[i], DEC);
    Serial.print(" ");
  }
  Serial.println("");
    if ( IP[i] == IP1[i]){
      voice();
      LedOn();
      myservo.write(90);
      delay(100);
      Time();
      LedOff();
      LCD_default();
      myservo.write(0);
      
    }

    else if(IP[i] != IP1[i]){
      lcd.clear();
      lcd.setCursor(3,0);
      lcd.print("WRONG CARD");
      tone(Voice, 493, 2000);
      delay(2000);
      LCD_default();

    }
  }
