This is a simple partial implementation of the pipboy interface for use on an ESP32 with a screen.
Tested device: ESP32-035 (3.5" variant of CYD)

You can pickup one of these boards really cheaply on Aliexpress

Key Features
* Moving Vaultboy on STAT screen
* User define QR code in the DATA screen
* Variable player attributes loaded from SD, allowing them to be edited without reprogramming the board.

===========================================================
Building your SD card
You must have a file called "pipboy.conf" in the root of your SD card directory if you want to set your own values. There is an example of the structure for this file in "SampleFiles".

===========================================================
To set up the development environment:
- Install Arduino IDE
- Under Tools > Boards > Board manager
	Install ESP32 by Espressif (v3.3.12 tested)
- Under Tools > board > esp32
    Select ESP32 Dev Module
- Set:
 - CPU Frequency: 240Mz (WiFi/BT)
 - artition Scheme: "No OTA (2MB APP/2MB FATFS)"
- Under Tools > Manage Libraries install
	TFT_eSPI
	AnimatedGIF
	ArduinoJSON (v7 onwards)


You will also need to copy the "User_Setup.h" file from the TFT_eSPI directory into your Arduino library installed TFT_eSPI folder. This will define the right parameters for the board. You can update this file if you are using a different board.


===========================================================
Image Conversion Tools
https://tomeko.net/online_tools/file_to_hex.php?lang=en
https://ezgif.com/
	
===========================================================

GIF location
Left 132 Top 63 Width 184 Height 206


