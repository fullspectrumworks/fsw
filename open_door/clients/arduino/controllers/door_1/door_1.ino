/////////////////////////////////////////////////////////////////
//                Open Door Client-side Script                 //
//                    James Scott McDowell                     //
/////////////////////////////////////////////////////////////////
//  Required Hardware: 1x Arduino Uno                          //
//                     1x Arduino Ethernet Shield              //
//                     1x Parallax RFID Card Reader(blue model)//
//                     1x Parallax 4x4 Matrix Membrane Keypad  // 
//                     2x Ethernet cable                       //
//                     1x Router(tested on NETGEAR Wireless-N  //
//                                       150 Router WNR1000 v2)//
//                     1x Server(tested with 2008 Unibody      //
//                               MacBook and Raspberry Pi)     //
//                     1x Pushbutton                           //
//                     1x 10k Ohm resistor                     //
//                     1x Door Latch and accompanying driver   //
//  Required Software: Tested in OS X 10.6.8 (Snow Leopard) and//
//                     Raspbian (Debian Wheezy, release date:  //
//                               2015-05-05)                   //
//                     Arduino IDE 1.0.6                       //
//                     Open Door Server-side Software          //
//  Script Influences:                                         //
//    Arduino IDE's button example by DojoDave and Tom Igoe    //
//    --Public Domain                                          //
//    Custom keypad code by Kerry Krauss                       //
//    --Public Domain                                          //
//    Parallax LCD example by Gordon McComb                    //
//    --Creative Commons 3.0 SA license                        //
//    Arduino Reference webpage's RFID reader example #1 by    //
//      BARRAGAN and djmatic                                   //
//    --Unknown License (assumed to be Public Domain)          //
//    Arduino Reference webpage's EthernetClient example by    //
//      an unknown author                                      //
//    --Creative Commons Attribution-ShareAlike 3.0 License    //
//    NOTE: original comments and variable names have been     //
//          slightly modified for readability and clarity,     //
//          but otherwise, I gave credit where credit is due.  //
/////////////////////////////////////////////////////////////////
#include <Ethernet.h>
#include <SPI.h>
#include <SoftwareSerial.h>

/////////////////////////////////////////////////////////////////
//                     Global Variables                        //
/////////////////////////////////////////////////////////////////
//exit button variables
const int buttonPin = 3;     // the number of the pushbutton pin
int buttonState = 0;         // variable for reading the pushbutton status

//keypad constants
int r1=A5;
int r2=A4;
int r3=A3;
int r4=A2;
int c1=6;
int c2=7;
int c3=8;
const String enterPinMessage = "ENTER YOUR PIN:";
const String keypadTimeoutMessage1 = "KEYPAD TIMEOUT ";
int pinDigitDelay = 500;
int enterPinMessageDelay = 3000;
int keypadTimeLimit = 10000;
int keypadTimeoutMessageDelay = 3000;

//display constants
const int TxPin = 5;
SoftwareSerial mySerial = SoftwareSerial(255, TxPin);
const String systemStartMessage = "SYSTEM ONLINE";
int systemStartMessageDelay = 3000;
const String blankLine = "                ";

//door constants
const String accessGrantedMessage = "WELCOME        ";
const String accessDeniedMessage = "DENIED         ";
const int accessGrantedMessageDelay = 1000;
const int accessDeniedMessageDelay = 1000;
const int doorDelay = 3000;
const int doorDriverPin = 9;

//rfid reader constants
const int cardReadDelay = 1500;
const int enablePin = 2;
const String swipeRfidMessage = "SWIPE RFID CARD";

//client and server constants
byte macAddress[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED}; //default mac address used for this arduino
const byte ipAddress[] = {192, 168, 1, 3};                //address of a server on the local network. WARNING: the ip address is randomly reassigned find the new address with ifconfig or ipconfig
const int serverPort = 100;                               //specifies which port to connect to on the server (use port 101, 102, 103, etc. if you plan on using more than one client system)
const String successfulConnectionMessage = "CONNECTED";
const String unsuccessfulConnectionMessage = "FAILED";
const String queryStringVariableName = "rfid_code";
const String connectingMessage = "CONNECTING";
const int connectingMessageDelay = 1000;
const int successfulConnectionMessageDelay = 1000;
const int unsuccessfulConnectionMessageDelay = 1000;
const int clientDelay = 2500;
EthernetClient client;

//global variables used by the rfid reading algorithm
int  val = 0;
char code[10];
int bytesread = 0;

/////////////////////////////////////////////////////////////////
//                        Setup Block                          //
/////////////////////////////////////////////////////////////////
void setup() {
    // initialize the pushbutton pin as an input:
    pinMode(buttonPin, INPUT); 

    pinMode(r1,OUTPUT);
    pinMode(r2,OUTPUT);
    pinMode(r3,OUTPUT);
    pinMode(r4,OUTPUT);
    pinMode(c1,INPUT);
    pinMode(c2,INPUT);
    pinMode(c3,INPUT);
  
    pinMode(TxPin, OUTPUT);
    digitalWrite(TxPin, HIGH);
    mySerial.begin(9600);
    delay(100);
    displayMessage(systemStartMessage, blankLine, systemStartMessageDelay);
    Serial.begin(2400);                                   //RFID reader SOUT pin connected to Serial RX pin at 2400bps
    pinMode(enablePin, OUTPUT);                           //set digital pin 2 as OUTPUT to connect it to the RFID /ENABLE pin. Enabling the RFID reader causes the display to be less bright
    digitalWrite(enablePin, LOW);                         //activate the RFID reader
    pinMode(doorDriverPin, OUTPUT);                       //initialize digital pin 6 as an output; the normal pin 13 will not work with this configuration.
    Ethernet.begin(macAddress);                           //an ethernet cable must be plugged into the Arduino for this line to work.
}

String getPIN(){
  //As soon as the system is ready to collect the user's PIN, fire off a short tone to notify the user.
  mySerial.write(212);                                  // Quarter note
  mySerial.write(220);                                  // A tone

  String pin = "";
  String maskedPIN = "";
  int keypadTimer = 0;
  int val;

  while(pin.length() < 4 && keypadTimer < keypadTimeLimit){
    //read the state of the pushbutton value
    buttonState = digitalRead(buttonPin);

    //if the pushbutton is high, this means a user is trying to leave from the inside,
    //this will assign a special exit code to send to the server instead of a normal PIN
    if (buttonState == HIGH){
        pin = "EXIT";
        break;
    }

    //setting the columns as high initially
    digitalWrite(c1,HIGH);
    digitalWrite(c2,HIGH);
    digitalWrite(c3,HIGH);
    //checking everything one by one
    //case 1: col1 =0 while other col as 1
    digitalWrite(r1,LOW);
    digitalWrite(r2,HIGH);
    digitalWrite(r3,HIGH);
    digitalWrite(r4,HIGH);
    //checking each column for row1 one by one
    if(digitalRead(c1)==0)
    { pin += "1"; maskedPIN += "*"; }
    else if(digitalRead(c2)==0)
    { pin += "2"; maskedPIN += "*"; }
    else if(digitalRead(c3)==0)
    { pin += "3"; maskedPIN += "*"; }
    //case 2: col2 =0 while other col as 1
    digitalWrite(r1,HIGH);
    digitalWrite(r2,LOW);
    digitalWrite(r3,HIGH);
    digitalWrite(r4,HIGH);
    //checking each column for row1 one by one
    if(digitalRead(c1)==0)
    { pin += "4"; maskedPIN += "*"; }
    else if(digitalRead(c2)==0)
    { pin += "5"; maskedPIN += "*"; }
    else if(digitalRead(c3)==0)
    { pin += "6"; maskedPIN += "*"; }
    //case 3: col3 =0 while other col as 1
    digitalWrite(r1,HIGH);
    digitalWrite(r2,HIGH);
    digitalWrite(r3,LOW);
    digitalWrite(r4,HIGH);
    //checking each column for row1 one by one
    if(digitalRead(c1)==0)
    { pin += "7"; maskedPIN += "*"; }
    else if(digitalRead(c2)==0)
    { pin += "8"; maskedPIN += "*"; }
    else if(digitalRead(c3)==0)
    { pin += "9"; maskedPIN += "*"; }
    //case 1: col1 =0 while other col as 1
    digitalWrite(r1,HIGH);
    digitalWrite(r2,HIGH);
    digitalWrite(r3,HIGH);
    digitalWrite(r4,LOW);
    //checking each column for row1 one by one
    if(digitalRead(c2)==0)
    { pin += "0"; maskedPIN += "*"; }
    delay(100);
    keypadTimer += 100; 
    
    int blankLine_maskedPIN_length_difference = blankLine.length() - maskedPIN.length();
    String tempMaskedPIN = maskedPIN;
    for(int i = 0; i < blankLine_maskedPIN_length_difference; i++){
        tempMaskedPIN += " ";
    }
    
    displayPersistentMessage(enterPinMessage, tempMaskedPIN);
  }
  if(keypadTimer == keypadTimeLimit){
    displayMessage(keypadTimeoutMessage1, blankLine, keypadTimeoutMessageDelay);
  }
  return pin;
}

//displays a message on the 2 x 16 Parallax lcd with no delay
//the message is meant to display "constantly;" in reality this means that the message is written to the lcd as fast as possible
//at the time of writing the only way to display a persistent message is to overwrite both lines on the lcd every time the function is called
//it is advised that you use this function in tight loops
void displayPersistentMessage(String firstLine, String secondLine){        
    mySerial.write(17);                                   // Turn backlight on
    mySerial.print(firstLine);                            // First line
    mySerial.write(13);                                   // Form feed
    mySerial.print(secondLine);                           // Second line
}

//displays a message on the 2 x 16 Parallax lcd for a short period of time
void displayMessage(String firstLine, String secondLine, int delayTime){
    mySerial.write(12);                                   // Clear             
    mySerial.write(17);                                   // Turn backlight on
    delay(5);                                             // Required delay
    mySerial.print(firstLine);                            // First line
    mySerial.write(13);                                   // Form feed
    mySerial.print(secondLine);                           // Second line
    mySerial.write(212);                                  // Quarter note
    mySerial.write(220);                                  // A tone
    delay(delayTime);                                     // Wait for a specified time
    mySerial.write(18);                                   // Turn backlight off
    mySerial.write(12);                                   // Clear
}

/////////////////////////////////////////////////////////////////
//                         Functions                           //
/////////////////////////////////////////////////////////////////
//rfid reading algorithm that returns the rfid code as a character array
char* getRFID(){
    if(Serial.available() > 0) {                          //if data available from reader
        if((val = Serial.read()) == 10) {                 //check for header
            bytesread = 0;
            while(bytesread<10) {                         //read 10 digit code
                if( Serial.available() > 0) {
                    val = Serial.read();
                    if((val == 10)||(val == 13)) {        //if header or stop bytes before the 10 digit reading
                        break;                            //stop reading
                    }
                    code[bytesread] = val;                //add the digit
                    bytesread++;                          //ready to read next digit
                }
            }
            if(bytesread == 10) {                         //if 10 digit read is complete
                //Serial.println(code);                   //print the TAG code
                bytesread = 0;
                Serial.end();                             //end Serial object to eliminate any possibility of reading any new bytes
                digitalWrite(enablePin, HIGH);            //deactivate the RFID reader for a moment so it will not flood
                delay(cardReadDelay);                     //wait for a bit
                digitalWrite(enablePin, LOW);             //activate the RFID reader
                Serial.begin(2400);                       //restart the Serial object to start reading bytes again
                return code;
            }
        }
    }
}

//activates the door driver for a few seconds upon receiving a success message from the server
//otherwise, a failure message is returned by the server and the door driver is not activated
void openDoor(char controlCharacter){
  if(controlCharacter == '1'){                              //a server response of '1' means that the user is allowed door access
    //Sends out a short tone to notify the user that they are welcome to enter
    mySerial.write(212);                                  // Quarter note
    mySerial.write(220);                                  // A tone
    
    displayPersistentMessage(accessGrantedMessage, blankLine);
    digitalWrite(doorDriverPin, HIGH);                    //turn the door driver on (HIGH is the voltage level)
    delay(doorDelay);                                     //holds the door driver output high for a brief period of time to allow the user time to open the door
    digitalWrite(doorDriverPin, LOW);                     //turn the door driver off by making the voltage LOW
  }
  else if(controlCharacter == '0'){                         //a server response of '0' means that the user has been denied door access
    displayMessage(accessDeniedMessage, blankLine, accessDeniedMessageDelay);
  }
}

//performs a custom HTTP POST that is specific to the Arduino
//the Arduino POST code is a slight modifcation of the POST request found in the "The POST Method" section of James Marshall's "HTTP Made Really Easy" website.
//returns a '1' if the user is allowed access, or '0' if the user is not allowed access
char serverRequest(String postData){
    char serverResponse = '0';

    if (client.connect(ipAddress, serverPort)) {
        client.println("POST / HTTP/1.1");                                   //i used a python flask script on the server, it simply listens for anything making a request to the server's root directory; which is what the first "/" is used for
        client.println("User-Agent: Arduino/1.0");
        client.print("Content-Length: ");
        client.println(postData.length());
        client.println();                                                    //this must be a blank line as specified by the HTTP protocol
        client.println(postData);
    }
    else {
        displayMessage(unsuccessfulConnectionMessage, blankLine, unsuccessfulConnectionMessageDelay);
    }

    delay(clientDelay);                                   //a delay between contacting the server and recieving a response from the server is necessary

    //Every loop stores one character read from the returned HTTP header
    //The last character read in the loop is the last character in the HTTP header; this character will either be a 1 or a 0
    while(client.available()) {
        serverResponse = client.read();
    }
    client.stop();
    return serverResponse;
}

/////////////////////////////////////////////////////////////////
//                         Main Loop                           //
/////////////////////////////////////////////////////////////////
void loop() {
    displayPersistentMessage(swipeRfidMessage, blankLine);
    char* rfidCode = getRFID();

    if(rfidCode[0] != 0){                                  //only connect to the server if a rfid has been read
        String pin = getPIN();
        if(pin.length() == 4){                             //only connect to the server if a pin has been entered
            String rfidCodeString(rfidCode);
            String rfidAndPin = rfidCodeString + "," + pin;
            char serverResponse = serverRequest(rfidAndPin);
            openDoor(serverResponse);
        }
    }
}
