#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "list.h"
#include "logger.h"


#define LAST_ITEM_INDEX (-1)


status_code_t list_init(list_t ** list,
                        linking_type_t type)
{
    status_code_t status = SC_OK;
    bool_t        rollback_alloc = false;

    if (list == NULL) {
        status = SC_PTR_NULL;
        LOG_ERR("List pointer is NULL.");
        goto out;
    }

    if ((*list) != NULL) {
        status = SC_PTR_NOT_NULL;
        LOG_ERR("List already initialized.");
        goto out;
    }
    rollback_alloc = true;

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
        if (rollback_alloc == true) {
            LOG_ERR("List pointer freed.");
            free(*list);
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

    if (index > list->len && index != LAST_ITEM_INDEX) {
        status = SC_INV_RANGE;
        LOG_ERR("Index doesn`t match valid range.");
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
        LOG_SUC("List item %d added.", value);
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
                if (list->len > 1) {
                    curr_item->next->prev = NULL;
                }
                list->head = curr_item->next;
            } else if (curr_item == list->end) {
                if (list->len > 1) {
                    curr_item->prev->next = NULL;
                }
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

    /* Entry wasn`t found */
    status = SC_NOT_FOUND;

out:
    if (status == SC_OK) {
        free(curr_item);
        list->len--;
        LOG_SUC("List item %d deleted.", value);
    };

    return status;
}

status_code_t list_find_item(list_t *      list,
                             uint32_t      value,
                             list_item_t * list_item)
{
    status_code_t status = SC_OK;
    list_item_t * curr_item = NULL;

    if (list == NULL) {
        status = SC_PTR_NULL;
        LOG_ERR("List pointer not initialized.");
        goto out;
    }

    if (list_item == NULL) {
        status = SC_PTR_NULL;
        LOG_ERR("List item pointer not initialized.");
        goto out;
    }

    curr_item = list->head;
    while(curr_item != NULL) {
        if (curr_item->value == value) {
            *list_item = *curr_item;
            goto out;
        }
        curr_item = curr_item->next;
    }

    status = SC_NOT_FOUND;

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

    LOG_MSG("List:\n");
    curr_item = list->head;
    while(curr_item != NULL) {
        printf(" %du ->", curr_item->value);
        curr_item = curr_item->next;
    }
    printf("\n\n");

out:
    return status;
}
