void uploadFont()
{
  unsigned char csum, i, ch = 0;
  unsigned char buf[64];
  const uint8_t ok[] = {'O', 'K', '\n'};
  int r;
  
  /* send ok to flag data receive ready */
  Serial.write(ok, 3);

  /* expect 256 chars */
  /* using do {} while to avoid using and integer for counting */
  do {
    csum = 0;
    /* expect 64 bytes per char although only 56 are useful */
    for (i = 0; i < 64; i++) {
      do { r = Serial.read(); } while (r == -1);
      buf[i] = (unsigned char) r;
      csum += (unsigned char) r;
    }
    
    osd.write_NVM(ch, buf);
    /* reply with the char index */
    Serial.write(ch);
    Serial.write(csum);
  } while (++ch != 0);
  Serial.write(ok, 3);
}

