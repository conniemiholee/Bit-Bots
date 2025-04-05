Updated Code: 

const int buttonPins[4] = {2, 3, 4, 5};    // Buttons
const int ledPins[4] = {6, 7, 8, 9};       // Matching LEDs
const int buzzer = 10;

const int maxRounds = 10;
int pattern[maxRounds];
int roundNum = 0;

unsigned long lastInputTime = 0;
const unsigned long debounceInterval = 200;

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 4; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP); // Active LOW
    pinMode(ledPins[i], OUTPUT);
  }

  pinMode(buzzer, OUTPUT);
  randomSeed(analogRead(0)); // Seed randomness
  Serial.println("Simon Says: Digital Version!");
}

void loop() {
  roundNum = 0;

  while (roundNum < maxRounds) {
    generateNextStep();
    playPattern();
    bool success = getPlayerInput();

    if (!success) {
      gameOver(false);
      return;
    }

    roundNum++;
    delay(1000);
  }

  gameOver(true); // All rounds passed
}

// Generate the next step in Simon's pattern
void generateNextStep() {
  pattern[roundNum] = random(0, 4); // Pick 0-3
}

// Show Simon’s pattern
void playPattern() {
  for (int i = 0; i <= roundNum; i++) {
    flashLED(pattern[i]);
    delay(300);
  }
}

// Show LED + sound
void flashLED(int index) {
  digitalWrite(ledPins[index], HIGH);
  tone(buzzer, 400 + index * 200);
  delay(300);
  digitalWrite(ledPins[index], LOW);
  noTone(buzzer);
}

// Wait for and read player input
bool getPlayerInput() {
  for (int i = 0; i <= roundNum; i++) {
    int input = waitForButtonPress();
    if (input == -1) return false;

    flashLED(input); // Feedback

    if (input != pattern[i]) {
      return false;
    }
  }
  return true;
}

// Detect button press with debounce
int waitForButtonPress() {
  lastInputTime = millis();
  Serial.println(lastInputTime);
  while (true) {
    for (int i = 0; i < 4; i++) {
      if (digitalRead(buttonPins[i]) == LOW && (millis() - lastInputTime > debounceInterval)) {
        lastInputTime = millis();
        while (digitalRead(buttonPins[i]) == LOW); // Wait for release
        
        return i;
        
      }
    }

    if (millis() - lastInputTime > 5000) {
      return -1; // Timeout
    }
  }
}

// Handle win or loss
void gameOver(bool win) {
  if (win) {
    Serial.println("You won!");
    tone(buzzer, 800); delay(200);
    tone(buzzer, 1000); delay(200);
    tone(buzzer, 1200); delay(200);
    noTone(buzzer);
  } else {
    Serial.println("Wrong input! Game Over!");
    for (int i = 0; i < 3; i++) {
      tone(buzzer, 200); delay(200); noTone(buzzer); delay(100);
    }
  }

  delay(2000);
}

