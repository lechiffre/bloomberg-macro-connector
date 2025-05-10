%module blpconngo

%{
// #include <string>
// #include <vector>
#include <stdint.h>
#include "blpconn_observer.h"
#include "blpconn.h"
%}

%include "exception.i"
%include "stdint.i"
%include "std_string.i"
%include "std_vector.i"

%exception {
    try {
        $action
    } catch (const std::exception& e) {
        SWIG_exception(SWIG_RuntimeError, e.what());
    } catch (...) {
        SWIG_exception(SWIG_RuntimeError, "Unknown exception");
    }
}

namespace BloombergLP {}

%include "blpconn_observer.h"
%include "blpconn.h"
