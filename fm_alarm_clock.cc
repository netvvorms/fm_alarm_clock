#include <libavrc++.hh>

/* --- LCD ------------------------------------------------------------------ */
#include <components/lcd/glcd.hh>
#include <components/lcd/st7565.hh>
typedef gLCD< ST7565<pin4, pin2, RGBLed<pin3, pin5, pin6>, SPI<pin7> > > lcd;

/* --- RTC ------------------------------------------------------------------ */
#include <components/rtc/rtc.hh>
#include <components/rtc/ds3234.hh>
typedef RTC< DS3234<SPI0> > rtc;

/* --- Neo ------------------------------------------------------------------ */
#include <components/leds/neo_pixel.hh>
typedef NeoPixel<pin8, 60> neo;

/* --- Barometer ------------------------------------------------------------ */
#include <components/sensors/bmp085.hh>
typedef BMP085<> barometer;

/* -------------------------------------------------------------------------- */
#include<avr/pgmspace.h>

/* -------------------------------------------------------------------------- */
#include "clock.hh"
typedef AlarmClock<lcd> clk;

/* -------------------------------------------------------------------------- */

#define ls 10
#define lm 2
#define lh 1

void setup(void) {
  rtc::activate();
  //  DateTime dt(2014, 8, 18, 0, 31, 0, 1);
  //  rtc::adjust(dt);

  clk::activate();
  clk::printAlarm<3, 26>();

  lcd::print(2, 5, "Couleur 3 - 110.5 Mhz");

  barometer::activate();
  int32_t t = barometer::getTemperature();
  int32_t t0 = t/10;
  int32_t t1 = t - t0*10;
  lcd::printf(4, 38, "%2ld.%ld%cC", t0, t1, 176);

  int32_t p = barometer::getPressure();
  int32_t p0 = p/100;
  lcd::printf(4, 50, "%4ld hPa", p0);
  lcd::display();

  neo::activate();
  neo::setBrightness(20);
  neo::display();
}

void loop(void) {
  DateTime dt = rtc::now();

  lcd::print(64-8*6, 16, dt.date_str());

  clk::printTime<60, 37>(dt);


  uint8_t seconds = dt.second();
  uint8_t minutes = dt.minute();
  uint8_t hours   = dt.hour();

  if (seconds % 30 == 0) {
    int32_t t = barometer::getTemperature();
    int32_t t0 = t/10;
    int32_t t1 = t - t0*10;
    lcd::printf(4, 38, "%2ld.%ld%cC", t0, t1, 176);

    int32_t p = barometer::getPressure();
    int32_t p0 = p/100;
    lcd::printf(4, 50, "%4ld hPa", p0);
    lcd::display();
  }


  if(hours > 11) hours -= 12;
  hours = (hours*60 + minutes) / 12;  //each red dot represent 24 minutes.

  for(uint8_t i=0; i < 60; i++) {
    Color<_rgb> color(0, 0, 0);

    int16_t si = i - 1- seconds + ls;
    if(i > (60 - ls + seconds)) {
      si -= 60;
    }

    if (si <= ls) {
      int16_t val = 255 * si / ls;
      color.blue() = val > 0 ? val : 0;
    }

    int32_t mi = (i - 1 - minutes + lm);
    mi = mi > 0 ? 255 -255 * mi / lm : 255 * mi / lm + 255;
    color.green() = mi > 0 ? mi : 0;

    int32_t hi = (i - 1 - hours + lh);
    hi = hi > 0 ? 255 -255 * hi / lh : 255 * hi / lh + 255;
    color.red() = hi > 0 ? hi : 0;

    neo::setLEDColor((i + 30)%60, color);
  }
  neo::display();

  _delay_ms(1000);
}
