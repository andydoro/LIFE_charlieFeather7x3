/*
  Conway's Game of Life on 7 x 3 array of Charlieplex (7 x 15) Featherwings

  uses 2 x TCA9548A because IS31FL3731 is limited to only two i2c addresses. 

  Features: 
  * wrap or non-wrap world
  * ability to draw patterns 
  * respawn feature when oscillating / dead world is detected

  TO DO:
  - optimize? some algorithms only check live cells and surrounding...

*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_IS31FL3731.h>

// for TCA9548A i2c multiplexer
// need two of these for 21 boards...
#define TCAADDR_0 0x70
#define TCAADDR_1 0x71

// try to use frameBuffer? need to optimize

#define COLS 49
#define ROWS 45

#define NUMCELLS 2205

#define CELLBYTES 276

#define NUMBOARDS 21

#define PROBABILITY_START_ALIVE 15

#define FILL 64

#define CHECKTIMERLIMIT 19 // how often to check for redundant, must be odd!
                           // - possible problem if multi-stage oscillation somehow equals this? 

#define RESPAWNLIMIT 5 // how soon to respond after duplicate world is detected 


uint8_t frameBuffer = 0; // frameBuffer counter 0-7  - start with 1 because 0 will be visible

uint8_t cells[CELLBYTES];
uint8_t cellsBuffer[CELLBYTES];

uint8_t cellsCheck[CELLBYTES];

uint8_t TCAaddrs[2] = { TCAADDR_0, TCAADDR_1 }; // array for our two i2c multiplexers
uint8_t IS31addrs[2] = { 0x74, 0x77 }; // these are the two allowed i2c addresses for IS31 driver...

uint8_t counter = 0; // count frames, check for duplicate every other frame

boolean respawn = false;

//boolean firstTime = 1;

Adafruit_IS31FL3731_Wing ledmatrix[NUMBOARDS] = { Adafruit_IS31FL3731_Wing(), Adafruit_IS31FL3731_Wing(), Adafruit_IS31FL3731_Wing(),
                                                  Adafruit_IS31FL3731_Wing(), Adafruit_IS31FL3731_Wing(), Adafruit_IS31FL3731_Wing(),
                                                  Adafruit_IS31FL3731_Wing(), Adafruit_IS31FL3731_Wing(), Adafruit_IS31FL3731_Wing(),
                                                  Adafruit_IS31FL3731_Wing(), Adafruit_IS31FL3731_Wing(), Adafruit_IS31FL3731_Wing(),
                                                  Adafruit_IS31FL3731_Wing(), Adafruit_IS31FL3731_Wing(), Adafruit_IS31FL3731_Wing(),
                                                  Adafruit_IS31FL3731_Wing(), Adafruit_IS31FL3731_Wing(), Adafruit_IS31FL3731_Wing(),
                                                  Adafruit_IS31FL3731_Wing(), Adafruit_IS31FL3731_Wing(), Adafruit_IS31FL3731_Wing()
                                                };


void setup() {
  //Serial.begin(9600);
  //while (!Serial);
  Serial.println("beginning!");
  Wire.begin(); // needed for M0 it seems!

  delay(500);
  initDisplays();
  //delay(500);

  // Initialization of cells
  randomWorld();
  //startWorldPattern();
  //drawGlider(cells, 5, 5);

  //drawAdafruit(cells, 1, 18);
  //drawAdaStar(cells, 30, 28);
  
  //drawGliderGunBytes(cells, 0, 0);
  //drawPentaDec(cells, 8, 20);
}

void loop() {
  drawWorld();
  //if (firstTime) delay(3000);
  calculateWorld();
  //firstTime = false;
}

