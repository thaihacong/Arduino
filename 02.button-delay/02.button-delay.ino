#define LED_PIN     3
#define BUTTON_PIN  6

bool isLedOn = false;
bool isButtonPressed = false;

unsigned long duration = 0;
unsigned long buttonStartTime = 0;
unsigned long ledEndTime = 0;

unsigned long currentTime = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  currentTime = millis();

  if (ledEndTime != 0 && ledEndTime <= currentTime) {
    turnOffLed();
    ledEndTime = 0;
  }

  if (isLedOn == false) {
    isButtonPressed = digitalRead(BUTTON_PIN) == LOW;
    
    if (isButtonPressed) {
      if (buttonStartTime == 0) {
        buttonStartTime = currentTime;
      }
    } else {
      if (buttonStartTime != 0) {
        duration = currentTime - buttonStartTime;
        ledEndTime = currentTime + duration;

        turnOnLed();
        buttonStartTime = 0;
      }
    }
  }
}

void turnOnLed() {
  digitalWrite(LED_PIN, HIGH);
  isLedOn = true;
}

void turnOffLed() {
  digitalWrite(LED_PIN, LOW);
  isLedOn = false;
}
