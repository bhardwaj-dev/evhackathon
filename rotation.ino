//MOTOR1 PINS
int ena = 5;
int in1 = 6;
int in2 = 7;
#include <dht11.h>
#include <SoftwareSerial.h> 
int offset =0;                                  
int rxPin = 10;                                               //ESP8266 RX pini
int txPin = 11; 
String agAdi = "Nit";                 // We write the name of our network here.    
String agSifresi = "topcoder";           // We write the password of our network here.
String ip = "184.106.153.149";                                //Thingspeak ip adresi
float sicaklik, nem;
SoftwareSerial esp(rxPin, txPin);
                                              //ESP8266 TX pini
void setup() {

  pinMode(ena, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  Serial.begin(9600);  // We are starting our communication with the serial port.
  Serial.println("Started");
  esp.begin(115200);                                          // We are starting serial communication with ESP8266.
  esp.println("AT");                                         // We do the module control with the AT command.
  Serial.println("AT  sent ");
  while(!esp.find("OK")){                                     // We wait until the module is ready.
    esp.println("AT");
    Serial.println("ESP8266 Not Find.");
  }
  Serial.println("OK Command Received");
  esp.println("AT+CWMODE=1");                                 // We set the ESP8266 module as a client.
  while(!esp.find("OK")){                                     // We wait until the setting is done.
    esp.println("AT+CWMODE=1");
    Serial.println("Setting is ....");
  }
  Serial.println("Set as client");
  Serial.println("Connecting to the Network ...");
  esp.println("AT+CWJAP=\""+agAdi+"\",\""+agSifresi+"\"");    // We are connecting to our network.
  while(!esp.find("OK"));                                     // We wait until it is connected to the network.
  Serial.println("connected to the network.");
  delay(1000);
     //CLOCKWISE MAX SPEED
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  analogWrite(ena, 255);
  sicaklik = 255;
  
  delay(2000);
  
  //STOP
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  delay(2000);

  //COUNTERCLOCKWISE MAX SPEED
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  analogWrite(ena, 255);
  sicaklik = 255;
  delay(2000);
  
  //STOP
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  delay(2000);

  //CLOCKWISE HALF SPEED
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  analogWrite(ena, 127);
  delay(2000);

  //STOP
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  delay(2000);

  //COUNTERCLOCKWISE MAX SPEED
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  analogWrite(ena, 127);
  delay(2000);

  //STOP
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  
}

void loop()
{
  esp.println("AT+CIPSTART=\"TCP\",\""+ip+"\",80");           // We connect to Thingspeak.
  if(esp.find("Error")){                                      // We check the connection error.
    Serial.println("AT+CIPSTART Error");
  }
  int volt = analogRead(A0);// read the input
  double nem = map(volt,0,1023, 0, 2500) + offset;// map 0-1023 to 0-2500 and add correction offset
  
  nem /=100;// divide by 100 to get the decimal values
  Serial.print("Voltage: ");
  Serial.print(nem);//print the voltge
  Serial.println("V");
   

  String veri = "GET https://api.thingspeak.com/update?api_key=Y8M078XU15AXCKNV";   // Thingspeak command. We write our own api key in the key part.                                   
  veri += "&field3=";
  veri += String(sicaklik);// The temperature variable we will send
  veri += "&field4=";
  veri += String(nem);// The moisture variable we will send
  veri += "\r\n\r\n"; 
  esp.print("AT+CIPSEND=");                                   // We give the length of data that we will send to ESP.
  esp.println(veri.length()+2);
  delay(2000);
  if(esp.find(">")){                                          // The commands in it are running when ESP8266 is ready..
    esp.print(veri);                                          // We send the data.
    Serial.println(veri);
    Serial.println("Data sent.");
    delay(1000);
  }
  Serial.println("Connection Closed.");
  esp.println("AT+CIPCLOSE");                                // we close the link
  delay(1000);                            
 
}
