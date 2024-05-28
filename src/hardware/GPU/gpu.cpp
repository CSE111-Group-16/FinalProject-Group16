#include "gpu.h"
#include "../../processes/os.h"

int GPU::getPixelAddress(int width, int height) {
    int pixel_index = width + (height * 128);
    int pixel_offset = 1 * pixel_index;
    return 0x3400 + pixel_offset;
}

void GPU::loopIter() {
    (*os).logger << "GPU: decoding frame buffer" << std::endl;
    void decodeFrameBuffer();
    (*os).logger << "GPU: displaying frame buffer" << std::endl;
    void displayFrameBuffer();
}