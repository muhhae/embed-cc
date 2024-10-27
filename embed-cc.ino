const uint8_t selectors[3] = {15, 22, 23};
const uint8_t bcdInput[4] = {19, 18, 33, 32};

struct Button {
    uint8_t pin;
    uint64_t debouncePrev;
};

volatile Button a = {25, 0};
volatile Button b = {26, 0};
volatile Button c = {27, 0};
volatile Button benar = {14, 0};
volatile Button salah = {12, 0};
volatile Button reset = {13, 0};

enum ButtonType {
    TEAM_A,
    TEAM_B,
    TEAM_C,
    BENAR,
    SALAH,
    RESET,
    NONE
};
volatile ButtonType buttonPressed = NONE;

const bool patterns[16][4] = {
    {0, 0, 0, 0}, // 0
    {0, 0, 0, 1}, // 1
    {0, 0, 1, 0}, // 2
    {0, 0, 1, 1}, // 3
    {0, 1, 0, 0}, // 4
    {0, 1, 0, 1}, // 5
    {0, 1, 1, 0}, // 6
    {0, 1, 1, 1}, // 7
    {1, 0, 0, 0}, // 8
    {1, 0, 0, 1}, // 9
    {1, 0, 1, 0}, // 10 A
    {1, 0, 1, 1}, // 11 B
    {1, 1, 0, 0}, // 12 C
    {1, 1, 0, 1}, // 13 D
    {1, 1, 1, 0}, // 14 E
    {1, 1, 1, 1} // 15 F
};

const uint64_t interval = 1000;
uint64_t previousTime = 0;
uint64_t currentTime = 0;

bool checkInterval() {
  currentTime = millis();
  if (currentTime - previousTime < interval) {
    return false;
  }
  previousTime = currentTime;
  return true;
}

const uint64_t debounceInterval = 50;

void IRAM_ATTR aCallback() {
    uint64_t debounceCurrent = millis();
    if (debounceCurrent - a.debouncePrev < debounceInterval) return;
    a.debouncePrev = debounceCurrent;

    if (buttonPressed != NONE) return;
    buttonPressed = TEAM_A;
}

void IRAM_ATTR bCallback() {
    uint64_t debounceCurrent = millis();
    if (debounceCurrent - b.debouncePrev < debounceInterval) return;
    b.debouncePrev = debounceCurrent;

    if (buttonPressed != NONE) return;
    buttonPressed = TEAM_B;
}

void IRAM_ATTR cCallback() {
    uint64_t debounceCurrent = millis();
    if (debounceCurrent - c.debouncePrev < debounceInterval) return;
    c.debouncePrev = debounceCurrent;

    if (buttonPressed != NONE) return;
    buttonPressed = TEAM_C;
}

void IRAM_ATTR benarCallback() {
    uint64_t debounceCurrent = millis();
    if (debounceCurrent - benar.debouncePrev < debounceInterval) return;
    benar.debouncePrev = debounceCurrent;

    if (buttonPressed != NONE) return;
    buttonPressed = BENAR;
}

void IRAM_ATTR salahCallback() {
    uint64_t debounceCurrent = millis();
    if (debounceCurrent - salah.debouncePrev < debounceInterval) return;
    salah.debouncePrev = debounceCurrent;

    if (buttonPressed != NONE) return;
    buttonPressed = SALAH;
}

void IRAM_ATTR resetCallback() {
    uint64_t debounceCurrent = millis();
    if (debounceCurrent - reset.debouncePrev < debounceInterval) return;
    reset.debouncePrev = debounceCurrent;

    if (buttonPressed != NONE) return;
    buttonPressed = RESET;
}

void showNumber(uint8_t num) {
    num--;
    for (int i = 0; i < 3; i++) {
        digitalWrite(selectors[i], LOW);
    }
    for (int i = 0; i < 3; i++) {
        digitalWrite(selectors[i], HIGH);
        for (int j = 0; j < 4; j++) {
            digitalWrite(bcdInput[j], patterns[num][j]);
        }
        digitalWrite(selectors[i], LOW);
    }
}

void setup() {
  pinMode(benar.pin, INPUT);
  pinMode(salah.pin, INPUT);
  pinMode(reset.pin, INPUT);
  pinMode(a.pin, INPUT);
  pinMode(b.pin, INPUT);
  pinMode(c.pin, INPUT);

  for (int i = 0; i < 3; i++) {
      pinMode(selectors[i], OUTPUT);
  }
  for (int i = 0; i < 4; i++) {
      pinMode(bcdInput[i], OUTPUT);
  }

  Serial.begin(9600);

  // attachInterrupt(benar.pin, benarCallback, FALLING);
  // attachInterrupt(salah.pin, salahCallback, FALLING);
  // attachInterrupt(reset.pin, resetCallback, FALLING);
  // attachInterrupt(a.pin, aCallback, FALLING);
  // attachInterrupt(b.pin, bCallback, FALLING);
  // attachInterrupt(c.pin, cCallback, FALLING);
}

uint64_t counter = 0;
void loop() {
    showNumber(8);
    if (checkInterval()) {
        Serial.printf("%d: Loop\n", counter);
        counter++;
    }

    if (buttonPressed == NONE) return;
    switch (buttonPressed) {
        case TEAM_A:
            Serial.println("TEAM_A");
            break;
        case TEAM_B:
            Serial.println("TEAM_B");
            break;
        case TEAM_C:
            Serial.println("TEAM_C");
            break;
        case BENAR:
            Serial.println("BENAR");
            break;
        case SALAH:
            Serial.println("SALAH");
            break;
        case RESET:
            Serial.println("RESET");
            break;
        case NONE:
            break;
    }
    buttonPressed = NONE;
}
