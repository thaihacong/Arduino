#define LED_PIN     3
#define BUTTON_PIN  6

unsigned long buttonStartTime = 0;
unsigned long ledEndTime = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  unsigned long currentTime = millis();

  if (ledEndTime != 0 && currentTime >= ledEndTime) {
    turnOffLed();
    ledEndTime = 0;
  }

  bool isLedOn = digitalRead(LED_PIN) == HIGH;
  if (isLedOn == false) {
    bool isButtonPressed = digitalRead(BUTTON_PIN) == LOW;
    
    if (isButtonPressed) {
      if (buttonStartTime == 0) {
        buttonStartTime = currentTime;
      }
    } else {
      if (buttonStartTime != 0) {
        unsigned long duration = currentTime - buttonStartTime;
        ledEndTime = currentTime + duration;

        turnOnLed();
        buttonStartTime = 0;
      }
    }
  }
}

void turnOnLed() {
  digitalWrite(LED_PIN, HIGH);
}

void turnOffLed() {
  digitalWrite(LED_PIN, LOW);
}
