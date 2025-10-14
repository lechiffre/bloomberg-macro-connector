#include <cstdio>
#include <readline/history.h>
#include <readline/readline.h>
#include <boost/algorithm/string.hpp>
#include "blpconn.h"
#include "blpconn_deserialize.h"

using namespace BlpConn;

enum {
    SUBSCRIBE,
    UNSUBSCRIBE
};

void eventRequest(Context& ctx, uint64_t corr, const std::string& rem, int action) {
    SubscriptionRequest request = {
        .topic = rem,
        .correlation_id = corr,
    };
    if (action == SUBSCRIBE) {
        ctx.subscribe(request);
    } else {
        ctx.unsubscribe(request);
    }
}

void processCommand(Context& ctx, const std::string& line) {
    size_t pos = line.find(' ');
    // Command
    if (pos == line.npos) {
        std::cout << "Invalidad command" << std::endl;
        return;
    }
    std::string cmd = line.substr(0, pos);
    boost::algorithm::trim(cmd);
    // Correlation ID
    std::string rem = line.substr(pos);
    boost::trim(rem);
    
    pos = rem.find(' ');
    if (pos == line.npos) {
        std::cout << "Invalidad event" << std::endl;
        return;
    }
    std::string corr = rem.substr(0, pos);
    boost::algorithm::trim(corr);
    std::string par = rem.substr(pos);
    boost::algorithm::trim(par);
    
    if (cmd == "subscribe") {
        eventRequest(ctx, std::stoi(corr), par, SUBSCRIBE); 
    } else {
        eventRequest(ctx, std::stoi(corr), par, UNSUBSCRIBE); 
    }
}

void run(Context& ctx) {
    char *input;
    // Setting line history parameters
    std::string history_path =
        std::string(std::getenv("HOME")) + "/.blpconn_history";
    using_history();
    read_history(history_path.c_str());
    stifle_history(1000);
    // Command loop
    while (true) {
        input = readline("> ");
        if (!input) {
            break;
        }
        std::string line(input);
        if (line.length() > 0) {
            add_history(input);
        }
        free(input);
        if (line == "quit") {
            break;
        }
        try {
            // Clear the current input line
            processCommand(ctx, line);
        } catch (int e) {
            // Clear the current input line
            std::cout << "Error processing line" << std::endl;
        }
        // Redisplay the prompt after output
        std::cout << "> " << std::flush;
    }
    write_history(history_path.c_str());
}

int main() {
    Context ctx;
    ctx.addNotificationHandler(defaultObserver);
    std::string config_path = "./config.json";
    ctx.initializeSession(config_path);
    run(ctx);     
}
