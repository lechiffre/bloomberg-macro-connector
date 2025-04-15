#include <blpapi_session.h>
#include <blpapi_sessionoptions.h>
#include <blpapi_tlsoptions.h>
#include <blpapi_service.h>
#include <blpapi_request.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <thread>
#include <chrono>
#include "eventhandler.h"

TickersEventHandler eventHandler;

std::string getEnvVar(const std::string& name) {
    const char* value = std::getenv(name.c_str());
    if (!value) {
        std::cerr << "error: environment variable '" << name << "' not set." << std::endl;
        exit(1);
    }
    return std::string(value);
}

BloombergLP::blpapi::Session* connectToBpipe() {
    std::cout << "Initializing connection to Bloomberg B-PIPE..." << std::endl;
    BloombergLP::blpapi::SessionOptions sessionOptions;
    std::string hostPrimary = getEnvVar("PRIMARY_HOST");
    std::string hostSecondary = getEnvVar("SECONDARY_HOST");
    int port = std::stoi(getEnvVar("PORT"));
    std::cout << "Setting up connection to primary host: " << hostPrimary << std::endl;
    sessionOptions.setServerAddress(hostPrimary.c_str(), port, 0);
    std::cout << "Setting up connection to secondary host: " << hostSecondary << std::endl;
    sessionOptions.setServerAddress(hostSecondary.c_str(), port, 1);
    std::string clientCertPath = getEnvVar("CLIENT_CERTIFICATE");
    std::string clientCertPassword = getEnvVar("PASSWORD");
    std::string rootCertPath = getEnvVar("ROOT_CERTIFICATE");
    std::cout << "Loading certificate files..." << std::endl;
    BloombergLP::blpapi::TlsOptions tlsOptions = 
        BloombergLP::blpapi::TlsOptions::createFromFiles(
            clientCertPath.c_str(),
            clientCertPassword.c_str(),
            rootCertPath.c_str()
        );
    sessionOptions.setTlsOptions(tlsOptions);
    // sessionOptions.setClientMode(BloombergLP::blpapi::SessionOptions::ClientMode::SAPI);
    sessionOptions.setAutoRestartOnDisconnection(true);
    sessionOptions.setNumStartAttempts(3);
    BloombergLP::blpapi::Session* session = new BloombergLP::blpapi::Session(sessionOptions, &eventHandler);
    if (!session->start()) {
        std::cerr << "Failed to start session" << std::endl;
        delete session;
        return nullptr;
    }
    std::cout << "Successfully connected to B-PIPE" << std::endl;
    return session;
}

int main() {
    BloombergLP::blpapi::Session* session = connectToBpipe();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    session->stop();
    return 0;
}
