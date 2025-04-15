#include <cstdlib>
#include <string.h>

std::string getEnvVar(const std::string& name) {
    const char* value = std::getenv(name.c_str());
    if (!value) {
        std::cerr << "error: environment variable '" << name << "' not set." << std::endl;
        exit(1);
    }
    return std::string(value);
}

