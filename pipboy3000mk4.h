
#define MENU_STAT 0
#define MENU_INV 1
#define MENU_DATA 2
#define MENU_MAP 3
#define MENU_RADIO 4

#define Light_green 0x35C2
#define Dark_green 0x0261 
#define Time_color 0x04C0
//========================POSITIONING VARIABLES=============================
//Please don't change these values unless you know what you are doing, changing them can alter the way the screen is displayed
const int display_menu_y = 8;
const int display_menu_drop = 17;
const int display_menu_STAT = 70;
const int display_menu_INV = 140;
const int display_menu_DATA = 210;
const int display_menu_MAP = 300;
const int display_menu_RADIO = 370;
const int display_menu_tiertwo_y = 32;

//STAT tier two menu
const int display_menu_STAT_STATUS = 40;
const int display_menu_STAT_SPECIAL = 130;
const int display_menu_STAT_PERKS = 240;

//INV tier two menu
const int display_menu_INV_WEAPONS = 40;
const int display_menu_INV_APPAREL = 150;
const int display_menu_INV_AID = 260;


void selectMenuItem(TFT_eSPI tft, int menuItem){
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