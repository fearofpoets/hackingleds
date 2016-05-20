// TODO: Comment explaining comments

// TODO: Comment explaining headers
#include <Adafruit_GPS.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#include <Time.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>


// TODO: Comment explaining variables
float targetLat = 37.4142744;
float targetLon = -122.077409;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, 6, NEO_GRB + NEO_KHZ800);

// Every time our microcontroller powers up, it call the setup() function 
// so that it knows how to setup our project
void setup()
{
  // This function sets up the GPS unit on our compass
  setupGPSSensor();

  // Below are two variables, the first for the latitude of our target desination (called targetLat)
  // and the second for the longitude of our target destination (called targetLon). You can 
  // change the values of what the compass points to by changing these variables (they're called
  // variables because you can "vary" their value. 
  targetLat = 37.4393216;
  targetLon = -122.125408;


  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}



/*
##############################################################################################################
.______    ___________    ____  ______   .__   __.  _______      __    __   _______ .______       _______ 
|   _  \  |   ____\   \  /   / /  __  \  |  \ |  | |       \    |  |  |  | |   ____||   _  \     |   ____|
|  |_)  | |  |__   \   \/   / |  |  |  | |   \|  | |  .--.  |   |  |__|  | |  |__   |  |_)  |    |  |__   
|   _  <  |   __|   \_    _/  |  |  |  | |  . `  | |  |  |  |   |   __   | |   __|  |      /     |   __|  
|  |_)  | |  |____    |  |    |  `--'  | |  |\   | |  '--'  |   |  |  |  | |  |____ |  |\  \----.|  |____ 
|______/  |_______|   |__|     \______/  |__| \__| |_______/    |__|  |__| |_______|| _| `._____||_______|
                                                                                                          
         .___________. __    __   _______ .______       _______          .______    _______               
         |           ||  |  |  | |   ____||   _  \     |   ____|         |   _  \  |   ____|              
         `---|  |----`|  |__|  | |  |__   |  |_)  |    |  |__            |  |_)  | |  |__                 
             |  |     |   __   | |   __|  |      /     |   __|           |   _  <  |   __|                
             |  |     |  |  |  | |  |____ |  |\  \----.|  |____          |  |_)  | |  |____               
             |__|     |__|  |__| |_______|| _| `._____||_______|         |______/  |_______|              
                                                                                                          
             _______  .______          ___       _______   ______   .__   __.      _______.               
            |       \ |   _  \        /   \     /  _____| /  __  \  |  \ |  |     /       |               
            |  .--.  ||  |_)  |      /  ^  \   |  |  __  |  |  |  | |   \|  |    |   (----`               
            |  |  |  ||      /      /  /_\  \  |  | |_ | |  |  |  | |  . `  |     \   \                   
            |  '--'  ||  |\  \----./  _____  \ |  |__| | |  `--'  | |  |\   | .----)   |                  
            |_______/ | _| `._____/__/     \__\ \______|  \______/  |__| \__| |_______/                   
                                                                                                          
###############################################################################################################
*/


// TODO: more variables
Adafruit_GPS GPS(&Serial1);
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);
#define GPSECHO false
float tripDistance;
int topLED = 13;
int compassOffset = 0;
int startLED = 0;
int startLEDlast = 16;
int start = 0;
int lastDir = 16;
int dirLED_r = 0;
int dirLED_g = 0;
int dirLED_b = 255;
int compassReading;
float magxOffset = 2.55;
float magyOffset = 27.95;
float fLat = 0.0;
float fLon = 0.0;
int offset = -8; // i.e. PST
uint32_t gpsTimer = millis();
uint32_t startupTimer = millis();
uint32_t compassTimer = millis();

void loop() // run over and over again
{
  compassCheck();

  //Serial.println(buttonState);
  // read data from the GPS in the 'main loop'
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if (GPSECHO)
    if (c) Serial.print(c);
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    Serial.println(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
  }

  // if millis() or timer wraps around, we'll just reset it
  if (gpsTimer > millis()) gpsTimer = millis();

  if (start == 0) {
    if (GPS.fix) {
      // set the Time to the latest GPS reading
      setTime(GPS.hour, GPS.minute, GPS.seconds, GPS.day, GPS.month, GPS.year);
      delay(50);
      adjustTime(offset * SECS_PER_HOUR);
      delay(500);
      tripDistance = (double)calc_dist(fLat, fLon, targetLat, targetLon);
      start = 1;
    }
  }    
  // approximately every 60 seconds or so, update time
  if ((millis() - gpsTimer > 60000) && (start == 1)) {
    gpsTimer = millis(); // reset the timer
    if (GPS.fix) {
      // set the Time to the latest GPS reading
      setTime(GPS.hour, GPS.minute, GPS.seconds, GPS.day, GPS.month, GPS.year);
      delay(50);
      adjustTime(offset * SECS_PER_HOUR);
      delay(500);
    }
  }

  if (GPS.fix) {
    fLat = decimalDegrees(GPS.latitude, GPS.lat);
    fLon = decimalDegrees(GPS.longitude, GPS.lon);
  }

  navMode();
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void setupGPSSensor() {
  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  Serial.begin(115200);
  Serial.println("Adafruit GPS library basic test!");

  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);

  /* Initialise the sensor */
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  // Ask for firmware version
  Serial1.println(PMTK_Q_RELEASE);
}

void navMode() {
  if (start == 1) {
    strip.setPixelColor(startLEDlast, strip.Color(0, 0, 0));
    compassCheck();

    headingDistance((double)calc_dist(fLat, fLon, targetLat, targetLon));

    if ((calc_bearing(fLat, fLon, targetLat, targetLon) - compassReading) > 0) {
      compassDirection(calc_bearing(fLat, fLon, targetLat, targetLon)-compassReading);
    } 
    else {
      compassDirection(calc_bearing(fLat, fLon, targetLat, targetLon)-compassReading+360);
    }

  } 
  else {
    // if millis() or timer wraps around, we'll just reset it
    if (startupTimer > millis()) startupTimer = millis();

    // approximately every 10 seconds or so, update time
    if (millis() - startupTimer > 200) {
      startupTimer = millis(); // reset the timer
      if (startLED == 16) {
        startLED = 0;
      }
      strip.setPixelColor(startLEDlast, strip.Color(0, 0, 0));
      strip.setPixelColor(startLED, strip.Color(0, 0, 255));
      strip.show();
      startLEDlast = startLED;
      startLED++;
    }
  }
}

int calc_bearing(float flat1, float flon1, float flat2, float flon2)
{
  float calc;
  float bear_calc;

  float x = 69.1 * (flat2 - flat1); 
  float y = 69.1 * (flon2 - flon1) * cos(flat1/57.3);

  calc=atan2(y,x);

  bear_calc= degrees(calc);

  if(bear_calc<=1){
    bear_calc=360+bear_calc; 
  }
  return bear_calc;
}
void headingDistance(int fDist)
{
  if (fDist < 10000) {
    dirLED_r = 255;
    dirLED_g = 0;
    dirLED_b = 0;
  }

  if (fDist < 1000) {
    dirLED_r = 100;
    dirLED_g = 0;
    dirLED_b = 0;
  }

  if (fDist < 100) {
    dirLED_r = 0;
    dirLED_g = 100;
    dirLED_b = 0;
  }

  if (fDist < 10) {
    dirLED_r = 0;
    dirLED_g = 255;
    dirLED_b = 0;
  }
}


unsigned long calc_dist(float flat1, float flon1, float flat2, float flon2)
{
  float dist_calc=0;
  float dist_calc2=0;
  float diflat=0;
  float diflon=0;

  diflat=radians(flat2-flat1);
  flat1=radians(flat1);
  flat2=radians(flat2);
  diflon=radians((flon2)-(flon1));

  dist_calc = (sin(diflat/2.0)*sin(diflat/2.0));
  dist_calc2= cos(flat1);
  dist_calc2*=cos(flat2);
  dist_calc2*=sin(diflon/2.0);
  dist_calc2*=sin(diflon/2.0);
  dist_calc +=dist_calc2;

  dist_calc=(2*atan2(sqrt(dist_calc),sqrt(1.0-dist_calc)));

  dist_calc*=6371000.0; //Converting to meters
  return dist_calc;
}

// Convert NMEA coordinate to decimal degrees
float decimalDegrees(float nmeaCoord, char dir) {
  uint16_t wholeDegrees = 0.01*nmeaCoord;
  int modifier = 1;

  if (dir == 'W' || dir == 'S') {
    modifier = -1;
  }

  return (wholeDegrees + (nmeaCoord - 100.0*wholeDegrees)/60.0) * modifier;
}

void compassCheck() {
  // if millis() or timer wraps around, we'll just reset it
  if (compassTimer > millis()) compassTimer = millis();

  // approximately every 10 seconds or so, update time
  if (millis() - compassTimer > 50) {
    /* Get a new sensor event */
    sensors_event_t event; 
    mag.getEvent(&event);

    float Pi = 3.14159;

    compassTimer = millis(); // reset the timer

    // Calculate the angle of the vector y,x
    float heading = (atan2(event.magnetic.y + magyOffset,event.magnetic.x + magxOffset) * 180) / Pi;

    // Normalize to 0-360
    if (heading < 0)
    {
      heading = 360 + heading;
    }
    compassReading = heading; 
  }  
}  

void compassDirection(int compassHeading) 
{
  //Serial.print("Compass Direction: ");
  //Serial.println(compassHeading);

  unsigned int ledDir = 2;
  int tempDir = 0;
  //Use this part of the code to determine which way you need to go.
  //Remember: this is not the direction you are heading, it is the direction to the destination (north = forward).

  if ((compassHeading > 348.75)||(compassHeading < 11.25)) {
      tempDir = topLED;
  }
  for(int i = 1; i < 16; i++){
    float pieSliceCenter = 45/2*i;
    float pieSliceMin = pieSliceCenter - 11.25;
    float pieSliceMax = pieSliceCenter + 11.25;
    if ((compassHeading >= pieSliceMin)&&(compassHeading < pieSliceMax)) {
      tempDir = topLED + i;
    }
  }

  if (tempDir > 15) {
    ledDir = tempDir - 16;
  }

  else if (tempDir < 0) {
    ledDir = tempDir + 16;
  } 
  else {
    ledDir = tempDir;
  }

  
    ledDir = ledDir + compassOffset;
    if (ledDir > 15) {
      ledDir = ledDir - 16;
    }
  

  if (lastDir != ledDir) {
    strip.setPixelColor(lastDir, strip.Color(0, 0, 0));
    strip.setPixelColor(ledDir, strip.Color(dirLED_r, dirLED_g, dirLED_b));
    strip.show();
    lastDir = ledDir;
  }
}
