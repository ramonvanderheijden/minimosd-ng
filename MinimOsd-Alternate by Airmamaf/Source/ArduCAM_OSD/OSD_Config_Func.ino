/* ******************************************************************/
/* *********************** GENERAL FUNCTIONS ********************** */

//Extract functions (get bits from the positioning bytes
#define ISa(whichBit) getBit(panA_REG, whichBit)
#define ISb(whichBit) getBit(panB_REG, whichBit)
#define ISc(whichBit) getBit(panC_REG, whichBit)

boolean getBit(byte Reg, byte whichBit) {
  boolean State;
  State = Reg & (1 << whichBit);
  return State;
}

byte setBit(byte &Reg, byte whichBit, boolean stat) {
  if (stat) {
    Reg = Reg | (1 << whichBit);
  } 
  else {
    Reg = Reg & ~(1 << whichBit);
  }
  return Reg;
}

// EEPROM reader/writers
// Utilities for writing and reading from the EEPROM
byte readEEPROM(int address) {

  return EEPROM.read(address);
}

void writeEEPROM(byte value, int address) {
  EEPROM.write(address, value);
}


void InitializeOSD() {
  
  loadBar();
  delay(500);

  writeEEPROM(42, CHK1);
  writeEEPROM(VER-42,CHK2);
  writeSettings();
  
  osd.setPanel(4,9);
  osd.openPanel();
  osd.printf_P(PSTR("OSD Initialized, reboot")); 
  osd.closePanel();
 
  // run for ever so user resets 
  for(;;) {}
   
}

// Write our latest FACTORY settings to EEPROM
void writeSettings() {
 // Writing all default parameters to EEPROM, ON = panel enabled  
 // All panels have 3 values:
 //  - Enable/Disable
 //  - X coordinate on screen
 //  - Y coordinate on screen
 writeEEPROM(off, panCenter_en_ADDR);
 writeEEPROM(13, panCenter_x_ADDR);
 writeEEPROM(7,  panCenter_y_ADDR);
 writeEEPROM(on, panPitch_en_ADDR);
 writeEEPROM(22, panPitch_x_ADDR);
 writeEEPROM(9,  panPitch_y_ADDR);
 writeEEPROM(on, panRoll_en_ADDR);
 writeEEPROM(11, panRoll_x_ADDR);
 writeEEPROM(1,  panRoll_y_ADDR);
 writeEEPROM(on, panBatt_A_en_ADDR);
 writeEEPROM(21, panBatt_A_x_ADDR);
 writeEEPROM(1,  panBatt_A_y_ADDR);
 //DMD BatB_BIT => Climb_BIT
 //writeEEPROM(on, panBatt_B_en_ADDR);
 //writeEEPROM(21, panBatt_B_x_ADDR);
 //writeEEPROM(3,  panBatt_B_y_ADDR);
 writeEEPROM(on, panClimb_en_ADDR);
 writeEEPROM(21, panClimb_x_ADDR);
 writeEEPROM(3,  panClimb_y_ADDR);
 
 writeEEPROM(on, panGPSats_en_ADDR);
 writeEEPROM(2,  panGPSats_x_ADDR);
 writeEEPROM(13, panGPSats_y_ADDR);
 writeEEPROM(on, panGPL_en_ADDR);
 writeEEPROM(5,  panGPL_x_ADDR);
 writeEEPROM(13, panGPL_y_ADDR);
 writeEEPROM(on, panGPS_en_ADDR);
 writeEEPROM(2,  panGPS_x_ADDR);
 writeEEPROM(14, panGPS_y_ADDR);
 writeEEPROM(on, panRose_en_ADDR);
 writeEEPROM(16, panRose_x_ADDR);
 writeEEPROM(14, panRose_y_ADDR);
 writeEEPROM(on, panHeading_en_ADDR);
 writeEEPROM(24, panHeading_x_ADDR);
 writeEEPROM(13, panHeading_y_ADDR);
 writeEEPROM(on, panMavBeat_en_ADDR);
 writeEEPROM(2,  panMavBeat_x_ADDR);
 writeEEPROM(9, panMavBeat_y_ADDR);
 writeEEPROM(on, panHomeDir_en_ADDR);
 writeEEPROM(14, panHomeDir_x_ADDR);
 writeEEPROM(3,  panHomeDir_y_ADDR);
 writeEEPROM(on, panHomeDis_en_ADDR);
 writeEEPROM(2,  panHomeDis_x_ADDR);
 writeEEPROM(1,  panHomeDis_y_ADDR);
 
 //DMD Current Ampere
 writeEEPROM(off,panWPDir_en_ADDR);
 writeEEPROM(0,  panWPDir_x_ADDR);
 writeEEPROM(0,  panWPDir_y_ADDR);
 
 //DMD Tuning value
 writeEEPROM(off,panWPDis_en_ADDR);
 writeEEPROM(0,  panWPDis_x_ADDR);
 writeEEPROM(0,  panWPDis_y_ADDR);
  //DMD RSSI
 writeEEPROM(on, panRSSI_en_ADDR);
 writeEEPROM(22, panRSSI_x_ADDR);
 writeEEPROM(6,  panRSSI_y_ADDR);
 
 //DMD Current % et battery
 writeEEPROM(on, panCur_A_en_ADDR);
 writeEEPROM(21, panCur_A_x_ADDR);
 writeEEPROM(2,  panCur_A_y_ADDR);

//DMD CurB_BIT => Temp_BIT
 writeEEPROM(on, panTemp_en_ADDR);
 writeEEPROM(21, panTemp_x_ADDR);
 writeEEPROM(4,  panTemp_y_ADDR);
 
 writeEEPROM(on, panAlt_en_ADDR);
 writeEEPROM(2,  panAlt_x_ADDR);
 writeEEPROM(2,  panAlt_y_ADDR);
 writeEEPROM(on, panVel_en_ADDR);
 writeEEPROM(2,  panVel_x_ADDR);
 writeEEPROM(3,  panVel_y_ADDR);
 writeEEPROM(on, panThr_en_ADDR);
 writeEEPROM(2,  panThr_x_ADDR);
 writeEEPROM(4,  panThr_y_ADDR);
 writeEEPROM(on, panFMod_en_ADDR);
 writeEEPROM(17,  panFMod_x_ADDR);
 writeEEPROM(13, panFMod_y_ADDR);
 writeEEPROM(on, panHorizon_en_ADDR);
 writeEEPROM(8,  panHorizon_x_ADDR);
 writeEEPROM(7,  panHorizon_y_ADDR);
}

void readSettings() {
  
  //****** First set of 8 Panels ******
  
  setBit(panA_REG, Cen_BIT, readEEPROM(panCenter_en_ADDR));
  panCenter_XY[0] = readEEPROM(panCenter_x_ADDR);
  panCenter_XY[1] = checkPAL(readEEPROM(panCenter_y_ADDR));
  
  setBit(panA_REG, Pit_BIT, readEEPROM(panPitch_en_ADDR));
  panPitch_XY[0] = readEEPROM(panPitch_x_ADDR);
  panPitch_XY[1] = checkPAL(readEEPROM(panPitch_y_ADDR));
  
  setBit(panA_REG, Rol_BIT, readEEPROM(panRoll_en_ADDR));
  panRoll_XY[0] = readEEPROM(panRoll_x_ADDR);
  panRoll_XY[1] = checkPAL(readEEPROM(panRoll_y_ADDR));
  
  setBit(panA_REG, BatA_BIT, readEEPROM(panBatt_A_en_ADDR));
  panBatt_A_XY[0] = readEEPROM(panBatt_A_x_ADDR);
  panBatt_A_XY[1] = checkPAL(readEEPROM(panBatt_A_y_ADDR));

  //DMD BatB_BIT => Climb_BIT
  //setBit(panA_REG, BatB_BIT, readEEPROM(panBatt_B_en_ADDR));
  //panBatt_B_XY[0] = readEEPROM(panBatt_B_x_ADDR);
  //panBatt_B_XY[1] = checkPAL(readEEPROM(panBatt_B_y_ADDR));
  setBit(panA_REG, Climb_BIT, readEEPROM(panClimb_en_ADDR));
  panClimb_XY[0] = readEEPROM(panClimb_x_ADDR);
  panClimb_XY[1] = checkPAL(readEEPROM(panClimb_y_ADDR));
  
  setBit(panA_REG, GPSats_BIT, readEEPROM(panGPSats_en_ADDR));
  panGPSats_XY[0] = readEEPROM(panGPSats_x_ADDR);
  panGPSats_XY[1] = checkPAL(readEEPROM(panGPSats_y_ADDR));

  setBit(panA_REG, GPL_BIT, readEEPROM(panGPL_en_ADDR));
  panGPL_XY[0] = readEEPROM(panGPL_x_ADDR);
  panGPL_XY[1] = checkPAL(readEEPROM(panGPL_y_ADDR));
  
  setBit(panA_REG, GPS_BIT, readEEPROM(panGPS_en_ADDR));
  panGPS_XY[0] = readEEPROM(panGPS_x_ADDR);
  panGPS_XY[1] = checkPAL(readEEPROM(panGPS_y_ADDR));

  //****** Second set of 8 Panels ******
  
  setBit(panB_REG, Rose_BIT, readEEPROM(panRose_en_ADDR));
  panRose_XY[0] = readEEPROM(panRose_x_ADDR);
  panRose_XY[1] = checkPAL(readEEPROM(panRose_y_ADDR));

  setBit(panB_REG, Head_BIT, readEEPROM(panHeading_en_ADDR));
  panHeading_XY[0] = readEEPROM(panHeading_x_ADDR);
  panHeading_XY[1] = checkPAL(readEEPROM(panHeading_y_ADDR));

  setBit(panB_REG, MavB_BIT, readEEPROM(panMavBeat_en_ADDR));
  panMavBeat_XY[0] = readEEPROM(panMavBeat_x_ADDR);
  panMavBeat_XY[1] = checkPAL(readEEPROM(panMavBeat_y_ADDR));

  setBit(panB_REG, HDis_BIT, readEEPROM(panHomeDis_en_ADDR));
  panHomeDis_XY[0] = readEEPROM(panHomeDis_x_ADDR);
  panHomeDis_XY[1] = checkPAL(readEEPROM(panHomeDis_y_ADDR));

  setBit(panB_REG, HDir_BIT, readEEPROM(panHomeDir_en_ADDR));
  panHomeDir_XY[0] = readEEPROM(panHomeDir_x_ADDR);
  panHomeDir_XY[1] = checkPAL(readEEPROM(panHomeDir_y_ADDR));
  
  //DMD Current Ampere 
  setBit(panB_REG, WDir_BIT, readEEPROM(panWPDir_en_ADDR));
  panWPDir_XY[0] = readEEPROM(panWPDir_x_ADDR);
  panWPDir_XY[1] = checkPAL(readEEPROM(panWPDir_y_ADDR));

  //DMD Tuning value 
  setBit(panB_REG, WDis_BIT, readEEPROM(panWPDis_en_ADDR));
  panWPDis_XY[0] = readEEPROM(panWPDis_x_ADDR);
  panWPDis_XY[1] = checkPAL(readEEPROM(panWPDis_y_ADDR));
  
  //RSSI DMD
  setBit(panB_REG, RSSI_BIT, readEEPROM(panRSSI_en_ADDR));
  panRSSI_XY[0] = readEEPROM(panRSSI_x_ADDR);
  panRSSI_XY[1] = checkPAL(readEEPROM(panRSSI_y_ADDR));

  //****** Third set of 8 Panels ******

  //DMD mesure du current
  setBit(panC_REG, CurA_BIT, readEEPROM(panCur_A_en_ADDR));
  panCur_A_XY[0] = readEEPROM(panCur_A_x_ADDR);
  panCur_A_XY[1] = checkPAL(readEEPROM(panCur_A_y_ADDR));

   //DMD CurB_BIT => Temp_BIT
  setBit(panC_REG, Temp_BIT, readEEPROM(panTemp_en_ADDR));
  panTemp_XY[0] = readEEPROM(panTemp_x_ADDR);
  panTemp_XY[1] = checkPAL(readEEPROM(panTemp_y_ADDR));

  setBit(panC_REG, Alt_BIT, readEEPROM(panAlt_en_ADDR));
  panAlt_XY[0] = readEEPROM(panAlt_x_ADDR);
  panAlt_XY[1] = checkPAL(readEEPROM(panAlt_y_ADDR));

  setBit(panC_REG, Vel_BIT, readEEPROM(panVel_en_ADDR));
  panVel_XY[0] = readEEPROM(panVel_x_ADDR);
  panVel_XY[1] = checkPAL(readEEPROM(panVel_y_ADDR));

  setBit(panC_REG, Thr_BIT, readEEPROM(panThr_en_ADDR));
  panThr_XY[0] = readEEPROM(panThr_x_ADDR);
  panThr_XY[1] = checkPAL(readEEPROM(panThr_y_ADDR));

  setBit(panC_REG, FMod_BIT, readEEPROM(panFMod_en_ADDR));
  panFMod_XY[0] = readEEPROM(panFMod_x_ADDR);
  panFMod_XY[1] = checkPAL(readEEPROM(panFMod_y_ADDR));

  setBit(panC_REG, Hor_BIT, readEEPROM(panHorizon_en_ADDR));
  panHorizon_XY[0] = readEEPROM(panHorizon_x_ADDR);
  panHorizon_XY[1] = checkPAL(readEEPROM(panHorizon_y_ADDR));

}

int checkPAL(int line){
  if(line >= osd.getCenter() && osd.getMode() == 0){
    line -= 3;//Cutting lines offset after center if NTSC
  }
  return line;
}

void updateSettings(byte panel, byte panel_x, byte panel_y, byte panel_s ) {
  if(panel >= 1 && panel <= 32) {
    
    writeEEPROM(panel_s, (6 * panel) - 6 + 0);
    if(panel_s != 0) {
      writeEEPROM(panel_x, (6 * panel) - 6 + 2);
      writeEEPROM(panel_y, (6 * panel) - 6 + 4);
    }
    osd.clear();
    readSettings();
  } 
}

