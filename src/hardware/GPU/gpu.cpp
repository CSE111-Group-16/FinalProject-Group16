#include "gpu.h"
#include "../../processes/os.h"

/// @brief gets the boolean value of the specified pixel
/// @param pixel_byte refers to the byte representing the pixel
/// @returns True if pixel is white, False is pixel is black
bool GPU::decodePixel(uint8_t pixel_byte) const {
    return (pixel_byte & 0x01) == 0x01; // should be able to discard evrything as junk besides the LSB
}

uint8_t GPU::encodePixel(bool pixel_state) const {
    if (pixel_state)
        return 0x01;
    else
        return 0x00;
}

size_t GPU::getPixelAddress(int width, int height) const {
    int pixel_index = width + (height * DISPLAY_RESOLUTION_WIDTH);
    int pixel_offset = 1 * pixel_index;
    return VRAM_SPACE_START_ADDRESS + pixel_offset;
}

bool GPU::readPixel(int width, int height) const {
    size_t pixel_address = getPixelAddress(width, height);
    uint8_t pixel_byte = os->memory.readByte(pixel_address); // unset is black, set is white
    bool pixel_state = decodePixel(pixel_byte);
    return pixel_state;
}

void GPU::setPixel(int width, int height, bool state) {
    uint8_t pixel_state = encodePixel(state);
    size_t pixel_address = getPixelAddress(width, height);
    (*os).memory.setByte(pixel_address, pixel_state);
}

// maybe just will be in displayFrameBuffer()
void GPU::decodeFrameBuffer() {
    
}

void GPU::displayFrameBuffer() {
    for (int w=0; w<WINDOW_WIDTH; w++) {
        for (int h=0; h<WINDOW_HEIGHT; h++) {

        }
    }
    // TODO: draw on texture with sdl2 here

    // TODO: render with sdl2 after drawing texture
}


void GPU::loopIter() {
    // decodeFrameBuffer();
    (*os).logger << "GPU: displaying frame buffer" << std::endl;
    // void displayFrameBuffer();
}