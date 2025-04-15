#include <string>
#include "eventhandler.h"

bool TickersEventHandler::processEvent(const Event& event, Session *session) {
    switch (event.eventType()) {

        case Event::SESSION_STATUS: {
            MessageIterator msgIter(event);
            while (msgIter.next()) {
                Message msg = msgIter.message();
                std::cout << "session_status: " << msg.messageType() << std::endl;
                if (msg.messageType() == Name("SessionStarted")) {
                    Identity identity = session->createIdentity();
                    Service authService = session->getService("//blp//apiauth");
                    Request request = authService.createAuthorizationRequest();
                    request.set(Name("ApplicationId"), "example");
                    session->sendAuthorizationRequest(request, &identity);
                    session->openServiceAsync("//blp/globalecoref", CorrelationId(99));
                } else if (msg.messageType() == Name("SessionTerminated") ||
                           msg.messageType() == Name("SessionStartupFailure")) {
                    // Handle session termination
                }
            }
            break;
        }

        case Event::SERVICE_STATUS: {
            MessageIterator msgIter(event);
            while (msgIter.next()) {
                Message msg = msgIter.message();
                std::cout << "service_status: " << msg.messageType() << std::endl;
                if (msg.messageType() == Name("ServiceOpened") && msg.correlationId() == CorrelationId(99)) {
                    Service refService = session->getService("//blp/globalecoref");
                    Request request = refService.createRequest("GetTickersRequest");
                    Identity identity = session->createIdentity();
                    session->sendRequest(request, identity, CorrelationId(100));
                } else if (msg.messageType() == Name("ServiceOpenFailure")) {
                    // Handle service open failure
                }
            }
            break;
        }

        case Event::RESPONSE:
        case Event::PARTIAL_RESPONSE: {
            MessageIterator msgIter(event);
            while (msgIter.next()) {
                Message msg = msgIter.message();
                std::cout << "message: " << msg.messageType() << std::endl;
                if (msg.messageType() == Name("GetTickersResponse") && msg.correlationId() == CorrelationId(100)) {
                    Element tickerArray = msg.getElement(Name("Ticker"));
                    int numTickers = tickerArray.numValues();
                    std::cout << "Received " << numTickers << " tickers:" << std::endl;
                    for (int i = 0; i < numTickers; ++i) {
                        Element tickerElement = tickerArray.getValueAsElement(i);
                        std::string ticker = tickerElement.getElementAsString(Name("Ticker"));
                        std::string figi = tickerElement.getElementAsString(Name("FIGI"));
                        std::string description = tickerElement.getElementAsString(Name("Description"));
                        std::cout << "  Ticker: " << ticker << ", FIGI: " << figi << ", Description: " << description << std::endl;
                    }
                } else if (msg.messageType() == Name("ErrorResponse")) {
                    Element errorCategory = msg.getElement(Name("ErrorCategory"));
                    Element errorDescription = msg.getElement(Name("ErrorDescription"));
                    std::cerr << "Error Response: Category = " << errorCategory.getValueAsString()
                              << ", Description = " << errorDescription.getValueAsString() << std::endl;
                }
            }
            break;
        }

        case Event::REQUEST_STATUS: {
            MessageIterator msgIter(event);
            while (msgIter.next()) {
                Message msg = msgIter.message();
                std::cerr << "REQUEST_STATUS: " << msg.messageType() << std::endl;
            }
            break;
        }

        default: {
            MessageIterator msgIter(event);
            while (msgIter.next()) {
                Message msg = msgIter.message();
                std::cout << "Unhandled Event: " << event.eventType() << " - " << msg.messageType() << std::endl;
            }
            break;
        }
    }
    return true;
}
