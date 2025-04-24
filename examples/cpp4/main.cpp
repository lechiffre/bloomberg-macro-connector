#include <blpapi_authoptions.h>
#include <blpapi_correlationid.h>
#include <blpapi_identity.h>
#include <blpapi_sessionoptions.h>
#include <blpapi_tlsoptions.h>
#include <blpapi_session.h>
#include <iostream>
#include <thread>

#define SERVICE_GECO "//blp/globalecoref"
#define SERVICE_ECONDATA "//blp/economic-data"
#define SERVICE_ECONDATARESOLVE "//blp/economic-data-resolve"

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
    // TLS parameters
    sessionOptions.setTlsOptions(defineTlsOptions());
    // Authentication Options
    std::string appName = getEnvVar("APP_NAME");
    std::string authOptions = "AuthenticationType=Application;ApplicationAuthenticationType=APPNAME_AND_KEY;ApplicationName=" + appName;
    sessionOptions.setAuthenticationOptions(authOptions.c_str());
    // Runtime parameters
    sessionOptions.setAutoRestartOnDisconnection(true);
    sessionOptions.setNumStartAttempts(3);
    // sessionOptions.setClientMode(blpapi::SessionOptions::ClientMode::SAPI);
    // Other
    sessionOptions.setDefaultServices("//blp/economic-data");
    sessionOptions.setDefaultSubscriptionService("//blp/economic-data");
    // sessionOptions.setDefaultTopicPrefix("/indicator/");
    std::cout << "Session options: " << sessionOptions << std::endl;
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

blpapi::Service openService(blpapi::Session *session, std::string service_id) {
    if (!session->openService(service_id.c_str())) {
        throw std::runtime_error("Failed to open service: " + service_id);
    
    }
    blpapi::Service service = session->getService(SERVICE_ECONDATA);
    if (!service.isValid()) {
        throw std::runtime_error("Failed to get service: " + service_id);
    }
    return service;
}

void getTickersList(blpapi::Session *session) {
    auto service = openService(session, SERVICE_GECO);
    blpapi::Request request = service.createRequest("GetTickersRequest");
    blpapi::CorrelationId corrId = session->sendRequest(request);
    std::cout << "Request sent with correlation ID: " << corrId << std::endl;
}

void getEco(blpapi::Session *session) {
    auto service = openService(session, SERVICE_ECONDATA);
    blpapi::Request request = service.createRequest("ReferenceDataRequest");
    blpapi::CorrelationId corrId = session->sendRequest(request);
    std::cout << "Request sent with correlation ID: " << corrId << std::endl;
}


void getEconomicData(blpapi::Session *session) {
    auto service = openService(session, SERVICE_ECONDATA);
    // std::cout << service << std::endl;
    blpapi::SubscriptionList sub;
    // sub.add("ECFC");
    // sub.add("/releasecalendar");
    sub.add(
        "USURTOT Index",
        "LAST_PRICE,RELEASE_DATE,SURVEY_MEDIAN,PREVIOUS_RELEASE",
        "",
        blpapi::CorrelationId(24)
    );
    session->subscribe(sub);
}

 
int main() {
    try {
        blpapi::Session *session = createSession();
        if (!session->start()) {
            throw std::runtime_error("Failed to start session");
        }
        std::cout << "Successfully connected to B-PIPE" << std::endl;
        /**** TESTING FEATURES ***/
        // getTickersList(session);
        getEconomicData(session);
        // getEco(session);
        /*** TESTING FEATURES ***/
        std::this_thread::sleep_for(std::chrono::seconds(5));
        session->stop();
        delete session;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
