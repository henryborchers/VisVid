//
// Created by Borchers, Henry Samuel on 2/26/21.
//
#include <iostream>
#include <sstream>
extern "C"{
#include <visvid/visVersion.h>
}
#include "utils.h"
std::string get_version(){
    std::ostringstream version;
    version << VERSION_VIS_MAJOR << "." << VERSION_VIS_MINOR;
    return version.str();
}