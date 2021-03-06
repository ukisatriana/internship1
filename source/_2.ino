#include <Wire.h> 
#include <Keypad.h>
#include <SPI.h>
#include <RFID.h>
#include <LCD.h>

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

#define SS_PIN 10
#define RST_PIN 9

#include "Arduino.h"
#include <SoftwareSerial.h>

#define DEBUG true

const String HOST = "192.168.43.165";
const String SSID = "Tolonglah aku";
const String PASS = "siapakankamitolong";

SoftwareSerial ser(7, 8); // RX 2, TX 3

RFID rfid(SS_PIN,RST_PIN);
int serNum[5];          
int cards[][5] = {     
  {                                                                                                                                                                                           36, 204, 170, 137, 203}
};

bool access = false;

const byte ROWS = 4; 
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = { 6,5,4,3 };
byte colPins[COLS] = { A3,A2,A1,A0 };


Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

unsigned long previousMillis = 0; 
const long interval = 1000;  
uint8_t pwMode=0;
uint8_t pwPos=0;
uint8_t pwFirst=0;

byte data_count = 1;
char value[0];

int total;
int jenis;

void setup(){
    Serial.begin(9600);
    ser.begin(9600);
    delay(100);
    connectWifi();
    delay(100); 
    lcd.begin(16,2);
    SPI.begin();
    rfid.init();
    keypad.addEventListener(keypadEvent); 

    lcd.setCursor (0,0);
    lcd.print(F("  RFID System   "));
    lcd.setCursor (0,1);
    lcd.print(F(" Control Access "));
    delay (2000);
    lcd.clear();
}

void loop(){
  keypad.getKey(); 

   
  if (pwMode  == 0) {
  if (pwPos == 0) {
    lcd.setCursor (0,0);
    lcd.print(F(" -System Ready- "));

    lcd.setCursor (0,1);
    
    lcd.print(F(" Scan Your Card "));
  }
  
    if(rfid.isCard()){
        if(rfid.readCardSerial()){
            Serial.println("");
            Serial.print(rfid.serNum[0]);
            Serial.print(" ");
            Serial.print(rfid.serNum[1]);
            Serial.print(" ");
            Serial.print(rfid.serNum[2]);
            Serial.print(" ");
            Serial.print(rfid.serNum[3]);
            Serial.print(" ");
            Serial.print(rfid.serNum[4]);
            Serial.println("");
            
            for(int x = 0; x < sizeof(cards); x++){
              for(int i = 0; i < sizeof(rfid.serNum); i++ ){
                  if(rfid.serNum[i] != cards[x][i]) {
                      access = false;
                      break;
                  } else {
                      access = true;
                  }
              }
              if(access) break;
            }   
        }
        
       if(access){
          Serial.println("Akses Diterima");
          Serial.println("----------------------------------------");
          lcd.setCursor (0,0);
          lcd.print(F(" Akses diterima "));
          lcd.setCursor (0,1);
          lcd.print("ID:");
          lcd.print(rfid.serNum[0]); lcd.print(rfid.serNum[1]);
          lcd.print(rfid.serNum[2]); lcd.print(rfid.serNum[3]);
          lcd.print(rfid.serNum[4]);
          delay (2000);
          ACCEPT ();
       } 
      
      else {
                    Serial.println("");
            Serial.print(rfid.serNum[0]);
            Serial.print(" ");
            Serial.print(rfid.serNum[1]);
            Serial.print(" ");
            Serial.print(rfid.serNum[2]);
            Serial.print(" ");
            Serial.print(rfid.serNum[3]);
            Serial.print(" ");
            Serial.print(rfid.serNum[4]);
            Serial.println("");
           Serial.println("Akses Ditolak");
           Serial.println("----------------------------------------");
           lcd.setCursor (0,0);
           lcd.print(F(" Akses ditolak  "));
           lcd.setCursor (0,1);
           lcd.print("ID:");
           lcd.print(rfid.serNum[0]); lcd.print(rfid.serNum[1]);
           lcd.print(rfid.serNum[2]); lcd.print(rfid.serNum[3]);
           lcd.print(rfid.serNum[4]);
           RIJECT ();    
       }        
    }

    rfid.halt();
  }
}

void keypadEvent(KeypadEvent eKey){
  if (pwMode == 1) {  
    switch (keypad.getState()){
      case PRESSED:
      
      if (pwFirst == 1) {
        if(pwPos<6){
          lcd.setCursor (-1+pwPos,1); 
          lcd.print(eKey);
          if (eKey != '#') {
            if (eKey != '*') {
              if (eKey != 'D') {
                if (eKey != 'C') {
                  if (eKey != 'B') {
                    if (eKey != 'A') { 
                      Serial.print(eKey);
                      value[data_count] = eKey;
                      data_count++;
                    }
                  }
                }
              }
            }
          }
          pwPos=pwPos+1;
        }
        total = atoi(value);
      }
      
      if (pwFirst == 0) {
         switch (eKey){
          case '#': first(); break;
          case '*': resetAll(); break;
        }
      }
      if (pwFirst == 1) {
         switch (eKey){
          case 'D': clear(); break;
          case 'C': premium(); break;
          case 'B': pertamax(); break;
          case 'A': solar(); break;
          case '*': resetAll(); break;
        }
      }
    }
  }
}

void first() {
  Serial.println("----------------------------------------");
  Serial.println("-----------------Lanjut-----------------");
  Serial.println("----------------------------------------");
  while(data_count !=0)
  { 
    value[data_count--] = 0; 
  } 
  pwFirst = 1;
  
  lcd.clear(); 
  lcd.setCursor (0,0);
  lcd.print(F("Jumlah Liter :   "));
  Serial.print("Jumlah Liter : ");
}

void premium() {
  Serial.println(); 
  Serial.println("----------------------------------------");
  Serial.print("value :");
  Serial.print(total);
  Serial.println(); 
  
  pwMode = 0;
  pwFirst = 0;
  lcd.clear(); 
  
  delay(500);
  lcd.setCursor (0,0);
  lcd.print("Jenis :         ");
  lcd.setCursor (0,1);
  lcd.print("Premium         ");
  
  delay(1500);
  lcd.setCursor (0,0);
  lcd.print(" ");
  lcd.setCursor (1,0);
  lcd.print(total);
  lcd.print(" Liter   ");
  lcd.setCursor (0,1);
  lcd.print(" 1 Liter = 7000 ");

  delay(1500);
  lcd.setCursor (0,0);
  lcd.print(" Tunggu Sebentar");
  lcd.setCursor (0,1);
  lcd.print("                ");
  
  jenis = 3;
  sendDataSaldo(total,jenis);
  
  lcd.setCursor (0,0);
  lcd.print("    Berhasil    ");
  lcd.setCursor (0,1);
  lcd.print("                ");
  
  delay(5000);
  lcd.setCursor (0,0);
  lcd.print(F(" -System Ready- "));
  lcd.setCursor (0,1);
  lcd.print(F(" Scan Your Card "));

  total = 0;
  while(data_count !=0)
  { 
    value[data_count--] = 0; 
  }

  Serial.println("----------------------------------------");
  Serial.println("----------------------------------------");
  Serial.println("----------------Selesai-----------------");
}

void pertamax() {
  Serial.println(); 
  Serial.println("----------------------------------------");
  Serial.print("value :");
  Serial.print(total);
  Serial.println(); 
  
  pwMode = 0;
  pwFirst = 0;
  lcd.clear(); 
  
  delay(500);
  lcd.setCursor (0,0);
  lcd.print("Jenis :         ");
  lcd.setCursor (0,1);
  lcd.print("Pertamax        ");
  
  delay(1500);
  lcd.setCursor (0,0);
  lcd.print(" ");
  lcd.setCursor (1,0);
  lcd.print(total);
  lcd.print(" Liter   ");
  lcd.setCursor (0,1);
  lcd.print(" 1 Liter = 8000 ");

  delay(1500);
  lcd.setCursor (0,0);
  lcd.print(" Tunggu Sebentar");
  lcd.setCursor (0,1);
  lcd.print("                ");
  
  jenis = 2;
  sendDataSaldo(total,jenis);
  
  lcd.setCursor (0,0);
  lcd.print("    Berhasil    ");
  lcd.setCursor (0,1);
  lcd.print("                ");
  
  delay(5000);
  lcd.setCursor (0,0);
  lcd.print(F(" -System Ready- "));
  lcd.setCursor (0,1);
  lcd.print(F(" Scan Your Card "));

  total = 0;
  while(data_count !=0)
  { 
    value[data_count--] = 0; 
  }

  Serial.println("----------------------------------------");
  Serial.println("----------------------------------------");
  Serial.println("----------------Selesai-----------------");
}

void solar() {
  Serial.println(); 
  Serial.println("----------------------------------------");
  Serial.print("value :");
  Serial.print(total);
  Serial.println(); 
  
  pwMode = 0;
  pwFirst = 0;
  lcd.clear(); 
  
  delay(500);
  lcd.setCursor (0,0);
  lcd.print("Jenis :         ");
  lcd.setCursor (0,1);
  lcd.print("Solar           ");
  
  delay(1500);
  lcd.setCursor (0,0);
  lcd.print(" ");
  lcd.setCursor (1,0);
  lcd.print(total);
  lcd.print(" Liter   ");
  lcd.setCursor (0,1);
  lcd.print(" 1 Liter = 5000 ");

  delay(1500);
  lcd.setCursor (0,0);
  lcd.print(" Tunggu Sebentar");
  lcd.setCursor (0,1);
  lcd.print("                ");
  
  jenis = 1;
  sendDataSaldo(total,jenis);
  
  lcd.setCursor (0,0);
  lcd.print("    Berhasil    ");
  lcd.setCursor (0,1);
  lcd.print("                ");
  
  delay(5000);
  lcd.setCursor (0,0);
  lcd.print(F(" -System Ready- "));
  lcd.setCursor (0,1);
  lcd.print(F(" Scan Your Card "));

  total = 0;
  while(data_count !=0)
  { 
    value[data_count--] = 0; 
  }

  Serial.println("----------------------------------------");
  Serial.println("----------------------------------------");
  Serial.println("----------------Selesai-----------------");
}

void resetAll() {
  while(data_count !=0)
  { 
    value[data_count--] = 0; 
  }
  pwMode = 0; 
  pwFirst = 0;
  lcd.clear(); 
  Serial.println();
  Serial.println("----------------------------------------");
  Serial.print("Reset");
  Serial.println();
  Serial.println("----------------------------------------");
  Serial.println("----------------------------------------");
  Serial.println("----------------Selesai-----------------");

  lcd.setCursor (0,0);
  lcd.print(F("      Reset     "));
  lcd.setCursor (0,1);
  lcd.print(F("                "));
    
  delay(2000);
    
  lcd.setCursor (0,0);
  lcd.print(F(" -System Ready- "));
  lcd.setCursor (0,1);
  lcd.print(F(" Scan Your Card "));
}

void clear(){  
  while(data_count !=0)
  { 
    value[data_count--] = 0; 
  }
  lcd.setCursor(0,1);
  lcd.print("                ");
  Serial.println();
  Serial.print("Clear");
  Serial.println();
  Serial.print("Jumlah Liter : ");
  pwPos = 1;
  pwFirst = 1;
}

void ACCEPT () {
  Serial.println("ID RFID Diterima");
  
  lcd.setCursor (0,0);
  lcd.print(F("Silahkan Masukan"));
  lcd.setCursor (0,1);
  lcd.print(F("  Jumlah Liter  "));
  pwPos = 1;
  pwMode =1;
  pwFirst = 0;

  delay(2000);
  
  lcd.clear();
  lcd.setCursor (0,0);
  lcd.print("Tekan # = Lanjut");
  lcd.setCursor (0,1);
  lcd.print("Tekan * = Cancel");
  Serial.print("Tekan # = Lanjut");
  Serial.println();
  Serial.print("Tekan * = Cancel");
  Serial.println();
}
 
void RIJECT () {
  Serial.println("ID RFID Ditolak");

  lcd.clear();
  lcd.setCursor (0,0);
  lcd.print(F("  RFID Ditolak  "));
  lcd.setCursor (0,1);
  lcd.print(F("                "));
  
  delay(4000);
  lcd.clear();
 }

void sendDataSaldo(int saldoValue, int jenis)
{
  String cmd = "AT+CIPSTART=\"TCP\",\"";

  cmd += HOST;
  cmd += "\",80\r\n";
  sendCommand(cmd,500,DEBUG);
  delay(200);

  String cmd2 = "GET /pos-arduino/updatesaldo.php";

  cmd2 += "?s=";
  cmd2 += String(saldoValue);
  cmd2 += "&r=";
  cmd2 += String(rfid.serNum[0]);
  cmd2 += String(rfid.serNum[1]);
  cmd2 += String(rfid.serNum[2]);
  cmd2 += String(rfid.serNum[3]);
  cmd2 += String(rfid.serNum[4]);
  cmd2 += "&j=";
  cmd2 += String(jenis);
  cmd2 += "&p=";
  cmd2 += "6";
  
  cmd2 += " HTTP/1.1\r\n";
  cmd2 += "Host: " +HOST+ "\r\n\r\n";
  
  delay(500);
  String pjg="AT+CIPSEND=";
  pjg += cmd2.length();
  pjg += "\r\n";

  delay(500);
  String closeCommand = "AT+CIPCLOSE";
  closeCommand+="\r\n";

  sendCommand(pjg,500,DEBUG);
  delay(100);
  sendCommand(cmd2,800,DEBUG);
  delay(50);
  sendCommand(closeCommand,500,DEBUG);
  delay(10);
}

String sendCommand(String command, const int timeout, boolean debug) {
  String response = "";

  ser.print(command); // send the read character to the esp8266

  long int time = millis();

  while( (time+timeout) > millis()) {
    while(ser.available()) {
      // The esp has data so display its output to the serial window
      char c = ser.read(); // read the next character.
      response+=c;
    }
  }

  if(debug) {
    Serial.print(response);
  }

  return response;
}

void connectWifi() {
  //Set-wifi
  Serial.print("Restart Module...\n");
  sendCommand("AT+RST\r\n",1000,DEBUG);
  delay(1000);
  Serial.print("Set wifi mode...\n");
  sendCommand("AT+CWMODE=1\r\n",1000,DEBUG); //
  delay(1000);
  Serial.print("Connect to access point...\n");
  sendCommand("AT+CWJAP=\"" +SSID+ "\",\""+PASS+"\"\r\n",1000,DEBUG);
  delay(1000);
  Serial.print("Check IP Address...\n");
  sendCommand("AT+CIFSR\r\n",1000,DEBUG); // get ip address

  delay(100);
}
 
