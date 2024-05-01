#include "tester.h"

using namespace std;

// using seperate tester class for now
Tester::Tester() {
    std::string path = "../../local-rom/hws/hello_world1.slug";
    OS os;
    os.startup(path);
}

