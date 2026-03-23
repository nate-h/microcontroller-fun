/*
  Distance As LEDs Demo Setup

  Parts:
  - 1 ultrasonic distance sensor (such as HC-SR04)
  - 6 LEDs
  - 6 resistors for the LEDs (220 ohm typical)

  Wiring:
  - Ultrasonic VCC -> 5V
  - Ultrasonic GND -> GND
  - Ultrasonic TRIG -> pin 9
  - Ultrasonic ECHO -> pin 10
  - LED 1 anode -> pin 2 through a resistor
  - LED 2 anode -> pin 3 through a resistor
  - LED 3 anode -> pin 4 through a resistor
  - LED 4 anode -> pin 5 through a resistor
  - LED 5 anode -> pin 6 through a resistor
  - LED 6 anode -> pin 7 through a resistor
  - All LED cathodes -> GND

  Notes:
  - Open the Serial Monitor at 9600 baud to see measured distance in centimeters.
*/
const int trigPin = 9;
const int echoPin = 10;

const int ledPins[6] = {2, 3, 4, 5, 6, 7};

long duration;
float distance;

void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  for (int i = 0; i < 6; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
}

float getDistance() {
  // Clear trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Send pulse
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read echo
  duration = pulseIn(echoPin, HIGH);

  // Convert to cm
  float dist = duration * 0.034 / 2;
  return dist;
}

void loop() {
  distance = getDistance();

  Serial.print("Distance: ");
  Serial.println(distance);

  // Clamp distance range
  float minDist = 5;    // very close
  float maxDist = 50;   // far

  distance = constrain(distance, minDist, maxDist);

  // Map to 0-6 LEDs
  int numLEDs = map(distance, maxDist, minDist, 0, 6);
  numLEDs = constrain(numLEDs, 0, 6);

  // Update LEDs
  for (int i = 0; i < 6; i++) {
    if (i < numLEDs) {
      digitalWrite(ledPins[i], HIGH);
    } else {
      digitalWrite(ledPins[i], LOW);
    }
  }

  delay(100);
}
