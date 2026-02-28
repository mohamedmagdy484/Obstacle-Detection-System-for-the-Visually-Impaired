#define trigPin 6
#define echoPin 4
#define buzzerPin 8
#define vibrator1Pin 5

#include <SoftwareSerial.h>

SoftwareSerial EchoGuardian(10,12);

long duration;
int distance;

// Variables for timing
unsigned long under20StartTime = 0;
bool under20TimerRunning = false;
const unsigned long alertDuration = 10000; // 10 seconds in milliseconds

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(vibrator1Pin, OUTPUT);

  Serial.begin(9600);
  EchoGuardian.begin(9600);
}

void loop() {
  // Trigger the ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.0343 / 2;

  Serial.print("Distance (cm): ");
  Serial.println(distance);

  // Vibration and Buzzer Alerts
  if (distance < 20) {
    digitalWrite(vibrator1Pin, HIGH);
    tone(buzzerPin, 1800);
    delay(100);
  }
  else if (distance < 60) {
    digitalWrite(vibrator1Pin, HIGH);
    tone(buzzerPin, 1200);
    delay(50);
    noTone(buzzerPin);
    delay(50);
  }
  else if (distance < 100) {
    digitalWrite(vibrator1Pin, LOW);
    tone(buzzerPin, 900);
    delay(100);
    noTone(buzzerPin);
    delay(100);
  }
  else if (distance < 150) {
    digitalWrite(vibrator1Pin, LOW);
    tone(buzzerPin, 700);
    delay(150);
    noTone(buzzerPin);
    delay(150);
  }
  else if (distance < 200) {
    digitalWrite(vibrator1Pin, LOW);
    delay(200);
    noTone(buzzerPin);
    delay(200);
  }
  else {
    digitalWrite(vibrator1Pin, LOW);
    noTone(buzzerPin);
  }

  // Bluetooth Message Logic
  if (distance < 20) {
    if (!under20TimerRunning) {
      under20StartTime = millis();    // Start timing
      under20TimerRunning = true;
    }
    else {
      if (millis() - under20StartTime >= alertDuration) {
        EchoGuardian.println("WARNING: Object under 20cm for 10 seconds!");
        Serial.println("Message Sent via Bluetooth!");
        under20TimerRunning = false; // Reset timer after sending
      }
    }
  }
  else {
    under20TimerRunning = false; // Reset if distance becomes safe
  }

  delay(30); // Small delay for smoother loop
}