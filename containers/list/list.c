#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "list.h"
#include "../../tools/logger.h"


#define LAST_ITEM_INDEX (-1)


status_code_t list_init(list_t ** list,
                        linking_type_t type)
{
    status_code_t status = SC_OK;

    if ((*list) != NULL) {
        status = SC_PTR_NOT_NULL;
        LOG_ERR("List pointer already initialized.");
        goto out;
    }

    if (type != LINKING_DOUBLE) {
        status = SC_UNSUPPORTED;
        LOG_ERR("List type %d unsupported.", type);
        goto out;
    }

    (*list) = (list_t *)malloc(sizeof(list_t));
    if ((*list) == NULL) {
        status = SC_PTR_NULL;
        LOG_ERR("List allocation failed.");
        goto out;
    }

    (*list)->type = type;
    (*list)->len  = 0;
    (*list)->end  = NULL;
    (*list)->head = NULL;

out:
    if (status != SC_OK) {
        if (list != NULL) {
            LOG_ERR("List pointer freed.");
            free(list);
        }
    } else {
        LOG_SUC("List initialized.");
    }

    return status;
}

status_code_t list_deinit(list_t * list)
{
    status_code_t status = SC_OK;

    if (list == NULL) {
        status = SC_PTR_NULL;
        LOG_ERR("List pointer not initialized.");
        goto out;
    }

    if (list->len > 0) {
        status = SC_RES_IN_USE;
        LOG_ERR("List is in use.");
        goto out;
    }

    free(list);
    LOG_SUC("List uninitialized.");
out:
    return status;
}

status_code_t list_add_item(list_t * list,
                            uint32_t value,
                            uint32_t index)
{
    status_code_t status = SC_OK;
    list_item_t * new_item = NULL;
    list_item_t * curr_item = NULL;
    list_item_t * prev_item = NULL;

    if (list == NULL) {
        status = SC_PTR_NULL;
        LOG_ERR("List pointer not initialized.");
        goto out;
    }

    new_item = (list_item_t *)malloc(sizeof(list_item_t));
    if (new_item == NULL) {
        status = SC_PTR_NULL;
        LOG_ERR("List item allocation falied.");
        goto out;
    }
    memset(new_item, 0, sizeof(*new_item));
    new_item->value = value;

    if (list->len == 0) {
        list->head = new_item;
        list->end  = new_item;
        goto out;
    }

    if (index == 0) {
        new_item->next = list->head;
        list->head = new_item;
    } else if (index == LAST_ITEM_INDEX) {
        list->end->next = new_item;
        new_item->prev = list->end;
        list->end = new_item;
    } else {
        curr_item = list->head;
        while(index-- > 0){
            prev_item = curr_item;
            curr_item = curr_item->next;
            if (curr_item == NULL) {
                status = SC_PTR_NULL;
                LOG_ERR("List position not found.");
                goto out;
            }
        }

        prev_item->next = new_item;
        new_item->next = curr_item;
    }

out:
    if (status == SC_OK) {
        list->len++;
        LOG_SUC("List item added.");
    } else {
        if (new_item != NULL) {
            free(new_item);
        }
    }
    return status;
}

status_code_t list_del_item(list_t * list,
                            uint32_t value)
{
    status_code_t status = SC_OK;
    list_item_t * curr_item = NULL;
    list_item_t * prev_item = NULL;

    if (list == NULL) {
        status = SC_PTR_NULL;
        LOG_ERR("List pointer not initialized.");
        goto out;
    }

    curr_item = list->head;
    while(curr_item != NULL) {
        if (curr_item->value == value) {
            if (curr_item == list->head) {
                curr_item->next->prev = NULL;
                list->head = curr_item->next;
            } else if (curr_item == list->end) {
                curr_item->prev->next = NULL;
                list->end = curr_item->prev;
            } else {
                prev_item->next = curr_item->next;
                curr_item->next->prev = prev_item;
            }
            goto out;
        }
        prev_item = curr_item;
        curr_item = curr_item->next;
    }

out:
    if (status == SC_OK) {
        free(curr_item);
        list->len--;
        LOG_SUC("List item deleted.");
    };

    return status;
}

status_code_t list_find_item(list_t *      list,
                             uint32_t      value,
                             list_item_t * list_item)
{
    status_code_t status = SC_OK;
    goto out;
out:
    return status;
}

status_code_t list_print(list_t * list)
{
    status_code_t status = SC_OK;
    list_item_t * curr_item = NULL;

    if (list == NULL) {
        status = SC_PTR_NULL;
        LOG_ERR("List pointer not initialized.");
        goto out;
    }

    curr_item = list->head;
    while(curr_item != NULL) {
        printf(" %du ->", curr_item->value);
        curr_item = curr_item->next;
    }

out:
    return status;
}
