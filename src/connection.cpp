#include <fstream>
#include <nlohmann/json.hpp>
#include <blpapi_authoptions.h>
#include <blpapi_correlationid.h>
#include <blpapi_identity.h>
#include <blpapi_sessionoptions.h>
#include <blpapi_tlsoptions.h>
#include "blpconn.h"


static const char *module_name = "ConnectionService";

/**
 * Check is configuration file exists
 * @param configPath Path to the configuration file
 * @return true if the file exists, false otherwise
 */
static bool fileExists(const std::string& configPath) {
    std::ifstream file(configPath);
    return file.good();
}

static json readConfiguration(std::string& config_path) {
    std::ifstream file(config_path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + config_path);
    }
    json config;
    try {
        file >> config;
    } catch (const json::parse_error& e) {
        throw std::runtime_error("Failed to parse JSON format in " + config_path);
    }
    return config;
}

/**
 * Defines the TLS options for the session.
 * This function retrieves the client certificate path,
 * client certificate password, and root certificate path
 * from environment variables and creates a TlsOptions object.
 */
static blpapi::TlsOptions defineTlsOptions(const json& config) {
    std::string client_cert_path = config["client_certificate"];
    std::string client_cert_password = config["password"];
    std::string root_cert_path = config["root_certificate"];
    blpapi::TlsOptions tlsOptions = blpapi::TlsOptions::createFromFiles(
        client_cert_path.c_str(),
        client_cert_password.c_str(),
        root_cert_path.c_str()
    );
    return tlsOptions;
}

/**
 * Defines the session options for the BLPAPI session.
 * This function retrieves the primary and secondary host addresses
 * and port from environment variables, sets the TLS options,
 * and configures the session for auto-restart on disconnection.
 */
static blpapi::SessionOptions defineSessionOptions(const json& config) {
    // Network connection parameters
    std::string host_primary = config["primary_host"];
    std::string host_secondary = config["secondary_host"];
    int port = config["port"].get<int>();

    blpapi::SessionOptions sessionOptions;
    sessionOptions.setServerAddress(host_primary.c_str(), port, 0);
    sessionOptions.setServerAddress(host_secondary.c_str(), port, 1);

    // TLS parameters
    sessionOptions.setTlsOptions(defineTlsOptions(config));

    // Authentication Options
    std::string appName = config["app_name"];
    std::string authOptions = "AuthenticationType=Application;ApplicationAuthenticationType=APPNAME_AND_KEY;ApplicationName=" + appName;
    sessionOptions.setAuthenticationOptions(authOptions.c_str());

    // Runtime parameters
    sessionOptions.setAutoRestartOnDisconnection(true);
    sessionOptions.setNumStartAttempts(3);

    // Other
    std::string default_service = config["default_service"];
    sessionOptions.setDefaultServices(default_service.c_str());
    sessionOptions.setDefaultSubscriptionService(default_service.c_str());
    return sessionOptions;
}


namespace BlpConn {

bool Context::initializeService(std::string& config_path) {
    json j = newLogMessage(module_name);
    if (!fileExists(config_path)) {
        j["result"] = "error";
        j["message"] = "Configuration file doesn't exist";
        logger_.log(j.dump());  
        return false;
    }
    json config;
    try {
        config = readConfiguration(config_path);
    } catch (const std::runtime_error& e) {
        j["result"] = "error";
        j["message"] = e.what();
        logger_.log(j.dump());  
        return false;
    }
    blpapi::SessionOptions session_options;
    try {
        session_options = defineSessionOptions(config);
    } catch (const std::exception& e) {
        j["result"] = "error";
        j["message"] = "Invalid configuration options: " + std::string(e.what());
        logger_.log(j.dump());
        return false;
    }
    service_ = config["default_service"];
    session_ = new blpapi::Session(session_options, new EventHandler());
    if (!session_->start()) {
        j["result"] = "error";
        j["message"] = "Failed to start session";
        logger_.log(j.dump());
        return false;
    }
    if (!session_->openService(session_options.defaultSubscriptionService())) {
        j["result"] = "error";
        j["message"] = "Failed to open service: " + std::string(session_options.defaultSubscriptionService());
        logger_.log(j.dump());
        return false;
    }
    j["result"] = "success";
    j["message"] = "Service initialized successfully";
    return true;
}

void Context::shutdownService() {
    json j = newLogMessage(module_name);
    if (session_) {
        session_->stop();
        delete session_;
        session_ = nullptr;
        j["result"] = "success";
        j["message"] = "Service shutdown successfully";
    } else {
        j["result"] = "error";
        j["message"] = "Service is not running";
    }
    logger_.log(j.dump());
}

}
