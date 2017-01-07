//
// Created by Borchers, Henry Samuel on 1/7/17.
//

#ifndef LIBVISVID_VISVID_H
#define LIBVISVID_VISVID_H
#ifdef __GNUC__
#define DEPRECATED(func) func __attribute__ ((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED(func) __declspec(deprecated) func
	#else
	#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
	#define DEPRECATED(func) func
#endif

#include "visFrame.h"
#include "visBuffer.h"
#include "visImageWriter.h"
#include "visResult.h"
#include "visView.h"
#include "visFrame.h"
#include "visVisualization.h"
#include "visVersion.h"
#endif //LIBVISVID_VISVID_H
