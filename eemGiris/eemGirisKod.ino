#include <SPI.h>                          //SPI kütüphanemizi tanımlıyoruz.
#include <MFRC522.h>                      //MFRC522 kütüphanemizi tanımlıyoruz.
#include <Servo.h>                        //Servo kütüphanemizi tanımlıyoruz.

int RST_PIN = 9;                          //RC522 modülü reset pinini tanımlıyoruz.
int SS_PIN = 10;                          //RC522 modülü chip select pinini tanımlıyoruz.
int servoPin = 8;                         //Servo motor pinini tanımlıyoruz.
Servo motor;
MFRC522 rfid(SS_PIN, RST_PIN);            //RC522 modülü ayarlarını yapıyoruz.s
byte ID[4] = {114, 33, 62, 81};          //Yetkili kart ID'sini tanımlıyoruz. 

int redLEDPin = 6;
int greenLEDPin = 3;

void setup() { 
  motor.attach(servoPin);                 //Servo motor pinini motor değişkeni ile ilişkilendiriyoruz.
  Serial.begin(9600);                     //Seri haberleşmeyi başlatıyoruz.
  SPI.begin();                            //SPI iletişimini başlatıyoruz.
  rfid.PCD_Init();                        //RC522 modülünü başlatıyoruz.

  pinMode(redLEDPin, OUTPUT);    // Kırmızı LED'i çıkış olarak ayarla
  pinMode(greenLEDPin, OUTPUT);  // Yeşil LED'i çıkış olarak ayarla
  digitalWrite(redLEDPin, LOW);  // Kırmızı LED'i başlangıçta kapalı yap
  digitalWrite(greenLEDPin, LOW); // Yeşil LED'i başlangıçta kapalı yap

}
 
void loop() {

  if ( ! rfid.PICC_IsNewCardPresent())    //Yeni kartın okunmasını bekliyoruz.
    return;

  if ( ! rfid.PICC_ReadCardSerial())      //Kart okunmadığı zaman bekliyoruz.
    return;

  if (rfid.uid.uidByte[0] == ID[0] &&     //Okunan kart ID'si ile ID değişkenini karşılaştırıyoruz.
    rfid.uid.uidByte[1] == ID[1] && 
    rfid.uid.uidByte[2] == ID[2] && 
    rfid.uid.uidByte[3] == ID[3] ) {
        Serial.println("Kapi acildi");
        ekranaYazdir();
        digitalWrite(greenLEDPin, HIGH);
        motor.write(100);                 //Servo motoru 180 dereceye getiriyoruz.
        delay(2000);
        motor.write(0);                   //Servo motoru 0 dereceye getiriyoruz.
        delay(500);
        digitalWrite(greenLEDPin, LOW);

    }
    else{                                 //Yetkisiz girişte içerideki komutlar çalıştırılır.
      Serial.println("Yetkisiz Kart");
      ekranaYazdir();
      digitalWrite(redLEDPin, HIGH);    // Kırmızı LED'i yak
      delay(500);
      digitalWrite(redLEDPin, LOW);     // Kırmızı LED'i kapat
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
