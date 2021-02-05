#define LED_PIN     3
#define BUTTON_PIN  6

bool isButtonPressed = false;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  isButtonPressed = digitalRead(BUTTON_PIN) == HIGH;

  if (isButtonPressed) {
    turnOnLed();
  } else {
    turnOffLed();
  }
}

void turnOnLed() {
  digitalWrite(LED_PIN, HIGH);
}

void turnOffLed() {
  digitalWrite(LED_PIN, LOW);
}
