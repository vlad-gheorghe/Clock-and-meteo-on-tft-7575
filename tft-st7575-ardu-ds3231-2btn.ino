// Arduino with DS3231 and ST7735R TFT display
// Real time clock & calendar with temperature monitor and 1.8" TFT screen
 
// include Wire library code (needed for I2C protocol devices)
#include <Wire.h>
// include ST7735 library
//#include <Adafruit_GFX.h>                  // Core graphics library
//#include <Adafruit_ST7735.h>               // Hardware-specific library
#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
//#include <SPI.h>
//#include <Fonts/FreeSerif9pt7b.h>
#include "DHT.h"
// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

#define DHTPIN 12     // what digital pin we're connected to
 #define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET 0 // Can alternately just connect to Arduino's reset pin

//#define TFT_CS     9                       // TFT CS pin is connected to arduino pin 9
//#define TFT_DC     10                      // TFT DC pin is connected to arduino pin 10
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC);

// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
 
#define button1    10                       // Button B1 is connected to Arduino pin 7
#define button2    11                       // Button B2 is connected to Arduino pin 6
 DHT dht(DHTPIN, DHTTYPE);
 
void setup(void) {
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  tft.reset();
//  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
 // tft.fillScreen(ST7735_BLACK);
 tft.begin(0x7575); // SDFP5408

  tft.setRotation(1); // Need for the Mega, please changed for your choice or rotation initial
   tft.fillScreen(BLACK);
  Wire.begin();    // Join i2c bus
  dht.begin();
  tft.drawFastHLine(0, 93, tft.width(), YELLOW);       // Draw horizontal blue line at position (0, 53)
  tft.drawFastHLine(0, 96, tft.width(), CYAN);  
   tft.drawFastHLine(0, 183, tft.width(),CYAN); 
  tft.drawFastHLine(0, 186, tft.width(), GREEN);      // Draw horizontal blue line at position (0, 106)
  //draw_text(52, 64, "TIME", 1, MAGENTA);
  draw_text(30, 205, "TEMPERATURA", 1, WHITE);
   draw_text(200, 205, "UMIDITATE", 1, WHITE);
    draw_text(255, 220, "%", 2, GREEN);
    draw_text(0, 05, "Ceas si meteo @2019", 1, BLUE);
    tft.drawFastHLine(0, 13, 115,BLUE);
  
}
char Time[]     = "  :  :  ";
char Calendar[] = "  -  -20  ";
char temperature[] = " 00.00";
char temperature_msb;
byte i, second, minute, hour, day, date, month, year, temperature_lsb;
 
void display_day(){
  switch(day){
    case 1:  draw_text(140, 10, "Duminica", 3, MAGENTA); break;
    case 2:  draw_text(140, 10, "    Luni", 3, MAGENTA); break;
    case 3:  draw_text(140, 10, "   Marti", 3, MAGENTA); break;
    case 4:  draw_text(140, 10, "Miercuri", 3, MAGENTA); break;
    case 5:  draw_text(140, 10, "     Joi", 3, MAGENTA); break;
    case 6:  draw_text(140, 10, "  Vineri", 3, MAGENTA); break;
    default: draw_text(140, 10, " Simbata", 3, MAGENTA);
  }
}
void DS3231_display(){
  // Convert BCD to decimal
  second = (second >> 4) * 10 + (second & 0x0F);
  minute = (minute >> 4) * 10 + (minute & 0x0F);
  hour   = (hour >> 4)   * 10 + (hour & 0x0F);
  date   = (date >> 4)   * 10 + (date & 0x0F);
  month  = (month >> 4)  * 10 + (month & 0x0F);
  year   = (year >> 4)   * 10 + (year & 0x0F);
  // End conversion
  Time[7]     = second % 10 + 48;
  Time[6]     = second / 10 + 48;
  Time[4]     = minute % 10 + 48;
  Time[3]     = minute / 10 + 48;
  Time[1]     = hour   % 10 + 48;
  Time[0]     = hour   / 10 + 48;
  Calendar[9] = year   % 10 + 48;
  Calendar[8] = year   / 10 + 48;
  Calendar[4] = month  % 10 + 48;
  Calendar[3] = month  / 10 + 48;
  Calendar[1] = date   % 10 + 48;
  Calendar[0] = date   / 10 + 48;
 /* if(temperature_msb < 0){
    temperature_msb = abs(temperature_msb);
    temperature[0] = '-';
  }
  else
    temperature[0] = ' ';
  temperature_lsb >>= 6;
  temperature[2] = temperature_msb % 10  + 48;
  temperature[1] = temperature_msb / 10  + 48;
  if(temperature_lsb == 0 || temperature_lsb == 2){
    temperature[5] = '0';
    if(temperature_lsb == 0) temperature[4] = '0';
    else                     temperature[4] = '5';
  }
  if(temperature_lsb == 1 || temperature_lsb == 3){
    temperature[5] = '5';
    if(temperature_lsb == 1) temperature[4] = '2';
    else                     temperature[4] = '7';
  }
  */
   float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  
  draw_text(20,  47, Calendar, 4, YELLOW); //DISPLAY DATA
  draw_text(56, 121, Time, 5, CYAN);  //DISPLAY ORA
  
  tft.setCursor(16, 220);
  tft.setTextColor(RED,BLACK);  tft.setTextSize(2);
  tft.print(t),
   tft.setCursor(190, 220);
  tft.setTextColor(GREEN,BLACK);  tft.setTextSize(2);
  tft.print(h),
  //draw_text(14, 134, t, 2, WHITE);
  tft.drawCircle(90, 220, 2, RED);                // Degree symbol (°)
  draw_text(96, 220, "C", 2, RED);
  // draw_text(260, 220, "%", 2, WHITE);
}
void blink_parameter(){
  byte j = 0;
  while(j < 10 && digitalRead(button1) && digitalRead(button2)){
    j++;
    delay(50);
  }
}
byte edit(byte x_pos, byte y_pos, byte parameter){
  char text[3];
  uint16_t color = YELLOW;
  sprintf(text,"%02u", parameter);
  if(i == 3 || i == 4)
    color = CYAN;
  while(!digitalRead(button1));                      // Wait until button B1 released
  while(true){
  //  tft.fillScreen(BLACK);
    while(!digitalRead(button2)){                    // If button (pin #9) is pressed
      parameter++;
      if(i == 0 && parameter > 31)                   // If date > 31 ==> date = 1
        parameter = 1;
      if(i == 1 && parameter > 12)                   // If month > 12 ==> month = 1
        parameter = 1;
      if(i == 2 && parameter > 99)                   // If year > 99 ==> year = 0
        parameter = 0;
      if(i == 3 && parameter > 23)                   // If hours > 23 ==> hours = 0
        parameter = 0;
      if(i == 4 && parameter > 59)                   // If minutes > 59 ==> minutes = 0
        parameter = 0;
      sprintf(text,"%02u", parameter);
      draw_text(x_pos, y_pos, text, 4, color);
      delay(200); // Wait 200ms
     // tft.fillScreen(BLACK);
    }
    tft.fillRect(x_pos, y_pos, 57, 45, BLACK);  //BLACK
    blink_parameter();
    draw_text(x_pos, y_pos, text, 4, color);
    blink_parameter();
    if(!digitalRead(button1)){                       // If button (pin #8) is pressed
      i++;                                           // Increament 'i' for the next parameter
      return parameter;                              // Return parameter value and exit
     // tft.fillScreen(BLACK);
    }
  }
}
void draw_text(byte x_pos, byte y_pos, char *text, byte text_size, uint16_t text_color) {
  tft.setCursor(x_pos, y_pos);
  tft.setTextSize(text_size);
  tft.setTextColor(text_color, BLACK);
  tft.print(text);
}
 
void loop() {
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  if(!digitalRead(button1)){                         // If button B1 is pressed
    i = 0;
    while(!digitalRead(button1));                    // Wait for button B1 release
    while(true){
      while(!digitalRead(button2)){                  // While button B2 pressed
        day++;                                       // Increment day
        if(day > 7) day = 1;
        display_day();                               // Call display_day function
        delay(200);                                  // Wait 200 ms
      }
      tft.fillRect(139, 2, 200, 32, BLACK);     // Draw rectangle (earase day from the display)
      blink_parameter();                             // Call blink_parameter function
      display_day();                                 // Call display_day function
      blink_parameter();                             // Call blink_parameter function
      if(!digitalRead(button1))                      // If button B1 is pressed
        break;
    }
    date   = edit(20, 47, date);                      // Edit date
    month  = edit(90, 47, month);                    // Edit month
    year   = edit(210, 47, year);                    // Edit year
    hour   = edit(56, 121, hour);                     // Edit hours
    minute = edit(146, 121, minute);                   // Edit minutes
    // Convert decimal to BCD
    minute = ((minute / 10) << 4) + (minute % 10);
    hour = ((hour / 10) << 4) + (hour % 10);
    date = ((date / 10) << 4) + (date % 10);
    month = ((month / 10) << 4) + (month % 10);
    year = ((year / 10) << 4) + (year % 10);
    // End conversion
    // Write data to DS3231 RTC
    Wire.beginTransmission(0x68);               // Start I2C protocol with DS3231 address
    Wire.write(0);                              // Send register address
    Wire.write(0);                              // Reset sesonds and start oscillator
    Wire.write(minute);                         // Write minute
    Wire.write(hour);                           // Write hour
    Wire.write(day);                            // Write day
    Wire.write(date);                           // Write date
    Wire.write(month);                          // Write month
    Wire.write(year);                           // Write year
    Wire.endTransmission();                     // Stop transmission and release the I2C bus
    delay(200);                                 // Wait 200ms
  }
  Wire.beginTransmission(0x68);                 // Start I2C protocol with DS3231 address
  Wire.write(0);                                // Send register address
  Wire.endTransmission(false);                  // I2C restart
  Wire.requestFrom(0x68, 7);                    // Request 7 bytes from DS3231 and release I2C bus at end of reading
  second = Wire.read();                         // Read seconds from register 0
  minute = Wire.read();                         // Read minuts from register 1
  hour   = Wire.read();                         // Read hour from register 2
  day    = Wire.read();                         // Read day from register 3
  date   = Wire.read();                         // Read date from register 4
  month  = Wire.read();                         // Read month from register 5
  year   = Wire.read();                         // Read year from register 6
  Wire.beginTransmission(0x68);                 // Start I2C protocol with DS3231 address
  Wire.write(0x11);                             // Send register address
  Wire.endTransmission(false);                  // I2C restart
  Wire.requestFrom(0x68, 2);                    // Request 2 bytes from DS3231 and release I2C bus at end of reading
 // temperature_msb = Wire.read();                // Read temperature MSB
 // temperature_lsb = Wire.read();                // Read temperature LSB

  temperature_msb = t;                // Read temperature MSB
  temperature_lsb = t;                // Read temperature LSB
  
  display_day();
  DS3231_display();                             // Diaplay time & calendar
  delay(50);                                    // Wait 50ms 
}
// End of code.
