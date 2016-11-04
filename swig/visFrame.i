%module visFrame
%{
#include "visFrame.h"
%}

%include "typemaps.i"
%include "stdint.i"
%apply int64_t *OUTPUT { int64_t *result };
%apply int *OUTPUT { int *width, int *height };

%include "visFrame.h"
