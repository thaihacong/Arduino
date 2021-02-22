/**
 * action = one time user presses button then releases it
 * 
 * buffer is a circular buffer containing raw timestamp pairs with delayDuration added
 *   in which, delayDuration is the duration of the first time button pressed
 * 
 * buffer can hold up to (BUFFER_SIZE / 2) pairs of rawStartTime and rawEndTime,
 *   which correspond to (BUFFER_SIZE / 2) actions
 *   if user presses and releases button more than (BUFFER_SIZE / 2) times within a delayDuration time range,
 *   only (BUFFER_SIZE / 2) actions are stored in buffer
 * 
 *   e.g.: with BUFFER_SIZE = 10 (max. 5 actions)
 *         press and hold button for 10 seconds, led will light up for 10 seconds
 *         within this 10 seconds, press and hold button (quickly) for 5 (or more) times
 *         after the led lights up for the first time (10 seconds), it will blink for only 4 times
 *         after that, buffer works as usual
 * 
 * a zero timestampt in buffer means that the slot is empty
 */

#define LED_PIN       3
#define BUTTON_PIN    6
#define BUFFER_SIZE   100

int pushIndex = 0;  // next possible pushIndex
int popIndex = 0;   // next possible popIndex
unsigned long buffer[BUFFER_SIZE] = {};

unsigned long delayDuration = 0;
unsigned long buttonStartTime = 0;
unsigned long ledEndTime = 0;

bool toBeRejected = false;
bool waitingForEndSignal = false;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  listenToButton();

  if (delayDuration > 0) {
    unsigned long currentTime = millis();
    bool isLedOn = digitalRead(LED_PIN) == HIGH;

    if (isLedOn == true) {
      if (ledEndTime == 0) {    // button has not been released in last loop
        unsigned long rawEndTime = read();
        if (rawEndTime > 0) {   // button has been released
          pop();
          ledEndTime = rawEndTime;
        }
      }

      if (ledEndTime > 0 && currentTime >= ledEndTime) {
        turnOffLed();

        ledEndTime = 0;
      }
    } else {
      unsigned long rawStartTime = read();

      if (rawStartTime > 0 && currentTime >= rawStartTime) {
        pop();
        turnOnLed();

        unsigned long rawEndTime = read();
        if (rawEndTime > 0) {   // button was released
          pop();
          ledEndTime = rawEndTime;
        }
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

void listenToButton() {
  unsigned long currentTime = millis();
  bool isButtonPressed = digitalRead(BUTTON_PIN) == LOW;

  // toBeRejected flag:
  //   if user presses a button when buffer is full (push failed on the first loop)
  //   then releases it when the buffer has empty slots,
  //   toBeRejected flag will be set to true to not store this action in buffer
  //   it will be reset everytime user releases the button

  if (isButtonPressed) {
    if (buttonStartTime == 0 && toBeRejected == false) {
      if (push(currentTime) == true) {
        buttonStartTime = currentTime;
      } else {
        toBeRejected = true;
      }
    }
  } else {
    if (buttonStartTime != 0) {
      if (delayDuration == 0) {
        delayDuration = currentTime - buttonStartTime;
        buffer[0] = buffer[0] + delayDuration;
      }

      push(currentTime);
      buttonStartTime = 0;
    }

    toBeRejected = false;
  }
}

bool push(unsigned long value) {
  if (buffer[pushIndex] == 0) {
    buffer[pushIndex] = value + delayDuration;
    pushIndex = (pushIndex + 1) % BUFFER_SIZE;

    return true;
  }

  return false;
}

unsigned long pop() {
  unsigned long value = buffer[popIndex];

  if (value > 0) {
    buffer[popIndex] = 0;
    popIndex = (popIndex + 1) % BUFFER_SIZE;
  }

  return value;
}

unsigned long read() {
  return buffer[popIndex];
}
