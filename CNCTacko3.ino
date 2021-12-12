//Serial.println(F(" Compiled " __DATE__ " at " __TIME__ " File: " __FILE__ ));
//#include <Arduino.h>
//#include <U8x8lib.h>
//#include <SPI.h>
//#include <Wire.h>
//U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

float rpmfloat;
unsigned long rpmtime;
unsigned int rpm = 0;
unsigned int oldrpm = 1;
bool tooslow = 1;
unsigned long currentTime;

void clearOldValue(int x, int y, unsigned int oldval) {
  display.setTextColor(SSD1306_BLACK);
  display.setCursor(x, y);     
  display.print(oldval);       
  display.setTextColor(SSD1306_WHITE);
}

void drawtext(int x, int y, String text) {
  display.setCursor(x, y);     
  display.print(text);       
  display.display();
  
}

void setup() {
  //u8x8.begin();
  //u8x8.setFont(u8x8_font_profont29_2x3_f);
  Serial.begin(115200);
  Serial.println(F(" Compiled " __DATE__ " at " __TIME__ " File: " __FILE__ ));
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (1 << CS12); //Prescaler 256
  TIMSK1 |= (1 << TOIE1); //enable timer overflow
  pinMode(2, INPUT);
  attachInterrupt(0, RPM, FALLING);
  // put your setup code here, to run once:
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  // Clear the buffer
  display.clearDisplay();
  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  drawtext(20, 10, "RPM:  ");
  drawtext(20, 20, "TEMP:  ");
  /*drawtext(55,20,String(20.5));*/

}

ISR(TIMER1_OVF_vect) {
  tooslow = 1;
}

void loop() {
  if (tooslow == 1) {
    if ( millis() - currentTime > 500) {
      Serial.println("SLOW!");
      drawtext(0, 0, "SLOW!");
      currentTime = millis();
    }
  }
  else {
    rpmfloat = 120 / (rpmtime / 31250.00);
    rpm = round(rpmfloat);
    
    if ( (rpm < 4000) && (rpm != oldrpm)  && (millis() - currentTime > 500) ) {
      drawtext(20, 10, "RPM:  ");
      drawtext(20, 20, "TEMP:  ");
      Serial.print("rpm:");
      Serial.println(rpm);

      clearOldValue(55,10,oldrpm);
      drawtext(55, 10, String(rpm));      
      oldrpm = rpm;
      
      currentTime = millis();      
    }
  }
}

void RPM () {
  rpmtime = TCNT1;
  TCNT1 = 0;
  tooslow = 0;
}
