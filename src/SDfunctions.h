#include "FS.h"
#include "SD.h"
#include "SPI.h"

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.path(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void loadConfig(fs::FS &fs, const char * path, PIPBOYCONFIG &pipboyconf){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open config file for reading");
        return;
    }
	//Declare json document
    JsonDocument jsonDoc;
	// Deserialize the json from file
	DeserializationError error = deserializeJson(jsonDoc, file);

	if (error){
		Serial.println("Failed to read file, using default configuration");
	}else{
		//================System======================
        pipboyconf.debug = jsonDoc["debug"] | false;
        pipboyconf.demo_mode = jsonDoc["demo_mode"] | false;
        //================STAT========================
		pipboyconf.player_name = jsonDoc["player_name"] | "Goldstein";
		pipboyconf.player_level = jsonDoc["player_level"] | "30";
		pipboyconf.player_exp = jsonDoc["player_exp"].as<int>() | 70;
		pipboyconf.player_hp = jsonDoc["player_hp"] | "212";
		pipboyconf.player_hp_max = jsonDoc["player_hp_max"] | "212";
		pipboyconf.player_ap = jsonDoc["player_ap"] | "70";
		pipboyconf.recalibrate_tft = jsonDoc["recalibrate_tft"] | false;
        //================DATA========================
        pipboyconf.demo_mode = jsonDoc["QRcode_URL"] | "https://www.instagram.com/sam_shute/";
	}
    file.close();
}

/*void saveConfig(fs::FS &fs, const char * path, PIPBOYCONFIG &pipboyconf){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open config file for reading");
        return;
    }
	serializeJson(doc, file);
	file.close();
}*/
