#include <SPI.h>                          //SPI kütüphanemizi tanımlıyoruz.
#include <MFRC522.h>                      //MFRC522 kütüphanemizi tanımlıyoruz.
#include <Servo.h>                        //Servo kütüphanemizi tanımlıyoruz.

int RST_PIN = 9;                         
int SS_PIN = 10;                         
int servoPin = 8;                        
Servo motor;
MFRC522 rfid(SS_PIN, RST_PIN);           
byte ID[4] = {114, 33, 62, 81};         

int redLEDPin = 6;
int greenLEDPin = 3;

void setup() { 
  motor.attach(servoPin);
  Serial.begin(9600);                    
  SPI.begin();                           
  rfid.PCD_Init();                       

  pinMode(redLEDPin, OUTPUT);    
  pinMode(greenLEDPin, OUTPUT);  
  digitalWrite(redLEDPin, LOW);  
  digitalWrite(greenLEDPin, LOW); 

}
 
void loop() {

  if ( ! rfid.PICC_IsNewCardPresent())  
    return;

  if ( ! rfid.PICC_ReadCardSerial())      
    return;

  if (rfid.uid.uidByte[0] == ID[0] &&   
    rfid.uid.uidByte[1] == ID[1] && 
    rfid.uid.uidByte[2] == ID[2] && 
    rfid.uid.uidByte[3] == ID[3] ) {
        Serial.println("Kapi acildi");
        ekranaYazdir();
        digitalWrite(greenLEDPin, HIGH);
        motor.write(100);                
        delay(2000);
        motor.write(0);                  
        delay(500);
        digitalWrite(greenLEDPin, LOW);

    }
    else{                                
      Serial.println("Yetkisiz Kart");
      ekranaYazdir();
      digitalWrite(redLEDPin, HIGH);
      delay(500);
      digitalWrite(redLEDPin, LOW);     
    }
  rfid.PICC_HaltA();
}
void ekranaYazdir(){
  Serial.print("ID Numarasi: ");
  for(int sayac = 0; sayac < 4; sayac++){
    Serial.print(rfid.uid.uidByte[sayac]);
    Serial.print(" ");
  }
  Serial.println("");
}
