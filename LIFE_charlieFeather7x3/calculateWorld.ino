
// read from the array: is the cell alive or dead?
boolean isAlive(uint8_t x, uint8_t y, uint8_t byteArray[]) {
  return bitRead(byteArray[(y * COLS + x) / 8], 7 - ((y * COLS + x) % 8));
}

// set the cell to alive or dead state
void setCell (uint8_t byteArray[], uint8_t x, uint8_t y, uint8_t cellStatus) {
  // add function to prevent out of bounds
  if (((x >= 0) && (x < COLS)) && ((y >= 0) && (y < ROWS))) {
    switch (cellStatus) {
      case 0: // dead
        bitClear(byteArray[(y * COLS + x) / 8], 7 - ((y * COLS + x) % 8)); // make 0 or dead
        break;
      case 1:
        bitSet(byteArray[(y * COLS + x) / 8], 7 - ((y * COLS + x) % 8));
        break;
    }
  }
}

// different ways to count the neighbors, based on edge... either wrap or no
// no wrap
uint8_t countNeighbors (uint8_t x, uint8_t y) {
  uint8_t count = 0;
  for (int i = -1; i < 2; i++) {
    for (int j = -1; j < 2; j++) {
      if (!(i == 0 && j == 0)) {
        int a = x + i;
        int b = y + j;
        if (((a >= 0) && (a < COLS)) && ((b >= 0) && (b < ROWS))) {
          if (isAlive(a, b, cellsBuffer)) {
            count++;
          }
        }
      }
    }
  }
  return count;
}

// wrap around universe
uint8_t countNeighborsWrap (uint8_t x, uint8_t y) {
  uint8_t count = 0;
  for (int i = -1; i < 2; i++) {
    for (int j = -1; j < 2; j++) {
      if (!(i == 0 && j == 0)) {
        int a = x + i;
        int b = y + j;
        if (a < 0) a = COLS - 1;
        else if (a >= COLS) a = 0;
        if (b < 0 ) b = ROWS - 1;
        else if (b >= ROWS) b = 0;
        if (isAlive(a, b, cellsBuffer)) {
          count++;
        }
      }
    }
  }
  return count;
}

// set cells randomly with certain alive probability
void randomWorld() {
  // Initialization of cells
  Serial.println("reset world!");

  randomSeed(analogRead(0)); // make more pseudo-random!

  for (int y = 0; y < ROWS; y++) {
    for (int x = 0; x < COLS; x++) {
      float state = random (100);
      uint8_t cellStatus;
      if (state > PROBABILITY_START_ALIVE) {
        cellStatus = 0;
      } else {
        cellStatus = 1;
      }
      setCell (cells, x, y, cellStatus);
    }
  }

}

void calculateWorld() { // calculate next world

  // copy arrays
  memcpy(cellsBuffer, cells, CELLBYTES);

  for (int y = 0; y < ROWS; y++) {
    for (int x = 0; x < COLS; x++) {
      // And visit all the neighbours of each cell
      uint8_t neighbors = countNeighborsWrap(x, y);
      // We've checked the neigbours: apply rules!
      switch (isAlive(x, y, cellsBuffer)) {
        case 0: // The cell is dead: make it live if necessary
          if (neighbors == 3 ) {
            setCell(cells, x, y, 1); // Only if it has 3 neighbours
          }
          break;
        case 1: // The cell is alive: kill it if necessary
          if (neighbors < 2 || neighbors > 3) {
            setCell(cells, x, y, 0); // Die unless it has 2 or 3 neighbours
          }
          break;
      } // end switch
    } // End of x loop
  } // End of y loop

  // check for duplicate world
  checkWorld();

}

void checkWorld(void) {

  //if (!respawn) {
  switch (respawn) {
    case 0:
      if (counter == CHECKTIMERLIMIT) { // check some multiple of 2... 
        // check if identical!
        int sameCounter = 0;
        for (int i = 0; i < CELLBYTES; i++) {
          if (cells[i] == cellsCheck[i]) {
            sameCounter++;
          } else {
            break; // escape if there is a difference
          }
        }

        memcpy(cellsCheck, cells, CELLBYTES); // store into cellsCheck byte array

        //Serial.print("sameCounter: ");
        //Serial.println(sameCounter);

        if (sameCounter == CELLBYTES) {
          Serial.println("respawn!");

          respawn = true; // set the countdown for respawn
        }

        counter = 0;
      } else {
        counter++;
        //Serial.println(counter);
      }
      break;
    case 1:
      // } else { // if respawn
      if (counter >= RESPAWNLIMIT) {
        respawn = false;
        counter = 0;
        // clear all and pause
        clearFrames();
        // display frames
        displayFrames();
        delay(1000);
        randomWorld();       // respawn!
      } else {
        counter++;
      }
      break;
  }
}

