/*

minimOSD-ng - Open source OSD for minimOSD hardware
Based on minimOSD-extra (which is based on minimOSD)


Original authors:
Author(s): Sandro Benigno
Coauthor(s):
Jani Hirvinen   (All the EEPROM routines)
Michael Oborne  (OSD Configutator)
Mike Smith      (BetterStream and Fast Serial libraries)
Gábor Zoltán
Pedro Santos
Special Contribuitor:
Andrew Tridgell by all the support on MAVLink
Doug Weibel by his great orientation since the start of this project
Contributors: James Goppert, Max Levine, Burt Green, Eddie Furey
and all other members of DIY Drones Dev team
Thanks to: Chris Anderson, Jordi Munoz


This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>

*/

/* *************************************************/
/* ***************** DEFINITIONS *******************/

/* MAVLink telemetry baud rate */
#define TELEMETRY_SPEED  57600

/* Delay to wait for user actions */
#define BOOTTIME         2000

/* Define the type of OSD here */
#define MINIMOSD_PLANE
//#define MINIMOSD_COPTER

//#define membug 
//#define FORCEINIT  // You should never use this unless you know what you are doing 

#if defined(MINIMOSD_PLANE)
#define MINIMOSD_TYPE_ID 0
#elif defined(MINIMOSD_COPTER)
#define MINIMOSD_TYPE_ID 1
#endif

#define VERSION_MAJOR 0
#define VERSION_MINOR 1

#undef PROGMEM
#define PROGMEM __attribute__(( section(".progmem.data") ))

#undef PSTR
#define PSTR(s) (__extension__({static prog_char __c[] PROGMEM = (s); &__c[0];}))

/* **********************************************/
/* ***************** INCLUDES *******************/

// AVR Includes
#include "../FastSerial/FastSerial.h"
#include <AP_Common.h>
#include <AP_Math.h>
#include <math.h>
#include <inttypes.h>
#include <avr/pgmspace.h>
// Get the common arduino functions
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "wiring.h"
#endif
#include <EEPROM.h>
#include <GCS_MAVLink.h>

#ifdef membug
#include <MemoryFree.h>
#endif

// Configurations
#include "OSD_Config.h"
#include "max7456.h"
#include "OSD_Vars.h"
#include "OSD_Func.h"
#include "config.h"

// Objects and Serial definitions
FastSerialPort0(Serial);
OSD osd; // OSD object


/* **********************************************/
/* ***************** SETUP() *******************/

void setup() 
{
  Serial.begin(TELEMETRY_SPEED);

  // setup mavlink port
  mavlink_comm_0_port = &Serial;

#ifdef membug
  Serial.println(freeMem());
#endif

  // Prepare OSD for displaying
  osd.init();

  /* display init screen */
  panLogo();

  /* load the unit conversion preferences */
  do_converts(); 
  //delay(500);

  // OSD debug for development (Shown at start)
#ifdef membug
  osd.setPanel(1,1);
  osd.openPanel();
  osd.printf("%i",freeMem()); 
  osd.closePanel();
#endif

  // Just to easy up development things
#ifdef FORCEINIT
  InitializeOSD();
#endif

#if 0
  // Check EEPROM to see if we have initialized it already or not
  // also checks if we have new version that needs EEPROM reset
  if(readEEPROM(CHK1) + readEEPROM(CHK2) != VER) {
    osd.setPanel(6,9);
    osd.openPanel();
    osd.printf_P(PSTR("Missing/Old Config"));
    osd.closePanel();
    InitializeOSD();
  }
#endif

  // Get correct panel settings from EEPROM
  readSettings();
  for(panel = 0; panel < NR_PANELS; panel++)
    readPanelSettings(panel);

  /* set panel to 0 to start in the first navigation screen */
  panel = 0;
  delay(1000);
  Serial.flush(); 
  // Startup MAVLink timers  
  //mavlinkTimer.Set(&OnMavlinkTimer, 120);

  // House cleaning, clear display and enable timers
  //osd.clear();
  //mavlinkTimer.Enable();
  update_id();
}


/* ***************** MAIN LOOP *******************/
void loop() 
{

    /*if(enable_mav_request == 1){//Request rate control
        //osd.clear();
        //osd.setPanel(3,10);
        //osd.openPanel();
        //osd.printf_P(PSTR("Requesting DataStreams...")); 
        //osd.closePanel();
        //for(int n = 0; n < 3; n++){
        //    request_mavlink_rates();//Three times to certify it will be readed
        //    delay(50);
        //}
        enable_mav_request = 0;
        //delay(2000);
        osd.clear();
        waitingMAVBeats = 0;
        lastMAVBeat = millis();//Preventing error from delay sensing
    }*/
    
    //Run "timer" every 120 miliseconds
    if(millis() > mavLinkTimer + 120){
      mavLinkTimer = millis();
      OnMavlinkTimer();
    }
    read_mavlink();
    //mavlinkTimer.Run();
}

/* *********************************************** */
/* ******** functions used in main loop() ******** */
void OnMavlinkTimer()
{
  setHeadingPatern();  // generate the heading patern

  //osd_battery_pic_A = setBatteryPic(osd_battery_remaining_A);     // battery A remmaning picture
  //osd_battery_pic_B = setBatteryPic(osd_battery_remaining_B);     // battery B remmaning picture

  setHomeVars(osd);   // calculate and set Distance from home and Direction to home

  render_panel();
  setFdataVars();
}

