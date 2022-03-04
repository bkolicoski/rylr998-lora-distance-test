#include <SoftwareSerial.h>

#define RXD2 14
#define TXD2 12

SoftwareSerial mySerial(RXD2, TXD2);

#define TARGET_ID 2

String Message = "Hello from Dev1";
String content = "";
int counter = 0;

void setup()
{
  Serial.begin(115200);
  mySerial.begin(115200);
  delay(1000);
  pinMode(0, INPUT_PULLUP);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
}

void loop() {
  if (Serial.available()){
    Serial.println("Writing");
    content = Serial.readString();
    content.trim();
    Serial.println();
    content = content + "\r\n";
    char* bufc = (char*) malloc(sizeof(char) * content.length() + 1);
    content.toCharArray(bufc, content.length() + 1);
    mySerial.write(bufc);
    free(bufc);
  }

  if (mySerial.available()) {
    String incomming = mySerial.readString();
    if (incomming.length() <= 10)
      Serial.println(incomming);
    else {
      String channel_ID = incomming.substring(incomming.indexOf('=') + 1, incomming.indexOf(','));
      Serial.println("Channel ID : " + channel_ID);

      String str = incomming.substring(incomming.indexOf(',') + 1);

      String msgLength = str.substring(0, str.indexOf(','));
      Serial.println("Message Length : " + msgLength);

      String str2 = str.substring(str.indexOf(',') + 1);

      String message = str2.substring(0, str2.indexOf(','));
      Serial.println("Message : " + message);
      
      digitalWrite(2, LOW);
      delay(2000);
      digitalWrite(2, HIGH);
    }
  }

  // When the button is pressed send the message to other module
  if (digitalRead(0) == LOW) {
    delay(1000);
    String data = Message + " - Count: " + counter;
    sendLoraData(data, TARGET_ID);
    //increase counter on each send
    counter++;
  }
}

void sendLoraData(String data, int address) {
  String myString = "AT+SEND=" + String(address) + "," + String(data.length()) + "," + data + "\r\n";
  char* buf = (char*) malloc(sizeof(char) * myString.length() + 1);
  Serial.println(myString);
  myString.toCharArray(buf, myString.length() + 1);
  mySerial.write(buf);
  free(buf);
}
