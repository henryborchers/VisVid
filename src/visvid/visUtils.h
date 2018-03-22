//
// Created by Borchers, Henry Samuel on 1/7/17.
//

#ifndef LIBVISVID_VISUTILS_H
#define LIBVISVID_VISUTILS_H
#ifdef __GNUC__
#define DEPRECATED __attribute__ ((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED __declspec(deprecated) func
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED func
#endif
#endif //LIBVISVID_VISUTILS_H
