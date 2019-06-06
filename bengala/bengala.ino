#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#define RXesp 3
#define TXesp 2
#define RXgps 10
#define TXgps 11

SoftwareSerial esp8266(RXesp,TXesp); 
SoftwareSerial gpsSerial(RXgps,TXgps);

int trigPin = 13;    // Trigger
int echoPin = 12;    // Echo
int vibrPin = 8;    // Vibr
long duration, cm;
TinyGPSPlus gps;
float lattitude,longitude;

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
  esp8266.begin(9600);
  sendData("AT+RST\r\n", 5000, true);
  sendData("AT+CIPMUX=1\r\n", 5000, true);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(vibrPin, OUTPUT);

}
 
void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
 
  cm = (duration/2) / 29.1;
  while (gpsSerial.available())
  {
    int data = gpsSerial.read();
    if (gps.encode(data))
    {
      lattitude = (gps.location.lat());
      longitude = (gps.location.lng());
      Serial.print ("lattitude: ");
      Serial.println (lattitude);
      Serial.print ("longitude: ");
      Serial.println (longitude);
    }
  }
  if(cm <= 180) {
    digitalWrite(vibrPin, HIGH);
    postData();
    return;
  } else {
    digitalWrite(vibrPin, LOW);
  }

  delay(250);
}

void postData() {
  sendData("AT+CIPSTART=0,\"TCP\",\"gameeduc.com.br\",80\r\n", 5000, true); // criar api
  String getData = "GET /api/disciplina HTTP/1.1\nHost: gameeduc.com.br\nConnection: keep-alive\r\n"; // enviar para api correta
  sendData("AT+CIPSEND=0," + String(getData.length() + 4) + "\r\n", 4000, true);
  esp8266.println(getData);
  sendData("AT+CIPCLOSE=0\r\n", 5000, true);
}

String sendData(String command, const int timeout, boolean debug)
{
  String response = "";
  esp8266.print(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (esp8266.available())
    {
      char c = esp8266.read();
      response += c;
    }
  }
  if (debug)
  {
    Serial.print(response);
  }
  return response;
}
