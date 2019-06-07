#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#define wifiRX 3
#define wifiTX 2
#define gpsRX 10
#define gpsTX 11
#define trigPin 13
#define echoPin 12
#define vibrPin 8

float lattitude = -15.7801, longitude = -47.9292;
long duration, cm;

const long ultraInterval = 30000;
const long gpsInterval = 10000;
const long wifiInterval = 5000;

int startUltra, endUltra, startGps, endGps, startPost, endPost;

SoftwareSerial esp8266(wifiRX, wifiTX);
SoftwareSerial gpsSerial(gpsRX, gpsTX);

TinyGPSPlus gps;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(vibrPin, OUTPUT);

  Serial.begin(9600);
  esp8266.begin(9600);

  sendData("AT+RST\r\n", wifiInterval, true);
  sendData("AT+CIPMUX=1\r\n", wifiInterval, true);
  sendData("AT+CIPSTART=0,\"TCP\",\"35.199.121.64\",80\r\n", wifiInterval, true);

  gpsSerial.begin(9600);
}

void loop() {
  startUltra = millis();
  endUltra = startUltra;
  while ((endUltra - startUltra) <= ultraInterval)
  {
    handleUltra();
    digitalWrite(trigPin, HIGH);
    digitalWrite(vibrPin, LOW);
    endUltra = millis();
  }

  startGps = millis();
  endGps = startGps;
  while ((endGps - startGps) <= gpsInterval)
  {
    handleGps();
    endGps = millis();
  }

  handlePost();
}

void handleUltra() {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  cm = (duration / 58);
  Serial.println(cm);

  if (cm > 20 && cm <= 180) {
    digitalWrite(vibrPin, HIGH);
  } else {
    digitalWrite(vibrPin, LOW);
  }
  delay(1000);
}

void handleGps() {
  while (gpsSerial.available())
  {
    int data = gpsSerial.read();
    if (gps.encode(data))
    {
      if (gps.location.lat() != 0.0 && gps.location.lng() != 0.0) {
        lattitude = (gps.location.lat());
        longitude = (gps.location.lng());
      }
      Serial.print ("lattitude: ");
      Serial.println (lattitude);
      Serial.print ("longitude: ");
      Serial.println (longitude);
    }
  }

}

void handlePost() {
  String post = "POST /api/map/?lat=" + String(lattitude, 6) + "&long=" + String(longitude, 6) + " HTTP/1.1\nHost: 35.199.121.64\r\n";
  sendData("AT+CIPSEND=0," + String(post.length() + 4) + "\r\n", 1000, true);
  esp8266.println(post);
  sendData("AT+CIPCLOSE=0", 500, true);
  sendData("AT+CIPSTART=0,\"TCP\",\"35.199.121.64\",80\r\n", 500, true);
}

String sendData(String command, const int timeout, boolean debug) {
  String response = "";
  esp8266.print(command);
  long int time = millis();
  while ((time + timeout) > millis()) {
    while (esp8266.available()) {
      char c = esp8266.read();
      response += c;
    }
  }
  if (debug) {
    Serial.print(response);
  }
  return response;
}
