// #define U8G2_USE_LARGE_FONTS
// #include <U8g2lib.h>
#include <Arduino_GFX_Library.h>

#define ESP32_1732S019 // set display type

#if defined(ESP32_1732S019)
#define GFX_BL 14 // DF_GFX_BL // default backlight pin
Arduino_DataBus *bus = new Arduino_ESP32SPI(
    11 /* DC */, 10 /* CS */, 12 /* SCK */,
    13 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
Arduino_GFX *gfx = new Arduino_ST7789(
    bus, 1 /* RST */, 1 /* rotation */,
    true /* IPS */, 170 /* width */, 320 /* height */,
    35 /* col offset 1 */, 0 /* row offset 1 */,
    35 /* col offset 2 */, 0 /* row offset 2 */);
#endif // ESP32-1732S019

/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

Arduino_Vtemu_Indexed * vt = NULL;
// Arduino_Canvas_Indexed * vt = NULL;
Arduino_Canvas_Indexed * frcnt = NULL;

void setup(void) {
  Serial.begin(115200);
  Serial.setDebugOutput(false);

#ifdef GFX_EXTRA_PRE_INIT
  GFX_EXTRA_PRE_INIT();
#endif

  // Init Display
  if (!gfx->begin()) {
    Serial.println("gfx->begin() failed!");
  } else {
    Serial.println("gfx->begin()");
  }
  gfx->fillScreen(WHITE);

  uint16_t w = (gfx->width()/6)*6;
  uint16_t h = ((gfx->height()-16)/8)*8;

  vt = new Arduino_Vtemu_Indexed( w /* width */, h /* height */, gfx, (gfx->width()-w)/2, (gfx->height()-h-16)/2);
  if (!vt->begin()) {
     Serial.println("vt->begin() failed!");
  } else {
     Serial.println("vt->begin()");
  }
  vt->fillScreen(NAVY);
  vt->setCursor(0,0);
  vt->setFont(NULL);
  vt->setTextColor(GREEN,NAVY);
  vt->flush();
  frcnt = new Arduino_Canvas_Indexed(136,16,gfx,gfx->width()-132,gfx->height()-16);
  if (!frcnt->begin()) {
     Serial.println("frcnt->begin() failed!");
  } else {
     Serial.println("frcnt->begin()");
  }
  frcnt->fillScreen(BLACK);
  frcnt->setTextColor(BLUE,BLACK);
  frcnt->setTextSize(2);
  frcnt->flush();

#ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
#endif
  Serial.println("ready!");
}

void serialEvent() {
  int16_t x1, y1;
  uint16_t w, h;

  while (Serial.available()) {
    uint8_t c = Serial.read();
    vt->write(c);
  }
}

uint32_t mtime=0;

void loop()
{
  if (millis()>mtime+100) { 
    uint16_t ftime=10000/(millis()-mtime);
    frcnt->setCursor(2,1);
    frcnt->printf("%2d,%1d Frames",ftime/10,ftime%10);
    mtime=millis();
    frcnt->flush();
    vt->flush();
  }
}