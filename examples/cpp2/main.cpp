#include <blpapi_authoptions.h>
#include <blpapi_correlationid.h>
#include <blpapi_identity.h>
#include <blpapi_sessionoptions.h>
#include <blpapi_tlsoptions.h>
#include <blpapi_session.h>
#include <iostream>
#include <thread>

#define SERVICE_GECO "//blp/globalecoref"
#define SERVICE_AUTH "//blp/apiauth"

using namespace BloombergLP;

/**
 * Retrieves the value of an environment variable.
 * 
 * @param name The name of the environment variable.
 * @return The value of the environment variable.
 * @throws std::runtime_error if the environment variable is not found.
 */
std::string getEnvVar(const std::string& name) {
    const char* value = std::getenv(name.c_str());
    if (!value) {
        throw std::runtime_error("environment variable not found: " + name);
    }
    return std::string(value);
}

class TickersEventHandler : public blpapi::EventHandler {
    public:
        bool processEvent(const blpapi::Event& event, blpapi::Session *session) override;
};

bool TickersEventHandler::processEvent(const blpapi::Event& event, blpapi::Session *session) {
    std::cout << "event type: " << event.eventType() << std::endl;
    blpapi::MessageIterator msgIter(event);
    while (msgIter.next()) {
        blpapi::Message msg = msgIter.message();
        std::cout << "service: " << msg.service() << std::endl
                  << "message type: " << msg.messageType() << std::endl
                  << "correlation id: " << msg.correlationId() << std::endl
                  << "number of elements: " << msg.numElements() << std::endl
                  << "status: " << msg.messageType() << std::endl
                  << "message: " << msg << std::endl;
    }
    return true;
}

/**
 * Defines the TLS options for the session.
 * This function retrieves the client certificate path,
 * client certificate password, and root certificate path
 * from environment variables and creates a TlsOptions object.
 */
blpapi::TlsOptions defineTlsOptions() {
    std::string clientCertPath = getEnvVar("CLIENT_CERTIFICATE");
    std::string clientCertPassword = getEnvVar("PASSWORD");
    std::string rootCertPath = getEnvVar("ROOT_CERTIFICATE");
    blpapi::TlsOptions tlsOptions = blpapi::TlsOptions::createFromFiles(
        clientCertPath.c_str(),
        clientCertPassword.c_str(),
        rootCertPath.c_str()
    );
    return tlsOptions;
}

/**
 * Defines the session options for the BLPAPI session.
 * This function retrieves the primary and secondary host addresses
 * and port from environment variables, sets the TLS options,
 * and configures the session for auto-restart on disconnection.
 */
blpapi::SessionOptions defineSessionOptions() {
    // Network connection parameters
    std::string hostPrimary = getEnvVar("PRIMARY_HOST");
    std::string hostSecondary = getEnvVar("SECONDARY_HOST");
    int port = std::stoi(getEnvVar("PORT"));
    blpapi::SessionOptions sessionOptions;
    sessionOptions.setServerAddress(hostPrimary.c_str(), port, 0);
    sessionOptions.setServerAddress(hostSecondary.c_str(), port, 1);
    // Authentication parameters
    sessionOptions.setTlsOptions(defineTlsOptions());
    // Runtime parameters
    sessionOptions.setAutoRestartOnDisconnection(true);
    sessionOptions.setNumStartAttempts(3);
    sessionOptions.setClientMode(blpapi::SessionOptions::ClientMode::SAPI);
    return sessionOptions;
}

/**
 * Creates a new BLPAPI session with the defined session options.
 * 
 * @return A pointer to the newly created BLPAPI session.
 */
blpapi::Session *createSession() {
    blpapi::SessionOptions sessionOptions = defineSessionOptions();
    return new blpapi::Session(sessionOptions, new TickersEventHandler());
}

blpapi::Identity getIdentity(blpapi::Session *session) {
    blpapi::Identity identity = session->createIdentity();
    if (!identity.isValid()) {
        throw std::runtime_error("Failed to create identity");
    }
    /*
    if (!identity.isAuthorized()) {
        throw std::runtime_error("Identity is not authorized");
    }
    */
    std::cout << "Seat type: " << identity.getSeatType() << std::endl;
    return identity;
}

void authorizeApp(blpapi::Session *session) {
    blpapi::Identity identity = getIdentity(session);
    if (!session->openService(SERVICE_AUTH)) {
        throw std::runtime_error("Failed to open service: " + std::string(SERVICE_AUTH));
    }
    blpapi::Service service = session->getService(SERVICE_AUTH);
    if (!service.isValid()){
        throw std::runtime_error("Failed to get service: " + std::string(SERVICE_AUTH));
    }
    /*
    if (!identity.isAuthorized(service)) {
        throw std::runtime_error("Identity is not authorized for service: " + std::string(SERVICE_AUTH));
    }
    */
    blpapi::Request request = service.createAuthorizationRequest();
    // Register the application
    request.set(blpapi::Name("ApplicationName"), "YourAppName");
    blpapi::CorrelationId corrId = session->sendAuthorizationRequest(request, &identity);
    std::cout << "Authorization request sent with correlation ID: " << corrId << std::endl;
}

void getTickers(blpapi::Session *session) {
    blpapi::Identity identity = getIdentity(session);
    if (!session->openService(SERVICE_GECO)) {
        throw std::runtime_error("Failed to open service: " + std::string(SERVICE_GECO));
    }
    blpapi::Service service = session->getService(SERVICE_GECO);
    if (!service.isValid()) {
        throw std::runtime_error("Failed to get service: " + std::string(SERVICE_GECO));
    }
    /*
    if (!identity.isAuthorized(service)) {
        throw std::runtime_error("Identity is not authorized for service: " + std::string(SERVICE_GECO));
    }*/
    blpapi::Request request = service.createRequest("GetTickersRequest");
    blpapi::CorrelationId corrId = session->sendRequest(request, identity);
    std::cout << "Request sent with correlation ID: " << corrId << std::endl;
}




int main() {
    try {
        blpapi::Session *session = createSession();
        if (!session->start()) {
            throw std::runtime_error("Failed to start session");
        }
        std::cout << "Successfully connected to B-PIPE" << std::endl;
        /**** TESTING FEATURES ***/
        getTickers(session);
        std::this_thread::sleep_for(std::chrono::seconds(5));
        /*** TESTING FEATURES ***/
        session->stop();
        delete session;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
