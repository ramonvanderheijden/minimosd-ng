#include "config.h"


/* ******************************************************************/
/* *********************** GENERAL FUNCTIONS ********************** */

byte readEEPROM(int address)
{
  return EEPROM.read(address);
}

void writeEEPROM(byte value, int address)
{
  EEPROM.write(address, value);
}

#ifdef FORCEINIT
void InitializeOSD()
{
//  delay(500);

  writeEEPROM(42, CHK1);
  writeEEPROM(VER-42,CHK2);
  for(panel = 0; panel < NR_PANELS; panel++)
    writeSettings();

  osd.setPanel(4,9);
  osd.openPanel();
  osd.printf_P(PSTR("OSD Initialized, reboot")); 
  osd.closePanel();

  // run for ever so user resets 
  for(;;) {}
}
#endif

/* Initialize OSD with the default settings */
void writeSettings(unsigned char panel)
{
  /* eeprom widget organization:
     2 byte per widget
     bits  [15] = enabled / disabled
     bits[11:8] = Y coordinate
     bits [4:0] = X coordinate */

#if 0





#else


    uint16_t offset = OffsetBITpanel * panel;
//    writeEEPROM(off, panCenter_en_ADDR + offset);
//    writeEEPROM(13, panCenter_x_ADDR + offset);
//    writeEEPROM(7,  panCenter_y_ADDR + offset);
    writeEEPROM(on, panPitch_en_ADDR + offset);
    writeEEPROM(23, panPitch_x_ADDR + offset);
    writeEEPROM(11, panPitch_y_ADDR + offset);
    writeEEPROM(on, panRoll_en_ADDR + offset);
    writeEEPROM(12, panRoll_x_ADDR + offset);
    writeEEPROM(1,  panRoll_y_ADDR + offset);
    writeEEPROM(on, panBatt_A_en_ADDR + offset);
    writeEEPROM(14, panBatt_A_x_ADDR + offset);
    writeEEPROM(14,  panBatt_A_y_ADDR + offset);
    //writeEEPROM(on, panBatt_B_en_ADDR);
    //writeEEPROM(21, panBatt_B_x_ADDR);
    //writeEEPROM(3,  panBatt_B_y_ADDR);
    writeEEPROM(on, panGPSats_en_ADDR + offset);
    writeEEPROM(1,  panGPSats_x_ADDR + offset);
    writeEEPROM(11, panGPSats_y_ADDR + offset);
    writeEEPROM(on, panCOG_en_ADDR + offset);
    writeEEPROM(5,  panCOG_x_ADDR + offset);
    writeEEPROM(11, panCOG_y_ADDR + offset);
    writeEEPROM(on, panGPS_en_ADDR + offset);
    writeEEPROM(1,  panGPS_x_ADDR + offset);
    writeEEPROM(14, panGPS_y_ADDR + offset);
    writeEEPROM(on, panRose_en_ADDR + offset);
    writeEEPROM(21, panRose_x_ADDR + offset);
    writeEEPROM(15, panRose_y_ADDR + offset);
    writeEEPROM(on, panHeading_en_ADDR + offset);
    writeEEPROM(24, panHeading_x_ADDR + offset);
    writeEEPROM(14, panHeading_y_ADDR + offset);
//    writeEEPROM(on, panMavBeat_en_ADDR + offset);
//    writeEEPROM(2,  panMavBeat_x_ADDR + offset);
//    writeEEPROM(9, panMavBeat_y_ADDR + offset);
    writeEEPROM(on, panHomeDir_en_ADDR + offset);
    writeEEPROM(14, panHomeDir_x_ADDR + offset);
    writeEEPROM(3,  panHomeDir_y_ADDR + offset);
    writeEEPROM(on, panHomeDis_en_ADDR + offset);
    writeEEPROM(22, panHomeDis_x_ADDR + offset);
    writeEEPROM(1,  panHomeDis_y_ADDR + offset);
 //   writeEEPROM(off,panWPDir_en_ADDR);
 //   writeEEPROM(27,  panWPDir_x_ADDR);
 //   writeEEPROM(12,  panWPDir_y_ADDR);
    writeEEPROM(off, panWPDis_en_ADDR);
    writeEEPROM(9,  panWPDis_x_ADDR);
    writeEEPROM(14, panWPDis_y_ADDR);
    writeEEPROM(on, panRSSI_en_ADDR + offset);
    writeEEPROM(8,  panRSSI_x_ADDR + offset);
    writeEEPROM(13, panRSSI_y_ADDR + offset);
    writeEEPROM(on, panCur_A_en_ADDR + offset);
    writeEEPROM(14, panCur_A_x_ADDR + offset);
    writeEEPROM(15, panCur_A_y_ADDR + offset);
    //writeEEPROM(on, panCur_B_en_ADDR);
    //writeEEPROM(21, panCur_B_x_ADDR);
    //writeEEPROM(4,  panCur_B_y_ADDR);
    writeEEPROM(on, panAlt_en_ADDR + offset);
    writeEEPROM(22, panAlt_x_ADDR + offset);
    writeEEPROM(3,  panAlt_y_ADDR + offset);
    writeEEPROM(on, panHomeAlt_en_ADDR + offset);
    writeEEPROM(22, panHomeAlt_x_ADDR + offset);
    writeEEPROM(2,  panHomeAlt_y_ADDR + offset);
    writeEEPROM(on, panVel_en_ADDR + offset);
    writeEEPROM(1,  panVel_x_ADDR + offset);
    writeEEPROM(2,  panVel_y_ADDR + offset);
    writeEEPROM(on, panAirSpeed_en_ADDR + offset);
    writeEEPROM(1,  panAirSpeed_x_ADDR + offset);
    writeEEPROM(1,  panAirSpeed_y_ADDR + offset); 
    writeEEPROM(on, panBatteryPercent_en_ADDR + offset);
    writeEEPROM(1,  panBatteryPercent_x_ADDR + offset);
    writeEEPROM(4,  panBatteryPercent_y_ADDR + offset); 
    writeEEPROM(on, panTime_en_ADDR + offset);
    writeEEPROM(23, panTime_x_ADDR + offset);
    writeEEPROM(13, panTime_y_ADDR + offset);
    writeEEPROM(on, panThr_en_ADDR + offset);
    writeEEPROM(1,  panThr_x_ADDR + offset);
    writeEEPROM(3,  panThr_y_ADDR + offset);
    writeEEPROM(on, panFMod_en_ADDR + offset);
    writeEEPROM(1,  panFMod_x_ADDR + offset);
    writeEEPROM(13, panFMod_y_ADDR + offset);
    writeEEPROM(on, panHorizon_en_ADDR + offset);
    writeEEPROM(8,  panHorizon_x_ADDR + offset);
    writeEEPROM(6,  panHorizon_y_ADDR + offset);
    writeEEPROM(on, panWarn_en_ADDR + offset);
    writeEEPROM(9,  panWarn_x_ADDR + offset);
    writeEEPROM(4,  panWarn_y_ADDR + offset);
    writeEEPROM(on, panOff_en_ADDR + offset);
//    writeEEPROM(10, panOff_x_ADDR + offset);
//    writeEEPROM(4,  panOff_y_ADDR + offset);
    writeEEPROM(on, panWindSpeed_en_ADDR + offset);
    writeEEPROM(24, panWindSpeed_x_ADDR + offset);
    writeEEPROM(7,  panWindSpeed_y_ADDR + offset);
    writeEEPROM(on, panClimb_en_ADDR + offset);
    writeEEPROM(1,  panClimb_x_ADDR + offset);
    writeEEPROM(8,  panClimb_y_ADDR + offset);
#ifdef MINIMOSD_PLANE
    writeEEPROM(on, panTune_en_ADDR + offset);
    writeEEPROM(10, panTune_x_ADDR + offset);
    writeEEPROM(4,  panTune_y_ADDR + offset);
#endif
    writeEEPROM(on, panEff_en_ADDR + offset);
    writeEEPROM(14, panEff_x_ADDR + offset);
    writeEEPROM(13, panEff_y_ADDR + offset);
 //   writeEEPROM(on, panCh_en_ADDR + offset);
 //   writeEEPROM(10, panCh_x_ADDR + offset);
 //   writeEEPROM(4,  panCh_y_ADDR + offset);
    writeEEPROM(off, panTemp_en_ADDR + offset);
    writeEEPROM(22, panTemp_x_ADDR + offset);
    writeEEPROM(14, panTemp_y_ADDR + offset);
//    writeEEPROM(on, panFata_en_ADDR + offset);
//    writeEEPROM(10, panFdata_x_ADDR + offset);
//    writeEEPROM(4,  panFdata_y_ADDR + offset);
    writeEEPROM(off, panDistance_en_ADDR + offset);
    writeEEPROM(22, panDistance_x_ADDR + offset);
    writeEEPROM(2,  panDistance_y_ADDR + offset);
 
    writeEEPROM(30,overspeed_ADDR);
    writeEEPROM(0,stall_ADDR);
    writeEEPROM(100,battv_ADDR); //10Volts
    writeEEPROM(6,ch_toggle_ADDR);
#endif
}

void readSettings()
{
  int i;

  overspeed = EEPROM.read(overspeed_ADDR);
  stall = EEPROM.read(stall_ADDR);
  battv = EEPROM.read(battv_ADDR);
  switch_mode = EEPROM.read(switch_mode_ADDR);
  panel_auto_switch = EEPROM.read(AUTO_SCREEN_SWITC_ADD);
//  if (EEPROM.read(ch_toggle_ADDR) < 4 || EEPROM.read(ch_toggle_ADDR) > 8){
//    EEPROM.write(ch_toggle_ADDR, 5);
//  }
  ch_toggle = EEPROM.read(ch_toggle_ADDR);
  //battp = EEPROM.read(battp_ADDR);
  rssical = EEPROM.read(OSD_RSSI_HIGH_ADDR);
  rssipersent = EEPROM.read(OSD_RSSI_LOW_ADDR);
  rssiraw_on = EEPROM.read(OSD_RSSI_RAW_ADDR);

  batt_warn_level = EEPROM.read(OSD_BATT_WARN_ADDR);
  rssi_warn_level = EEPROM.read(OSD_RSSI_WARN_ADDR);
  for(i = 0; i < OSD_CALL_SIGN_TOTAL; i++) {
    char_call[i] = EEPROM.read(OSD_CALL_SIGN_ADDR + i);
    if(char_call[i] == 0)
      break;
  }
  char_call[i+1] ='\0'; //null terminate the string 
}

#define WIDGET(id_, p_, en_, x_, y_, draw_)  { \
  p_->widget[id_].cfg.en = en_; \
  p_->widget[id_].cfg.x = x_; \
  p_->widget[id_].cfg.y = y_; \
  p_->widget[id_].draw = draw_; \
  p_->widget[id_].visible = 1; \
}
  
  

void readPanelSettings(unsigned char panel)
{
  struct panel_config *p = &panels[panel];

  //****** First set of 8 Panels ******
  uint16_t offset = OffsetBITpanel * panel;

  /* pitch widget */
  WIDGET(WIDGET_PITCH_ID, p, readEEPROM(panPitch_en_ADDR + offset),
          readEEPROM(panPitch_x_ADDR + offset), checkPAL(readEEPROM(panPitch_y_ADDR + offset)),
          widget_draw_pitch);

  /* roll widget */
  WIDGET(WIDGET_ROLL_ID, p, readEEPROM(panRoll_en_ADDR + offset),
          readEEPROM(panRoll_x_ADDR + offset), checkPAL(readEEPROM(panRoll_y_ADDR + offset)),
          widget_draw_roll);

  /* nr_gps_sats widget */
  WIDGET(WIDGET_NRGPSSATS_ID, p, readEEPROM(panGPSats_en_ADDR + offset),
          readEEPROM(panGPSats_x_ADDR + offset), checkPAL(readEEPROM(panGPSats_y_ADDR + offset)),
          widget_draw_nrgpssats);

  /* nr_gps_sats widget */
  WIDGET(WIDGET_ALTITUDE_ID, p, readEEPROM(panAlt_en_ADDR + offset),
          readEEPROM(panAlt_x_ADDR + offset), checkPAL(readEEPROM(panAlt_y_ADDR + offset)),
          widget_draw_altitude);

  /* battery widget */
  WIDGET(WIDGET_BATTERY_ID, p, readEEPROM(panBatt_A_en_ADDR + offset),
          readEEPROM(panBatt_A_x_ADDR + offset), checkPAL(readEEPROM(panBatt_A_y_ADDR + offset)),
          widget_draw_battery);

  WIDGET(WIDGET_BATTERYPERCENT_ID, p, readEEPROM(panBatteryPercent_en_ADDR + offset),
          readEEPROM(panBatteryPercent_x_ADDR + offset), checkPAL(readEEPROM(panBatteryPercent_y_ADDR + offset)),
          widget_draw_batterypercent);

  WIDGET(WIDGET_GPSCOORD_ID, p, readEEPROM(panGPS_en_ADDR + offset),
          readEEPROM(panGPS_x_ADDR + offset), checkPAL(readEEPROM(panGPS_y_ADDR + offset)),
          widget_draw_gpscoord);

  WIDGET(WIDGET_COG_ID, p, readEEPROM(panCOG_en_ADDR + offset),
          readEEPROM(panCOG_x_ADDR + offset), checkPAL(readEEPROM(panCOG_y_ADDR + offset)),
          widget_draw_cog);

  WIDGET(WIDGET_CURRENT_ID, p, readEEPROM(panCur_A_en_ADDR + offset),
          readEEPROM(panCur_A_x_ADDR + offset), checkPAL(readEEPROM(panCur_A_y_ADDR + offset)),
          widget_draw_current);

  WIDGET(WIDGET_ROSE_ID, p, readEEPROM(panRose_en_ADDR + offset),
          readEEPROM(panRose_x_ADDR + offset), checkPAL(readEEPROM(panRose_y_ADDR + offset)),
          widget_draw_rose);

  WIDGET(WIDGET_HEADING_ID, p, readEEPROM(panHeading_en_ADDR + offset),
          readEEPROM(panHeading_x_ADDR + offset), checkPAL(readEEPROM(panHeading_y_ADDR + offset)),
          widget_draw_heading);

  WIDGET(WIDGET_HOMEDISTANCE_ID, p, readEEPROM(panHomeDis_en_ADDR + offset),
          readEEPROM(panHomeDis_x_ADDR + offset), checkPAL(readEEPROM(panHomeDis_y_ADDR + offset)),
          widget_draw_homedistance);

  WIDGET(WIDGET_HOMEDIRECTION_ID, p, readEEPROM(panHomeDir_en_ADDR + offset),
          readEEPROM(panHomeDir_x_ADDR + offset), checkPAL(readEEPROM(panHomeDir_y_ADDR + offset)),
          widget_draw_homedirection);
/*
  WIDGET(WIDGET_WPDIRECTION_ID, p, readEEPROM(panWPDir_en_ADDR + offset),
          readEEPROM(panWPDir_x_ADDR + offset), checkPAL(readEEPROM(panWPDir_y_ADDR + offset)),
          widget_draw_wpdirection);
*/
  WIDGET(WIDGET_WPDISTANCE_ID, p, readEEPROM(panWPDis_en_ADDR + offset),
          readEEPROM(panWPDis_x_ADDR + offset), checkPAL(readEEPROM(panWPDis_y_ADDR + offset)),
          widget_draw_wpdistance);

/*
  WIDGET(WIDGET_WPDIRECTION_ID, p, readEEPROM(panMavBeat_en_ADDR + offset),
          readEEPROM(panMavBeat_x_ADDR + offset), checkPAL(readEEPROM(panMavBeat_y_ADDR + offset)),
          widget_draw_mavbeat);
*/

  WIDGET(WIDGET_TIME_ID, p, readEEPROM(panTime_en_ADDR + offset),
          readEEPROM(panTime_x_ADDR + offset), checkPAL(readEEPROM(panTime_y_ADDR + offset)),
          widget_draw_time);

  WIDGET(WIDGET_WARNING_ID, p, readEEPROM(panWarn_en_ADDR + offset),
          readEEPROM(panWarn_x_ADDR + offset), checkPAL(readEEPROM(panWarn_y_ADDR + offset)),
          widget_draw_warning);

  WIDGET(WIDGET_RELATIVEALTITUDE_ID, p, readEEPROM(panHomeAlt_en_ADDR + offset),
          readEEPROM(panHomeAlt_x_ADDR + offset), checkPAL(readEEPROM(panHomeAlt_y_ADDR + offset)),
          widget_draw_relativealtitude);

  WIDGET(WIDGET_AIRSPEED_ID, p, readEEPROM(panAirSpeed_en_ADDR + offset),
          readEEPROM(panAirSpeed_x_ADDR + offset), checkPAL(readEEPROM(panAirSpeed_y_ADDR + offset)),
          widget_draw_airspeed);

  WIDGET(WIDGET_GROUNDSPEED_ID, p, readEEPROM(panVel_en_ADDR + offset),
          readEEPROM(panVel_x_ADDR + offset), checkPAL(readEEPROM(panVel_y_ADDR + offset)),
          widget_draw_groundspeed);

  WIDGET(WIDGET_THROTTLE_ID, p, readEEPROM(panThr_en_ADDR + offset),
          readEEPROM(panThr_x_ADDR + offset), checkPAL(readEEPROM(panThr_y_ADDR + offset)),
          widget_draw_throttle);

  WIDGET(WIDGET_FLIGHTMODE_ID, p, readEEPROM(panFMod_en_ADDR + offset),
          readEEPROM(panFMod_x_ADDR + offset), checkPAL(readEEPROM(panFMod_y_ADDR + offset)),
          widget_draw_flightmode);

  WIDGET(WIDGET_HORIZON_ID, p, readEEPROM(panHorizon_en_ADDR + offset),
          readEEPROM(panHorizon_x_ADDR + offset), checkPAL(readEEPROM(panHorizon_y_ADDR + offset)),
          widget_draw_horizon);

  WIDGET(WIDGET_WINDSPEED_ID, p, readEEPROM(panWindSpeed_en_ADDR + offset),
          readEEPROM(panWindSpeed_x_ADDR + offset), checkPAL(readEEPROM(panWindSpeed_y_ADDR + offset)),
          widget_draw_windspeed);

  WIDGET(WIDGET_CLIMBRATE_ID, p, readEEPROM(panClimb_en_ADDR + offset),
          readEEPROM(panClimb_x_ADDR + offset), checkPAL(readEEPROM(panClimb_y_ADDR + offset)),
          widget_draw_climbrate);

  WIDGET(WIDGET_TUNE_ID, p, readEEPROM(panTune_en_ADDR + offset),
          readEEPROM(panTune_x_ADDR + offset), checkPAL(readEEPROM(panTune_y_ADDR + offset)),
          widget_draw_tune);

  WIDGET(WIDGET_RSSI_ID, p, readEEPROM(panRSSI_en_ADDR + offset),
          readEEPROM(panRSSI_x_ADDR + offset), checkPAL(readEEPROM(panRSSI_y_ADDR + offset)),
          widget_draw_rssi);

  WIDGET(WIDGET_EFFICIENCY_ID, p, readEEPROM(panEff_en_ADDR + offset),
          readEEPROM(panEff_x_ADDR + offset), checkPAL(readEEPROM(panEff_y_ADDR + offset)),
          widget_draw_efficiency);

  WIDGET(WIDGET_CALLSIGN_ID, p, readEEPROM(panCALLSIGN_en_ADDR + offset),
          readEEPROM(panCALLSIGN_x_ADDR + offset), checkPAL(readEEPROM(panCALLSIGN_y_ADDR + offset)),
          widget_draw_callsign);

  WIDGET(WIDGET_TEMPERATURE_ID, p, readEEPROM(panTemp_en_ADDR + offset),
          readEEPROM(panTemp_x_ADDR + offset), checkPAL(readEEPROM(panTemp_y_ADDR + offset)),
          widget_draw_temperature);

  WIDGET(WIDGET_DISTANCE_ID, p, readEEPROM(panDistance_en_ADDR + offset),
          readEEPROM(panDistance_x_ADDR + offset), checkPAL(readEEPROM(panDistance_y_ADDR + offset)),
          widget_draw_distance);

/*
  WIDGET(WIDGET_CHANNELS_ID, p, readEEPROM(panCh_en_ADDR + offset),
          readEEPROM(panCh_x_ADDR + offset), checkPAL(readEEPROM(panCh_y_ADDR + offset)),
          widget_draw_channels);
*/

}

int checkPAL(int line)
{
  if(line >= osd.getCenter() && osd.getMode() == 0) {
    line -= 3;//Cutting lines offset after center if NTSC
  }
  return line;
}

void updateSettings(byte panelu, byte panel_x, byte panel_y, byte panel_s)
{
  if(panel >= 1 && panel <= 32) {
    writeEEPROM(panel_s, (6 * panelu) - 6 + 0);
    if(panel_s != 0) {
      writeEEPROM(panel_x, (6 * panelu) - 6 + 2);
      writeEEPROM(panel_y, (6 * panelu) - 6 + 4);
    }
    osd.clear();
    readSettings();
    for(panel = 0; panel < NR_PANELS; panel++)
      readPanelSettings(panel);
  }
}


void update_id()
{
  if (readEEPROM(MODELL_TYPE_ADD) != MINIMOSD_TYPE_ID)
    writeEEPROM(MINIMOSD_TYPE_ID, MODELL_TYPE_ADD);
  if (readEEPROM(FW_VERSION1_ADDR) != VERSION_MAJOR)
    writeEEPROM(VERSION_MAJOR, FW_VERSION1_ADDR);
  if (readEEPROM(FW_VERSION2_ADDR) != VERSION_MINOR)
    writeEEPROM(VERSION_MINOR, FW_VERSION2_ADDR);
}
