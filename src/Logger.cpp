#include "open62541pp/Logger.h"

#include <cstdarg>  // va_list
#include <string>

#include "open62541pp/Client.h"
#include "open62541pp/Server.h"
#include "open62541pp/detail/open62541/common.h"

namespace opcua {

static void logImpl(UA_Logger& logger, LogLevel level, LogCategory category, std::string_view msg) {
    if (logger.log == nullptr) {
        return;
    }
    va_list args{};  // NOLINT
    logger.log(
        logger.context,
        static_cast<UA_LogLevel>(level),
        static_cast<UA_LogCategory>(category),
        std::string(msg).c_str(),
        args  // NOLINT
    );
}

void log(UA_Client* client, LogLevel level, LogCategory category, std::string_view msg) {
    auto* config = detail::getConfig(client);
    if (config != nullptr) {
        logImpl(config->logger, level, category, msg);
    }
}

void log(Client& client, LogLevel level, LogCategory category, std::string_view msg) {
    log(client.handle(), level, category, msg);
}

void log(UA_Server* server, LogLevel level, LogCategory category, std::string_view msg) {
    auto* config = detail::getConfig(server);
    if (config != nullptr) {
        logImpl(config->logger, level, category, msg);
    }
}

void log(Server& server, LogLevel level, LogCategory category, std::string_view msg) {
    log(server.handle(), level, category, msg);
}


std::string getLogLevelName(LogLevel level) {
    switch (level) {
        case LogLevel::Trace:       return "Trace";
        case LogLevel::Debug:       return "Debug";
        case LogLevel::Info:        return "Info";
        case LogLevel::Warning:     return "Warning";
        case LogLevel::Error:       return "Error";
        case LogLevel::Fatal:       return "Fatal";
        // Handle other cases if necessary
        default:                    return "Unknown";
    }
}

std::string getLogCategoryName(LogCategory category) {
    switch (category) {
        case LogCategory::Network:          return "Network";
        case LogCategory::SecureChannel:    return "SecureChannel";
        case LogCategory::Session:          return "Session";
        case LogCategory::Server:           return "Server";
        case LogCategory::Client:           return "Client";
        case LogCategory::Userland:         return "Userland";
        case LogCategory::SecurityPolicy:    return "SecurityPolicy";
        // Handle other cases if necessary
        default:                            return "Unknown";
    }
}
}  // namespace opcua
