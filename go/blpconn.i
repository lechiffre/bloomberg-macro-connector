%module blpconngo

%{
#include <string>
#include <vector>
#include "blpapi_session.h"
#include "blpconn.h"
#include "blpapi_session.h" // Include the header for blpapi::EventHandler
%}

%include "exception.i"
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

%rename(BlpapiEventHandler) BloombergLP::blpapi::EventHandler;

namespace BloombergLP::blpapi {
    class EventHandler {
        virtual ~EventHandler() {}
        virtual void processEvent(const Event& event, Session* session) = 0;
    };
}
%include <std_string.i>
%include "blpconn.h"
