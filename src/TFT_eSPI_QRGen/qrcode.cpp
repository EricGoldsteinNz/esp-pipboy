#include <Arduino.h>
#include "qrcode.h"
#include "qrencode.h"

int offsetsX;
int offsetsY;
int screenwidth;
int screenheight;
int multiply=4;
uint32_t _BGColor;
uint32_t _PixelColor;

QRcode::QRcode(TFT_eSPI *tft){
  this->tft = tft;
}

void QRcode::init(){
    //((TFT_eSPI *)tft)->init();
    screenwidth = tft->width();
    screenheight = tft->height(); 
    //tft->setRotation(1);
    //tft->fillScreen(TFT_WHITE);
    int min = screenwidth;
    if (screenheight<screenwidth)
      min = screenheight;
    //multiply = min/WD;
    offsetsX = (screenwidth-(WD*multiply))/2;
    offsetsY = (screenheight-(WD*multiply))/2;
  //Set default colors
  _BGColor = TFT_WHITE;
  _PixelColor = TFT_BLACK;
}

void QRcode::render(int x, int y, int color){
  x=(x*multiply)+offsetsX;
  y=(y*multiply)+offsetsY;
  for (int multx = 0; multx<multiply;multx++){
    for (int multy = 0; multy<multiply;multy++){
      tft->drawPixel(x + multx,y + multy,color);
    }
  }
}

void QRcode::create(String message) {
  // create QR code
  create(message, (screenwidth-(WD*multiply))/2, (screenheight-(WD*multiply))/2);
}

void QRcode::create(String message, int x, int y){
  offsetsX = x;
  offsetsY = y;
  // create QR code
  message.toCharArray((char *)strinbuf,260);
  qrencode();
  // print QR Code
  for (byte WDx = 0; WDx < WD; WDx+=2) {
    for (byte WDy = 0; WDy < WD; WDy++) {
      render (WDx,WDy, QRBIT(WDx,WDy)?_PixelColor : _BGColor);
      render((WDx+1), WDy, QRBIT(WDx+1,WDy)?_PixelColor : _BGColor);
    }
  }
}

void QRcode::setBGColor(uint32_t bgcolor){
  _BGColor = bgcolor;
}

void QRcode::setPixelColor(uint32_t pixelcolor){
  _PixelColor = pixelcolor;
}
