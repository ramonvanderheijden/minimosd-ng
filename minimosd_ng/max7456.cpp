
#include <FastSerial.h>

// Get the common arduino functions
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "wiring.h"
#endif

#include <EEPROM.h>

#include "max7456.h"
#include "spi.h"
#include "OSD_Config.h"

/* If defined, mode autodetection will be used instead of setting */
//#define AUTO_DETECT_MODE

volatile int16_t x;
volatile int font_count;
volatile byte character_bitmap[0x40];

OSD::OSD()
{
}

//------------------ init ---------------------------------------------------

void OSD::init()
{
  uint8_t reg;

  /* set chip select as output */ 
  pinMode(MAX7456_SELECT,OUTPUT);
  /* set vsync as input */
  pinMode(MAX7456_VSYNC, INPUT);
  /* enable pull resistor (vsync is an open-drain output)  */
  digitalWrite(MAX7456_VSYNC, HIGH);
  /* enable CS */
  digitalWrite(MAX7456_SELECT,LOW);

#ifdef AUTO_DETECT_MODE
  /* read STAT register and try to auto detect mode */
  Spi.transfer(MAX7456_STAT_reg_read);
  reg = Spi.transfer(0xff);

  if (reg & 0x3) {
    /* mode detected */
    reg &= 0x01;
  } else
#endif
  {
    reg = EEPROM.read(PAL_NTSC_ADDR) & 0x01;
  }

  if (reg == NTSC) {
    video_mode = MAX7456_MODE_MASK_NTSC;
    video_center = MAX7456_CENTER_NTSC;
  } else {
    video_mode = MAX7456_MODE_MASK_PAL;
    video_center = MAX7456_CENTER_PAL;
  }

  /* read black level register */
  Spi.transfer(MAX7456_OSDBL_reg_read);//black level read register
  reg = Spi.transfer(0xff);
  Spi.transfer(MAX7456_VM0_reg);
  Spi.transfer(MAX7456_RESET | video_mode);
  delay(50);
  //set black level
  reg &= 0xef; //Set bit 4 to zero 11101111
  Spi.transfer(MAX7456_OSDBL_reg); //black level write register
  Spi.transfer(reg);

  setBrightness();
  // define sync (auto,int,ext) and
  // making sure the Max7456 is enabled
  control(1);
}


//------------------ Set Brightness  ---------------------------------
void OSD::setBrightness()
{
  uint8_t blevel = EEPROM.read(OSD_BRIGHTNESS_ADDR);

  if(blevel == 0) //low brightness
    blevel = MAX7456_WHITE_level_80;
  else if(blevel == 1) 
    blevel = MAX7456_WHITE_level_90;
  else if(blevel == 2)
    blevel = MAX7456_WHITE_level_100;
  else if(blevel == 3) //high brightness
    blevel = MAX7456_WHITE_level_120;
  else 
    blevel = MAX7456_WHITE_level_80; //low brightness if bad value

  // set all rows to same charactor white level, 90%
  for (x = 0x0; x < 0x10; x++) {
    Spi.transfer(x + 0x10);
    Spi.transfer(blevel);
  }
}

//------------------ Get Mode (PAL 0/NTSC 1) --------------------------------

uint8_t OSD::getMode()
{
  return (video_mode == MAX7456_MODE_MASK_NTSC) ? 0 : 1;
}

//------------------ Get Center (PAL/NTSC) ----------------------------------

uint8_t OSD::getCenter()
{
  return video_center; //first line for center panel
}

//------------------ plug ---------------------------------------------------

void OSD::plug()
{
  digitalWrite(MAX7456_SELECT,LOW);
}

//------------------ clear ---------------------------------------------------

void OSD::clear()
{
  // clear the screen
  digitalWrite(MAX7456_SELECT,LOW);
  Spi.transfer(MAX7456_DMM_reg);
  Spi.transfer(MAX7456_CLEAR_display);
  digitalWrite(MAX7456_SELECT,HIGH);
}

//------------------ set panel -----------------------------------------------

void OSD::setPanel(uint8_t st_col, uint8_t st_row)
{
  start_col = st_col;
  start_row = st_row;
  col = st_col;
  row = st_row;
}

//------------------ open panel ----------------------------------------------

void OSD::openPanel(void)
{
  unsigned int linepos;
  byte settings, char_address_hi, char_address_lo;
 
  //find [start address] position
  linepos = row * 30 + col;
  
  // divide 16 bits into hi & lo byte
  char_address_hi = linepos >> 8;
  char_address_lo = linepos;

  //Auto increment turn writing fast (less SPI commands).
  //No need to set next char address. Just send them
  settings = MAX7456_INCREMENT_auto; //To Enable DMM Auto Increment
  digitalWrite(MAX7456_SELECT, LOW);
  Spi.transfer(MAX7456_DMM_reg); //dmm
  Spi.transfer(settings);

  Spi.transfer(MAX7456_DMAH_reg); // set start address high
  Spi.transfer(char_address_hi);

  Spi.transfer(MAX7456_DMAL_reg); // set start address low
  Spi.transfer(char_address_lo);
  //Serial.printf("setPos -> %d %d\n", col, row);
}

//------------------ close panel ---------------------------------------------

void OSD::closePanel(void)
{  
  Spi.transfer(MAX7456_DMDI_reg);
  Spi.transfer(MAX7456_END_string); //This is needed "trick" to finish auto increment
  digitalWrite(MAX7456_SELECT,HIGH);
  //Serial.println("close");
  row++; //only after finish the auto increment the new row will really act as desired
}

//------------------ write single char ---------------------------------------------

void OSD::openSingle(uint8_t x, uint8_t y)
{
  unsigned int linepos;
  byte char_address_hi, char_address_lo;
 
  //find [start address] position
  linepos = y*30+x;
  
  // divide 16 bits into hi & lo byte
  char_address_hi = linepos >> 8;
  char_address_lo = linepos;
  
  digitalWrite(MAX7456_SELECT,LOW);
  
  Spi.transfer(MAX7456_DMAH_reg); // set start address high
  Spi.transfer(char_address_hi);

  Spi.transfer(MAX7456_DMAL_reg); // set start address low
  Spi.transfer(char_address_lo);
  //Serial.printf("setPos -> %d %d\n", col, row);
}

//------------------ write ---------------------------------------------------

size_t OSD::write(uint8_t c)
{
  if(c == '|'){
    closePanel(); //It does all needed to finish auto increment and change current row
    openPanel(); //It does all needed to re-enable auto increment
  }
  else{
    Spi.transfer(MAX7456_DMDI_reg);
    Spi.transfer(c);
  }
  return 1;
}

//---------------------------------

void OSD::control(uint8_t ctrl)
{
  digitalWrite(MAX7456_SELECT,LOW);
  Spi.transfer(MAX7456_VM0_reg);
  switch(ctrl){
    case 0:
      Spi.transfer(MAX7456_DISABLE_display | video_mode);
      break;
    case 1:
      //Spi.transfer((MAX7456_ENABLE_display_vert | video_mode) | MAX7456_SYNC_internal);
      //Spi.transfer((MAX7456_ENABLE_display_vert | video_mode) | MAX7456_SYNC_external);
      Spi.transfer((MAX7456_ENABLE_display_vert | video_mode) | MAX7456_SYNC_autosync); 
      break;
  }
  digitalWrite(MAX7456_SELECT,HIGH);
}

void OSD::write_NVM(int font_count, uint8_t *character_bitmap)
{
  byte x;
  byte char_address_hi, char_address_lo;
  byte screen_char;

  char_address_hi = font_count;
  char_address_lo = 0;
 //Serial.println("write_new_screen");   

  // disable display
  digitalWrite(MAX7456_SELECT,LOW);
  Spi.transfer(MAX7456_VM0_reg); 
  Spi.transfer(MAX7456_DISABLE_display);

  Spi.transfer(MAX7456_CMAH_reg); // set start address high
  Spi.transfer(char_address_hi);

  for(x = 0; x < NVM_ram_size; x++) // write out 54 (out of 64) bytes of character to shadow ram
  {
    screen_char = character_bitmap[x];
    Spi.transfer(MAX7456_CMAL_reg); // set start address low
    Spi.transfer(x);
    Spi.transfer(MAX7456_CMDI_reg);
    Spi.transfer(screen_char);
  }

  // transfer a 54 bytes from shadow ram to NVM
  Spi.transfer(MAX7456_CMM_reg);
  Spi.transfer(WRITE_nvr);
  
  // wait until bit 5 in the status register returns to 0 (12ms)
  while ((Spi.transfer(MAX7456_STAT_reg_read) & STATUS_reg_nvr_busy) != 0x00);

  Spi.transfer(MAX7456_VM0_reg); // turn on screen next vertical
  Spi.transfer(MAX7456_ENABLE_display_vert);
  digitalWrite(MAX7456_SELECT,HIGH);  
}

//------------------ pure virtual ones (just overriding) ---------------------

int  OSD::available(void)
{
  return 0;
}

int  OSD::read(void)
{
  return 0;
}

int  OSD::peek(void)
{
  return 0;
}

void OSD::flush(void)
{
}

