// *************************************************************
// Code and Variables for Controlling Display
// *************************************************************
// 0 <= nx < 20
// 0 <= ny <  3
// *************************************************************

#include <U8g2lib.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

uint8_t selfCurs_nx, selfCurs_ny;
uint8_t selfCurs_on;

void selfSetup() {
  u8g2.begin();
  u8g2.setDisplayRotation(U8G2_R0);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_mf);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
  u8g2.drawFrame(0,0, 128, 64);
  u8g2.sendBuffer();

  selfCurs_on = 0;
}

void selfClrScrn() {
  u8g2.clearBuffer();
  u8g2.drawFrame(0,0, 128, 64);
  selfCurs_on = 0;
}

void selfDraw() {
  u8g2.sendBuffer();
}

void selfSetCurs(uint8_t nx, uint8_t ny) {
  if (selfCurs_on) {
    u8g2.setDrawColor(0);
    u8g2.drawFrame(4+selfCurs_nx*6-1, 2+selfCurs_ny*10-2, 7, 12);
    u8g2.setDrawColor(1);
  }

  selfCurs_nx = nx;
  selfCurs_ny = ny;
  u8g2.drawFrame(4+selfCurs_nx*6-1, 2+selfCurs_ny*10-2, 7, 12);

  selfCurs_on = 1;
}

void selfDelCurs() {
  if (selfCurs_on) {
    u8g2.setDrawColor(0);
    u8g2.drawFrame(4+selfCurs_nx*6-1, 2+selfCurs_ny*10-2, 7, 12);
    u8g2.setDrawColor(1);
    selfCurs_on = 0;
  }
}

void selfText(uint8_t nx, uint8_t ny, char *jstr) {
  u8g2.drawStr( 4+nx*6,  2+ny*10, jstr);
}

void selfTextN(uint8_t nx, uint8_t ny, char *jstr, uint8_t n) {
  char kstr[32];
  uint8_t jlen;

  strcpy(kstr, jstr);

  if (n > 20) {
    n = 20;
  }

  jlen = strlen(jstr);
  while (jlen < n) {
    kstr[jlen] = ' ';
    jlen++;
  }
  kstr[jlen] = '\0';

  u8g2.drawStr( 4+nx*6,  2+ny*10, kstr);
}
// *************************************************************

