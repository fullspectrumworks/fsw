#include <Ethernet.h>
#include <SPI.h>


byte macAddress[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED}; //default mac address used for this arduino
byte ipAddress[] = { 192, 168, 1, 4 }; // Google
const int serverPort = 80;
EthernetClient client;

void setup()
{
  Ethernet.begin(macAddress);
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
        //displayMessage(unsuccessfulConnectionMessage, blankLine, unsuccessfulConnectionMessageDelay);
    }

    delay(1000);                                   //a delay between contacting the server and recieving a response from the server is necessary

    //Every loop stores one character read from the returned HTTP header
    //The last character read in the loop is the last character in the HTTP header; this character will either be a 1 or a 0
    while(client.available()) {
        serverResponse = client.read();
    }
    client.stop();
    return serverResponse;
}

void loop()
{
    serverRequest("hello, world");
}
