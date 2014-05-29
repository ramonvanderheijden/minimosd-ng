#ifndef _CONFIG_H_
#define _CONFIG_H_

/* osd config */

#define NR_PANELS 2

typedef enum {
  WIDGET_PITCH_ID = 0,
  WIDGET_ROLL_ID,
  WIDGET_NRGPSSATS_ID,
  WIDGET_GPSCOORD_ID,
  WIDGET_ALTITUDE_ID,
  WIDGET_BATTERY_ID,
  WIDGET_BATTERYPERCENT_ID,
  WIDGET_COG_ID,
  WIDGET_CURRENT_ID,
  WIDGET_ROSE_ID,
  WIDGET_HEADING_ID,
  WIDGET_HOMEDISTANCE_ID,
  WIDGET_HOMEDIRECTION_ID,
  WIDGET_WPDISTANCE_ID,
  //WIDGET_WPDIRECTION_ID,
  //WIDGET_MAVBEAT_ID,
  WIDGET_TIME_ID,
  WIDGET_WARNING_ID,
  WIDGET_RELATIVEALTITUDE_ID,
  WIDGET_AIRSPEED_ID,
  WIDGET_GROUNDSPEED_ID,
  WIDGET_THROTTLE_ID,
  WIDGET_FLIGHTMODE_ID,
  WIDGET_HORIZON_ID,
  WIDGET_WINDSPEED_ID,
  WIDGET_CLIMBRATE_ID,
  WIDGET_TUNE_ID,
  WIDGET_RSSI_ID,
  WIDGET_EFFICIENCY_ID,
  WIDGET_CALLSIGN_ID,
  WIDGET_TEMPERATURE_ID,
  WIDGET_DISTANCE_ID,
//  WIDGET_CHANNELS_ID,
  
  
  NR_WIDGETS
} widget_id_t;

struct global_config {
  unsigned char overspeed;
  unsigned char stall;
  unsigned char batv;
  unsigned char switch_mode;
  unsigned char panel_auto_switch;
  unsigned char ch_toggle;
  unsigned char rssical;
  unsigned char rssipercent;
  unsigned char rssiraw_on;
  unsigned char batt_warn_level;
  unsigned char rssi_warn_level;
  unsigned char char_call[OSD_CALL_SIGN_TOTAL+1];
};


struct widget_config {
  /* enabled */
  unsigned char en;
  /* x,y coordinates */
  unsigned char x, y;
};

struct widgets {
  /* config */
  struct widget_config cfg;
  
  unsigned char visible;
  
  /* drawing function */
  void (*draw)(void);
};

struct panel_config {
  struct widgets widget[NR_WIDGETS];
};


struct panel_config panels[NR_PANELS];


#endif

