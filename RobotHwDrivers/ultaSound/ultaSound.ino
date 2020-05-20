#define trigPin A0
#define echoPin A1

void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  digitalWrite(trigPin, 0);
}

void loop() {
  long duration, distance;
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // needed for correct readings
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;

  Serial.println("CM: " + String(distance) + "\t Dura: " + String(duration));
}
