

void panLogo(){
    osd.setPanel(5, 5);
    osd.openPanel();
#ifdef MINIMOSD_PLANE
    osd.printf_P(PSTR("MinimOSD-ng - Plane"));
#endif
#ifdef MINIMOSD_COPTER
    osd.printf_P(PSTR("MinimOSD-ng - Copter"));
#endif
    osd.closePanel();
}


/******* PANELS - POSITION *******/

void render_panel(void)
{
  unsigned char i;
  struct widgets *w;

#ifdef MINIMOSD_PLANE 
  if ((takeofftime == 1) && (osd_alt_to_home > 10 || osd_groundspeed > 1 || osd_throttle > 1 || osd_home_distance > 100)){
    landed = millis();
  }
#endif

  //Base panel selection
  //No mavlink data available panel
  if(millis() > (lastMAVBeat + 2200)){
    if (currentBasePanel != 2){
      osd.clear();
      currentBasePanel = 2;
    }   
    //panLogo();
    //waitingMAVBeats = 1;
    mavlink_active = 0;
    draw_waiting_mavbeats();
  }
  //Flight summary panel
#ifdef MINIMOSD_PLANE
  //Only show flight summary 7 seconds after landing
  else if ((millis() - 7000) > landed){ 
#endif
#ifdef MINIMOSD_COPTER
  //Only show flight summary 10 seconds after landing and if throttle < 15
  else if (!motor_armed && (((millis() / 10000) % 2) == 0) && (tdistance > 50)){ 
#endif
    if (currentBasePanel != 1){
      osd.clear();
      currentBasePanel = 1;
    }
    draw_flightdata(); 
  }
  //Normal osd panel
  else{
    /* TODO fix warninigs
    if(ISd(0,Warn_BIT)) panWarn(panWarn_XY[0][0], panWarn_XY[1][0]); // this must be here so warnings are always checked */
    
    if (panels[0].widget[WIDGET_WARNING_ID].cfg.en)
      panels[0].widget[WIDGET_WARNING_ID].draw();
    
    //Check for panel toggle
    if(ch_toggle > 3) panOff(); // This must be first so you can always toggle
    if ((osd_clear == 1) || (currentBasePanel != 0)){
      osd.clear();
      osd_clear = 0;
      currentBasePanel = 0;
    }
    
    if(panel != NR_PANELS) {

      panels[panel].widget[WIDGET_HOMEDISTANCE_ID].visible = osd_got_home;
      panels[panel].widget[WIDGET_HOMEDIRECTION_ID].visible = osd_got_home;
      panels[panel].widget[WIDGET_WPDISTANCE_ID].visible = (wp_number > 0) ? 1: 0;
      
#ifdef MINIMOSD_COPTER
      panels[panel].widget[WIDGET_WINDSPEED_ID].visible = 0;
#endif

      /* draw all widgets */
      for (i = 0; i < NR_WIDGETS; i++) {
        w = &panels[panel].widget[i];
        if (w->cfg.en && w->visible) {
          osd.setPanel(w->cfg.x, w->cfg.y);
          osd.openPanel();
          w->draw();
          osd.closePanel();
        }
      }
    }
  }
  
  
  if (panels[0].widget[WIDGET_CALLSIGN_ID].cfg.en)
    panels[0].widget[WIDGET_CALLSIGN_ID].draw();

  timers();

    // OSD debug for development (Shown on top-middle panels) 
#ifdef membug
  osd.setPanel(13,4);
  osd.openPanel();
  osd.printf("%i",freeMem()); 
  osd.closePanel();
#endif
}



void panOff()
{
  bool rotatePanel = 0;

  //If there is a warning force switch to panel 0
  if(canswitch == 0){
    if(panel != panel_auto_switch){
      //osd.clear();
      osd_clear = 1;
    }
    panel = panel_auto_switch; 
  }
  else{
    //Flight mode switching
    if (ch_toggle == 4){
      if ((osd_mode != 6) && (osd_mode != 7)){
        if (osd_off_switch != osd_mode){ 
          osd_off_switch = osd_mode;
            osd_switch_time = millis();
            if (osd_off_switch == osd_switch_last){
              rotatePanel = 1;
            }
        }
        if ((millis() - osd_switch_time) > 2000){
          osd_switch_last = osd_mode;
        }
      }
    }
    else {
      if(ch_toggle == 5) ch_raw = chan5_raw;
      else if(ch_toggle == 6) ch_raw = chan6_raw;
      else if(ch_toggle == 7) ch_raw = chan7_raw;
      else if(ch_toggle == 8) ch_raw = chan8_raw;

      //Switch mode by value
      if (switch_mode == 0){
        //First panel
        if (ch_raw < 1300 && panel != 0) {
          osd_clear = 1;
          //osd.clear();
          panel = 0;
        }
        //Second panel
        else if ((ch_raw < 1700) && (panel != 1)) { //second panel
          osd_clear = 1;
          //osd.clear();
          panel = 1;
        }
        //Panel off
        else if (panel != NR_PANELS) {
          osd_clear = 1;
          //osd.clear();
          panel = NR_PANELS; //off panel
        }
      }
      //Rotation switch
      else{
        if (ch_raw > 1200)
          if (osd_switch_time + 1000 < millis()){
            rotatePanel = 1;
            osd_switch_time = millis();
        }
      }    
    }
    if(rotatePanel == 1){
      osd_clear = 1;
      //osd.clear();
      panel++;
      if (panel > NR_PANELS)
        panel = 0;
    }
  }
}



/* non-widgets */

void draw_flightdata()
{
#ifdef MINIMOSD_PLANE
  osd.setPanel(11, 3);
  osd.openPanel();                          
  //osd.printf("%c%3i%c%02i|%c%5.0f%c|%c%5.0f%c|%c%5.0f%c|%c%5.0f%c|%c%5.0f%c|%c%5.0f%c", 0x08,((int)start_Time/60)%60,0x3A,(int)start_Time%60, 0x0b, ((max_home_distance) * converth), high, 0x1b, ((tdistance) * converth), high, 0x13,(max_osd_airspeed * converts), spe,0x14,(max_osd_groundspeed * converts),spe,0x12, (max_osd_home_alt * converth), high,0x1d,(max_osd_windspeed * converts),spe);
  osd.printf("%c%3i%c%02i|%c%5.0f%c|%c%5.0f%c|%c%5i%c|%c%5i%c|%c%5i%c|%c%5i%c|%c%5.0f%c|%c%11.6f|%c%11.6f",
          0x08, (int) start_Time / 60, 0x3A, (int) start_Time % 60,
          0x0b, max_home_distance * converth, high,
          0x8f, tdistance * converth, high,
          0x13, (int) max_osd_airspeed * converts, spe,
          0x14, (int) max_osd_groundspeed * converts, spe,
          0x12, (int) max_osd_home_alt * converth, high,
          0x1d, (int) max_osd_windspeed * converts, spe,
          0x17, mah_used, 0x01,
          0x03, (double) osd_lat,
          0x04, (double) osd_lon);
#endif
#ifdef MINIMOSD_COPTER
  osd.setPanel(11, 4);
  osd.openPanel();
  osd.printf("%c%3i%c%02i|%c%5i%c|%c%5i%c|%c%5i%c|%c%5i%c|%c%10.6f|%c%10.6f",
          0x08,((int)total_flight_time_seconds/60)%60,0x3A,(int)total_flight_time_seconds%60,
          0x0B, (int)((max_home_distance) * converth), high,
          0x8F, (int)((tdistance) * converth), high,
          0x14,(int)(max_osd_groundspeed * converts),spe,
          0x12, (int)(max_osd_home_alt * converth), high,
          0x03, (double)osd_lat,
          0x04, (double)osd_lon);
#endif
  osd.closePanel();
}

void draw_waiting_mavbeats()
{
  osd.setPanel(5, 10);
  osd.openPanel();
  osd.printf_P(PSTR("No mav data!"));
  osd.closePanel();
}

/* **************************************************************** */
// Panel  : panGPL
// Needs  : X, Y locations
// Output : 1 static symbol with changing FIX symbol
// Size   : 1 x 2  (rows x chars)
// Staus  : done

//void panGPL(int first_col, int first_line){
//    osd.setPanel(first_col, first_line);
//    osd.openPanel();
//    char* gps_str;
//    if(osd_fix_type == 0 || osd_fix_type == 1) gps_str = "\x10\x20"; 
        //osd.printf_P(PSTR("\x10\x20"));
//    else if(osd_fix_type == 2 || osd_fix_type == 3) gps_str = "\x11\x20";
        //osd.printf_P(PSTR("\x11\x20"));
//    osd.printf("%s",gps_str);

    /*  if(osd_fix_type <= 1) {
    osd.printf_P(PSTR("\x10"));
    } else {
    osd.printf_P(PSTR("\x11"));
    }  */
//    osd.closePanel();
//}



void showArrow(uint8_t rotate_arrow,uint8_t method) {  

#ifdef MINIMOSD_PLANE
    char arrow_set1 = 0x0;
  
    if(rotate_arrow == 0) {
      rotate_arrow = 1;
    }
    arrow_set1 = rotate_arrow * 2 + 0x8E;

#endif
#ifdef MINIMOSD_COPTER
    int arrow_set1 = 0x90;
    //We trust that we receive rotate_arrow [1, 16] so 
    //it's no needed (rotate_arrow <= 16) in the if clause
    arrow_set1 += rotate_arrow * 2 - 2;
    //arrow_set2 = arrow_set1 + 1;
//    if(method == 1) osd.printf("%c%3.0f%c|%c%c%2.0f%c",0x1D,(double)(osd_windspeed * converts),spe, (byte)arrow_set1, (byte)(arrow_set1 + 1),(double)(osd_windspeedz * converts),spe);

#endif

//    if(method == 1) osd.printf("%c%3.0f%c|%c%c%2.0f%c",0x1d,(double)(osd_windspeed * converts),spe, arrow_set1, arrow_set2,(double)(osd_windspeedz * converts),spe);
    if(method == 1) osd.printf("%c%3.0f%c|%c%c%2.0f%c",0x1d,(double)(osd_windspeed * converts),spe, arrow_set1, arrow_set1 + 1,(double)(nor_osd_windspeed * converts),spe);
    else if(method == 2) osd.printf("%c%c%4i%c", arrow_set1, arrow_set1 + 1, off_course, 0x05);   
    else osd.printf("%c%c", arrow_set1, arrow_set1 + 1);
}

// Calculate and shows Artificial Horizon
// Smooth horizon by Jörg Rothfuchs
							// with different factors we can adapt do different cam optics
#define AH_PITCH_FACTOR		0.010471976		// conversion factor for pitch
#define AH_ROLL_FACTOR		0.017453293		// conversion factor for roll
#define AH_COLS			12			// number of artificial horizon columns
#define AH_ROWS			5			// number of artificial horizon rows
#define CHAR_COLS		12			// number of MAX7456 char columns
#define CHAR_ROWS		18			// number of MAX7456 char rows
#define CHAR_SPECIAL		9			// number of MAX7456 special chars for the artificial horizon
#define AH_TOTAL_LINES		AH_ROWS * CHAR_ROWS	// helper define


#define LINE_SET_STRAIGHT__	(0xC7 - 1)		// code of the first MAX7456 straight char -1
#define LINE_SET_STRAIGHT_O	(0xD0 - 3)		// code of the first MAX7456 straight overflow char -3
#define LINE_SET_P___STAG_1	(0xD1 - 1)		// code of the first MAX7456 positive staggered set 1 char -1
#define LINE_SET_P___STAG_2	(0xDA - 1)		// code of the first MAX7456 positive staggered set 2 char -1
#define LINE_SET_N___STAG_1	(0xE3 - 1)		// code of the first MAX7456 negative staggered set 1 char -1
#define LINE_SET_N___STAG_2	(0xEC - 1)		// code of the first MAX7456 negative staggered set 2 char -1
#define LINE_SET_P_O_STAG_1	(0xF5 - 2)		// code of the first MAX7456 positive overflow staggered set 1 char -2
#define LINE_SET_P_O_STAG_2	(0xF9 - 1)		// code of the first MAX7456 positive overflow staggered set 2 char -1
#define LINE_SET_N_O_STAG_1	(0xF7 - 2)		// code of the first MAX7456 negative overflow staggered set 1 char -2
#define LINE_SET_N_O_STAG_2	(0xFC - 1)		// code of the first MAX7456 negative overflow staggered set 2 char -1


#define OVERFLOW_CHAR_OFFSET	6			// offset for the overflow subvals


#define ANGLE_1			9			// angle above we switch to line set 1
#define ANGLE_2			25			// angle above we switch to line set 2


// Calculate and show artificial horizon
// used formula: y = m * x + n <=> y = tan(a) * x + n
void showHorizon(int start_col, int start_row) {
    int col, row, pitch_line, middle, hit, subval;
    int roll;
    int line_set = LINE_SET_STRAIGHT__;
    int line_set_overflow = LINE_SET_STRAIGHT_O;
    int subval_overflow = 9;
    
    // preset the line char attributes
    roll = osd_roll;
    if ((roll >= 0 && roll < 90) || (roll >= -179 && roll < -90)) {	// positive angle line chars
	roll = roll < 0 ? roll + 179 : roll;
        if (abs(roll) > ANGLE_2) {
	    line_set = LINE_SET_P___STAG_2;
	    line_set_overflow = LINE_SET_P_O_STAG_2;
            subval_overflow = 7;
	} else if (abs(roll) > ANGLE_1) {
	    line_set = LINE_SET_P___STAG_1;
	    line_set_overflow = LINE_SET_P_O_STAG_1;
            subval_overflow = 8;
	}
    } else {								// negative angle line chars
	roll = roll > 90 ? roll - 179 : roll;
        if (abs(roll) > ANGLE_2) {
	    line_set = LINE_SET_N___STAG_2;
	    line_set_overflow = LINE_SET_N_O_STAG_2;
            subval_overflow = 7;
	} else if (abs(roll) > ANGLE_1) {
	    line_set = LINE_SET_N___STAG_1;
	    line_set_overflow = LINE_SET_N_O_STAG_1;
            subval_overflow = 8;
	}
    }
    
    pitch_line = round(tan(-AH_PITCH_FACTOR * osd_pitch) * AH_TOTAL_LINES) + AH_TOTAL_LINES/2;	// 90 total lines
    for (col=1; col<=AH_COLS; col++) {
        middle = col * CHAR_COLS - (AH_COLS/2 * CHAR_COLS) - CHAR_COLS/2;	  // -66 to +66	center X point at middle of each column
        hit = tan(AH_ROLL_FACTOR * osd_roll) * middle + pitch_line;	          // 1 to 90	calculating hit point on Y plus offset
        if (hit >= 1 && hit <= AH_TOTAL_LINES) {
	    row = (hit-1) / CHAR_ROWS;						  // 0 to 4 bottom-up
	    subval = (hit - (row * CHAR_ROWS) + 1) / (CHAR_ROWS / CHAR_SPECIAL);  // 1 to 9
	    
	    // print the line char
            osd.openSingle(start_col + col - 1, start_row + AH_ROWS - row - 1);
            osd.printf("%c", line_set + subval);
	    
	    // check if we have to print an overflow line char
	    if (subval >= subval_overflow && row < 4) {	// only if it is a char which needs overflow and if it is not the upper most row
                osd.openSingle(start_col + col - 1, start_row + AH_ROWS - row - 2);
                osd.printf("%c", line_set_overflow + subval - OVERFLOW_CHAR_OFFSET);
	    }
        }
    }
}

// Calculate and shows ILS
#ifdef MINIMOSD_COPTER
void showILS(int start_col, int start_row) { 
//Show line on panel center because horizon line can be
    //high or low depending on pitch attitude
    int subval_char = 0xCF;

    //shift alt interval from [-5, 5] to [0, 10] interval, so we
    //can work with remainders.
    //We are using a 0.2 altitude units as resolution (1 decimal place)
    //so convert we convert it to times 10 to work 
    //only with integers and save some bytes
    //int alt = (osd_alt_to_home * converth + 5) * 10;
    int alt = (osd_alt_rel * converth + 5) * 4.4; //44 possible position 5 rows times 9 chars
    
    if((alt < 44) && (alt > 0)){
        //We have 9 possible chars
        //(alt * 5) -> 5 represents 1/5 which is our resolution. Every single
        //line (char) change represents 0,2 altitude units
        //% 10 -> Represents our 10 possible characters
        //9 - -> Inverts our selected char because when we gain altitude
        //the selected char has a lower position in memory
        //+ 5 -> Is the memory displacement od the first altitude charecter 
        //in memory (it starts at 0x05
        //subval_char = (99 - ((alt * 5) % 100)) / 9 + 0xC7;
        subval_char = (8 - (alt  % 9)) + 0xC7;
        //Each row represents 2 altitude units
        start_row += (alt / 9);
    }
    else if(alt >= 44){
        //Copter is too high. Ground is way too low to show on panel, 
        //so show down arrow at the bottom
        subval_char = 0xC8; 
        start_row += 4;
    }

    //Enough calculations. Let's show the result
    osd.openSingle(start_col + AH_COLS + 2, start_row);
    osd.printf("%c", subval_char);
}
#endif


void do_converts()
{
//  if(EEPROM.read(SIGN_MSL_ON_ADDR) == 0) EEPROM.write(876, 1);
//  if(EEPROM.read(SIGNS_ON_ADDR) != 0) iconGS = 1;
//  if(EEPROM.read(SIGNS_ON_ADDR) != 0) iconHA = 1;
//  if(EEPROM.read(SIGNS_ON_ADDR) != 0) iconMSL = 1;
    //      signDist = 0x8f;
 //     signTemp = 0x0a;
 //     signEff = 0x16;
 //     signRssi = 0x09;
 //     signCurr = 0xbd;
 //     signAlt = 0x11;
 //     signClimb = 0x15;
 //     signHomeAlt = 0x12;
 //     signVel = 0x14;
//      signASpeed = 0x13;
//      signThrot = 0x02;
//      signBat = 0x17;
 //     signTime = 0x08;
//      signHomeDist = 0x0b;
//      signBatA = 0xbc;
//      signMode = 0x7f;
//      signLat = 0x03;
//      signLon = 0x04;

    if (EEPROM.read(measure_ADDR) == 0) {
        converts = 3.6;
        converth = 1.0;
        spe = 0x10;
        high = 0x0c;
        temps = 0xba;
        tempconv = 10;
        tempconvAdd = 0;
        distchar = 0x1b;
        distconv = 1000;
        climbchar = 0x1a;
    } else {
        converts = 2.23;
        converth = 3.28;
        spe = 0x19;
        high = 0x66;
        temps = 0xbb;
        tempconv = 18;
#ifdef MINIMOSD_PLANE
        tempconvAdd = 32000;
#endif
#ifdef MINIMOSD_COPTER
        tempconvAdd = 3200;
#endif
        distchar = 0x1c;
        distconv = 5280;
        climbchar = 0x1e;
    }
}

void timers()
{
  if (one_sec_timer_switch == 1){ 
    one_sec_timer = millis() + 1000;
    one_sec_timer_switch = 0;
    blinker = !blinker;
  }
  if (millis() > one_sec_timer) one_sec_timer_switch = 1;  
}





/********************** widgets **********************/

void widget_draw_pitch(void)
{
    osd.printf("%4i%c%c", osd_pitch, 0x05, 0x07);
}

void widget_draw_roll(void)
{
    osd.printf("%4i%c%c", osd_roll, 0x05, 0x06);
}


void widget_draw_altitude(void)
{
#ifdef MINIMOSD_PLANE
  //osd.printf("%c%5.0f%c",0x11, (double)(osd_alt * converth), high);
  //if (iconMSL == 1) 
  if(EEPROM.read(SIGN_MSL_ON_ADDR) != 0) osd.printf_P(PSTR("\x11"));
  osd.printf("%5.0f%c", (double)(osd_alt * converth), high);
#endif
#ifdef MINIMOSD_COPTER
  //if(EEPROM.read(SIGN_MSL_ON_ADDR) != 0) osd.printf_P("\x11");
  if(EEPROM.read(SIGN_MSL_ON_ADDR) != 0) osd.printf("%c", 0x11);
  osd.printf("%5.0f%c", (double)(osd_alt_gps * converth), high);
#endif
}


void widget_draw_battery(void)
{
    /*************** This commented code is for the next ArduPlane Version
    if(osd_battery_remaining_A > 100){
        osd.printf(" %c%5.2f%c", 0xbc, (double)osd_vbat_A, 0x0d);
    else osd.printf("%c%5.2f%c%c", 0xbc, (double)osd_vbat_A, 0x0d, osd_battery_pic_A);
    */
//    osd.printf("%c%5.2f%c", 0xbc, (double)osd_vbat_A, 0x0d);
    osd.printf("%5.2f%c", (double)osd_vbat_A, 0x0d);
}

void widget_draw_batterypercent(void)
{
  if (EEPROM.read(OSD_BATT_SHOW_PERCENT_ADDR) == 1) {
    osd.printf("%c%3.0i%c", 0x17, osd_battery_remaining_A, 0x25);
  } else {
    osd.printf("%c%4.0f%c", 0x17, mah_used, 0x01);
  }
}

void widget_draw_nrgpssats(void)
{
  byte gps_chr = 0x2a;

  if ((eph >= 200) && blinker)
    gps_chr = 0x20;
  else if (osd_fix_type == 2)
    gps_chr = 0x1f;
  else if (osd_fix_type == 3)
    gps_chr = 0x0f;

  osd.printf("%c%2i", gps_chr, osd_satellites_visible);
}

void widget_draw_gpscoord(void)
{
#ifdef MINIMOSD_PLANE
  //osd.printf("%c%10.6f|%c%10.6f", 0x03, (double)osd_lat, 0x04, (double)osd_lon);
  osd.printf("%11.6f|%11.6f", (double)osd_lat, (double)osd_lon);
  //if (blinker == 0) osd.printf("%c%10.6f", 0x03, (double)osd_lat);
  //else osd.printf("%c%10.6f", 0x04, (double)osd_lon);
#endif
#ifdef MINIMOSD_COPTER
  osd.printf("%10.6f|%10.6f", (double)osd_lat, (double)osd_lon);
#endif
}

void widget_draw_cog(void)
{
//    osd_COG_arrow_rotate_int = (((int)osd_cog / 100 - (int)osd_heading)/360 * 16 + 16) % 16 + 1; // [1, 16]
//    osd_COG_arrow_rotate_int = (osd_cog / 100 - osd_heading) / 360 * 16 + 1;
    osd_COG_arrow_rotate_int = round(((osd_cog / 100) - osd_heading)/360.0 * 16.0 +1); //Convert to int 1-16 

#ifdef MINIMOSD_PLANE
    if(osd_COG_arrow_rotate_int < 1 ) osd_COG_arrow_rotate_int += 16;
//    if(osd_COG_arrow_rotate_int == 0) osd_COG_arrow_rotate_int = 16;    
    else if(osd_COG_arrow_rotate_int > 16) osd_COG_arrow_rotate_int -= 16;
#endif
#ifdef MINIMOSD_COPTER
    if(osd_COG_arrow_rotate_int < 0 ) osd_COG_arrow_rotate_int += 16;
    if(osd_COG_arrow_rotate_int == 0) osd_COG_arrow_rotate_int = 16;    
    if(osd_COG_arrow_rotate_int == 17) osd_COG_arrow_rotate_int = 1;
#endif

    if (((osd_cog / 100) - osd_heading) > 180) {
       off_course = (osd_cog / 100 - osd_heading) - 360;
    } else if (((osd_cog / 100) - osd_heading) < -180) {
       off_course = (osd_cog / 100 - osd_heading) + 360;
    } else {
       off_course = (osd_cog / 100 - osd_heading);
    }
    
    showArrow((uint8_t)osd_COG_arrow_rotate_int,2);
}

void widget_draw_current(void)
{
  //osd.printf("%c%5.2f%c", 0xbd, (float(osd_curr_A) * 0.01), 0x0e);
  osd.printf("%5.2f%c", (float(osd_curr_A) * 0.01), 0x0e);
}

void widget_draw_rose(void)
{
  //osd_heading  = osd_yaw;
  //if(osd_yaw < 0) osd_heading = 360 + osd_yaw;
  //osd.printf("%s|%c%s%c", "\x20\xc0\xc0\xc0\xc0\xc0\xc7\xc0\xc0\xc0\xc0\xc0\x20", 0xc3, buf_show, 0x87);
  osd.printf("%c%s%c", 0xc3, buf_show, 0x87);
}

void widget_draw_heading(void)
{
  osd.printf("%4.0f%c", (double) osd_heading, 0x05);
}


void widget_draw_homedistance(void)
{
#ifdef MINIMOSD_PLANE
//    osd.printf("%c%5.0f%c", 0x0b, (double)((osd_home_distance) * converth), high);
    if ((osd_home_distance * converth) > 9999.0) {
      osd.printf("%c%5.2f%c", 0x0b, ((osd_home_distance * converth) / distconv), distchar);
    }else{
      osd.printf("%c%5.0f%c", 0x0b, (osd_home_distance * converth), high);
    }
#endif
#ifdef MINIMOSD_COPTER
  osd.printf("%c%5.0f%c", 0x0b, (double)((osd_home_distance) * converth), high);
#endif  
}

void widget_draw_homedirection(void)
{
    showArrow((uint8_t)osd_home_direction, 0);
}

/*
void widget_draw_wpdirection(void)
{
  wp_target_bearing_rotate_int = round(((float)wp_target_bearing - osd_heading)/360.0 * 16.0) + 1; //Convert to int 0-16 
  if(wp_target_bearing_rotate_int < 0 ) wp_target_bearing_rotate_int += 16; //normalize  

  showArrow((uint8_t)wp_target_bearing_rotate_int,0);
}
*/

void widget_draw_wpdistance(void)
{
#ifdef MINIMOSD_PLANE
    wp_target_bearing_rotate_int = round(((float)wp_target_bearing - osd_heading)/360.0 * 16.0) + 1; //Convert to int 0-16 
    if(wp_target_bearing_rotate_int < 1 ) wp_target_bearing_rotate_int += 16; //normalize
//    else if(wp_target_bearing_rotate_int == 0 ) wp_target_bearing_rotate_int = 16; //normalize
    else if(wp_target_bearing_rotate_int > 16 ) wp_target_bearing_rotate_int -= 16; //normalize
      if (xtrack_error > 999) xtrack_error = 999;
      else if (xtrack_error < -999) xtrack_error = -999;

      osd.printf("%c%c%2i%c%4.0f%c|",0x57, 0x70, wp_number,0x0,(double)((float)(wp_dist) * converth),high);
      showArrow((uint8_t)wp_target_bearing_rotate_int,0);
      
      if (osd_mode == 10 || osd_mode == 15 || osd_mode == 7){     
        osd.printf("%c%c%c%4.0f%c", 0x20, 0x58, 0x65, (xtrack_error* converth), high);
      }else{
        osd.printf_P(PSTR("\x20\x20\x20\x20\x20\x20\x20\x20"));
          }
#endif
#ifdef MINIMOSD_COPTER
    wp_target_bearing_rotate_int = ((int)round(((float)wp_target_bearing - osd_heading)/360.0 * 16.0) + 16) % 16 + 1; //[1, 16]
    
    if (xtrack_error > 999) xtrack_error = 999;
    else if (xtrack_error < -999) xtrack_error = -999;

    osd.printf("%c%c%2i%c%4.0f%c|",0x57, 0x70, wp_number,0x0,(double)((float)(wp_dist) * converth),high);
    showArrow((uint8_t)wp_target_bearing_rotate_int,0);
    if (osd_mode == 10){

        osd.printf("%c%c%c%4.0f%c", 0x20, 0x58, 0x65, (xtrack_error* converth), high);
    }else{
        osd.printf_P(PSTR("\x20\x20\x20\x20\x20\x20\x20\x20"));
    }
#endif
}

void widget_draw_time(void)
{
#ifdef MINIMOSD_PLANE
    start_Time = (millis()/1000) - FTime;
//    osd.printf("%c%2i%c%02i", 0x08,((int)start_Time/60)%60,0x3A,(int)start_Time%60);
//    osd.printf("%2i%c%02i",((int)start_Time/60)%60,0x3A,(int)start_Time%60);
//    if((int)start_Time < 36000) oszt=60;
    osd.printf("%3i%c%02i", ((int)start_Time/60), 0x3A, (int)start_Time%60);
//    }else{
//    osd.printf("%2i%c%02i", ((int)start_Time/3600)%60, 0x3A, ((int)start_Time/60)%60);
//    }
#endif
#ifdef MINIMOSD_COPTER
  osd.printf("%2i%c%02i",((int)total_flight_time_seconds/60)%60,0x3A,(int)total_flight_time_seconds%60);
#endif
}

void widget_draw_warning(void){
  if (one_sec_timer_switch == 1){

    boolean warning[]={0,0,0,0,0,0}; // Make and clear the array


                // check all warnings at once
                if ((osd_fix_type) < 2) {
                  warning[1] = 1; 
                  warning[0] = 1;
                  }
#ifdef MINIMOSD_PLANE
                if (osd_airspeed * converts < stall && takeofftime == 1) {
#endif
#ifdef MINIMOSD_COPTER
                if (abs(vs) > stall * 10) {
#endif
                  warning[2] = 1; 
                  warning[0] = 1;
                  }
                if ((osd_airspeed * converts) > (float)overspeed) {
                  warning[3] = 1; 
                  warning[0] = 1;
                  }
                if (osd_vbat_A < float(battv)/10.0 || (osd_battery_remaining_A < batt_warn_level && batt_warn_level != 0)) {
                  warning[4] = 1; 
                  warning[0] = 1;
                  }
                if (rssi < rssi_warn_level && rssi != -99 && !rssiraw_on) {
                  warning[5] = 1; 
                  warning[0] = 1;
                  }
//                if (eph > 150){  
//                  warning[6] = 1;
//                  warning[0] = 1;
//                  }
                  
  

            // Prepare for printf in rotation
            if (rotation == 0) if (warning[0] == 0 || warning[0] + warning[1] + warning[2] + warning[3] + warning[4] + warning[5] == 2) {
                warning_string = "\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20"; // Blank
              }else{
                  rotation = 1; 
              } 
              
            if (rotation == 1) if (warning[1] == 1) {
                warning_string = "\x20\x4E\x6F\x20\x47\x50\x53\x20\x66\x69\x78\x21";  // No GPS fix!
              }else{
                  rotation = 2; 
              }
              
            if (rotation == 2) if (warning[2] == 1) {
#ifdef MINIMOSD_PLANE
                warning_string = "\x20\x20\x20\x53\x74\x61\x6c\x6c\x21\x20\x20\x20";  // Stall!
#endif
#ifdef MINIMOSD_COPTER
                warning_string = "\x48\x69\x67\x68\x20\x56\x53\x70\x65\x65\x64\x21"; // Hi VSpeed!
#endif
              }else{
                  rotation = 3; 
              }
              
            if (rotation == 3) if (warning[3] == 1) {
                warning_string = "\x20\x4f\x76\x65\x72\x53\x70\x65\x65\x64\x21\x20"; // Overspeed!
              }else{
                  rotation = 4; 
              }
              
            if (rotation == 4) if (warning[4] == 1) {
                warning_string = "\x42\x61\x74\x74\x65\x72\x79\x20\x4c\x6f\x77\x21"; // Battery Low!
              }else{
                  rotation = 5; 
              }
              
            if (rotation == 5) if (warning[5] == 1) {
                warning_string = "\x20\x20\x4c\x6f\x77\x20\x52\x73\x73\x69\x20\x20"; // Low Rssi
//                  rotation = 6;
              }
            
//            if (rotation == 6) if (warning[6] == 1) {
//                warning_string = "\x20\x20\x4c\x6f\x77\x20\x48\x44\x4f\x50\x20\x20";            
//              }
            rotation++;
          
          // Auto switch decesion
          if (warning[0] == 1 && panel_auto_switch < 3){
          canswitch = 0;  
          }else if (ch_raw < 1200) {
          canswitch = 1;
          }
 if (rotation > 5) rotation = 0;
            
 osd.printf("%s",warning_string);
 
  }
}

void widget_draw_relativealtitude(void)
{
  if(EEPROM.read(SIGN_HA_ON_ADDR) != 0) osd.printf("%c", 0x12);
#ifdef MINIMOSD_PLANE
//    osd.printf("%c%5.0f%c",0x12, (double)(osd_alt_to_home * converth), high);
//    if (iconHA == 1) 
    osd.printf("%5.0f%c", (double)(osd_alt_to_home * converth), high);
#endif
#ifdef MINIMOSD_COPTER
    osd.printf("%5.0f%c", (double)(osd_alt_rel * converth), high);
#endif
}

void widget_draw_groundspeed(void)
{
#ifdef MINIMOSD_PLANE
//    osd.printf("%c%3.0f%c",0x14,(double)(osd_groundspeed * converts),spe);
//    if (iconGS == 1) 
    if(EEPROM.read(SIGN_GS_ON_ADDR) != 0) osd.printf_P(PSTR("\x14"));
#endif
#ifdef MINIMOSD_COPTER
    //if(EEPROM.read(SIGN_GS_ON_ADDR) != 0) osd.printf_P("\x14");
    if(EEPROM.read(SIGN_GS_ON_ADDR) != 0) osd.printf("%c", 0x14);
#endif
    osd.printf("%3.0f%c",(double)(osd_groundspeed * converts),spe);
}

void widget_draw_airspeed(void)
{
#ifdef MINIMOSD_PLANE
//    osd.printf("%c%3.0f%c", 0x13, (double)(osd_airspeed * converts), spe);
//    if (iconAS == 1) 
    if(EEPROM.read(SIGN_AS_ON_ADDR) != 0) osd.printf_P(PSTR("\x13"));        
#endif
#ifdef MINIMOSD_COPTER
    //if(EEPROM.read(SIGN_AS_ON_ADDR) != 0) osd.printf_P("\x13");
    if(EEPROM.read(SIGN_AS_ON_ADDR) != 0) osd.printf("%c", 0x13);
#endif
    osd.printf("%3.0f%c", (double)(osd_airspeed * converts), spe); 
}

void widget_draw_throttle(void)
{
//    osd.printf("%c%3.0i%c",0x14,osd_throttle,0x25);
  osd.printf("%3.0i%c", osd_throttle, 0x25);
}

/* TODO: fix */
void widget_draw_horizon(void)
{
#if 0
#ifdef MINIMOSD_PLANE
    osd.printf("\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20|\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20|\xc6\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\xc5|\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20|\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20|");
    osd.closePanel();
    showHorizon((first_col + 1), first_line);
    //Show ILS on  HUD
//    showILS(first_col, first_line);
#endif
#ifdef MINIMOSD_COPTER
    osd.printf_P(PSTR("\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20|\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20|\xC6\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\xC5\x20|\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20|\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20"));

    osd.closePanel();
    showHorizon((first_col + 1), first_line);
    //Show ground level on  HUD
    showILS(first_col, first_line);
#endif
#endif
}

void widget_draw_flightmode(void)
{
    char* mode_str = "";

#ifdef MINIMOSD_PLANE
    //char c1 = 0x7f ;//"; char c2; char c3; char c4; char c5; 
    if (osd_mode == 0) mode_str = "manu"; //Manual 
    else if (osd_mode == 1) mode_str = "circ"; //CIRCLE
    else if (osd_mode == 2) mode_str = "stab"; //Stabilize
    else if (osd_mode == 3) mode_str = "trai"; //Training
    else if (osd_mode == 4) mode_str = "acro"; //ACRO
    else if (osd_mode == 5) mode_str = "fbwa"; //FLY_BY_WIRE_A
    else if (osd_mode == 6) mode_str = "fbwb"; //FLY_BY_WIRE_B
    else if (osd_mode == 7) mode_str = "cruz"; //Cruise
    else if (osd_mode == 10) mode_str = "auto"; //AUTO
    else if (osd_mode == 11) mode_str = "retl"; //Return to Launch
    else if (osd_mode == 12) mode_str = "loit"; //Loiter
    else if (osd_mode == 15) mode_str = "guid"; //GUIDED
    
//    osd.printf("%c%s", 0x7f, mode_str);
    osd.printf("%s", mode_str);
#endif
#ifdef MINIMOSD_COPTER
    //char c1 = 0xE0 ;//"; char c2; char c3; char c4; char c5; 
    if (osd_mode == 0) mode_str = "stab"; //Stabilize
    else if (osd_mode == 1) mode_str = "acro"; //Acrobatic
    else if (osd_mode == 2) mode_str = "alth"; //Alt Hold
    else if (osd_mode == 3) mode_str = "auto"; //Auto
    else if (osd_mode == 4) mode_str = "guid"; //Guided
    else if (osd_mode == 5) mode_str = "loit"; //Loiter
    else if (osd_mode == 6) mode_str = "retl"; //Return to Launch
    else if (osd_mode == 7) mode_str = "circ"; //Circle
    //else if (osd_mode == 8) mode_str = "posi"; //Position
    else if (osd_mode == 9) mode_str = "land"; //Land
    else if (osd_mode == 10) mode_str = "oflo"; //OF_Loiter
    else if (osd_mode == 11) mode_str = "drif"; //OF_Loiter
    else if (osd_mode == 13) mode_str = "sprt"; //OF_Loiter
    else if (osd_mode == 14) mode_str = "flip"; //OF_Loiter
    else if (osd_mode == 15) mode_str = "tune"; //OF_Loiter
    else if (osd_mode == 16) mode_str = "hybr"; //OF_Loiter
    osd.printf("%c%s%c", 0x7F, mode_str, motor_armed * 0x86);
#endif
}

void widget_draw_windspeed(void)
{
//    osd_wind_arrow_rotate_int = round((osd_winddirection - osd_heading)/360.0 * 16.0) + 1; //Convert to int 1-16 
//    if(osd_wind_arrow_rotate_int < -7 ) {
//    osd_wind_arrow_rotate_int += 24;
//    }else if(osd_wind_arrow_rotate_int > 8 ) {
//    osd_wind_arrow_rotate_int -= 8;
//    }else{
//    osd_wind_arrow_rotate_int += 8;
//    }
//    nor_osd_windspeed = osd_windspeed * 0.005 + nor_osd_windspeed * 0.995;
    
    if (osd_winddirection < 0){
    osd_wind_arrow_rotate_int = round(((osd_winddirection + 360) - osd_heading)/360.0 * 16.0) + 9; //Convert to int 1-16
    }else{
    osd_wind_arrow_rotate_int = round((osd_winddirection - osd_heading)/360.0 * 16.0) + 9; //Convert to int 1-16
    }
    if(osd_wind_arrow_rotate_int > 16 ) osd_wind_arrow_rotate_int -= 16; //normalize
    else if(osd_wind_arrow_rotate_int < 1 ) osd_wind_arrow_rotate_int += 16; //normalize
    nor_osd_windspeed = osd_windspeed * 0.010 + nor_osd_windspeed * 0.990;    
    
    showArrow((uint8_t)osd_wind_arrow_rotate_int,1); //print data to OSD
}

void widget_draw_climbrate(void)
{
#ifdef MINIMOSD_PLANE
    vs = (osd_climb * converth * 60) * 0.1 + vs * 0.9;
#endif
    osd.printf("%c%4.0f%c%c", 0x15, int(vs / 10.0) * 10.0, climbchar, 0x20);
}

void widget_draw_tune(void)
{
  osd.printf("%c%3.0f%c%c|%c%3.0f%c%c", 0xb0, (alt_error * converth), high, 0x20, 0xb1, ((aspd_error / 100.0) * converts), spe, 0x20);
}

void widget_draw_efficiency(void)
{
#ifdef MINIMOSD_PLANE
    if (osd_throttle >= 1){
      if (ma == 0) {
              ma = 1;
            }
        if (osd_groundspeed != 0) eff = (float(osd_curr_A * 10.0) / (osd_groundspeed * converts))* 0.1 + eff * 0.9;
//        eff = eff * 0.2 + eff * 0.8;
          if (eff > 0 && eff <= 9999) {
            osd.printf("%c%4.0f%c", 0x16, eff, 0x01);
          }else{
            osd.printf("\x16\x20\x20\x20\x20\x20");
          }
          
    }else{
         
        if ((osd_throttle < 1)){
            if (ma == 1) {
              palt = osd_alt_to_home;
//              descendt = millis();
              ddistance = tdistance;
              ma = 0;
            }
          }
            if (osd_climb < -0.05){ 
//            glide = ((osd_alt_to_home / (palt - osd_alt_to_home)) * ((millis() - descendt) / 1000)) * osd_groundspeed;
            glide = ((osd_alt_to_home / (palt - osd_alt_to_home)) * (tdistance - ddistance)) * converth;
            if (glide > 9999) glide = 9999;
             if (glide != 'inf' && glide > -0){
            osd.printf("%c%4.0f%c", 0x18, glide, high);
             }
            }
            else if (osd_climb >= -0.05 && osd_pitch < 0) {
              osd.printf_P(PSTR("\x18\x20\x20\x90\x91\x20"));   
            }else{
              osd.printf_P(PSTR("\x18\x20\x20\x20\x20\x20")); 
            }
            
        
    }
#endif
#ifdef MINIMOSD_COPTER
  //Check takeoff just to prevent inicial false readings
  if (motor_armed)
  {
    if(osd_battery_remaining_A != last_battery_reading)
    {
      remaining_estimated_flight_time_seconds = ((float)osd_battery_remaining_A * total_flight_time_milis / (max_battery_reading - osd_battery_remaining_A)) / 1000;
      last_battery_reading = osd_battery_remaining_A;
    }
    osd.printf("%c%2i%c%02i", 0x17,((int)remaining_estimated_flight_time_seconds/60)%60,0x3A,(int)remaining_estimated_flight_time_seconds%60);
  }
#endif
}

void widget_draw_callsign(void)
{
    if(((millis() / 1000) % 60) < 2){
      osd.printf("%s", char_call);
    }else{
      osd.printf("%s", strclear);
//osd.printf_P(PSTR("\x20\x20\x20\x20\x20\x20\x20\x20"));
    }
}

void widget_draw_temperature(void)
{
#ifdef MINIMOSD_PLANE
//    osd.printf("%c%5.1f%c", 0x0a, (float(temperature * tempconv + tempconvAdd) / 100), temps);
    osd.printf("%5.1f%c", (float(temperature * tempconv + tempconvAdd) / 1000), temps);
#endif
#ifdef MINIMOSD_COPTER
    //do_converts();
    osd.printf("%5.1f%c", (float(temperature / 10 * tempconv + tempconvAdd) / 100), temps);
#endif
}

void widget_draw_channels(void)
{
  //osd.printf("%c%c%5i|%c%c%5i|%c%c%5i|%c%c%5i|%c%c%5i|%c%c%5i", 0x43, 0x31, chan1_raw, 0x43, 0x32, chan2_raw, 0x43, 0x33, chan3_raw, 0x43, 0x34, chan4_raw, 0x43, 0x35, chan5_raw, 0x43, 0x36, chan6_raw); 
}

void widget_draw_rssi(void)
{
    if(rssiraw_on == 0) rssi = (int16_t)((float)((int16_t)osd_rssi - rssipersent)/(float)(rssical-rssipersent)*100.0f);
    if(rssiraw_on == 1) rssi = (int16_t)osd_rssi;
    
    if(rssiraw_on == 8) rssi = (int16_t)((float)(chan8_raw / 10 - rssipersent)/(float)(rssical-rssipersent)*100.0f);
    if(rssiraw_on == 9) rssi = chan8_raw;
    
//    if (rssi < -99) rssi = -99;
    osd.printf("%c%3i%c", 0x09, rssi, 0x25);
//    osd.printf("%c%3i%c", 0x09, osd_clear, 0x25); 
}

void widget_draw_distance(void)
{
    //do_converts();
    if ((tdistance * converth) > 9999.0) {
      osd.printf("%c%5.2f%c", 0x8f, ((tdistance * converth) / distconv), distchar);
    }else{
      osd.printf("%c%5.0f%c", 0x8f, (tdistance * converth), high);
    }
}


/*
void widget_draw_mavbeat(void)
{
  if(mavbeat == 1){
    osd.printf_P(PSTR("\xEA\xEC"));
    mavbeat = 0;
  } else {
    osd.printf_P(PSTR("\xEA\xEB"));
  }
}
*/


/*
void widget_draw_center(void)
{
  osd.printf_P(PSTR("\x05\x03\x04\x05|\x15\x13\x14\x15"));
}
*/

