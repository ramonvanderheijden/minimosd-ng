// Get the common arduino functions
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "wiring.h"
#endif

#include "spi.h"

//---------- constructor ----------------------------------------------------

SPI::SPI()
{
  // initialize the SPI pins
  pinMode(SCK_PIN, OUTPUT);
  pinMode(MOSI_PIN, OUTPUT);
  pinMode(MISO_PIN, INPUT);
  pinMode(SS_PIN, OUTPUT);

  // enable SPI Master, MSB, SPI mode 0, FOSC/4
  mode(0);
}

//------------------ mode ---------------------------------------------------

void SPI::mode(byte config)
{
  byte tmp;

  // enable SPI master with configuration byte specified
  SPCR = 0;
  SPCR = (config & 0x7F) | (1<<SPE) | (1<<MSTR);
  tmp = SPSR;
  tmp = SPDR;
}

//------------------ transfer -----------------------------------------------

byte SPI::transfer(byte value)
{
  SPDR = value;
  while (!(SPSR & (1<<SPIF))) ;
  return SPDR;
}

byte SPI::transfer(byte value, byte period)
{
  SPDR = value;
  if (period > 0) delayMicroseconds(period);
  while (!(SPSR & (1<<SPIF))) ;
  return SPDR;
}


//---------- preinstantiate SPI object --------------------------------------

SPI Spi = SPI();

