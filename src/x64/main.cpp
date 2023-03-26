
#include <iostream>
#include "ui/manager.hpp"


u8g2_t local_lcd;

class SDL_U8G2 : public U8G2 {
public: SDL_U8G2(): U8G2() {
        u8g2 = local_lcd;
        u8g2_SetupBuffer_SDL_128x64(&u8g2, &u8g2_cb_r0);
        u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
        u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);
    }
};

SDL_U8G2 u8g2_lcd;

UI::Manager ui(reinterpret_cast<U8G2 &>(u8g2_lcd));

int main(void)
{
    std::cout << "Starting" << std::endl;
    ui.init();
    std::cout << "Initialized" << std::endl;
    ui.loop();
    std::cout << "Finished" << std::endl;
}
