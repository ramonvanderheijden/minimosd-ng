#define MAVLINK_COMM_NUM_BUFFERS 1
#define MAVLINK_USE_CONVENIENCE_FUNCTIONS

// this code was moved from libraries/GCS_MAVLink to allow compile
// time selection of MAVLink 1.0
BetterStream	*mavlink_comm_0_port;
BetterStream	*mavlink_comm_1_port;

mavlink_system_t mavlink_system = {12,1,0,0};

#include "Mavlink_compat.h"

#ifdef MAVLINK10
#include "../GCS_MAVLink/include/mavlink/v1.0/mavlink_types.h"
#include "../GCS_MAVLink/include/mavlink/v1.0/ardupilotmega/mavlink.h"
#else
#include "../GCS_MAVLink/include/mavlink/v0.9/mavlink_types.h"
#include "../GCS_MAVLink/include/mavlink/v0.9/ardupilotmega/mavlink.h"
#endif

// true when we have received at least 1 MAVLink packet
static bool mavlink_active;
static uint8_t crlf_count = 0;

static int packet_drops = 0;
static int parse_error = 0;

void request_mavlink_rates()
{
  const int  maxStreams = 6;
  const uint8_t MAVStreams[maxStreams] = {MAV_DATA_STREAM_RAW_SENSORS,
					  MAV_DATA_STREAM_EXTENDED_STATUS,
                                          MAV_DATA_STREAM_RC_CHANNELS,
					  MAV_DATA_STREAM_POSITION,
                                          MAV_DATA_STREAM_EXTRA1, 
                                          MAV_DATA_STREAM_EXTRA2};
  const uint16_t MAVRates[maxStreams] = {0x02, 0x02, 0x05, 0x02, 0x05, 0x02};
  for (int i=0; i < maxStreams; i++) {
    	  mavlink_msg_request_data_stream_send(MAVLINK_COMM_0,
					       apm_mav_system, apm_mav_component,
					       MAVStreams[i], MAVRates[i], 1);
  }
}

void read_mavlink(){
  mavlink_message_t msg; 
  mavlink_status_t status;
  
  //grabing data 
  while(Serial.available() > 0) { 
    uint8_t c = Serial.read();
    
            /* allow CLI to be started by hitting enter 3 times, if no
           heartbeat packets have been received */
        if (mavlink_active == 0 && millis() < 20000) {
            if (c == '\n' || c == '\r') {
                crlf_count++;
            } else {
                crlf_count = 0;
            }
            if (crlf_count == 3) {
              //uploadFont(); //DMD a commenter pour sup. message
            }
        }
    
    //trying to grab msg  
    if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)) {
       mavlink_active = 1;
      //handle msg
      switch(msg.msgid) {
        case MAVLINK_MSG_ID_HEARTBEAT:
          {
            mavbeat = 1;
	    apm_mav_system    = msg.sysid;
	    apm_mav_component = msg.compid;
            apm_mav_type      = mavlink_msg_heartbeat_get_type(&msg);
#ifdef MAVLINK10             
            osd_mode = mavlink_msg_heartbeat_get_custom_mode(&msg);
            osd_nav_mode = 0;
            armed = (mavlink_msg_heartbeat_get_base_mode(&msg) & MAV_MODE_FLAG_SAFETY_ARMED); // DMD define armed variable
#endif            
            lastMAVBeat = millis();
            if(waitingMAVBeats == 1){
              enable_mav_request = 1;
            }
          }
          break;
        case MAVLINK_MSG_ID_SYS_STATUS:
          {
#ifndef MAVLINK10            
            osd_vbat_A = (mavlink_msg_sys_status_get_vbat(&msg) / 1000.0f);
            osd_mode = mavlink_msg_sys_status_get_mode(&msg);
            osd_nav_mode = mavlink_msg_sys_status_get_nav_mode(&msg);
            armed = (mavlink_msg_sys_status_get_status(&msg) > MAV_STATE_STANDBY); // DMD define armed variable
#else
            osd_vbat_A = (mavlink_msg_sys_status_get_voltage_battery(&msg) / 1000.0f);

#endif      
            osd_curr_A = mavlink_msg_sys_status_get_current_battery(&msg); //Battery current, in 10*milliamperes (1 = 10 milliampere)         
            //float curr_A = (float)mavlink_msg_sys_status_get_current_battery(&msg) * 0.01f * 0.02778f; //DMD battery in mah
            //float curr_A = ((float)mavlink_msg_sys_status_get_current_battery(&msg)) * 0.0044f; //0.004f; //DMD battery in mah
            //osd_curr_A += curr_A; //DMD battery in mah
            osd_battery_remaining_A = constrain(mavlink_msg_sys_status_get_battery_remaining(&msg), 0, 100);
    
            //osd_mode = apm_mav_component;//Debug
            //osd_nav_mode = apm_mav_system;//Debug
          }
          break;
#ifndef MAVLINK10 
        case MAVLINK_MSG_ID_GPS_RAW:
          {
            osd_lat = mavlink_msg_gps_raw_get_lat(&msg);
            osd_lon = mavlink_msg_gps_raw_get_lon(&msg);
            osd_fix_type = mavlink_msg_gps_raw_get_fix_type(&msg);
          }
          break;
        case MAVLINK_MSG_ID_GPS_STATUS:
          {
            osd_satellites_visible = mavlink_msg_gps_status_get_satellites_visible(&msg);
          }
          break;
#else
        case MAVLINK_MSG_ID_GPS_RAW_INT:
          {
            osd_lat = mavlink_msg_gps_raw_int_get_lat(&msg) / 10000000.0f;
            osd_lon = mavlink_msg_gps_raw_int_get_lon(&msg) / 10000000.0f;
            osd_fix_type = mavlink_msg_gps_raw_int_get_fix_type(&msg);
            osd_satellites_visible = mavlink_msg_gps_raw_int_get_satellites_visible(&msg);
            osd_eph = mavlink_msg_gps_raw_int_get_eph(&msg);
          }
          break;
#endif          
        //Modif DMD
        case MAVLINK_MSG_ID_RC_CHANNELS_RAW:
          {
            if( rssi_signal_from_apm ) {
                osd_rssi = constrain(mavlink_msg_rc_channels_raw_get_rssi(&msg) - 97,0,100); //Valeur initiale de 0 � 255 (en fait 390/4 � 820/4 => 97 � 205)
            }
            rc1_roll_value      = mavlink_msg_rc_channels_raw_get_chan1_raw(&msg);
            rc2_pitch_value     = mavlink_msg_rc_channels_raw_get_chan2_raw(&msg);
            rc3_throttle_value  = mavlink_msg_rc_channels_raw_get_chan3_raw(&msg);
            rc4_yaw_value       = mavlink_msg_rc_channels_raw_get_chan4_raw(&msg);
            rc6_ch6_value       = mavlink_msg_rc_channels_raw_get_chan6_raw(&msg);
            channel7_value      = mavlink_msg_rc_channels_raw_get_chan7_raw(&msg);
          }
          break;
        case MAVLINK_MSG_ID_PARAM_VALUE:
          {
            //Get a parameter value and index
            
            float param_value = mavlink_msg_param_value_get_param_value(&msg); 
            uint16_t param_index = mavlink_msg_param_value_get_param_index(&msg);

            //Get tuning value, when OSD is in normal mode (the only information get from MAVLINK_MSG_ID_PARAM_VALUE)
            if( osd_state_screen & MASK_OSD_NORMAL_MODE || osd_state_screen & MASK_OSD_LOGGING_MODE ) {
              current_tuning_value = param_value;
            }
            
            //Get the RSSI source : APM (apm rssi pin is not -1) or from PIN 12 in MinimOSD (apm rssi pin is -1)
            if( osd_state_screen & MASK_OSD_GET_RSSI_CONF ) {
              rssi_signal_from_apm = (param_value != -1);
              osd_state_screen = MASK_OSD_NORMAL_MODE;
            }
               
            if( osd_state_screen & MASK_OSD_TUNING_MODE) {
              if( osd_state_screen & MASK_OSD_TUNING_CH6_PARAM ) {
                int8_t tune_param_value = (int8_t) param_value;
            //If parameter index is CH6 tuning param, then the value is the parameter to tune (0 to 39)
                for (int i=1; i < TUNE_ITEM_NUMBER; i++){
                  if( tune_param_value == TUNING_OPT_ID[i] ) { //Get position of the current parameter
                      tuning_item_selected = i;
                      current_tuning_item = i;
                  }
                } 
                osd_state_screen &= ~MASK_OSD_TUNING_CH6_PARAM;
              } else {
                //Dsiplay current parameter value in Tuning mode
                current_tuning_value = param_value;
              }
            }
            
          }
          break;
        //FIN Modif DMD
        case MAVLINK_MSG_ID_SCALED_PRESSURE:
          {
            temp = mavlink_msg_scaled_pressure_get_temperature(&msg); 
          }
          break;
        // FIN Modif DMD
        case MAVLINK_MSG_ID_VFR_HUD:
          {
            osd_groundspeed = mavlink_msg_vfr_hud_get_groundspeed(&msg);
            osd_heading = mavlink_msg_vfr_hud_get_heading(&msg);// * 3.60f;//0-100% of 360
            osd_throttle = mavlink_msg_vfr_hud_get_throttle(&msg);
            if(osd_throttle > 100 && osd_throttle < 150) osd_throttle = 100;//Temporary fix for ArduPlane 2.28
            if(osd_throttle < 0 || osd_throttle > 150) osd_throttle = 0;//Temporary fix for ArduPlane 2.28
            osd_alt = mavlink_msg_vfr_hud_get_alt(&msg);
            climb_rate = mavlink_msg_vfr_hud_get_climb(&msg); //DMD Climb
          }
          break;
        case MAVLINK_MSG_ID_ATTITUDE:
          {
            osd_pitch = ToDeg(mavlink_msg_attitude_get_pitch(&msg));
            osd_roll = ToDeg(mavlink_msg_attitude_get_roll(&msg));
            osd_yaw = ToDeg(mavlink_msg_attitude_get_yaw(&msg));
          }
          break;
        default:
          //Do nothing
          break;
      }
    }
    delayMicroseconds(138);
    //next one
  }
  // Update global packet drops counter
  packet_drops += status.packet_rx_drop_count;
  parse_error += status.parse_error;

}
