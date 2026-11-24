//#include <LovyanGFX.hpp>
#include <TFT_eSPI.h>

class QRcode
{
	private:
		TFT_eSPI *tft;
		void render(int x, int y, int color);

	public:
		QRcode(TFT_eSPI *display);
		void init();
		void create(String message);
		void create(String message, int x, int y);
		void setBGColor(uint32_t bgcolor);
		void setPixelColor (uint32_t pixelcolor);
};
