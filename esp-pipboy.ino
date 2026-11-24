//TODO User_Setup.h in TFT_eSPI

// Tested on 3.5" Hosyond CYD board
#include <SPI.h>
#include <TFT_eSPI.h>
#include "FS.h"
#include "SD.h"
#include <ArduinoJson.h>
#include <AnimatedGIF.h>

#include "pipboyconfig.h"
#include "src/SDfunctions.h"
#include "GIFOffset.h"
#include "src/TFT_eSPI_QRGen/qrcode.h"

/* Images */
#include "images/INIT.h"
#include "images/STAT.h"
#include "images/DATA_1.h"
#include "images/TIME.h"
#include "images/Bottom_layer_2.h"
#include "images/Date.h"
#include "images/INV.h"

#define INIT INIT
#define TIME TIME
#define STAT STAT
#define DATA_1 DATA_1
#define INV INV

#define IN_STAT 25
#define IN_INV 26
#define IN_DATA 27
#define IN_TIME 32
#define IN_RADIO 33

const int MENU_STAT = 0;
const int MENU_INV = 1;
const int MENU_DATA = 2;
const int MENU_MAP = 3;
const int MENU_RADIO = 4;

#define Light_green 0x35C2
#define Dark_green 0x0261 
#define Time_color 0x04C0

//========================USEFUL VARIABLES=============================
const bool skip_intro = false;
const bool demo_mode = true;
const bool settings_wifi_enabled = false;
const char *ssid     = "SSID"; 
const char *password = "PASSWORD";
const bool settings_radio_enabled = false;
const bool settings_temp_enabled = false;
int UTC = 2; //Set your time zone ex: france = UTC+2
uint16_t notification_volume = 30;  
//Default player variables
PIPBOYCONFIG pipboyconf;

//=====================================================================
long demo_mode_lastswitch = -1;
//========================POSITIONING VARIABLES=============================
//Please don't change these values unless you know what you are doing, changing them can alter the way the screen is displayed
const int display_menu_y = 8;
const int display_menu_drop = 17;
const int display_menu_STAT = 70;
const int display_menu_INV = 140;
const int display_menu_DATA = 210;
const int display_menu_MAP = 300;
const int display_menu_RADIO = 370;

//STAT tier two menu
const int display_menu_STAT_menu_y = 32;
const int display_menu_STAT_STATUS = 40;
const int display_menu_STAT_SPECIAL = 130;
const int display_menu_STAT_PERKS = 240;
//=====================================================================
// Load GIF library
AnimatedGIF gif;

//========================DISPLAY VARIABLES=============================
TFT_eSPI tft = TFT_eSPI();
QRcode qrcode (&tft);

const byte RXD2 = 16;  // Connects to module's TX => 16
const byte TXD2 = 17;  // Connects to module's RX => 17


int menu = 0;
int prevmenu = -1;

void setup() {
  pinMode(IN_RADIO, INPUT_PULLUP);
  pinMode(IN_STAT, INPUT_PULLUP);
  pinMode(IN_DATA, INPUT_PULLUP);
  pinMode(IN_INV, INPUT_PULLUP);
  pinMode(IN_TIME, INPUT_PULLUP);
  Serial.begin(115200);
  Serial.println("Starting Setup");

  // Start the SPI for the touchscreen and init the touchscreen
  //touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  //touchscreen.begin(touchscreenSPI);
  // Set the Touchscreen rotation in landscape mode
  // 0 is landscape, 2 is flipped landscape
  //touchscreen.setRotation(0);

  //Start the Display
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  
  tft.setTextSize(1);
  tft.setTextColor(Light_green, TFT_BLACK);

  //Mount the SD card and load configs
  if(!SD.begin()){
    tft.drawString("SD Card Mount Failure. Is it in yet?", 10, 20, 4);
    tft.drawString("Booting with defaults", 10, 50, 4);
    delay(5000);
  }else if(SD.cardType() == CARD_NONE){
    tft.drawString("SD Card may be corrupted.", 10, 20, 4);
    while (true)delay(0);
  }else{
    Serial.println("SD card attached. Attempting to load config.");
    loadConfig(SD, "/pipboy.conf", pipboyconf);
    
  }
  gif.begin(BIG_ENDIAN_PIXELS);
  //Play the Intro graphics
  if (!skip_intro){
    if (gif.open((uint8_t *)INIT, sizeof(INIT), GIFDraw))
      {
        tft.startWrite(); // The TFT chip select is locked low
        while (gif.playFrame(true, NULL))
        {
          yield();
        }

        gif.close();
        tft.endWrite(); // Release TFT chip select for other SPI devices
      }
  }
  //Have to init the QR code library
  qrcode.init();
  qrcode.setBGColor(TFT_BLACK);
  qrcode.setPixelColor(Light_green);
  Serial.println("Finished Setup");
}

void loop(){
  if(prevmenu != menu){
    Serial.println("Menu switch detected");
    //Clear screen
    tft.fillScreen(TFT_BLACK);
    //Draw Menu Categories
    tft.setTextColor(Light_green);
    tft.setTextSize(2);
    tft.drawString("STAT",display_menu_STAT,display_menu_y, 1);
    tft.drawString("INV",display_menu_INV,display_menu_y, 1);
    tft.drawString("DATA",display_menu_DATA,display_menu_y, 1);
    tft.drawString("MAP",display_menu_MAP,display_menu_y, 1);
    tft.drawString("RADIO",display_menu_RADIO,display_menu_y, 1);
    tft.drawLine(0,display_menu_y + display_menu_drop,480,display_menu_y + display_menu_drop,Light_green);

    // Do Menu selected specific things
    if (menu == MENU_STAT) {
      Serial.println("Drawing STAT menu.");
      selectMenuItem(MENU_STAT);
      //Draw tier two menu
      tft.setTextSize(2);
      tft.drawString("STATUS",display_menu_STAT_STATUS,display_menu_STAT_menu_y, 1);
      tft.drawString("SPECIAL",display_menu_STAT_SPECIAL,display_menu_STAT_menu_y, 1);
      tft.drawString("PERKS",display_menu_STAT_PERKS,display_menu_STAT_menu_y, 1);
      //TODO draw character damage
      //Draw player info footer
      tft.fillRect(5,300,115,13,Dark_green);
      tft.fillRect(125,300,230,13,Dark_green);
      tft.fillRect(360,300,115,13,Dark_green);
      tft.setTextSize(1);
      tft.drawString("HP " + pipboyconf.player_hp + "/" + pipboyconf.player_hp_max, 10, 303);
      tft.drawString("Level " + pipboyconf.player_level, 130, 303);
      tft.drawString("AP " + pipboyconf.player_ap + "/" + pipboyconf.player_ap, 420, 303);
      //Draw XP bar
      tft.drawRect(200,302,150,9,Light_green);

      //Player name
      tft.drawString(pipboyconf.player_name, 210,280);
    }
    if (menu == MENU_INV) {
      Serial.println("Drawing INV menu.");
      selectMenuItem(MENU_INV);
      /*if (gif.open((uint8_t *)INV, sizeof(INV), GIFDraw)) {
        tft.startWrite();  // The TFT chip select is locked low
        while (gif.playFrame(true, NULL)) {
          yield();
        }
        gif.close();
        tft.endWrite();  // Release TFT chip select for other SPI devices
      }*/
    }
    if (menu == MENU_DATA) {
      Serial.println("Drawing DATA menu.");
      selectMenuItem(MENU_DATA);
      // create qrcode
      qrcode.create(pipboyconf.QRcode_URL);
    }
    if (menu == MENU_MAP) {
      Serial.println("Drawing MAP menu.");
      selectMenuItem(MENU_MAP);
    }
    if(menu == MENU_RADIO) {
      Serial.println("Drawing RADIO menu.");
      selectMenuItem(MENU_RADIO);
    }
    prevmenu = menu;
  }
  //Do ongoing menu activities
  if (menu == MENU_STAT) {
  //STATS GIF
    //TODO try sprites
      if (gif.open((uint8_t *)STAT, sizeof(STAT), GIFDraw)){
        tft.startWrite(); // The TFT chip select is locked low
         while (gif.playFrame(true, NULL))
        {
          yield();
        }
        gif.close();
        tft.endWrite(); // Release TFT chip select for other SPI devices
      }
  }
  //Do demo mode check
  long curtime = millis();
  if (demo_mode && curtime >= demo_mode_lastswitch +10000){
      demo_mode_lastswitch = curtime;
      menu = menu +1;
      if (menu > 4) menu = 0;
  }
  delay(5);           // let this time pass
}


void selectMenuItem(int menuItem){
  int menuItemstartx = 0;
  int menuItemWidth = 0;
  switch(menuItem){
    //case(MENU_STAT):
    case (MENU_INV):
      menuItemstartx = display_menu_INV;
      menuItemWidth = 33;
      break;
    case (MENU_DATA):
      menuItemstartx = display_menu_DATA;
      menuItemWidth = 44;
      break;
    case (MENU_MAP):
      menuItemstartx = display_menu_MAP;
      menuItemWidth = 33;
      break;
    case (MENU_RADIO):
      menuItemstartx = display_menu_RADIO;
      menuItemWidth = 55;
      break;
    //Default to STAT
    default:
      menuItemstartx = display_menu_STAT;
      menuItemWidth = 44;
      break;
  }
  //blackout the menu line under selected item
  tft.drawLine(menuItemstartx-10,display_menu_y + display_menu_drop,menuItemstartx+menuItemWidth+10,display_menu_y + display_menu_drop,TFT_BLACK);
  //draw menu selected
  tft.drawLine(menuItemstartx-10,display_menu_y + display_menu_drop,menuItemstartx-10,display_menu_y + display_menu_drop - 10,Light_green);
  tft.drawLine(menuItemstartx+menuItemWidth+10,display_menu_y + display_menu_drop,menuItemstartx+menuItemWidth+10,display_menu_y + display_menu_drop - 10,Light_green);
  tft.drawLine(menuItemstartx-10,display_menu_y + display_menu_drop - 10,menuItemstartx-10+5,display_menu_y + display_menu_drop - 10,Light_green);
  tft.drawLine(menuItemstartx+menuItemWidth+10,display_menu_y + display_menu_drop - 10,menuItemstartx+menuItemWidth+10-5,display_menu_y + display_menu_drop - 10,Light_green);
}
