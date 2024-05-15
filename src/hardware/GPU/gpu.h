#pragma once

class OS;
class GPU {
public:
    GPU(OS* set_os) : os(set_os) {};

    OS* os;
    int getPixelAddress(int width, int height);
    void loopIter(); // function to be run every loop iteration

private:
    // internal gpu operations
    void decodeFrameBuffer();
    void displayFrameBuffer();
    void clearFrameBuffer();
};