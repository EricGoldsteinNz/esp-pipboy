#ifndef STRUCT_PIPBOYCONF
#define STRUCT_PIPBOYCONF

typedef struct pipboyconfig
{
  //================System======================
  bool recalibrate_tft = false;
  bool debug = false;
  bool demo_mode = false;
  //================STAT========================
  String player_name = "Goldstein";
  String player_level = "30";
  int player_exp = 70;
  String player_hp = "212";
  String player_hp_max = "212";
  String player_ap = "70";
  //================INV=========================
  //================DATA========================
  String QRcode_URL = "https://www.instagram.com/sam_shute/";
  //================MAP=========================
  //================RADIO=======================
  

} PIPBOYCONFIG;


#endif