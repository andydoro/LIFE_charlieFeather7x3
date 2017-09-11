

// switching function for TCA i2c multiplexer
void tcaSelect(uint8_t tcaAddr, uint8_t bus) {
  if (bus > 7) return;
  //prevent crosschat on multiple multiplexers, have to disable the other i2c multiplexer! otherwise weird artifacts occur
  Wire.beginTransmission(TCAaddrs[(tcaAddr + 1) % 2]);
  Wire.write(0);  // no channel selected
  Wire.endTransmission();

  Wire.beginTransmission(TCAaddrs[(tcaAddr)]);
  //delay(1);
  Wire.write(1 << bus);
  //delay(1);
  Wire.endTransmission();
  //delay(1);
}

// initialize and address displays!
void initDisplays() {

  for (uint8_t i = 0; i < NUMBOARDS; i++) {
    tcaSelect(i / 16, (i / 2) % 8);
    if (! ledmatrix[i].begin(IS31addrs[i % 2])) { // every other board needs alternating i2c address
      Serial.print("IS31 number ");
      Serial.print(i);
      Serial.println(" not found");
      while (1);
    }
    Serial.print("IS31 number ");
    Serial.print(i);
    Serial.println(" found!");
  }

  // rotate & clear matrices
  for (uint8_t i = 0; i < NUMBOARDS; i++) {
    // use TCA;
    tcaSelect(i / 16, (i / 2) % 8);
    ledmatrix[i].setFrame(0);
    ledmatrix[i].fillScreen(0);
    ledmatrix[i].setRotation(1);
  }
}


void drawWorld() {
  //draw world


  clearFrames();

  // only draw live pixels
  for (uint8_t y = 0; y < ROWS; y++) {
    for (uint8_t x = 0; x < COLS; x++) {

      switch (isAlive(x, y, cells)) {
        case 0:
          // dead
          //fill = 0;
          // basically do nothing...
          break;
        case 1:
          //fill = FILL;


          //uint8_t fill;

          // draw the pixels
          uint8_t i2cBusNum; // figure out which bus to select
          uint8_t matrixNum; // figure out which matrix to select
          uint8_t TCA_Addr;
          // select correct TCA
          if ( y < 30 || x < 14) {
            TCA_Addr = 0;
            if (x < 14 && y < 15) {
              i2cBusNum = 0;
              if (x < 7) {
                matrixNum = 0;
              } else {
                matrixNum = 1;
              }
            } else if (x < 28 && y < 15) {
              i2cBusNum = 1;
              if (x < 21) {
                matrixNum = 2;
              } else {
                matrixNum = 3;
              }
            } else if (x < 42 && y < 15) {
              i2cBusNum = 2;
              if (x < 35) {
                matrixNum = 4;
              } else {
                matrixNum = 5;
              }
            } else if ((y < 15) || (x < 7 && y < 30)) {
              i2cBusNum = 3;
              if (y < 15) {
                matrixNum = 6;
              } else {
                matrixNum = 7;
              }
            } else if (x < 21 && y < 30) {
              i2cBusNum = 4;
              if (x < 14) {
                matrixNum = 8;
              } else {
                matrixNum = 9;
              }
            } else if (x < 35 && y < 30) {
              i2cBusNum = 5;
              if (x < 28) {
                matrixNum = 10;
              } else {
                matrixNum = 11;
              }
            } else if (y < 30) { // x < 49
              i2cBusNum = 6;
              if (x < 42) {
                matrixNum = 12;
              } else {
                matrixNum = 13;
              }
            } else {
              i2cBusNum = 7;
              if (x < 7) {
                matrixNum = 14;
              } else {
                matrixNum = 15;
              }
            }
          } else {
            TCA_Addr = 1;
            if (x < 28) {
              i2cBusNum = 0;
              if (x < 21) {
                matrixNum = 16;
              } else {
                matrixNum = 17;
              }
            } else if (x < 42) {
              i2cBusNum = 1;
              if (x < 35) {
                matrixNum = 18;
              } else {
                matrixNum = 19;
              }
            } else {
              i2cBusNum = 2;
              matrixNum = 20;
            }
          }
          tcaSelect(TCA_Addr, i2cBusNum);
          ledmatrix[matrixNum].setFrame(frameBuffer);
          ledmatrix[matrixNum].drawPixel(x % 7, y % 15, FILL);
          break;
      } // end switch
    } // end x loop
  } // end y loop

  displayFrames();
}

void displayFrames(void) {
  // display frames
  for (int i = 0; i < NUMBOARDS; i++) {
    // use TCA
    tcaSelect(i / 16, (i / 2) % 8);
    ledmatrix[i].displayFrame(frameBuffer);
  }
}

void clearFrames(void) {
  // advance frameBuffer so we can write everything offscreen
  frameBuffer++;
  frameBuffer = frameBuffer % 8; // return to 0 if go over 7...

  // draw to buffer
  for (uint8_t i = 0; i < NUMBOARDS; i++) {
    // use TCA
    tcaSelect(i / 16, (i / 2) % 8);
    ledmatrix[i].setFrame(frameBuffer);
    //ledmatrix[i].clear();
    ledmatrix[i].fillScreen(0);
  }
}

