const int rowsPins[] = {10, 11, 12, 13, A0, A1, A2, A3};
const int colsPins[] = {2, 3, 4, 5, 6, 7, 8, 9};
const int buttonPin = A4;

bool lastButtonPress = true;
bool blockDir = true;
int blockPos = 0;
int blockLevel = 0;
int blockSize = 3;
int blockSpeed = 1;
const int blockSizes[] = {3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1};
int blocks[16];
int blockSizes1[16];
bool lose = false;

unsigned long lastMillis;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  for (int i = 0; i < 8; ++i) {
    pinMode(rowsPins[i], OUTPUT);
    pinMode(colsPins[i], OUTPUT);
  }
  lastMillis = millis();
}

void loop() {
  draw();
  if (blockLevel < 16 && !lose) {
    if (millis() >= lastMillis + ((16 - blockSpeed) * 20) + 60) {
      if (blockDir) {
        ++blockPos;
      } else {
        --blockPos;
      }
      lastMillis = millis();
    }
    if (blockPos + blockSize >= 8) {
      blockDir = false;
    }
    if (blockPos <= 0) {
      blockDir = true;
    }
    if (!buttonPressed()) {
      lastButtonPress = false;
    } else if (!lastButtonPress) {
      if (blockLevel > 0) {
        dropBlockPart();
      }
      if (!lose) {
        blocks[blockLevel] = blockPos;
        blockSizes1[blockLevel] = blockSize;
        ++blockLevel;
        if (blockLevel < 16) {
          blockSize = min(blockSizes[blockLevel], blockSize);
          ++blockSpeed;
        }
      }
      lastButtonPress = true;
    }
  }
}

bool buttonPressed() {
  if (!lastButtonPress) {
    if (!digitalRead(buttonPin)) {
      delayWithDrawing(50);
      if (!digitalRead(buttonPin)) {
        return true;
      }
    }
    return false;
  } else {
    if (digitalRead(buttonPin)) {
      delayWithDrawing(50);
      if (digitalRead(buttonPin)) {
        return false;
      }
    }
    return true;
  }
  return false;
}

void dropBlockPart() {
  bool array1[8];
  bool array2[8];
  for (int i = 0; i < 8; ++i) {
    array1[i] = false;
    array2[i] = false;
  }
  for (int i = blockPos; i < blockPos + blockSize; ++i) {
    array1[i] = true;
  }
  for (int i = blocks[blockLevel - 1]; i < blocks[blockLevel - 1] + blockSizes1[blockLevel - 1]; ++i) {
    array2[i] = true;
  }
  int subSize = 0;
  int addPos = 0;
  for (int i = 0; i < 8; ++i) {
    if (array1[i] && !array2[i]) {
      ++subSize;
      if (blockPos < blocks[blockLevel - 1]) {
        ++addPos;
      }
    }
  }
  if (blockSize - subSize > 0) {
    blockSize -= subSize;
    blockPos += addPos;
  } else {
    lose = true;
  }
}

void draw() {
  for (int i = 0; i < blockLevel; ++i) {
    clear();
    drawBlock(i);
  }
  if (blockLevel < 16 && !lose) {
    clear();
    drawMovingBlock(blockLevel);
  }
  if (lose) {
    clear();
    drawLoseBlock(blockLevel);
  }
}

void drawMovingBlock(int level) {
  int y = level;
  if (blockLevel > 6) {
    y -= blockLevel - 6;
  }
  if (blockLevel == 15) {
    y = 7;
  }
  if (y >= 0) {
    for (int x = blockPos; x < blockPos + blockSize; ++x) {
      drawPixel(x, 7 - y, true);
    }
    delay(1);
  }
}

void drawBlock(int level) {
  int y = level;
  if (blockLevel > 6) {
    y -= blockLevel - 6;
  }
  if (blockLevel == 15) {
    ++y;
  }
  if (blockLevel == 16) {
    y += 2;
  }
  if (y >= 0) {
    for (int x = blocks[level]; x < blocks[level] + blockSizes1[level]; ++x) {
      drawPixel(x, 7 - y, true);
    }
    delay(1);
  }
}

void drawLoseBlock(int level) {
  int y = level;
  if (blockLevel > 6) {
    y -= blockLevel - 6;
  }
  if (blockLevel == 15) {
    y = 7;
  }
  if (y >= 0) {
    for (int x = blockPos; x < blockPos + blockSize; ++x) {
      drawPixel(x, 7 - y, bool((millis() / 500) % 2));
    }
    delay(1);
  }
}

void clear() {
  for (int y = 0; y < 8; ++y) {
    for (int x = 0; x < 8; ++x) {
      drawPixel(x, y, false);
    }
  }
}

void drawPixel(int x, int y, bool on) {
  digitalWrite(rowsPins[x], !on);
  digitalWrite(colsPins[y], on);
}

void delayWithDrawing(int time) {
  unsigned long lastMillis1 = millis();
  while (millis() < lastMillis1 + time) {
    draw();
  }
}
