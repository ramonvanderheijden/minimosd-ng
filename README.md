minimosd-ng
===========

MinimOSD firmware

Initial release based on MinimOSD-extra SVN r793

Pull requests are welcome.




Panels that can be used on ArduCAM-OSD hardware. Also their order numbers and other information.<br>
TODO: update panels<br>
<br>
  Order number are also used when configuring their location. Look below<br>
<br>
  Register , Order number , Panel name, Data/Size , Definitions<br>
  -------------------------------------------------------<br>
  panA_REG.0    01    panCenter       "IIII" \r "IIII"  - Center xrosshair <br>
  panA_REG.1    02    panPitch        "DDDDII"          - Pitch angle with symbols<br>
  panA_REG.2    03    panRoll         "DDDDII"          - Roll angle with symbols<br>
  panA_REG.3    04    panBattery1     "DD.DII"          - Voltage sensing #1 symbol with voltage reading<br>
  panA_REG.4    05    panBattery2     "DD.DII"          - Voltage sensing #2 symbol with voltage reading  (!Not Implemented)<br>
  panA_REG.5    06    panGPSats       "I  CC"           - Amount of locked satelliset with symbols<br>
  panA_REG.6    07    panGPL          "II"              - GPS Lock symbol<br>
  panA_REG.7    08    panGPS          "I DDD.DDDDDD" \r "I DDD.DDDDDD" - GPS location data (lat/lon)<br>
 <br>
  panB_REG.0    09    panRose         "IIIIIIIIIIIII" \r "ICCCCCCCCCCCCI"  - Compass rose<br>
  panB_REG.1    10    panHeading      "IIDDDDI"         - Compass heading with symbols<br>
  panB_REG.2    11    panMavBeat      "II"              - MAVLink heartbeat<br>
  panB_REG.3    12    panHomeDir      "II"      N/Y     - Home location arrows<br>
  panB_REG.4    13    panHomeDis      "IDDDDDI" N/Y     - Distance to home<br>
  panB_REG.5    14    panWPDir        "II"      N/Y     - Waypoint location arrows (!Not Implemented)<br>
  panB_REG.6    15    panWPDis        "IDDDDDI"         - Distance to next waypoint (!Not Implemented)<br>
  panB_REG.7    16    panRSSI                           - RSSI data from RC <br>
  <br>
  panC_REG.0    17    panCurrent1                       - Current sensing #1  <br>
  panC_REG.1    18    panCurrent2                       - Current sensing #2 (!Not Implemented)<br>
  panC_REG.2    19    panAlt          "IDDDDDI"         - Altitude information<br>
  panC_REG.3    20    panVel          "IDDDDDI"         - Velocity information<br>
  panC_REG.4    21    panThr          "IDDDDDI"         - MAVLink Throttle data<br>
  panC_REG.5    22    panFMod         "II"              - Flight mode display<br>
  panC_REG.6    05    panHorizon                        - Artificial Horizon<br>
  panC_REG.7    24    --<br>
<br>
<br>
I = Icon<br>
D = Digit<br>
C = Char<br>
<br>
N/Y = Not Yet working<br>
N/C = Not Confirmed<br>
<br>
Screen sizes: <br>
PAL   16 Rows x 30 Chars<br>
NTSC  13 Rows x 30 Chars<br>
 <br>
<br>
