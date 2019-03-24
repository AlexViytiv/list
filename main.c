#include "stdio.h"

#include "./containers/list/list.h"
#include "./tools/logger.h"

int main(int argc, char * argv[])
{
    uint8_t i = 0;
    list_t * list = NULL;
    status_code_t status = SC_OK;

    LOG_INF("List initialization ...");
    status = list_init(&list, LINKING_DOUBLE);
    if (status != SC_OK) {
        LOG_ERR("Failed to init list. Error code: %u [%s].", status, status_to_str(status));
        goto out;
    }

    if (list == NULL) {
        LOG_DBG("List is NULL.");
    }

    for (i = 1; i <= 3; i++) {
        LOG_INF("Add item %d to the list end.", i);
        status = list_add_item(list, i, LAST_ITEM_INDEX);
        if (status != SC_OK) {
            LOG_ERR("Failed add number %d. Error code: %u [%s].", i, status, status_to_str(status));
            goto out;
        }
    }

    LOG_INF("Del item %d from the list.", 3);
    status = list_del_item(list, 3);
    if (status != SC_OK) {
        LOG_ERR("Failed to delete item. Error code: %u [%s].", status, status_to_str(status));
        goto out;
    }

    status = list_print(list);
    if (status != SC_OK) {
        LOG_ERR("Failed to print list. Error code: %u [%s].", status, status_to_str(status));
        goto out;
    }


out:
    return (int)status;
}