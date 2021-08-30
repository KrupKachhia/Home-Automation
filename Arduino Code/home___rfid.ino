
//INCLUDING THE FILES FOR 
// THE SERVO MOTOR & RFID 
#include<SPI.h>
#include<MFRC522.h>
#include<Servo.h>

// LAMP DECLARATION
#define Lamp1 2
#define Lamp2 4
#define Lamp3 7

//LED FOR RFID
#define Led1 5
#define Led2 6

// RFID PIN DECLARATION
#define SS_PIN 10
#define RST_PIN 9

// SERVO MOTOR DECLARATION
#define SERVO_PIN 3
Servo myservo;


//PIORITY DECLARATION
#define ON HIGH
#define OFF LOW

// ACCESS & DELAY
#define ACCESS_DELAY 2500
#define DENIED_DELAY 2500

//CREATE MFRC522
MFRC522 mfrc522(SS_PIN,RST_PIN);

char val;
String statusLamp1;
String statusLamp2;
String statusLamp3;

void setup() 
{
  
  // INITIALIZATION OF INPUT/OUTPUT OF LAMP
  pinMode(Lamp1,OUTPUT);
  digitalWrite(Lamp1,ON);
  delay(75000);
  pinMode(Lamp2,OUTPUT);
  digitalWrite(Lamp2,ON);
  pinMode(Lamp3,OUTPUT);
  digitalWrite(Lamp3,ON);

  // INITIALIZATION OF LED
  pinMode(Led1,OUTPUT);
  digitalWrite(Led1,OFF);
  pinMode(Led2,OUTPUT);
  digitalWrite(Led2,OFF);

  //INITIATE A SERIAL COMMUNICATION
  Serial.begin(9600);

  //INITIATE SPI BUS
  SPI.begin();

  //INITIATE MFRC522
  mfrc522.PCD_Init();

  myservo.attach(SERVO_PIN);
  myservo.write(70);
  myservo.write(0);
  Serial.println("Put your card to the reader ....");
  Serial.println();
  
}

void loop() 
{

  //LOOK FOR THE NEW CARD
  if(!mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }

  //SELECT ONE OF THE CARD
  if(!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

  //SHOW UID ON SERIAL MONITOR
  Serial.print("UID tag :- ");
  String content="";

  byte letter;
  for(byte i=0;i<mfrc522.uid.size;i++)
  {
    Serial.print(mfrc522.uid.uidByte[i]<0x10?"0":" ");
    Serial.print(mfrc522.uid.uidByte[i],HEX);
    content.concat(String(mfrc522.uid.uidByte[i]<0x10?"0":" "));
    content.concat(String(mfrc522.uid.uidByte[i],HEX));
  }

  Serial.println();
  Serial.print("Message :- ");
  content.toUpperCase();

  //CHANGE HERE THE UID
  if(content.substring(1)=="B7 D5 79 60")
  {
    Serial.println("Authorized access");
    Serial.println();
    myservo.write(70);
    delay(ACCESS_DELAY);
    digitalWrite(Led1,ON);
    delay(ACCESS_DELAY);
    myservo.write(0);  
  }
  else
  {
    Serial.println("Access denied");
    delay(DENIED_DELAY);
    digitalWrite(Led2,ON);
    delay(DENIED_DELAY);
  }
  
  // BLUETHOOT INITIALIZATION FROM THE MOBILE
  while (Serial.available()>0)
  {
    val = Serial.read();
    Serial.println(val);
  }

  // LAMP IS ON
  if(val=='1')
  {
    digitalWrite(Lamp1,ON);
    statusLamp1="1";
  }
  else if(val=='2')
  {
    digitalWrite(Lamp2,ON);
    statusLamp2="2";
  }
  else if(val=='3')
  {
    digitalWrite(Lamp3,ON);
    statusLamp3="3";
  }
  else if(val=='4')
  {
    digitalWrite(Lamp1,ON);
    digitalWrite(Lamp2,ON);
    statusLamp1="1";
    statusLamp2="2";
  }
  else if(val=='5')
  {
    digitalWrite(Lamp1,ON);
    digitalWrite(Lamp3,ON);
    statusLamp1="1";
    statusLamp3="3";
  }
  else if(val=='6')
  {
    digitalWrite(Lamp3,ON);
    digitalWrite(Lamp2,ON);
    statusLamp2="2";
    statusLamp3="3";
  }
  else if(val=='9')
  {
    digitalWrite(Lamp1,ON);
    digitalWrite(Lamp2,ON);
    digitalWrite(Lamp3,ON);
    statusLamp1="1";
    statusLamp2="2";
    statusLamp3="3";
  }

  //LAMP IS OFF
  else if (val=='A')
  {
    digitalWrite(Lamp1,OFF);
    statusLamp1="A";  
  }
  else if(val=='B')
  {
    digitalWrite(Lamp2,OFF);
    statusLamp2="B";
  }
  else if(val=='C')
  {
    digitalWrite(Lamp3,OFF);
    statusLamp3="C";
  }
  else if(val=='D')
  {
    digitalWrite(Lamp1,OFF);
    digitalWrite(Lamp2,OFF);
    statusLamp1="A";
    statusLamp2="B";
  }
  else if(val=='E')
  {
    digitalWrite(Lamp1,OFF);
    digitalWrite(Lamp3,OFF);
    statusLamp1="A";
    statusLamp3="C";
  }
  else if(val=='F')
  {
    digitalWrite(Lamp3,OFF);
    digitalWrite(Lamp2,OFF);
    statusLamp2="B";
    statusLamp3="C";
  }
  else if(val=='I')
  {
    digitalWrite(Lamp1,OFF);
    digitalWrite(Lamp2,OFF);
    digitalWrite(Lamp3,OFF);
    statusLamp1="A";
    statusLamp2="B";
    statusLamp3="C";
  }

  //SYNCHRONIZE ARDUINO TO APK
  else if(val=='S')
  {

    //SEND TO ANDRIOD APK //delay(500);
    delay(500);
    Serial.println(statusLamp1+statusLamp2+statusLamp3+"J");
    val=' ';
  }
}
