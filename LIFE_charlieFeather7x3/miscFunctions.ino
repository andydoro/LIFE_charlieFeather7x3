
// start with a certain repeating pattern, mostly created to bug test
/*
void startWorldPattern() {
  for (int y = 0; y < ROWS; y++) {
    for (int x = 0; x < COLS; x++) {
      uint8_t cellStatus;
      if (x % 6 == 0) {
        cellStatus = 1;
      } else {
        cellStatus = 0;
      }
      setCell (cells, x, y, cellStatus);
    } // end x
  } // end y
}
*/

// to draw a creature provide position x,y and enough data!
// it get creature byte size wrong-- must pass array size explicitly
void draw(byte buffer[], uint8_t creatureSize, uint8_t _cbyteswidth, uint8_t x, uint8_t y, const byte creature[]) {

  for (uint8_t n = 0; n < creatureSize; n++) { 
    for (uint8_t i = 0; i < 8; i++) {
      // funky math to deal with creatures that are wider than 8 cells
      if (creature[n] & (1 << (7 - i))) setCell(buffer, x + i + ((n % _cbyteswidth) * 8), y + (n / _cbyteswidth), 1);
    }
  }
}

void drawGlider(byte buffer[], uint8_t x, uint8_t y) {
  uint8_t cbyteswidth = 1;
  byte creature[3] = {
    0b0000111,
    0b0000001,
    0b0000010
  };
  draw(buffer, sizeof(creature), cbyteswidth, x, y, creature);
}

void drawGliderGunBytes(byte buffer[], uint8_t x, uint8_t y) {
  // TODO: is that really a glider gun?
  uint8_t cbyteswidth = 5;
  byte creature[45] = {
    0b00000000, 0b00000000, 0b00000000, 0b01000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000001, 0b01000000, 0b00000000,
    0b00000000, 0b00000110, 0b00000110, 0b00000000, 0b00011000,
    0b00000000, 0b00001000, 0b10000110, 0b00000000, 0b00011000,
    0b01100000, 0b00010000, 0b01000110, 0b00000000, 0b00000000,
    0b01100000, 0b00010001, 0b01100001, 0b01000000, 0b00000000,
    0b00000000, 0b00010000, 0b01000000, 0b01000000, 0b00000000,
    0b00000000, 0b00001000, 0b10000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000110, 0b00000000, 0b00000000, 0b00000000
  };

  draw(buffer, sizeof(creature), cbyteswidth, x, y, creature);
}

void drawAdafruit(byte buffer[], uint8_t x, uint8_t y) {
  uint8_t cbyteswidth = 6;
  byte creature[36] = {
    0b00011100, 0b11000011, 0b10011110, 0b11100010, 0b00100100, 0b11111000,
    0b00100010, 0b10100100, 0b01010000, 0b10010010, 0b00100100, 0b00100000,
    0b00100010, 0b10010100, 0b01011110, 0b10010010, 0b00100100, 0b00100000,
    0b00111110, 0b10010111, 0b11010000, 0b11100010, 0b00100100, 0b00100000,
    0b00100010, 0b10100100, 0b01010000, 0b10010010, 0b00100100, 0b00100000,
    0b00100010, 0b11000100, 0b01010000, 0b10001001, 0b11000100, 0b00100000
  };

  draw(buffer, sizeof(creature), cbyteswidth, x, y, creature);
}


void drawAdaStar(byte buffer[], uint8_t x, uint8_t y) {
  uint8_t cbyteswidth = 2;
  const byte PROGMEM creature[32] = {
    0b00000000, 0b11000000,
    0b00000000, 0b11000000,
    0b00000001, 0b11100000,
    0b11110001, 0b11100000,
    0b11111011, 0b11100000,
    0b01111111, 0b11100000,
    0b01111111, 0b01111100,
    0b00011101, 0b11111111,
    0b00011101, 0b10111110,
    0b00111111, 0b01111100,
    0b00111111, 0b11100000,
    0b01111101, 0b11110000,
    0b01111001, 0b11110000,
    0b01100000, 0b11110000,
    0b00000000, 0b11100000,
    0b00000000, 0b01100000
  };

  draw(buffer, sizeof(creature), cbyteswidth, x, y, creature);
}

void drawPentaDec(byte buffer[], uint8_t x, uint8_t y) {
  uint8_t cbyteswidth = 1;
  const byte PROGMEM creature[12] = {
    0b01110000,
    0b00100000,
    0b00100000,
    0b01110000,
    0b00000000,
    0b01110000,
    0b01110000,
    0b00000000,
    0b01110000,
    0b00100000,
    0b00100000,
    0b01110000,
  };

  draw(buffer, sizeof(creature), cbyteswidth, x, y, creature);
}




