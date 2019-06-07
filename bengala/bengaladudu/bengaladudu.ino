

int trigPin = 13;
int echoPin = 12;
int vibrPin = 8;
long duration, cm;


void setup() {
  Serial.begin(9600);

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

  cm = (duration / 2) / 29.1;
  Serial.println(cm);

  if (cm <= 180) {
    digitalWrite(vibrPin, HIGH);

  } else {
    digitalWrite(vibrPin, LOW);
  }

  delay(250);
}
