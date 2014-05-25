
void uploadFont()
{
#ifdef FONT_UPDATABLE
  unsigned char bit_count = 0, byte_count = 0, r;
  unsigned int font_count = 0;
  unsigned char character_bitmap[64];

  /* ready to receive data */
  Serial.printf_P(PSTR("Ready for Font\n"));

  while(font_count < 256) {
    r = Serial.read();
    switch(r) {
      case 0x0d: // CR
        //Serial.println("cr");
        if (bit_count == 8) {
          byte_count++;
          character_bitmap[byte_count] = 0;
        }
        bit_count = 0;
        break;
      case '0':
      case '1':
        character_bitmap[byte_count] <<= 1;
        character_bitmap[byte_count] |= (r & 0x01);
        bit_count++;
        break;
      default:
        break;
    }

    /* write character to max7456 eeprom */
    if(byte_count == 64) {
        osd.write_NVM(font_count, character_bitmap);  
        byte_count = 0;
        font_count++;
        Serial.printf_P(PSTR("Char Done\n"));
    }
  }
#endif
}

