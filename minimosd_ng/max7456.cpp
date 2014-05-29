  
#include "../FastSerial/FastSerial.h"

// Get the common arduino functions
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "wiring.h"
#endif

#include <EEPROM.h>

#include "max7456.h"
#include "OSD_Config.h"


/* If defined, mode autodetection will be used instead of setting */
//#define AUTO_DETECT_MODE

#define SPI_START 0x01
#define SPI_END   0x02

static void spi_init(void)
{
   /* initialize the standard SPI pins */
  DDRB = (1 << DDB2) | (1 << DDB3) | (1 << DDB5);
 
  /* enable SPI, master mode */
  SPCR = (1<<SPE) | (1<<MSTR);

  /* max7456 chipselect as output */ 
  pinMode(MAX7456_CS, OUTPUT);
  /* max7456 chipselect high */
  digitalWrite(MAX7456_CS,HIGH);
}


unsigned char spi_transfer(unsigned char b,
      unsigned char cs_ctrl)
{
  if (cs_ctrl & SPI_START)
    digitalWrite(MAX7456_CS, LOW);
  SPDR = b;
  while (!(SPSR & (1<<SPIF)));
  if (cs_ctrl & SPI_END)
    digitalWrite(MAX7456_CS, HIGH);
  return SPDR;
}


OSD::OSD()
{
}


//------------------ init ---------------------------------------------------

void OSD::init()
{
  uint8_t reg;

  /* init spi */
  spi_init();

  /* set vsync as input */
  pinMode(MAX7456_VSYNC, INPUT);
  /* enable pull resistor (vsync is an open-drain output) */
  digitalWrite(MAX7456_VSYNC, HIGH);

#ifdef AUTO_DETECT_MODE
  /* read STAT register and try to auto detect mode */
  spi_transfer(MAX7456_STAT_reg_read, SPI_START);
  video_mode = spi_transfer(0xff, SPI_END);
  if (video_mode & 0x3) {
    /* mode detected */
    video_mode &= 0x01;
  } else
#endif
  {
    video_mode = EEPROM.read(PAL_NTSC_ADDR) & 0x01;
    video_mode <<= MAX7456_VIDEO_STD_BIT;
  }

  if (video_mode == 0)
    video_center = MAX7456_CENTER_NTSC;
  else
    video_center = MAX7456_CENTER_PAL;
  
  /* reset max7456 and set video mode */
  spi_transfer(MAX7456_VM0_reg, SPI_START);
  spi_transfer(MAX7456_RESET | video_mode, SPI_END);
  delay(50);

   /* set auto black level */
  spi_transfer(MAX7456_OSDBL_reg_read, SPI_START);
  reg = spi_transfer(0xff, SPI_END) & 0xef;
  spi_transfer(MAX7456_OSDBL_reg, SPI_START);
  spi_transfer(reg, SPI_END);

  setBrightness();

  // define sync (auto,int,ext) and
  // making sure the Max7456 is enabled
  control(1);
}


//------------------ Set Brightness  ---------------------------------
void OSD::setBrightness()
{
  uint8_t blevel = EEPROM.read(OSD_BRIGHTNESS_ADDR);
  unsigned char x;

  blevel = 3 - (blevel & 0x3);

#if 0
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
#endif

  // set all rows to same charactor white level, 90%
  for (x = 0; x < 0x10; x++) {
    spi_transfer(x + 0x10, SPI_START);
    spi_transfer(blevel, SPI_END);
  }
}

//------------------ Get Mode --------------------------------

uint8_t OSD::getMode()
{
  return (video_mode == 0) ? MAX7456_NTSC : MAX7456_PAL;
}

//------------------ Get Center ----------------------------------

uint8_t OSD::getCenter()
{
  return video_center; //first line for center panel
}

//------------------ plug ---------------------------------------------------

void OSD::plug()
{
  digitalWrite(MAX7456_CS, LOW);
}

//------------------ clear ---------------------------------------------------

void OSD::clear()
{
  // clear the screen
  spi_transfer(MAX7456_DMM_reg, SPI_START);
  spi_transfer(MAX7456_CLEAR_display, SPI_END);
}

//------------------ set panel -----------------------------------------------

void OSD::setPanel(uint8_t st_col, uint8_t st_row)
{
  col = st_col;
  row = st_row;
}

//------------------ open panel ----------------------------------------------

void OSD::openPanel(void)
{
  unsigned short memaddr = row * 30 + col;

  /* set auto increment on */
  spi_transfer(MAX7456_DMM_reg, SPI_START);
  spi_transfer(MAX7456_INCREMENT_auto, 0);

  /* set start addr msb */
  spi_transfer(MAX7456_DMAH_reg, 0);
  spi_transfer((unsigned char) (memaddr >> 8), 0);

  /* set start addr lsb */
  spi_transfer(MAX7456_DMAL_reg, 0);
  spi_transfer((unsigned char) memaddr, 0);
}

//------------------ close panel ---------------------------------------------

void OSD::closePanel(void)
{  
  spi_transfer(MAX7456_DMDI_reg, 0);
  spi_transfer(MAX7456_END_string, SPI_END);
  row++;
}

//------------------ write single char ---------------------------------------------

void OSD::openSingle(uint8_t x, uint8_t y)
{
  unsigned short memaddr = y * 30 + x;

  /* send start address msb */
  spi_transfer(MAX7456_DMAH_reg, SPI_START);
  spi_transfer((unsigned char) (memaddr >> 8), 0);

  /* set start address lsb */
  spi_transfer(MAX7456_DMAL_reg, 0);
  spi_transfer((unsigned char) memaddr, 0);
}

//------------------ write ---------------------------------------------------

size_t OSD::write(uint8_t c)
{
  if(c == '|'){
    closePanel(); //It does all needed to finish auto increment and change current row
    openPanel(); //It does all needed to re-enable auto increment
  }
  else{
    spi_transfer(MAX7456_DMDI_reg, 0);
    spi_transfer(c, 0);
  }
  return 1;
}

//---------------------------------

void OSD::control(uint8_t ctrl)
{
  unsigned char b = video_mode;

  if (ctrl)
    /* sync modes available: auto, external and internal */
    b |= MAX7456_ENABLE_display_vert | MAX7456_SYNC_autosync;
  else
    b |= MAX7456_DISABLE_display;
  
  spi_transfer(MAX7456_VM0_reg, SPI_START);
  spi_transfer(b, SPI_END);
}

void OSD::write_NVM(int memaddr, uint8_t *character_bitmap)
{
  unsigned char i, *p = character_bitmap;

  /* disable display */
  spi_transfer(MAX7456_VM0_reg, SPI_START); 
  spi_transfer(MAX7456_DISABLE_display, 0);

  /* set address msb */
  spi_transfer(MAX7456_CMAH_reg, 0);
  spi_transfer(memaddr, 0);

  /* send 54 bytes */
  for(i = 0; i < NVM_ram_size; i++) {
    /* set address lsb */
    spi_transfer(MAX7456_CMAL_reg, 0);
    spi_transfer(i, 0);
    /* send byte */
    spi_transfer(MAX7456_CMDI_reg, 0);
    spi_transfer(*p++, 0);
  }

  /* write data to eeprom */
  spi_transfer(MAX7456_CMM_reg, 0);
  spi_transfer(WRITE_nvr, 0);
  
  /* wait until done - bit 5 in the status register returns to 0 (12ms) */
  while (spi_transfer(MAX7456_STAT_reg_read, 0) & STATUS_reg_nvr_busy);

  /* turn on display on the next vertical sync */
  spi_transfer(MAX7456_VM0_reg, 0);
  spi_transfer(MAX7456_ENABLE_display_vert, SPI_END);
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

