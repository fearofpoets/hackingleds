#include <Adafruit_NeoPixel.h>

#define PIN 8
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);

enum Color { aqua, blue, brown, green, orange, pink, purple, red, teal, white, yellow};
enum Speed { fast, medium, slow };

void fadeOnce(Color color1, Color color2, uint16_t steps = 100, uint8_t interval = 50) {
  // Calculate linear interpolation between Color1 and Color2
  // Optimise order of operations to minimize truncation error

  uint16_t Interval = interval;
  uint16_t TotalSteps = steps;
  uint32_t Color1 = translateColor(color1);
  uint32_t Color2 = translateColor(color2);

  for (int i = 0; i < TotalSteps; i++)
  {
    uint8_t red = ((Red(Color1) * (TotalSteps - i)) + (Red(Color2) * i)) / TotalSteps;
    uint8_t green = ((Green(Color1) * (TotalSteps - i)) + (Green(Color2) * i)) / TotalSteps;
    uint8_t blue = ((Blue(Color1) * (TotalSteps - i)) + (Blue(Color2) * i)) / TotalSteps;
    
    strip.setPixelColor(0, strip.Color(red, green, blue));
    strip.show();
    delay(100);
  }
}

// Returns the Red component of a 32-bit color
uint8_t Red(uint32_t color)
{
    return (color >> 16) & 0xFF;
}

// Returns the Green component of a 32-bit color
uint8_t Green(uint32_t color)
{
    return (color >> 8) & 0xFF;
}

// Returns the Blue component of a 32-bit color
uint8_t Blue(uint32_t color)
{
    return color & 0xFF;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos)
{
    WheelPos = 255 - WheelPos;
    if(WheelPos < 85)
    {
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    else if(WheelPos < 170)
    {
        WheelPos -= 85;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    else
    {
        WheelPos -= 170;
        return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(50);
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











void loop() {
  // CHANGE YOUR NIGHTLIGHT CODE HERE
  
  showColor(red);
  showColor(blue);
  fadeOnce(red, blue);
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












void showColor(Color color) {
   uint32_t c_translated = translateColor(color);
   strip.setPixelColor(0, c_translated);
   strip.show();
   delay(2000);
   return; 
}


uint32_t translateColor(Color color) {
  switch (color) 
  {
    case aqua: 
      return strip.Color(0,255,255);
    case brown:
      return strip.Color(139,69,19);
    case red: 
      return strip.Color(255, 0, 0); 
    case pink: 
      return strip.Color(255,20,147);
    case purple:
      return strip.Color(128,0,128);
    case orange: 
      return strip.Color(255,165,0); 
    case green:
      return strip.Color(0, 255, 0); 
    case blue:
      return strip.Color(0, 0, 255); 
    case teal: 
      return strip.Color(0,128,128); 
    case yellow:
      return strip.Color(255,255,0);
    case white:
      return strip.Color(0, 0, 0); 
    default: // set to white
      return strip.Color(0, 0, 0); 
  }
}

uint16_t translateSpeed(Speed speed) {
  switch (speed) {
    case fast:
      return 500;
    case medium:
      return 2000;
    case slow:
      return 10000;
    default:
      return 100;
  }
}

