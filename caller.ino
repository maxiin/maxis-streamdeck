#include <Keyboard.h>

#define CLOCK 4
#define DATA 2

#define INTSIZE 8

int lastClockState = HIGH;
int currentIndex = 0;
int result = 0;

void setup() {
  Serial.begin(9600);
  
  pinMode(CLOCK, INPUT_PULLUP);
  pinMode(DATA, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(CLOCK) != lastClockState) {
    lastClockState = !lastClockState;
    Serial.print(digitalRead(DATA));
    if (digitalRead(DATA) == HIGH) {
      result |= 1UL << currentIndex;
    }
    currentIndex++;
    if (currentIndex == 8) { // passed, reset and set the sent bit
      Serial.println(result);
      callKeyboard(result);
      result = 0;
      currentIndex = 0;
    }
    delay(10);
  }

}

void callKeyboard(int code) {
  Keyboard.begin();
  delay(10);
  switch (code) {
      // case 4 is not working for now
    case 0:
      Keyboard.press(KEY_F13);
      break;
    case 1:
      Keyboard.press(KEY_F14);
      break;
    case 2:
      Keyboard.press(KEY_F15);
      break;
    case 3:
      Keyboard.press(KEY_F16);
      break;
    case 5:
      Keyboard.press(KEY_F17);
      break;
    default:
      Serial.println("noKeys");
      break;
  }
  delay(100);
  Keyboard.releaseAll();
}
