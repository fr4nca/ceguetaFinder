#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#define wifiRX 3
#define wifiTX 2
#define gpsRX 10
#define gpsTX 11
#define buttonPin 7
#define trigPin 13
#define echoPin 12
#define vibrPin 8

float lattitude, longitude;
long duration, cm;
int buttonState = 0;

unsigned long previousMillis = 0; //Will store time that the sensor last ran

const long interval = 1000;

SoftwareSerial esp8266(wifiRX, wifiTX);
SoftwareSerial gpsSerial(gpsRX, gpsTX);

TinyGPSPlus gps;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(vibrPin, OUTPUT);

  Serial.begin(9600);
  esp8266.begin(9600);

  sendData("AT+RST\r\n", 5000, true);
  sendData("AT+CIPMUX=1\r\n", 5000, true);
  sendData("AT+CIPSTART=0,\"TCP\",\"35.199.121.64\",80\r\n", 5000, true);

  gpsSerial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();

  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) {
    handlePost();
  }

  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    handleUltra();
    handleGps();
  }
}

unsigned handleUltra() {
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

unsigned handleGps() {
  while (gpsSerial.available()) {
    int data = gpsSerial.read();
    if (gps.encode(data)) {
      if (gps.location.isValid()) {
        lattitude = (gps.location.lat());
        longitude = (gps.location.lng());
      }
      else {
        lattitude = -15.7801;
        longitude = -47.9292;
      }
    }
  }
}

unsigned handlePost() {
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
