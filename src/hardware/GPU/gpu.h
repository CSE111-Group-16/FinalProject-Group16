#pragma once
#include <memory>

class Console;
class GPU {
private:
    const size_t VRAM_SPACE_START_ADDRESS = 0x3400;
    const size_t VRAM_SPACE_END_ADDRESS = 0x6FFF;
    // helper functions
    bool decodePixel(uint8_t pixel_byte) const;
    uint8_t encodePixel(bool pixed_state) const;
    // internal gpu operations
    void displayFrameBuffer();
    void decodeFrameBuffer();
    void clearFrameBuffer();
public:
    const int DISPLAY_RESOLUTION_WIDTH = 128;
    const int DISPLAY_RESOLUTION_HEIGHT = 120;

    GPU(Console* set_os) : console(set_os) {};

    Console* console;
    size_t getPixelAddress(int width, int height) const;
    bool readPixel(int witdth, int height) const;
    void setPixel(int width, int height, bool state);
    void loopIter(); // function to be run every loop iteration
};