//
// Created by henry on 3/2/21.
//

#include "visvid_exceptions.h"

PyVisVidException::PyVisVidException(const std::string &arg) : runtime_error(arg) {}
