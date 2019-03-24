#ifndef LOGGER_H
#define LOGGER_H

#define NRM "\e[0m"
#define RED(TEXT) "\e[31m" TEXT NRM
#define GRN(TEXT) "\e[32m" TEXT NRM
#define YLW(TEXT) "\e[33m" TEXT NRM
#define BLE(TEXT) "\e[34m" TEXT NRM

#define COLORIZE(COLOR, TEXT) \
        COLOR(TEXT)

#define LOG(FORMAT, ...) \
        printf( "[%30s:%-4d]"FORMAT"\n", __FILE__, __LINE__, ## __VA_ARGS__)

#define LOG_LEVEL(FORMAT, LEVEL, ...) \
        LOG(" | " LEVEL " | " FORMAT, ## __VA_ARGS__)

#define LOG_ERR(FORMAT, ...) \
        LOG_LEVEL(FORMAT, COLORIZE(RED, "ERR"), ## __VA_ARGS__)
#define LOG_INF(FORMAT, ...) \
        LOG_LEVEL(FORMAT, COLORIZE(YLW, "INF"), ## __VA_ARGS__)
#define LOG_DBG(FORMAT, ...) \
        LOG_LEVEL(FORMAT, COLORIZE(BLE, "DBG"), ## __VA_ARGS__)
#define LOG_SUC(FORMAT, ...) \
        LOG_LEVEL(FORMAT, COLORIZE(GRN, "SUC"), ## __VA_ARGS__)
#define LOG_MSG(FORMAT, ...) \
        LOG_LEVEL(FORMAT, "MSG", ## __VA_ARGS__)

#endif /* LOGGER_H */