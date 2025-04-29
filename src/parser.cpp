#include <sstream>
#include <blpconn.h>

void parseSequence(const blpapi::Element& elem, json j) {
    for (size_t i = 0; i < elem.numElements(); i++) {
        blpapi::Element child = elem.getElement(i);
        std::string name = child.name().string();
        std::cout << "Name: " << name << std::endl;
        parseElement(child, j);
        std::cout << j.dump() << std::endl;
    }
}

void parseComplex(const blpapi::Element& elem, json j) {
    if (elem.datatype() == BLPAPI_DATATYPE_SEQUENCE) {
        parseSequence(elem, j);
        return;
    } else if (elem.datatype() ==   BLPAPI_DATATYPE_CHOICE) {
        parseElement(elem.getChoice(), j);
        return;
    }
    std::cout << j.dump() << std::endl;
}

void parseArray(const blpapi::Element& elem, json j) {
    for (size_t i = 0; i < elem.numValues(); i++) {
        // TODO        
    } 
}

void parseScalar(const blpapi::Element& elem, json j, size_t pos = 0) {
    std::string name = elem.name().string();
    switch (elem.datatype()) {
        case BLPAPI_DATATYPE_BOOL:
            j[name] = elem.getValueAsBool();
            break;
        case BLPAPI_DATATYPE_CHAR:
            j[name] = elem.getValueAsChar();
            break;
        case BLPAPI_DATATYPE_BYTE:
        case BLPAPI_DATATYPE_INT32:
            j[name] = elem.getValueAsInt32();
            break;
        case BLPAPI_DATATYPE_INT64:
            j[name] = elem.getValueAsInt64();
            break;
        case BLPAPI_DATATYPE_FLOAT32:
            j[name] = elem.getValueAsFloat32();
            break;
        case BLPAPI_DATATYPE_FLOAT64:
        case BLPAPI_DATATYPE_DECIMAL:
            j[name] = elem.getValueAsFloat64();
            break;
        case BLPAPI_DATATYPE_STRING:
            std::cout << "String: " << elem.getValueAsString() << std::endl;
            j[name] = elem.getValueAsString();
            break;
        case BLPAPI_DATATYPE_DATE:
        case BLPAPI_DATATYPE_TIME:
        case BLPAPI_DATATYPE_DATETIME:
            std::ostringstream oss;
            oss << elem.getValueAsDatetime(pos);
            j[name] = oss.str();
            break;
    //     default:
            // TODO
         /*   
          BLPAPI_DATATYPE_BYTEARRAY = 9,
          BLPAPI_DATATYPE_ENUMERATION = 14,
          BLPAPI_DATATYPE_CORRELATION_ID = 17
          */
    }
    std::cout << j.dump() << std::endl;
}

void parseElement(const blpapi::Element& elem, json j) {
    std::cout << "Parse element: " << j.dump() << std::endl;
    if (elem.isComplexType()) {
        parseComplex(elem, j);
        return;
    } else if (elem.isArray()) {
        parseArray(elem, j);
        return;
    } else if (elem.isNull()) {
        // TODO
        return;
    }
    parseScalar(elem, j, 0); 
}


