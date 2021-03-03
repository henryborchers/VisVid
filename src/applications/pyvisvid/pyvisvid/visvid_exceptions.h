//
// Created by henry on 3/2/21.
//

#ifndef LIBVISVID_VISVID_EXCEPTIONS_H
#define LIBVISVID_VISVID_EXCEPTIONS_H
#include <stdexcept>
class PyVisVidException: public std::runtime_error{
public:
    PyVisVidException(const std::string &arg);
};
#endif //LIBVISVID_VISVID_EXCEPTIONS_H
