#include "status.h"

char * status_str[] = {
    STATUS_LIST(STR)
};

char * status_to_str(status_code_t code)
{
    if (SC_CHECK_RANGE(code)) {
        return status_str[code];
    }

    return "unknown";
}
